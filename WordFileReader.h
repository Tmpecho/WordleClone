//
// Created by Johannes Aamot Skeidsvoll on 16/08/2023.
//

#ifndef WORDLECLONE_WORDFILEREADER_H
#define WORDLECLONE_WORDFILEREADER_H

#include <iostream>
#include <string>
#include <vector>
#include <fstream>

std::vector<std::string> getWordData(const std::string& filename);
void checkIfFileIsOpen(const std::string &filename, const std::ifstream &file);

#endif //WORDLECLONE_WORDFILEREADER_H
