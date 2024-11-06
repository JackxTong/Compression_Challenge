#include <iostream>
#include <fstream>
#include <cstdint>
#include <direct.h>
#include <string>
#include <unordered_map>
#include <iomanip>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;


struct samples_t {
	uint32_t sample_rate;
	uint16_t bits_per_sample;
	uint32_t num_samples;
	uint16_t channels;
};

//see https://github.com/phoboslab/neuralink_brainwire/blob/master/bwenc.c
//the code author implemented this method to downsample the data by 6 bits, as for some reason ...
//it's not done by just a simple bit shift.

static inline int brainwire_quant(int v) {
	return static_cast<int>(floor(v / 64.0));
}

static inline int brainwire_dequant(int v) {
	if (v >= 0) {
		return round(v * 64.061577 + 31.034184);
	}
	else {
		return -round((-v - 1) * 64.061577 + 31.034184) - 1;
	}
}

static short* wav_read(const string& path, samples_t* desc) {

	ifstream file(path, ios::binary);
	if (!file.is_open()) {
		cerr << "Error opening file: " << path << endl;
		return nullptr;
	}
	
	// Wav File header (44 bytes)

	char header[44];
	file.read(header, 44);
	if (file.gcount() < 44) {
		cerr << "Error reading file header" << endl;
		return nullptr;
	}

	desc->sample_rate = *reinterpret_cast<uint32_t*>(&header[24]);
	desc->bits_per_sample = *reinterpret_cast<uint16_t*>(&header[34]);
	desc->channels = *reinterpret_cast<uint16_t*>(&header[22]);

	uint32_t data_size = *reinterpret_cast<uint32_t*>(&header[40]);
	desc->num_samples = data_size / (desc->bits_per_sample / 8 * desc->channels);


	// see https://docs.fileformat.com/audio/wav/


	short* samples = new short[desc->num_samples];

	//read audio data
	file.read(reinterpret_cast<char*>(samples), desc->num_samples * sizeof(short));
	if (file.gcount() < desc->num_samples * sizeof(short)) {
		cerr << "Error reading WAV data" << endl;
		delete[] samples;
		return nullptr;
	}

	for (uint32_t i = 0; i < desc->num_samples; ++i) {

		//CHECK QUANTIZE LOGIC (can be removed just to check)
		if (brainwire_dequant(brainwire_quant(samples[i])) != samples[i]) {
			cout << "QUANTIZE ERROR" << endl;
		}

		samples[i] = brainwire_quant(samples[i]);
	}
	file.close();
	return samples;
}

int main() {

	string directory = "../Neuralink/data_neuralink/";
	unordered_map<int, uint32_t> symbol_counts;
	uint32_t total_samples = 0;

	for (const auto& entry : fs::directory_iterator(directory)) {
		if (entry.is_regular_file()) {
			string path = entry.path().string();
			cout << "Processing File: " << path << endl;

			samples_t desc;
			short* samples = wav_read(path, &desc);

			if (samples == nullptr) {
				cout << "Failed to read .wav file ..." << endl;
				continue;
			}

			cout << "\nMetadata from WAV: \n";
			cout << "Sample Rate: " << desc.sample_rate << " Hz" << endl;
			cout << "Channels: " << desc.channels << endl;
			cout << "Bits per sample: " << desc.bits_per_sample << " bits" << endl;
			cout << "Number of samples:" << desc.num_samples << endl;

			for (uint32_t i = 0; i < desc.num_samples; ++i) {
				symbol_counts[samples[i]]++;
			}
			total_samples += desc.num_samples;
			delete[] samples;
		}
	}

	double prob_sum = 0.0;

	cout << "\nSymbol Probabilities:\n";
	for (const auto& pair : symbol_counts) {
		double probability = static_cast<double>(pair.second) / total_samples;
		prob_sum += probability;
		cout << "Symbol: " << setw(4) << pair.first << " | Count: " << setw(6) << pair.second << " | Probability: " << fixed << setprecision(6) << probability << endl;
	}
	cout << "\nSum of all probabilities: " << prob_sum << endl;
	return 0;
}
