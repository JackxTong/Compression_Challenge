#ifndef FINDPROBABILITIES_H
#define FINDPROBABILITIES_H

#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include <cmath>
#include <unordered_map>
#include <filesystem>
#include <vector>
#include <utility> // For std::pair
#include <memory>  // For std::unique_ptr

// Function declarations
std::unique_ptr<std::vector<std::pair<int, double>>> returnSortedProb();
int outputTxt();

#endif // FINDPROBABILITIES_H