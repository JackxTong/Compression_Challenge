#include "../include/wavReader.h"

//see https://github.com/phoboslab/neuralink_brainwire/blob/master/bwenc.c
//the code author implemented this method to downsample the data by 6 bits, as for some reason ...
//it's not done by just a simple bit shift.

static inline int brainwireQuant(int v) {
    return static_cast<int>(std::floor(v/64.0));
};

static inline int brainwireDequant(int v) {
    if (v >= 0) {
        return std::round(v * 64.061577 + 31.034184);
    }
    else {
        return -(std::round((-v - 1) * 64.061577 + 31.034184) - 1);
    }
};

void wavReader::getData(const std::string& path, wav_header* header) {

    // CLEAR PREVIOUS DATA
    this->quantisedData.clear();

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
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

    const size_t num_samples = header->subChunk2Size / (header->bits_per_sample / 8);

    int16_t* samples = new int16_t[num_samples];

    //read audio data
    file.read(reinterpret_cast<char*>(samples), header->subChunk2Size);

    if (file.gcount() != header->subChunk2Size) {
        std::cerr << "Error reading WAV data" << std::endl;
        delete[] samples;
    }

    for (size_t i = 0; i < num_samples; ++i) {

        int quant = brainwireQuant(samples[i]);
        this->quantisedData.push_back(quant);

        // Check quantize logic (gives error)
        // if (brainwireDequant(brainwireQuant(samples[i])) != samples[i]) {
        //     std::cout << "QUANTIZE ERROR - " << quant << " --> " << brainwireDequant(quant) << " != " << samples[i]  << "at i = " << i << std::endl;
        // }
    }

    this->totalSamples += num_samples;

    for (size_t i = 0; i < num_samples; ++i) {
        this->symbol_counts[this->quantisedData[i]]++;
    }

    file.close();
    delete[] samples;
 }

void wavReader::makeSortedProb() {

    for (const auto &pair : this->symbol_counts) {
        double probability = static_cast<double>(pair.second) / this->totalSamples;
        this->sortedProbs.emplace_back(pair.first, probability);
    }

    std::sort(this->sortedProbs.begin(), this->sortedProbs.end(),
         [](const std::pair<int, double> &a, const std::pair<int, double> &b)
         {
             return a.second > b.second;
        });

}

int wavReader::outputTxt() {

    double prob_sum = 0.0;

    // Write output to .txt
    std::ofstream output_file("single_symbol_probabilities.txt");
    
    if (!output_file.is_open())
    {
        std::cerr << "Cannot open output file ..." << std::endl;
        return 1;
    }
    
    output_file << "Symbol Probabilities:\n";
    std::cout << "\nSymbol Probabilities:\n";
    for (const auto& [first, second] : this->sortedProbs)
    {
        prob_sum += second;
        output_file << "Symbol: " << std::setw(4) << first << " | Probability: " << std::fixed << std::setprecision(20) << second << "\n";
        std::cout << "Symbol: " << std::setw(4) << first << " | Probability: " << std::fixed << std::setprecision(20) << second << "\n";
    }

    std::cout << "\nSum of all probabilities: " << prob_sum <<  " , num symbols = " << this->sortedProbs.size() << std::endl;
    
    return 0;
}

wavReader::wavReader() {

    const std::string directory = "./data/";
	std::cout << "READING FILES..." << std::endl;

    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {

			std::string path = entry.path().string();
			std::cout << "Processing File: " << path << std::endl;

			// wav_header header;

			wavReader::getData(path, &currHeader);
            //std::cout << "Data Length = " << this->quantisedData.size() << std::endl;

            // std::cout << "Num Channels: " << header.num_channels << std::endl;
            // std::cout << "Sample Rate: " << header.sample_rate << std::endl;
            // std::cout << "SubChunk1 Size: " << header.subChunk1Size << "bytes" << std::endl;
            // std::cout << "SubChunk2 Size: " << header.subChunk2Size << "bytes" << std::endl;
            // std::cout << "Audio Format" <<  header.audioFormat << std::endl;
            // std::cout << "Bits per sample: " << header.bits_per_sample << std::endl;

        }
    }

    wavReader::makeSortedProb();

    int success = wavReader::outputTxt();
  
}

wavReader::~wavReader() {

    std::cout << "READING TERMINATED" << std::endl;

}