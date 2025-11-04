//
// Created by Tomasz on 11/04/2025.
//

#include "serializable.h"
#include <fstream>
#include <memory>

constexpr char SEPARATOR_LINE = '=';

std::string getSourceDir() {
    const std::string path = __FILE__;
    const auto pos = path.find_last_of("/\\");
    return path.substr(0, pos + 1);
}

bool writeStr(const std::string &filename, const std::string &str) {
    const std::string filePath = getSourceDir() + filename;
    std::cout << "Opening: " << filePath << std::endl;
    if (std::ofstream outFile(filePath); outFile.is_open()) {
        outFile << str;
        outFile.close();
        std::cout << "Dane dla "<< filename << " zostaly zapisane pomyslnie !!!" << std::endl;
        return true;
    } else {
        std::cout << "Nieudalo sie otworzyc pliku !!!" << std::endl;
        return false;
    }
}

// filepath: storeClassList.cpp
