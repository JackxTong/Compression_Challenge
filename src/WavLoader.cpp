#include <iostream>
#include <fstream>
#include <cstdint>
#include <direct.h>
#include <string>
#include <unordered_map>
#include <iomanip>

using namespace std;

struct samples_t {
	uint32_t sample_rate;
	uint16_t bits_per_sample;
	uint32_t num_samples;
	uint16_t channels;
};

//see https://github.com/phoboslab/neuralink_brainwire/blob/master/bwenc.c

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

short* wav_read(const string& path, samples_t* desc) {

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

		//CHECK QUANTIZE LOGIC
		if (brainwire_dequant(brainwire_quant(samples[i])) != samples[i]) {
			cout << "QUANTIZE ERROR" << endl;
		}

		samples[i] = brainwire_quant(samples[i]);
	}
	file.close();
	return samples;
}

int main() {
	
	string path = "../Neuralink/data_neuralink//00d4f842-fc92-45f5-8cae-3effdc2245f5.wav";

	{
		char cwd[512];
		if (_getcwd(cwd, sizeof(cwd)) != nullptr) {
			std::cout << "Current working directory: " << cwd << std::endl;
		}
		else {
			std::cerr << "Error getting current working directory" << std::endl;
		}

		char buffer[512];
		_fullpath(buffer, path.c_str(), 512);
		std::cout << "Trying to open: " << buffer << std::endl;
	}
	
	samples_t desc;

	short* samples = wav_read(path, &desc);

	if (samples == nullptr) {
		cerr << "Failed to read .wav file!" << endl;
		return 1;
	}

	cout << "\nMetadata from WAV: \n";
	cout << "Sample Rate: " << desc.sample_rate << " Hz" << endl;
	cout << "Channels: " << desc.channels << endl;
	cout << "Bits per sample: " << desc.bits_per_sample << " bits" << endl;
	cout << "Number of samples:" << desc.num_samples << endl;

	unordered_map<int, uint32_t> symbol_counts;

	for (uint32_t i = 0; i < desc.num_samples; ++i) {
		symbol_counts[samples[i]]++;
	}

	//double prob_sum = 0.0;

	//calculate probabilities from count

	cout << "\nSymbol Probabilities:\n";
	for (const auto& pair : symbol_counts) {
		double probability = static_cast<double>(pair.second) / desc.num_samples;
		cout << "Symbol: " << setw(4) << pair.first << " | Count: " << setw(6) << pair.second << " | Probability: " << fixed << setprecision(6) << probability << endl;

		//prob_sum += probability;

	}

	//cout << "Sum of Probabilities: " << prob_sum << endl;

	std::cout << "\nFirst few sample values:\n";
	for (int i = 0; i < 10 && i < desc.num_samples; ++i) {
		std::cout << "Sample " << i << ": " << samples[i] << std::endl;
	}

	delete[] samples;

	return 0;
}
