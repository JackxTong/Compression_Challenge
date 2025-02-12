// Main.cpp

//#include "huffmanCode.h"
#include "findProbabilities.h"
#include "wavReader.h"
#include "arithmeticCode.h"

using namespace std;

int main () {

    cout << "Write probabilities to .txt? (y/n)";
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

    uint64_t test = code.encode(testInts);

    cout << "Encoded: " << test << endl;

    vector<int> decodedInts = code.decode(test, streamSize);

    return 0;
}   