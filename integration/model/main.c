#include <pthread.h>
#include <alsa/asoundlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "main.h"
#include "predictML.h"
#include "predictML_terminate.h"

#define BUFFER_SIZE 48000 // One second of audio at 48 kHz
#define SAMPLE_RATE 48000

short shared_audio_buf[BUFFER_SIZE];
pthread_mutex_t audio_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t audio_cond = PTHREAD_COND_INITIALIZER;

// Capture thread to detect sample position in the buffer
void *alsa_capture_thread(void *device_name) {
    snd_pcm_t *capture_handle;
    snd_pcm_hw_params_t *hw_params;
    int err;
    short buffer[BUFFER_SIZE];

    // Set up ALSA capture
    if ((err = snd_pcm_open(&capture_handle, (char *)device_name, SND_PCM_STREAM_CAPTURE, 0)) < 0) {
        fprintf(stderr, "Error opening audio device %s: %s\n", (char *)device_name, snd_strerror(err));
        return NULL;
    }
    snd_pcm_hw_params_alloca(&hw_params);
    snd_pcm_hw_params_any(capture_handle, hw_params);
    snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
    unsigned int rate = SAMPLE_RATE;
    snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
    snd_pcm_hw_params_set_channels(capture_handle, hw_params, 1);

    if ((err = snd_pcm_hw_params(capture_handle, hw_params)) < 0) {
        fprintf(stderr, "Error setting HW parameters: %s\n", snd_strerror(err));
        snd_pcm_close(capture_handle);
        return NULL;
    }

    // Capture loop
    while (1) {
        if ((err = snd_pcm_readi(capture_handle, buffer, BUFFER_SIZE)) != BUFFER_SIZE) {
            fprintf(stderr, "Error reading from audio interface: %s\n", snd_strerror(err));
            break;
        }

        // Lock, copy data to shared buffer, signal main thread
        pthread_mutex_lock(&audio_mutex);
        memcpy(shared_audio_buf, buffer, BUFFER_SIZE * sizeof(short));
        pthread_cond_signal(&audio_cond);
        pthread_mutex_unlock(&audio_mutex);
    }

    snd_pcm_close(capture_handle);
    return NULL;
}

int main(int argc, char **argv) {
    pthread_t capture_thread;
    const char *device_name = "default"; // Set to your audio device name

    // Start ALSA capture thread
    if (pthread_create(&capture_thread, NULL, alsa_capture_thread, (void *)device_name) != 0) {
        fprintf(stderr, "Failed to create capture thread\n");
        return 1;
    }

  while (1) {
    pthread_mutex_lock(&audio_mutex);
    pthread_cond_wait(&audio_cond, &audio_mutex);

    double audioData[BUFFER_SIZE];
    for (int i = 0; i < BUFFER_SIZE; ++i) {
        audioData[i] = (double)shared_audio_buf[i] / 0x7fff;
    }
    pthread_mutex_unlock(&audio_mutex);

    int positionArray_data[100];
    int positionArray_size[2] = {1, 0};  // Initialize with zero elements
    unsigned char predictNumArray_data[100];
    int predictNumArray_size[2] = {1, 0};  // Initialize with zero elements

    predictML(audioData, positionArray_data, positionArray_size,
              predictNumArray_data, predictNumArray_size);

    // Check if we received meaningful predictions
    if (positionArray_size[1] > 0 && predictNumArray_size[1] > 0) {
        for (int i = 0; i < positionArray_size[1]; i++) {
            int sample_position = positionArray_data[i];
            double time_position = (double)sample_position / SAMPLE_RATE;
            printf("Position: %d, Time: %.3f seconds\n", sample_position, time_position);

            if (predictNumArray_data[i] == 78) {
                printf("Prediction: N\n");
            } else if (predictNumArray_data[i] == 70) {
                printf("Prediction: F\n");
            } else {
                printf("Prediction: X (unknown)\n");
            }
        }
    } else {
        printf("No meaningful prediction found.\n");
    }
}


    predictML_terminate();
    return 0;
}
