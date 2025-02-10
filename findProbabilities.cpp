// findProbabilities.cpp
// Converts Neuralink .wav file (see https://content.neuralink.com/compression-challenge/README.html) ..
// into symbol probabilities outputted into single_symbol_probabilities.txt.
// or returns ptr to vector of pairs for sorted probabilities.

#include "findProbabilities.h"
#include "wavReader.h"

using namespace std;

unique_ptr<vector<pair<int, double>>> returnSortedProb()
{
    string directory = "./data/";
    unordered_map<int, uint32_t> symbol_counts;
    uint32_t total_samples = 0;

    for (const auto &entry : std::filesystem::directory_iterator(directory))
    {
        if (entry.is_regular_file())
        {
            string path = entry.path().string();
            cout << "Processing File: " << path << endl;

            wav_header header;
            short *samples = file_read(path, &header);

            if (samples == nullptr)
            {
                cout << "Failed to read .wav file ..." << endl;
                continue;
            }

            int num_samples = header.subChunk2Size / (header.bits_per_sample / 8);

            for (int i = 0; i < num_samples; ++i)
            {
                symbol_counts[samples[i]]++;
            }
            total_samples += num_samples;
            delete[] samples;
        }
    }

    auto sorted_probabilities = std::make_unique<std::vector<std::pair<int, double>>>();
    for (const auto &pair : symbol_counts)
    {
        double probability = static_cast<double>(pair.second) / total_samples;
        sorted_probabilities->emplace_back(pair.first, probability);
    }

    sort(sorted_probabilities->begin(), sorted_probabilities->end(),
         [](const pair<int, double> &a, const pair<int, double> &b)
         {
             return a.second > b.second;
         });

    return sorted_probabilities;
}


int outputTxt()  {

    auto probs = returnSortedProb();
    double prob_sum = 0.0;

    // Write output to .txt
    ofstream output_file("single_symbol_probabilities.txt");
    
    if (!output_file.is_open())
    {
        cerr << "Cannot open output file ..." << endl;
        return 1;
    }
    
    output_file << "Symbol Probabilities:\n";
    cout << "\nSymbol Probabilities:\n";
    for (const auto& [first, second] : *probs)
    {
        prob_sum += second;
        output_file << "Symbol: " << setw(4) << first << " | Probability: " << fixed << setprecision(20) << second << "\n";
        cout << "Symbol: " << setw(4) << first << " | Probability: " << fixed << setprecision(20) << second << "\n";
    }

    cout << "\nSum of all probabilities: " << prob_sum << endl;
    
    return 0;
}