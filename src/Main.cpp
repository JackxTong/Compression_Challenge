// Main.cpp

#include "findProbabilities.h"
#include "huffmanCode.h"
#include <iostream>

using namespace std;

int main() {

	cout << "Write to .txt? (y/n) ? ";
	char choice;
	cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int result = outputtxt();
        if (result == 0) {
            cout << "Probabilities have been written to the file." << endl;
        }
        else {
            cout << "Failed to write probabilities." << endl;
        }
    }

    int run = Huffman();

	return 0;
}