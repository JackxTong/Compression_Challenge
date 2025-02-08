// Main.cpp

//#include "huffmanCode.h"
#include "findProbabilities.h"
#include "wavReader.h"

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
    return 0;
}   