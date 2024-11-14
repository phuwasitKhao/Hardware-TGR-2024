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
#define ONE_MINUTE_SAMPLES (SAMPLE_RATE * 60 * CHANNELS) // จำนวน sample สำหรับ 1 นาที

// ฟังก์ชันสำหรับบันทึกข้อมูลเสียง 1 นาทีต่อไฟล์
void *fft_thr_fcn(void *ptr)
{
    static double tmp_buf[4096];
    static double freq_buf[2048]; // FFT buffer สำหรับเก็บข้อมูลความถี่

    int sample_rate = SAMPLE_RATE;
    int channels = CHANNELS;
    int bits_per_sample = BITS_PER_SAMPLE;
    int file_count = 0;  // ตัวนับไฟล์สำหรับตั้งชื่อไฟล์ใหม่ทุก 1 นาที
    int samples_written = 0;

    char filename[256];
    int16_t *record_buffer = malloc(ONE_MINUTE_SAMPLES * sizeof(int16_t)); // บัฟเฟอร์สำหรับเก็บข้อมูล 1 นาที

    if (record_buffer == NULL) {
        fprintf(stderr, "Memory allocation for record buffer failed.\n");
        return NULL;
    }

    printf("FFT processing and recording thread started.\n");

    while (1) {
        // ตั้งชื่อไฟล์ใหม่สำหรับบันทึกเสียง
        snprintf(filename, sizeof(filename), "output_%d.wav", file_count++);
        samples_written = 0;

        while (samples_written < ONE_MINUTE_SAMPLES) {
            // รอให้ข้อมูลใหม่ใน shared_buf พร้อมใช้งาน
            pthread_mutex_lock(&data_cond_mutex);
            pthread_cond_wait(&data_cond, &data_cond_mutex);

            printf("Processing audio data...\n");

            // คัดลอกข้อมูลจาก shared_buf ไปยัง record_buffer เพื่อเก็บข้อมูล 1 นาที
            int buffer_size = 4096; // จำนวนตัวอย่างใน shared_buf
            int remaining_samples = ONE_MINUTE_SAMPLES - samples_written;
            int samples_to_copy = (remaining_samples < buffer_size) ? remaining_samples : buffer_size;

            memcpy(&record_buffer[samples_written], shared_buf, samples_to_copy * sizeof(int16_t));
            samples_written += samples_to_copy;

            // Normalize data and copy to tmp_buf for FFT processing
            for (int i = 0; i < buffer_size; i++) {
                tmp_buf[i] = (double)shared_buf[i] / SHRT_MAX;
            }

            // Perform FFT and store the result in freq_buf
            sound_freq_last(tmp_buf, freq_buf);

            // สามารถใช้ freq_buf วิเคราะห์ความถี่ที่ต้องการได้ที่นี่

            // Unlock the mutex after processing
            pthread_mutex_unlock(&data_cond_mutex);
        }

        // เมื่อครบ 1 นาทีแล้ว ให้บันทึกข้อมูลลงในไฟล์ WAV
        save_buffer_to_wav(filename, record_buffer, ONE_MINUTE_SAMPLES, sample_rate, channels, bits_per_sample, false);
        printf("File %s saved successfully.\n", filename);
    }

    free(record_buffer);
    return NULL;
}
