#include "sound_app.h"
#include "sound_freq_last.h"
#include <limits.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>
#include <math.h>
#include <stdlib.h> // For exit function

void *fft_thr_fcn(void *ptr)
{
    static double tmp_buf[4096];
    static double freq_buf[4096];
    char message[20]; 
    int message_len = 0;
    int sampling_rate = 48000;
    int fft_length = 4096;
    int target_frequencies[] = {697, 770, 852, 941, 1209, 1336, 1477, 1633};
    int num_targets = sizeof(target_frequencies) / sizeof(target_frequencies[0]);

    const char dtmf_map[4][4] = {
        {'1', '2', '3', 'A'},
        {'4', '5', '6', 'B'},
        {'7', '8', '9', 'C'},
        {'*', '0', '#', 'D'}
    };

    int low_freq_indices[] = {0, 1, 2, 3};
    int high_freq_indices[] = {4, 5, 6, 7};
    char last_dtmf_tone = '\0'; // To prevent duplicate detections
    double min_amplitude_threshold = 0.03; // Minimum amplitude threshold

    printf("Start waiting DTMF\n"); // Initial message

    while (1)
    {
        pthread_mutex_lock(&data_cond_mutex);
        pthread_cond_wait(&data_cond, &data_cond_mutex);

        for (int i = 0; i < 4096; i++)
        {
            tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
        }

        sound_freq_last(tmp_buf, freq_buf);

        double detected_amplitudes[8] = {0};

        // Calculate amplitudes for target frequencies
        for (int j = 0; j < num_targets; j++)
        {
            int frequency = target_frequencies[j];
            int index = (frequency * fft_length) / sampling_rate;

            if (index < 2048)
            {
                detected_amplitudes[j] = freq_buf[index];
            }
        }

        // Detect DTMF tone
        int detected_low = -1;
        int detected_high = -1;
        double max_low_amp = 0;
        double max_high_amp = 0;

        for (int i = 0; i < 4; i++)
        {
            if (detected_amplitudes[low_freq_indices[i]] > max_low_amp)
            {
                max_low_amp = detected_amplitudes[low_freq_indices[i]];
                detected_low = i;
            }
        }

        for (int i = 0; i < 4; i++)
        {
            if (detected_amplitudes[high_freq_indices[i]] > max_high_amp)
            {
                max_high_amp = detected_amplitudes[high_freq_indices[i]];
                detected_high = i;
            }
        }

        if (detected_low != -1 && detected_high != -1)
        {
            char dtmf_tone = dtmf_map[detected_low][detected_high];
            double avg_amplitude = (max_low_amp + max_high_amp) / 2;

            // Check amplitude threshold and add to message if tone is new and valid
            if (avg_amplitude >= min_amplitude_threshold && dtmf_tone != last_dtmf_tone && message_len < 10)
            {
                message[message_len++] = dtmf_tone;
                last_dtmf_tone = dtmf_tone;
                printf("Detect!\n"); // Detection message
                printf("Phone: %s\n", message); // Final phone number display
                    // printf("Done stop program.\n");
                // Stop if message length reaches 10 (Phone number complete)
                if (message_len == 10)
                {
                    message[message_len] = '\0'; // Null-terminate message
                    printf("Success: %s\n", message); // Final phone number display
                    // printf("Done stop program.\n");
                    exit(0); // Exit the program
                }
            }else{
                printf("-\0"); // Not detected message
            }
        }

        pthread_mutex_unlock(&data_cond_mutex);
    }
}
