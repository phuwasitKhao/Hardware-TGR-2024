#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <limits.h>

// Declare global variables or ensure they're available in the linked code
extern pthread_mutex_t data_cond_mutex;
extern pthread_cond_t data_cond;
extern short shared_buf[4096];

void *fft_thr_fcn(void *ptr) {
    // Setup
    static double tmp_buf[4096];
    static double freq_buf[2048];  // Assuming only the first half of the FFT is used
    bool continue_processing = true;
    int sampling_rate = 44100;  // Replace with your actual sampling rate
    int fft_length = 4096;      // FFT length

    // Frequencies of interest
    int target_frequencies[] = {1, 100, 1000, 10000};
    int num_targets = sizeof(target_frequencies) / sizeof(target_frequencies[0]);

    while (continue_processing) {
        // Wait for new data to be available
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);
        
        printf("Start processing\n");

        // Normalize data and copy to tmp_buf
        for (int i = 0; i < 4096; i++) {
            tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
        }

        // Perform FFT and store the result in freq_buf
        sound_freq(tmp_buf, freq_buf);

        // Print only the amplitudes for the specific target frequencies
        printf("Frequency (Hz) - Amplitude\n");
        for (int j = 0; j < num_targets; j++) {
            int frequency = target_frequencies[j];
            int index = (frequency * fft_length) / sampling_rate;

            if (index < 2048) {  // Check to ensure index is within bounds
                printf("%d Hz - %f\n", frequency, freq_buf[index]);
            } else {
                printf("%d Hz is out of range for this FFT resolution.\n", frequency);
            }
        }

        // Unlock the mutex after processing
        pthread_mutex_unlock(&data_cond_mutex);

        // Optional: add a condition to stop processing if desired
        // continue_processing = false; // Uncomment to run the loop only once
    }

    return NULL;
}
