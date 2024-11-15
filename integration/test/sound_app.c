#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include "../db/db_1/db_helper.h"

#define DB_NAME "../db/db_1/logs.db"

int main(int argc, char *argv[])
{
	if (argc < 3)
	{
		printf("Usage: %s <device> <rate>\n", argv[0]);
		return 1;
	}

	int err;
	short buf[9600];
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;
	snd_pcm_sw_params_t *sw_params;

	// Initialize sound capture
	if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		fprintf(stderr, "Cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
		return 1;
	}
	printf("Audio device opened successfully.\n");

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
	printf("Hardware parameters initialized.\n");

	snd_pcm_sw_params_current(capture_handle, sw_params);
	snd_pcm_sw_params_set_start_threshold(capture_handle, sw_params, 0);
	snd_pcm_sw_params(capture_handle, sw_params);
	printf("Software parameters initialized.\n");

	// Initialize the database
	dbase_init(DB_NAME);

	// Start the audio stream
	if ((err = snd_pcm_prepare(capture_handle)) < 0)
	{
		fprintf(stderr, "Unable to prepare audio stream: %s\n", snd_strerror(err));
		return 1;
	}

	// Loop reading sound data and logging it
	for (int i = 0; i < 1000; ++i)
	{
		if ((err = snd_pcm_readi(capture_handle, buf, 4800)) != 4800)
		{
			fprintf(stderr, "Read from audio interface failed (%s)\n", snd_strerror(err));
			return 1;
		}

		double avg_val = 0.0;
		for (int j = 0; j < 4800; j++)
		{
			avg_val += abs(buf[j]);
		}
		avg_val /= 4800;

		printf("Average sound value: %.6f\n", avg_val);

		// Print a line of dashes if avg_val > 28000
		if (avg_val > 20)
		{
			printf("-----------------------------------------------------------\n");
		}

		// Log the value to the database
		if (dbase_append(DB_NAME, avg_val) == 0)
		{
			printf("Logged value: %.6f to database\n", avg_val);
		}
		else
		{
			printf("Failed to log value to database\n");
		}
	}

	snd_pcm_close(capture_handle);
	snd_pcm_hw_params_free(hw_params);
	snd_pcm_sw_params_free(sw_params);
	return 0;
}
