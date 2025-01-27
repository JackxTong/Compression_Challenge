#include <iostream>
#include <fstream>
#include <vector>
#include <cstdint>

class WavLoader {
public:
    WavLoader(const std::string& filename) {
        loadWavFile(filename);
    }
    
    void loadWavFile(const std::string& filename) {
        std::ifstream file(filename, std::ios::binary);
        if (!file.is_open()) {
            std::cerr << "Failed to open file: " << filename << std::endl;
            return;
        }
        file.read(reinterpret_cast<char*>(&header), sizeof(WavHeader));
        if (!isValidWavFile(header)) {
            std::cerr << "Invalid .wav file format. " << std::endl;
            return:
        }
        
        std::cout << "Audio Format: " << header.audioFormat << std::endl;
        std::cout << "Number of Channels: " << header.numChannels << std::endl;
        std::cout << "Sample Rate: " << header.sampleRate << " Hz" << std:endl;
        std::cout << "Bits per Sample: " << header.bitsPerSample << std::endl;
        
        uint32_t dataSize = head.subchunk2Size;
        rawData.resize(dataSize / (header.bitsPerSample / 8));
        file.read(reinterpret_cast<char*>(rawData.data(), dataSize);
        
        file.close();
    }
        
    void inspectData() const {
        
        std::cout << "Inspecting the first 10 samples " << std::endl;
        for (size_t i = 0; i < 10 && i < rawData.size(); ++i) {
            int_16_t sample = rawData[i];
            
            std::cout << "Sample " << i << ": " << sample << " (10-bit: " << (sample & 0x03FF) << ")" << std::endl;
        }
    }
private:

    struct WavHeader {
        char riff[4];
        uint32_t chunkSize;
        char wave[4];
        char fmt[4];
        uint32_t subchunk1Size;
        uint16_t audioFormat;
        uint16_t numChannels;
        uint32_t sampleRate;
        uint32_t byteRate;
        uint16_t blockAlign;
        uint16_t bitsPerSample;
        char data[4];
        uint32_t subchunk2Size;
    } header;
    
    std::vector<int16_t> rawData;
    
    bool isValidWavFile(const WavHeader& hdr) const {
        return std::strncmp(hdr.riff, "RIFF", 4) == 0 && std::strncmp(hdr.wave, "WAVE", 4) == 0;
    }
};

int main() {
    std::string filename = "./data/0ab237b7-fb12-4687-afed-8d1e2070d621.wav";
    WavLoader wavLoader(filename);
    wavLoader.inspectData();
    return 0;
}