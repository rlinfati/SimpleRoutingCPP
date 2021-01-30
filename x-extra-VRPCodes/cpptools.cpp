#include <iostream>
#include <vector>
#include "cpptools.hpp"

void printarg(int argc, char **argv) {
    std::cout << "argc: " << argc << std::endl;
    for (int i = 0; i < argc; ++i) {
        std::cout << "argv[" << i << "]: " << argv[i] << std::endl;
    }
}

template<typename T> void printvec(std::vector <T> v) {
    for (size_t i = 0; i < v.size(); ++i) {
        std::cout << " " << v[i];
    }
    std::cout << std::endl;
}

template<typename T> void printmat(std::vector <std::vector <T> > m) {
    for (size_t i = 0; i < m.size(); ++i) {
        printvec(m[i]);
    }
}
