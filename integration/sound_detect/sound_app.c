#include "stdlib.h"
#include "stdio.h"
#include <alsa/asoundlib.h>

void print_timestamp(const char *status)
{
	time_t now;
	struct tm *local;
	now = time(NULL);
	local = localtime(&now);
	printf("%s at %02d:%02d:%02d\n", status, local->tm_hour, local->tm_min, local->tm_sec);
}

int main(int argc, char *argv[])
{
	int i;
	int err;
	short buf[9600];
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;

	// initialize sound
	if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		fprintf(stderr, "cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
		exit(1);
	}
	else
	{
		printf("snd_pcm_open OK\n");
	}
	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_sw_params_alloca(&sw_params);

	unsigned int rate = atoi(argv[2]);
	unsigned int channels = 1;
	snd_pcm_hw_params_any(capture_handle, hw_params);
	snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &rate, 0);
	snd_pcm_hw_params_set_channels(capture_handle, hw_params, channels);
	snd_pcm_hw_params(capture_handle, hw_params);
	printf("HW init ok\n");

	snd_pcm_sw_params_current(capture_handle, sw_params);
	snd_pcm_sw_params_set_start_threshold(capture_handle, sw_params, 0);
	snd_pcm_sw_params(capture_handle, sw_params);
	printf("SW init ok\n");

	// Start the audio stream
	if ((err = snd_pcm_prepare(capture_handle)) < 0)
	{
		fprintf(stderr, "unable to prepare audio stream: %s\n", snd_strerror(err));
		exit(1);
	}

	int sound_detected = 0;
	// loop reading sound data
	for (i = 0; i < 1000; ++i)
	{
		if ((err = snd_pcm_readi(capture_handle, buf, 4800)) != 4800)
		{
			if (err == -EPIPE)
			{
				fprintf(stderr, "\nBuffer underrun occurred! Recovering...\n");
				snd_pcm_recover(capture_handle, err, 0);
				snd_pcm_prepare(capture_handle);
				continue;
			}
			else
			{
				fprintf(stderr, "Read from audio interface failed (%s)\n", snd_strerror(err));
				exit(1);
			}
		}

		float avg_val = 0.0;
		for (int j = 0; j < 4800; j++)
		{
			avg_val += abs(buf[j]);
		}
		avg_val /= 4800;

		if (avg_val < 20)
		{
			if (sound_detected)
			{
				printf("\n");
				print_timestamp("sound silent at");
				sound_detected = 0;
			}
			printf(".");
			fflush(stdout);
		}
		else
		{
			if (!sound_detected)
			{
				printf("\n");
				print_timestamp("sound detected at");
				sound_detected = 1;
			}
		}
	}

	snd_pcm_close(capture_handle);
	snd_pcm_hw_params_free(hw_params);
	snd_pcm_sw_params_free(sw_params);
	exit(0);
}