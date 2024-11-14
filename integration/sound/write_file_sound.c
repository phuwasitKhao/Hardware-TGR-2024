#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
    char riff[4];
    uint32_t overall_size;
    char wave[4];
    char fmt_chunk_marker[4];
    uint32_t length_of_fmt;
    uint16_t format_type;
    uint16_t channels;
    uint32_t sample_rate;
    uint32_t byterate;
    uint16_t block_align;
    uint16_t bits_per_sample;
    char data_chunk_header[4];
    uint32_t data_size;
} WAVHeader;

void save_buffer_to_wav(const char *filename, int16_t *buffer, int buffer_size, int sample_rate, int channels, int bits_per_sample, bool append) {
    FILE *file;
    WAVHeader header;

    if (append) {
        // Open existing file to append data and update header
        file = fopen(filename, "r+b");
        if (file == NULL) {
            fprintf(stderr, "Could not open file for appending: %s\n", filename);
            return;
        }

        // Read the existing header to update the size fields
        fread(&header, sizeof(WAVHeader), 1, file);
        fseek(file, 0, SEEK_END);  // Move to the end of the data to append

        // Update the header's data size fields
        header.data_size += buffer_size * sizeof(int16_t);
        header.overall_size = header.data_size + 36;
    } else {
        // Create a new file with an initial WAV header
        file = fopen(filename, "wb");
        if (file == NULL) {
            fprintf(stderr, "Could not open file for writing\n");
            return;
        }

        // Prepare the initial WAV header
        memcpy(header.riff, "RIFF", 4);
        header.overall_size = buffer_size * sizeof(int16_t) + 36;
        memcpy(header.wave, "WAVE", 4);
        memcpy(header.fmt_chunk_marker, "fmt ", 4);
        header.length_of_fmt = 16;
        header.format_type = 1; // PCM format
        header.channels = channels;
        header.sample_rate = sample_rate;
        header.bits_per_sample = bits_per_sample;
        header.byterate = sample_rate * channels * bits_per_sample / 8;
        header.block_align = channels * bits_per_sample / 8;
        memcpy(header.data_chunk_header, "data", 4);
        header.data_size = buffer_size * sizeof(int16_t);

        // Write the initial header to the new file
        fwrite(&header, sizeof(WAVHeader), 1, file);
    }

    // Write the new audio buffer to the file
    fwrite(buffer, sizeof(int16_t), buffer_size, file);

    // Update the header with new sizes
    if (append) {
        // Move to the beginning to rewrite the header with updated sizes
        fseek(file, 0, SEEK_SET);
        fwrite(&header, sizeof(WAVHeader), 1, file);
    }

    fclose(file);
    printf("Data %s to %s successfully.\n", append ? "appended" : "saved", filename);
}
