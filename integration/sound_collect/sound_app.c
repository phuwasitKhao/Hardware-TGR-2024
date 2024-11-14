#include <stdlib.h>
#include <stdio.h>
#include <alsa/asoundlib.h>
#include <math.h>

#define INITIAL_THRESHOLD 20000.0	  // ค่าเริ่มต้นของ Threshold
#define PEAK_DELTA 3000.0			  // ระยะที่ต้องเพิ่มเพื่อถือว่าเป็น Peak
#define BUFFER_SIZE 100				  // ขนาดของ Buffer สำหรับ Sliding Window
#define SUDDEN_CHANGE_THRESHOLD 500.0 // Delta ที่ถือว่าเป็นการเปลี่ยนแปลงฉับพลัน

// ฟังก์ชันคำนวณค่า Moving Average
float calculate_moving_average(float *buffer, int size)
{
	float sum = 0.0;
	for (int i = 0; i < size; i++)
	{
		sum += buffer[i];
	}
	return sum / size;
}

int main(int argc, char *argv[])
{
	int i, err;
	short buf[9600]; // Buffer สำหรับเก็บข้อมูลเสียงดิบ
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;

	// *** 1. Initializing ALSA capture device ***
	if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		fprintf(stderr, "Cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
		exit(1);
	}
	printf("snd_pcm_open OK\n");

	// กำหนดค่า Hardware และ Software Parameters
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);

	unsigned int rate = atoi(argv[2]); // Sample Rate
	unsigned int channels = 1;		   // จำนวน Channel (Mono)
	snd_pcm_hw_params_any(capture_handle, hw_params);
	snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
	snd_pcm_hw_params_set_channels(capture_handle, hw_params, channels);
	snd_pcm_hw_params(capture_handle, hw_params);
	printf("HW init OK\n");

	snd_pcm_sw_params_current(capture_handle, sw_params);
	snd_pcm_sw_params_set_start_threshold(capture_handle, sw_params, 0);
	snd_pcm_sw_params(capture_handle, sw_params);
	printf("SW init OK\n");

	// *** 2. Initializing variables ***
	float dynamic_threshold = INITIAL_THRESHOLD; // ค่า Threshold ที่ปรับได้
	float buffer[BUFFER_SIZE] = {0};			 // Buffer สำหรับ Sliding Window
	int buffer_index = 0;						 // ตำแหน่งใน Buffer

	int event_detected = 0;		  // สถานะว่ามี Event (เสียง Peak) หรือไม่
	float previous_avg_val = 0.0; // ค่าเฉลี่ยรอบก่อนหน้า (ใช้คำนวณ Delta)

	// *** 3. Reading and processing audio data ***
	for (i = 0; i < 1000; ++i)
	{
		// อ่านข้อมูลเสียงจาก ALSA
		if ((err = snd_pcm_readi(capture_handle, buf, 4800)) != 4800)
		{
			fprintf(stderr, "Read from audio interface failed (%s)\n", snd_strerror(err));
			exit(1);
		}

		// *** 4. Calculating the average amplitude ***
		float avg_val = 0.0;
		for (int j = 0; j < 4800; j++)
		{
			avg_val += fabs(buf[j]); // ใช้ค่าความเป็นบวกของเสียง
		}
		avg_val /= 4800;

		// อัปเดต Buffer Sliding Window
		buffer[buffer_index] = avg_val;
		buffer_index = (buffer_index + 1) % BUFFER_SIZE;

		// คำนวณค่า Moving Average
		float moving_avg = calculate_moving_average(buffer, BUFFER_SIZE);

		// *** 5. Adjusting dynamic threshold ***
		dynamic_threshold = moving_avg + PEAK_DELTA; // ปรับ Threshold ตามค่าเฉลี่ย

		// *** 6. Detecting sudden changes and peaks ***
		float delta = avg_val - previous_avg_val; // ความต่างระหว่างค่าเสียงรอบก่อนหน้า
		if (avg_val > dynamic_threshold || delta > SUDDEN_CHANGE_THRESHOLD)
		{
			if (!event_detected)
			{
				printf("Peak detected! Amplitude: %.2f (Threshold: %.2f, Delta: %.2f)\n",
					   avg_val, dynamic_threshold, delta);
				event_detected = 1; // เปลี่ยนสถานะเป็นมี Peak
			}
		}
		else
		{
			// เมื่อค่าเสียงลดลงต่ำกว่า Threshold
			if (event_detected)
			{
				printf("Peak ended. Amplitude: %.2f\n", avg_val);
				event_detected = 0; // รีเซ็ตสถานะ
			}
		}

		// เก็บค่าเสียงรอบก่อนหน้าเพื่อใช้คำนวณ Delta
		previous_avg_val = avg_val;

		// *** 7. Debugging output ***
		printf("snd_pcm_readi %.2f (Threshold: %.2f, Delta: %.2f)\n", avg_val, dynamic_threshold, delta);
	}

	// *** 8. Cleanup resources ***
	snd_pcm_close(capture_handle);
	snd_pcm_hw_params_free(hw_params);
	snd_pcm_sw_params_free(sw_params);
	return 0;
}
