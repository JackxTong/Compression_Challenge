// Main.cpp

//#include "huffmanCode.h"
#include "findProbabilities.h"
#include "wavReader.h"
#include "arithmeticCode.h"

using namespace std;

int main () {

    cout << "Write probabilities to .txt? (y/n) ";
    char choice;
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {

        int result = outputTxt();
        
        if (result == 0) {
            cout << "Probabilites written to .txt!" << endl;
        }
        else {
            cout << "Failed to write probabilities..." << endl;
        }
    }

    unique_ptr<vector<pair<int, double>>> sortedProbs = returnSortedProb();

    auto code = arithmeticCode(move(sortedProbs));

    vector<int> testInts = {11, 7, 23, 19, 18, 27, 3, 26, 2, -6};

    int streamSize = (testInts).size();

    cout << "Encoding stream of ints: {";
    for (int i = 0; i < streamSize; ++i) {
        cout << testInts[i] << " ";
    }
    cout << "} " << endl;

    uint64_t test = code.encode(testInts);

    cout << "Encoded Form: " << test << endl;

    vector<int> decodedInts = code.decode(test, streamSize);

    cout << "Decoded stream of ints: {";
    for (int i = 0; i < decodedInts.size(); ++i) {
        cout << decodedInts[i] << " ";
    }
    cout << "} " << endl;

    float compression_ratio = 10.0*testInts.size() / 64.0;

    cout << "In reality, Compression Ratio: " << compression_ratio  << endl;

    return 0;
}   