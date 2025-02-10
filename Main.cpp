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

    vector<int> testInts = {23, 11, 27, 19, 23, 3, 10, 14, 6};

    uint64_t test = code.encode(testInts);

    return 0;
}   