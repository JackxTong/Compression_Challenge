#ifndef ENCODEDICT_H
#define ENCODEDICT_H

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

#endif // ENCODEDICT_H