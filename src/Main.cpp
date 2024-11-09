// Main.cpp

#include "findProbabilities.h"
//#include "huffmanCoding.h"
#include <iostream>

using namespace std;

int main() {

	cout << "Run probability calculation? (y/n) ? ";
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

	return 0;
}