// findProbabilities.cpp
// Converts Neuralink .wav file (see https://content.neuralink.com/compression-challenge/README.html) ..
// and returns a pointer to allocated memory with samples.

#include "wavReader.h"

using namespace std;


//see https://github.com/phoboslab/neuralink_brainwire/blob/master/bwenc.c
//the code author implemented this method to downsample the data by 6 bits, as for some reason ...
//it's not done by just a simple bit shift.

static inline int brainwire_quant(int v) {
    return static_cast<int>(floor(v/64.0));
}  

static inline int brainwire_dequant(int v) {
	if (v >= 0) {
		return round(v * 64.061577 + 31.034184);
	}
	else {
		return -round((-v - 1) * 64.061577 + 31.034184) - 1;
	}
 }


 short* file_read(const string& path, wav_header* header) {

    ifstream file(path, ios::binary);

    if (!file.is_open()) {
        cerr << "Error opening file: " << path << endl;
        return nullptr;
    }
    
    // see http://soundfile.sapp.org/doc/WaveFormat/

    file.read(reinterpret_cast<char*>(&header->chunkID),4);
    file.read(reinterpret_cast<char*>(&header->chunkSize),4);
    file.read(reinterpret_cast<char*>(&header->format),4);
    file.read(reinterpret_cast<char*>(&header->subChunk1ID),4);
    file.read(reinterpret_cast<char*>(&header->subChunk1Size),4);
    file.read(reinterpret_cast<char*>(&header->audioFormat),2);
    file.read(reinterpret_cast<char*>(&header->num_channels),2);
    file.read(reinterpret_cast<char*>(&header->sample_rate),4);
    file.read(reinterpret_cast<char*>(&header->byte_rate),4);
    file.read(reinterpret_cast<char*>(&header->block_align),2);
    file.read(reinterpret_cast<char*>(&header->bits_per_sample),2);
    file.read(reinterpret_cast<char*>(&header->subChunk2ID),4);
    file.read(reinterpret_cast<char*>(&header->subChunk2Size),4);

    int num_samples = header->subChunk2Size / (header->bits_per_sample / 8);

    short* samples = new short[num_samples];

    //read audio data
    file.read(reinterpret_cast<char*>(samples), header->subChunk2Size);

    if (file.gcount() < header->subChunk2Size) {
        cerr << "Error reading WAV data" << endl;
        delete[] samples;
        return nullptr;
    }

    for (int i = 0; i < num_samples; ++i) {

        //Check quantize logic (can be removed)
        // if (brainwire_dequant(brainwire_quant(samples[i])) != samples[i]) {
        //     cout << "QUANTIZE ERROR" << endl;
        // }

        samples[i] = brainwire_quant(samples[i]);
    }
    file.close();
    return samples;
 }

 int wavReader() {

	string directory = "./data/";
	
	uint32_t total_samples = 0;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			string path = entry.path().string();
			cout << "Processing File: " << path << endl;

			wav_header header;
			short* samples = file_read(path, &header);

            // cout << "Num Channels: " << header.num_channels << endl;
            // cout << "Sample Rate: " << header.sample_rate << endl;
            // cout << "SubChunk1 Size: " << header.subChunk1Size << "bytes" << endl;
            // cout << "SubChunk2 Size: " << header.subChunk2Size << "bytes" << endl;
            // cout << "Audio Format" <<  header.audioFormat << endl;
            // cout << "Bits per sample: " << header.bits_per_sample << endl;

        }
    }

	return 0;
}