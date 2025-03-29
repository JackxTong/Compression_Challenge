// Main.cpp

#include <iostream>
#include <boost/program_options.hpp>

#include "../include/wavReader.h"

namespace po = boost::program_options;

int main (int argc, char** argv) {

    po::options_description opts("Allowed Options");
    opts.add_options()
        ("help,h", "Print help message")
        ("ngram,n", po::value<int>()->default_value(1), "Set n-gram size");

    po::variables_map vm;
    po::store(po::parse_command_line(argc, argv, opts), vm);
    po::notify(vm);

    if (vm.count("help")) {
        std::cout << opts << std::endl;
        return 0;
    }

    int n = vm["ngram"].as<int>();
    
    wavReader dataReader = wavReader(n);

    return 0;
}   