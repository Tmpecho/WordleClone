//
// Created by Johannes Aamot Skeidsvoll on 16/08/2023.
//

#include "WordFileReader.h"

std::vector<std::string> getWordData(const std::string& filename) {
    std::vector<std::string> words;

    std::ifstream file(filename);
    checkIfFileIsOpen(filename, file);

    std::string word;
    while (file >> word) {
        words.push_back(word);
    }
    file.close();

    return reinterpret_cast<const std::vector<std::string> &>(words);
}

void checkIfFileIsOpen(const std::string &filename, const std::ifstream &file) {
    if (!file.is_open()) {
        std::cerr << "Unable to open file: " << filename << std::endl;
    }
}
