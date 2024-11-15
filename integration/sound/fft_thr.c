#include "../main_thr.h"
#include "sound_freq_last.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "write_file_sound.c"

#define SAMPLE_RATE 48000
#define CHANNELS 1
#define BITS_PER_SAMPLE 16
#define ONE_MINUTE_SAMPLES (SAMPLE_RATE * 60 * CHANNELS) // Number of samples for 1 minute
#define DB_NAME "logs_1.db"
#include <sys/stat.h>

int get_file_size(const char *filename)
{
    struct stat st;
    if (stat(filename, &st) == 0)
    {
        return st.st_size; // คืนค่าขนาดไฟล์ในหน่วยไบต์
    }
    else
    {
        fprintf(stderr, "Could not determine file size for %s\n", filename);
        return -1;
    }
}

void *fft_thr_fcn(void *ptr)
{
    static double tmp_buf[4096];
    static double freq_buf[2048]; // FFT buffer for frequency data
    char mes[256];
    int file_size = 0; // ตัวแปรเก็บขนาดไฟล์
                       // ประกาศตัวแปร file_size เพื่อใช้เก็บขนาดไฟล์

    double avg_value = 0.0; // To calculate average amplitude
    int sample_rate = SAMPLE_RATE;
    int channels = CHANNELS;
    int bits_per_sample = BITS_PER_SAMPLE;
    int file_count = 0; // Counter for file naming
    int samples_written = 0;
    int buffer_size = 4096;      // Size of shared_buf
    bool sound_detected = false; // Flag for sound detection
    char db_name[] = "logs_1.db";

    time_t now;
    struct tm *timeinfo;

    char table_name[] = "logs_1";

    // Initialize the database
    // dbase_init(db_name, "CREATE TABLE IF NOT EXISTS logs ("
    //                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
    //                     "file TEXT, "
    //                     "time DATETIME DEFAULT (DATETIME('now', 'localtime')));");
    
    // setup
    time(&now);
    timeinfo = localtime(&now);
    char filename[256];
    int16_t *record_buffer = malloc(ONE_MINUTE_SAMPLES * sizeof(int16_t)); // Buffer to store 1 minute of audio data

    if (record_buffer == NULL)
    {
        fprintf(stderr, "Memory allocation for record buffer failed.\n");
        return NULL;
    }

    printf("FFT processing and recording thread started.\n");

    while (1)
    {
        pthread_mutex_lock(&audio_cond_mutex);
        pthread_cond_wait(&audio_cond, &audio_cond_mutex);

        avg_value = 0.0;
        for (int i = 0; i < 4096; i++)
        {
            tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
            avg_value += fabs(tmp_buf[i]);
        }
        avg_value /= 4096;

        // Check if there is no significant sound
        if (avg_value < 20.0 / SHRT_MAX) // Adjust threshold to the normalized scale
        {
            // printf("Stroll -----------------------------------\n");
            fflush(stdout);
            sound_detected = false; // Reset the sound detection flag
            pthread_mutex_unlock(&audio_cond_mutex);
            continue;
        }
        else
        {
            if (!sound_detected)
            {
                printf("\nSignificant sound detected (avg_value: %.6f)\n", avg_value);
                printf("------------------------------------------------------------\n");
                sound_detected = true; // Start recording
            }
            pthread_mutex_unlock(&audio_cond_mutex);
        }
        // Name the file for recording audio
        char timestamp[64];
        strftime(timestamp, sizeof(timestamp), "%Y-%m-%d_%H-%M-%S", timeinfo);
        snprintf(filename, sizeof(filename), "/home/coekku/workshop/integration/output/output_%d_%s.wav", file_count++, timestamp);

        // Reset samples_written for each new file
        samples_written = 0;
        while (samples_written < ONE_MINUTE_SAMPLES)

        {
            pthread_mutex_lock(&audio_cond_mutex);
            pthread_cond_wait(&audio_cond, &audio_cond_mutex);

            // Copy data from shared_buf to record_buffer to accumulate 1 minute of audio
            int remaining_samples = ONE_MINUTE_SAMPLES - samples_written;
            int samples_to_copy = (remaining_samples < buffer_size) ? remaining_samples : buffer_size;

            memcpy(&record_buffer[samples_written], shared_buf, samples_to_copy * sizeof(int16_t));
            samples_written += samples_to_copy;

            // Normalize data and copy to tmp_buf for FFT processing
            for (int i = 0; i < buffer_size; i++)
            {
                tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
            }

            // Perform FFT and store the result in freq_buf
            sound_freq_last(tmp_buf, freq_buf);
            pthread_mutex_unlock(&audio_cond_mutex);
            // Check for sound in the buffer (optional additional analysis)

            // Wait for new data
        }

        // Once 1 minute of data is collected, save it to a WAV file

        // dbase_init(DB_NAME, "CREATE TABLE IF NOT EXISTS logs ("
        //                     "id INTEGER PRIMARY KEY AUTOINCREMENT, "
        //                     "details FLOAT),"
        //                     "timestamp DATETIME DEFAULT (DATETIME('now', 'localtime')), ");
        save_buffer_to_wav(filename, record_buffer, ONE_MINUTE_SAMPLES, sample_rate, channels, bits_per_sample, false);
        pthread_mutex_lock(&rest_cond_mutex);
        strcpy(shared_message_sw, filename);
        pthread_cond_signal(&rest_cond);
        pthread_mutex_unlock(&rest_cond_mutex);
        // dbase_append(DB_NAME, db_name, filename);
        int file_size = get_file_size(filename);
        if (file_size < 0)
        {
            fprintf(stderr, "Failed to get file size for %s\n", filename);
            file_size = 0;
        }

        if (dbase_append_with_size(db_name, table_name, filename, file_size) != 0)
        {
            fprintf(stderr, "Failed to append file: %s to database\n", filename);
        }
        else
        {
            printf("Successfully logged file: %s with size: %d bytes\n", filename, file_size);
        }

        // else
        // {
        //     printf("Filename logged to database successfully: %s\n", filename);
        // }
        // dbase_append(DB_NAME, sizeof(filename));
        // dbase_append(DB_NAME, "logs", "sd");
        // if (dbase_append(DB_NAME, sizeof(filename)) == 0)
        // {
        //     printf("Logged value: %.6f to database\n", filename);
        // }
        // else
        // {
        //     printf("Failed to log value to database\n");
        // }
        printf("File %s saved successfully.\n", filename);
    }

    free(record_buffer);
    return NULL;
}
