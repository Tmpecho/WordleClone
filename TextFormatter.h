//
// Created by Johannes Aamot Skeidsvoll on 16/08/2023.
//

#ifndef WORDLECLONE_TEXTFORMATTER_H
#define WORDLECLONE_TEXTFORMATTER_H

#include <iostream>

class TextFormatter {
private:
    static const std::string resetColor;

public:
    static std::string formatGreenText(const std::string &str);
    static std::string formatYellowText(const std::string &str);
};

#endif //WORDLECLONE_TEXTFORMATTER_H
