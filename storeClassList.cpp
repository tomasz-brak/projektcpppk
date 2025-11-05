//
// Created by Tomasz on 11/04/2025.
//

#include "serializable.h"
#include <fstream>
#include <iostream>
#include <memory>

constexpr char SEPARATOR_LINE = '=';

std::string getSourceDir() {
    // TODO: Change to the script path
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
    }
    std::cout << "Nieudalo sie otworzyc pliku !!!" << std::endl;
    return false;
}

std::string readFile(const std::string& filename) {
    const std::string filePath = getSourceDir() + filename;
    std::ifstream file(filePath);
        std::string result;
        std::string t;
        while (std::getline (file, t)) {
            result += t + "\n";
        }
        return result;
}

std::vector<std::string> split(const std::string& a, char b) {
    std::vector<std::string> result;
    std::string current;

    for (char ch : a) {
        if (ch == b) {
            if (!current.empty()) {
                result.push_back(current);
                current.clear();
            }
        } else {
            current += ch;
        }
    }

    if (!current.empty()) {
        result.push_back(current);
    }
    return result;
}

void separateParenthesis(std::string& data) {
    std::string result;
    for (auto it = data.begin(); it != data.end(); ++it) {
        result += *it;
        if ((result == "[" or result == "]" or result == "{" or result == "}") and *(it + 1) != '\n') {
            result += "\n";
        }
    }
    data = result;
}

void format(const std::string& filename) {
    auto file = readFile(filename + ".json");
    std::string result;
    int scope = 0;
    separateParenthesis(file);
    for (auto line : split(file, '\n')) {
        scope -= std::count_if(line.begin(), line.end(), [](const char ch) {
    return ch == ']' || ch == '}';
});
        if (scope < 0) {
            std::cout << "Format Error! not saving!\n";
            return;
        }
        result +=  std::string(4*scope, ' ') + line + "\n";

        scope += std::count_if(line.begin(), line.end(), [](const char ch) {
    return ch == '[' || ch == '{';
});



    }

    writeStr(filename + ".json", result);
}



bool storeClass(const std::vector<std::shared_ptr<ISerializable>>& data) {
    std::string result = "[";
    std::string filename;

    for (auto it = data.begin(); it != data.end(); ++it) {
        filename = (*it)->name;
        result += (*it)->serialize();
        if (it != data.end() -1 ) {
            result += ",\n";
            continue;
        }
        result += "\n";
    }

    result += "]";
    const auto succes = writeStr(filename + ".json", result);
    if (succes) {
        format(filename);
    }
    return succes;
}



// filepath: storeClassList.cpp
