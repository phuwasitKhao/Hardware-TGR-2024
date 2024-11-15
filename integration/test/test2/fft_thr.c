#include "../../main_thr.h"
// #include "..sound_freq_last.h"
#include "/home/coekku/workshop/integration/sound/sound_freq_last.h"
#include <limits.h>
#include <pthread.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "/home/coekku/workshop/integration/sound/write_file_sound.c"

#define SAMPLE_RATE 48000
#define CHANNELS 1
#define BITS_PER_SAMPLE 16
#define ONE_MINUTE_SAMPLES (SAMPLE_RATE * 60 * CHANNELS) // จำนวน sample สำหรับ 1 นาที

// ฟังก์ชันสำหรับบันทึกข้อมูลเสียง 1 นาทีต่อไฟล์
void *fft_thr_fcn(void *ptr)
{
    static double tmp_buf[4096];
    static double freq_buf[2048]; // FFT buffer สำหรับเก็บข้อมูลความถี่

    double avg_value = 0.0; // To calculate average amplitude
    int sample_rate = SAMPLE_RATE;
    int channels = CHANNELS;
    int bits_per_sample = BITS_PER_SAMPLE;
    int file_count = 0; // ตัวนับไฟล์สำหรับตั้งชื่อไฟล์ใหม่ทุก 1 นาที
    int samples_written = 0;
    int buffer_size = 4096;      // จำนวนตัวอย่างใน shared_buf
    bool sound_detected = false; // Flag for sound detection

    char filename[256];
    int16_t *record_buffer = malloc(ONE_MINUTE_SAMPLES * sizeof(int16_t)); // บัฟเฟอร์สำหรับเก็บข้อมูล 1 นาที

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

        // Check if there is no sound
        if (avg_value < 20.0 / SHRT_MAX) // Adjust threshold to the normalized scale
        {
            if (!sound_detected)
            {
                printf("Stroll-----------------------------------");
                fflush(stdout);
            }
            pthread_mutex_unlock(&data_cond_mutex);
            continue;
        }
        else
        {
            if (!sound_detected)
            {
                printf("\nSignificant sound detected (avg_value: %.6f)\n", avg_value);
                printf("------------------------------------------------------------\n");
                sound_detected = true;
            }
        }

        // ตั้งชื่อไฟล์ใหม่สำหรับบันทึกเสียง
        snprintf(filename, sizeof(filename), "output_%d.wav", file_count++);
        samples_written = 0;

        while (samples_written < ONE_MINUTE_SAMPLES)
        {
            // รอให้ข้อมูลใหม่ใน shared_buf พร้อมใช้งาน

            printf("Processing audio data...\n");

            // คัดลอกข้อมูลจาก shared_buf ไปยัง record_buffer เพื่อเก็บข้อมูล 1 นาที
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

            // สามารถใช้ freq_buf วิเคราะห์ความถี่ที่ต้องการได้ที่นี่

            // Unlock the mutex after processing
            pthread_mutex_unlock(&audio_cond_mutex);
        }

        // เมื่อครบ 1 นาทีแล้ว ให้บันทึกข้อมูลลงในไฟล์ WAV
        save_buffer_to_wav(filename, record_buffer, ONE_MINUTE_SAMPLES, sample_rate, channels, bits_per_sample, false);
        printf("File %s saved successfully.\n", filename);
    }

    free(record_buffer);
    return NULL;
}
