#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <alsa/asoundlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <time.h>
#include <json-c/json.h>

#define SAMPLE_RATE 44100
#define CHANNELS 1
#define FRAME_SIZE 4800
#define SILENCE_THRESHOLD 28000.0

typedef struct
{
	char riff[4];
	uint32_t chunk_size;
	char wave[4];
	char fmt[4];
	uint32_t subchunk1_size;
	uint16_t audio_format;
	uint16_t num_channels;
	uint32_t sample_rate;
	uint32_t byte_rate;
	uint16_t block_align;
	uint16_t bits_per_sample;
	char data[4];
	uint32_t subchunk2_size;
} WAVHeader;

// เขียน WAV Header
void write_wav_header(FILE *file, uint32_t data_size, uint32_t sample_rate)
{
	WAVHeader header;
	memcpy(header.riff, "RIFF", 4);
	header.chunk_size = 36 + data_size;
	memcpy(header.wave, "WAVE", 4);
	memcpy(header.fmt, "fmt ", 4);
	header.subchunk1_size = 16;
	header.audio_format = 1; // PCM
	header.num_channels = 1;
	header.sample_rate = sample_rate;
	header.byte_rate = sample_rate * 1 * sizeof(short);
	header.block_align = 1 * sizeof(short);
	header.bits_per_sample = 16;
	memcpy(header.data, "data", 4);
	header.subchunk2_size = data_size;

	fwrite(&header, sizeof(WAVHeader), 1, file);
}

// แปลง WAV เป็น JSON
void wav_to_json(const char *wav_file, const char *json_file)
{
	FILE *file = fopen(wav_file, "rb");
	if (!file)
	{
		fprintf(stderr, "Failed to open WAV file: %s\n", wav_file);
		return;
	}

	fseek(file, 0, SEEK_END);
	uint32_t file_size = ftell(file);
	fseek(file, sizeof(WAVHeader), SEEK_SET); // ข้าม header

	uint32_t data_size = file_size - sizeof(WAVHeader);
	int16_t *audio_data = malloc(data_size);
	fread(audio_data, 1, data_size, file);
	fclose(file);

	struct json_object *json = json_object_new_object();
	json_object_object_add(json, "sampleRate", json_object_new_int(SAMPLE_RATE));
	json_object_object_add(json, "numChannels", json_object_new_int(1));
	json_object_object_add(json, "bitsPerSample", json_object_new_int(16));

	struct json_object *audio_array = json_object_new_array();
	for (uint32_t i = 0; i < data_size / sizeof(int16_t); i++)
	{
		json_object_array_add(audio_array, json_object_new_int(audio_data[i]));
	}
	json_object_object_add(json, "audioData", audio_array);

	FILE *output = fopen(json_file, "w");
	fprintf(output, "%s", json_object_to_json_string(json));
	fclose(output);

	json_object_put(json);
	free(audio_data);

	printf("Converted %s to %s\n", wav_file, json_file);
}

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
	if (argc < 2)
	{
		printf("Usage: %s <device>\n", argv[0]);
		return 1;
	}

	int err;
	short buf[FRAME_SIZE];
	snd_pcm_t *capture_handle;
	snd_pcm_hw_params_t *hw_params;

	FILE *wav_file = NULL;
	char wav_filename[50];
	uint32_t total_samples = 0;
	bool is_recording = false;

	// Initialize ALSA
	if ((err = snd_pcm_open(&capture_handle, argv[1], SND_PCM_STREAM_CAPTURE, 0)) < 0)
	{
		fprintf(stderr, "Cannot open audio device %s (%s)\n", argv[1], snd_strerror(err));
		return 1;
	}

	snd_pcm_hw_params_alloca(&hw_params);
	snd_pcm_hw_params_any(capture_handle, hw_params);
	snd_pcm_hw_params_set_access(capture_handle, hw_params, SND_PCM_ACCESS_RW_INTERLEAVED);
	snd_pcm_hw_params_set_format(capture_handle, hw_params, SND_PCM_FORMAT_S16_LE);
	snd_pcm_hw_params_set_rate_near(capture_handle, hw_params, &(unsigned int){SAMPLE_RATE}, 0);
	snd_pcm_hw_params_set_channels(capture_handle, hw_params, CHANNELS);
	snd_pcm_hw_params(capture_handle, hw_params);

	snd_pcm_prepare(capture_handle);

	// Main loop
	for (int i = 0; i < 1000; ++i)
	{
		if ((err = snd_pcm_readi(capture_handle, buf, FRAME_SIZE)) != FRAME_SIZE)
		{
			fprintf(stderr, "Read from audio interface failed (%s)\n", snd_strerror(err));
			break;
		}

		float avg_val = 0.0;
		for (int j = 0; j < FRAME_SIZE; j++)
		{
			avg_val += abs(buf[j]);
		}
		avg_val /= FRAME_SIZE;

		if (avg_val > SILENCE_THRESHOLD)
		{
			if (!is_recording)
			{
				snprintf(wav_filename, sizeof(wav_filename), "output_%ld.wav", time(NULL));
				wav_file = fopen(wav_filename, "wb");
				if (!wav_file)
				{
					fprintf(stderr, "Failed to create WAV file: %s\n", wav_filename);
					break;
				}
				write_wav_header(wav_file, 0, SAMPLE_RATE);
				total_samples = 0;
				print_timestamp("sound detected");
				is_recording = true;
			}

			fwrite(buf, sizeof(short), FRAME_SIZE, wav_file);
			total_samples += FRAME_SIZE;
		}
		else
		{
			if (is_recording)
			{
				fseek(wav_file, 0, SEEK_SET);
				write_wav_header(wav_file, total_samples * sizeof(short), SAMPLE_RATE);
				fclose(wav_file);

				char json_filename[50];
				snprintf(json_filename, sizeof(json_filename), "%s.json", wav_filename);
				wav_to_json(wav_filename, json_filename);

				print_timestamp("sound silent");
				is_recording = false;
			}
		}
	}

	if (is_recording)
	{
		fseek(wav_file, 0, SEEK_SET);
		write_wav_header(wav_file, total_samples * sizeof(short), SAMPLE_RATE);
		fclose(wav_file);

		char json_filename[50];
		snprintf(json_filename, sizeof(json_filename), "%s.json", wav_filename);
		wav_to_json(wav_filename, json_filename);
	}

	snd_pcm_close(capture_handle);
	return 0;
}
