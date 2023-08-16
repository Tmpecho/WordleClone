//
// Created by Johannes Aamot Skeidsvoll on 16/08/2023.
//
#include "TextFormatter.h"

std::string TextFormatter::formatGreenText(const std::string &str) {
    return "\x1b[32m" + str + TextFormatter::resetColor;
}

std::string TextFormatter::formatYellowText(const std::string &str) {
    return "\x1b[33m" + str + TextFormatter::resetColor;
}

const std::string TextFormatter::resetColor = "\x1b[0m";  // Define and initialize the static member variable
