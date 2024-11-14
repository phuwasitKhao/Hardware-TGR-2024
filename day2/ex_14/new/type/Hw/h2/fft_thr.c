#include "sound_app.h"
#include "sound_freq_last.h"
#include <limits.h>

void *fft_thr_fcn(void *ptr)
{
    // Setup
    static double tmp_buf[4096];
    static double freq_buf[2048]; // Assuming only the first half of the FFT is used
    bool continue_processing = true;
    int sampling_rate = 44100; // Replace with your actual sampling rate
    int fft_length = 4096;     // FFT length

    while (continue_processing)
    {
        // Wait for new data to be available
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        printf("Start processing\n");

        // Normalize data and copy to tmp_buf
        for (int i = 0; i < 4096; i++)
        {
            tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
        }

        // Perform FFT and store the result in freq_buf
        sound_freq_last(tmp_buf, freq_buf);

        double max_amplitudes[3] = {0.0, 0.0, 0.0};
        int max_frequencies[3] = {0, 0, 0};

        for (int i = 0; i < 2048; i++)
        {
            if (freq_buf[i] > max_amplitudes[0])
            {
                max_amplitudes[2] = max_amplitudes[1];
                max_frequencies[2] = max_frequencies[1];
                max_amplitudes[1] = max_amplitudes[0];
                max_frequencies[1] = max_frequencies[0];

                max_amplitudes[0] = freq_buf[i];
                max_frequencies[0] = (i * sampling_rate) / fft_length;
            }
            else if (freq_buf[i] > max_amplitudes[1])
            {
                max_amplitudes[2] = max_amplitudes[1];
                max_frequencies[2] = max_frequencies[1];

                max_amplitudes[1] = freq_buf[i];
                max_frequencies[1] = (i * sampling_rate) / fft_length;
            }
            else if (freq_buf[i] > max_amplitudes[2])
            {
                max_amplitudes[2] = freq_buf[i];
                max_frequencies[2] = (i * sampling_rate) / fft_length;
            }
        }

        // Print the top 3 frequencies and their amplitudes
        printf("Top 3 Frequencies (Hz) - Amplitudes\n");
        for (int j = 0; j < 3; j++)
        {
            printf("%d Hz - %f\n", max_frequencies[j], max_amplitudes[j]*10);
        }

        // Unlock the mutex after processing
        pthread_mutex_unlock(&data_cond_mutex);

        // Optional: add a condition to stop processing if desired
        // continue_processing = false; // Uncomment to run the loop only once
    }

    return NULL;
}
