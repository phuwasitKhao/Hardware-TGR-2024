#ifndef MAIN_H
#define MAIN_H

#include <pthread.h>

// Buffer size for ALSA capture
#define BUFFER_SIZE 48000

// Shared variables for audio data and synchronization
extern short shared_audio_buf[BUFFER_SIZE];
extern pthread_mutex_t audio_mutex;
extern pthread_cond_t audio_cond;

// Function prototypes
void *alsa_capture_thread(void *device_name);

#endif // MAIN_H
