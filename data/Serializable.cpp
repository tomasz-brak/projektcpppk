//
// Created by tomasz on 10/21/2025.
//

#include "Serializable.h"

#include <format>
#include <fstream>
#include <ranges>

bool fileExists(const std::string& path) {
    const std::ifstream file(path);
    return file.good();
}

std::vector<std::string> split(const std::string& s, const char* delimiter) {
    std::vector<std::string> tokens;
    std::string currentToken;
    for (char a: s) {
        if (currentToken == delimiter && !currentToken.empty()) {
            tokens.push_back(currentToken);
            continue;
        }
        currentToken += a;
    }
}

void Serializable::write() {
    auto data = this->serialize();
    bool existedBefore = fileExists(storageLocation());
    std::ofstream out(storageLocation());
    if (!existedBefore) {
        for (const auto &field_name: data | std::views::keys) {
            out << field_name << separator;
        }
        out << std::endl;
    }
    for (const auto &dataPoint: data | std::views::values) {
        out << dataPoint << separator;
    }
    out << std::endl;
    out.close();

}



void Serializable::read() {
    std::unordered_map<field_name, data> data;
    if (!fileExists(storageLocation())) {
        throw std::runtime_error(std::format("File {} does not exist!", storageLocation()));
    }
    std::ifstream in(storageLocation());
    auto first = true;

    for (std::string line; std::getline(in, line);) {
        if (first) {
            first = false;
            std::vector<field_name> fieldNames = split(line, &this->separator);
            continue;
        }

    }
}
