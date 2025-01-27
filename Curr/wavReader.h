#ifndef WAVREADER_H
#define WAVREADER_H

#include <string>
#include <unordered_map>
#include <filesystem>
#include <iostream>
#include <fstream>
#include <cmath>

struct samples_t {
    uint32_t sample_rate;
    uint16_t bits_per_sample;
    uint32_t num_samples;
    uint16_t channels;
};

struct wav_header {
    uint32_t chunkID;
    uint32_t chunkSize;
    uint32_t format;
    uint32_t subChunk1ID;
    uint32_t subChunk1Size;
    uint16_t audioFormat;
    uint16_t num_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;   
    uint16_t bits_per_sample;
    uint32_t subChunk2ID;
    uint32_t subChunk2Size;
};


// Function declarations
short* file_read(const std::string& path, wav_header* header);

#endif // WAVREADER_H