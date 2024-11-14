#ifndef WAV_HELPER_H
#define WAV_HELPER_H

#include <stdio.h>
#include <stdint.h>

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

void generate_wav(const char *filename, int sample_rate, int duration);

#endif // WAV_HELPER_H
