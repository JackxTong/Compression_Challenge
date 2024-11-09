// calculates Huffman Code from single_symbol_probabilities .txt 

#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>
#include <map>
#include <sstream>
#include <vector>
#include <algorithm>

using namespace std;

unordered_map<int, double> readProbabilities(const string& filename) {
	unordered_map<int, double> probabilities;
	ifstream file(filename);
	string line;

	if (!file.is_open()) {
		cerr << "Error opening the file ..." << endl;
		return probabilities;
	}

	while (getline(file, line)) {
		if (line.find("Symbol:") == 0) {
			break;
		}
	}

	while (getline(file, line)) {
		stringstream ss(line);
		string temp;
		int symbol;
		double probability;

		ss >> temp >> symbol >> temp >> temp >> probability;
		probabilities[symbol] = probability;
	}

	file.close();
	return probabilities;
}

int Huffman() {
	string filename = "../Neuralink/single_symbol_probabilities.txt";

	unordered_map<int, double> probabilities = readProbabilities(filename);

	cout << "CODE IS RUNNING" << endl;
	for (const auto& pair : probabilities) {
		cout << "Symbol: " << pair.first << " | Probability: " << pair.second << endl;
	}

	return 0;
}