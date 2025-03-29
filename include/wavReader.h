#ifndef WAVREADER_H
#define WAVREADER_H

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <string>
#include <map>
#include <unordered_map>
#include <utility>
#include <vector>

#include <omp.h>

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

struct FileStats {  
    std::map<std::vector<int16_t>, size_t> ngram_counts;
    size_t total_count = 0;
};


class wavReader {

    private:

        std::vector<int> quantisedData;
        std::vector<std::pair<std::vector<int16_t>, double>> sortedProbs;
        wav_header currHeader;

        FileStats globalStats;

        std::unordered_map<int, uint32_t> symbol_counts;
        uint32_t totalSamples = 0;

        void getData(const std::string& path, wav_header* header);
        void makeSortedProb();

        FileStats processFile(const std::string& path, int ngram_size);

    public:
    
        int outputTxt();

        wavReader();

        ~wavReader();
};



// Function declarations
short* file_read(const std::string& path, wav_header* header);

#endif // WAVREADER_H