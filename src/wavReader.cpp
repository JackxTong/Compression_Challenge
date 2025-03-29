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


void wavReader::makeSortedProb() {

    size_t divisor = this->globalStats.total_count;

    for (const auto &pair : this->globalStats.ngram_counts) {
        double probability = static_cast<double>(pair.second) / divisor;
        this->sortedProbs.emplace_back(pair.first, probability);
    }

    std::sort(this->sortedProbs.begin(), this->sortedProbs.end(),
         [](const std::pair<std::vector<int16_t>, double> &a, const std::pair<std::vector<int16_t>, double> &b)
         {
             return a.second > b.second;
        });

}

void wavReader::processFile(const std::string& path, int ngram_size, FileStats& stats) {

    wav_header header;  

    std::ifstream file(path, std::ios::binary);

    if (!file.is_open()) {
        std::cerr << "Error opening file: " << path << std::endl;
    }

    // see http://soundfile.sapp.org/doc/WaveFormat/
    
    file.read(reinterpret_cast<char*>(&header.chunkID),4);
    file.read(reinterpret_cast<char*>(&header.chunkSize),4);
    file.read(reinterpret_cast<char*>(&header.format),4);
    file.read(reinterpret_cast<char*>(&header.subChunk1ID),4);
    file.read(reinterpret_cast<char*>(&header.subChunk1Size),4);
    file.read(reinterpret_cast<char*>(&header.audioFormat),2);
    file.read(reinterpret_cast<char*>(&header.num_channels),2);
    file.read(reinterpret_cast<char*>(&header.sample_rate),4);
    file.read(reinterpret_cast<char*>(&header.byte_rate),4);
    file.read(reinterpret_cast<char*>(&header.block_align),2);
    file.read(reinterpret_cast<char*>(&header.bits_per_sample),2);
    file.read(reinterpret_cast<char*>(&header.subChunk2ID),4);
    file.read(reinterpret_cast<char*>(&header.subChunk2Size),4);

    const size_t num_samples = header.subChunk2Size / (header.bits_per_sample / 8);

    int16_t* samples = new int16_t[num_samples];
    //int thread_num = omp_get_thread_num();

    //read audio data
    file.read(reinterpret_cast<char*>(samples), header.subChunk2Size);

    if (file.gcount() != header.subChunk2Size) {
        std::cerr << "Error reading WAV data" << std::endl;
        delete[] samples;
        return;
    }

    // Quantise Data
    for (size_t i = 0; i < num_samples; ++i) {
        samples[i] = brainwireQuant(samples[i]);
    }

    for (size_t i = 0; i + ngram_size <= num_samples; ++i) {
        std::vector<int16_t> ngram(ngram_size);
        for (int j = 0; j < ngram_size; ++j) {
            ngram[j] = samples[i + j];
        }
        stats.ngram_counts[ngram]++;
        stats.total_count++;
    }

    file.close();
    delete[] samples;
    return;
}


int wavReader::outputTxt() {

    double prob_sum = 0.0;

    // Write output to .txt
    std::ofstream output_file("ngram_probabilities.txt");
    
    if (!output_file.is_open())
    {
        std::cerr << "Cannot open output file ..." << std::endl;
        return 1;
    }
    
    output_file << "Symbol Probabilities:\n";
    //std::cout << "\nSymbol Probabilities:\n";
    for (const auto& [first, second] : this->sortedProbs)
    {   
        prob_sum += second;
        output_file << "Symbol: ["; 
        //std::cout << "Symbol: [";
        for (size_t i = 0; i < first.size(); ++i) {
            output_file << first[i];
            //std::cout << first[i];
            if (i != first.size() - 1) {
                output_file << ", ";
                //std::cout << ", ";
            }
        }
        output_file << "] | Probability: " << std::fixed << std::setprecision(20) << second << "\n";
        //std::cout << "] | Probability: " << std::fixed << std::setprecision(20) << second << "\n";
    }

    std::cout << "\nSum of all probabilities: " << prob_sum <<  " , num symbols = " << this->sortedProbs.size() << std::endl;
    
    return 0;
}

wavReader::wavReader(int NGRAM) : ngram_size(NGRAM) {

    const std::string directory = "./data/";    
    std::vector<std::string> file_paths;

    // POPULATE file_paths
    for (const auto& entry : std::filesystem::directory_iterator(directory)) {
		if (entry.is_regular_file()) {

			std::string path = entry.path().string();
            file_paths.push_back(path);
        }
    }
    
    int num_files = file_paths.size();

    std::vector<FileStats> allStats(num_files);

    std::cout << "READING " << num_files << " FILES ..." << std::endl;

    {
    #pragma omp parallel for 
    for (int i = 0; i < num_files;  ++i) {
        wavReader::processFile(file_paths[i], this->ngram_size, allStats[i]);
    }

    for (const auto& local : allStats) {
        for (const auto& [ngram, count] : local.ngram_counts) {
            this->globalStats.ngram_counts[ngram] += count;
        }
        this->globalStats.total_count += local.total_count;
    }
    }

    std::cout << "TOTAL COUNT FOUND: " << this->globalStats.total_count << std::endl;
    wavReader::makeSortedProb();

    int success = wavReader::outputTxt();

}

wavReader::~wavReader() {

    std::cout << "READING TERMINATED" << std::endl;

}