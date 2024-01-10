#ifndef _PCM_H
#define _PCM_H

#include <stdio.h>

struct PcmHeader {
	int chunk_size;
	int subchunk1_size;
	short audio_format;
	short num_channels;
	int byte_rate;
	short block_align;
	int subchunk2_size;
	int bit_depth;
	int sample_rate;
};

struct PcmHeader get_pcm_header(int num_samples, int sample_rate, int bit_depth) {

	short num_channels = 1;

	struct PcmHeader header = {
		.chunk_size = 36 + num_samples + bit_depth / 8,
		.subchunk1_size = 16,		// Subchunk 1 size
		.audio_format = 1, 		// 1 = PCM
		.num_channels = num_channels,	// Number of channels
		.byte_rate = sample_rate * num_channels * bit_depth / 8,
		.block_align = num_channels * bit_depth / 8,
		.subchunk2_size = num_samples * bit_depth / 8,
		.bit_depth = bit_depth,
		.sample_rate = sample_rate,
	};

	return header;  // return a copy
}

void write_pcm_header(FILE * fp, struct PcmHeader * header) {
	fwrite("RIFF", 1, 4, fp);  // Chunk ID
	fwrite(&(header->chunk_size), 4, 1, fp);  // Chunk size
	fwrite("WAVE", 1, 4, fp);  // Format
	fwrite("fmt ", 1, 4, fp);  // Subchunk 1 ID
	fwrite(&(header->subchunk1_size), 4, 1, fp);  // Subchunk 1 size
	fwrite(&(header->audio_format), 2, 1, fp);  // Audio format (1 = PCM)
	fwrite(&(header->num_channels), 2, 1, fp);  // Number of channels
	fwrite(&(header->sample_rate), 4, 1, fp);  // Sample rate
	fwrite(&(header->byte_rate), 4, 1, fp);  // Byte rate
	fwrite(&(header->block_align), 2, 1, fp);  // Block align
	fwrite(&(header->bit_depth), 2, 1, fp);  // Bits per sample
	fwrite("data", 1, 4, fp);  // Subchunk 2 ID
	fwrite(&(header->subchunk2_size), 4, 1, fp);  // Subchunk 2 size
}

#endif
