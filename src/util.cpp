//
// Created by tomasz on 11/11/2025.
//

#include "util.h"

#include <filesystem>
#include <iostream>

std::vector<std::string> splitString(std::string s, const std::string &d) {
    std::vector<std::string> v;
    while (s.find(d) != std::string::npos) {
        const int dpos = static_cast<int>(s.find(d));
        v.push_back(s.substr(0,  dpos));
        s.erase(0, dpos+1);
    }
    v.push_back(s);
    return v;
}

argMap argParser(const int argc, char *argv[]) {
    argMap args;
    for (int i = 1; i < argc; i++) {
        if (std::string arg = argv[i]; arg.find('=') != std::string::npos) {
            auto bits = splitString(arg, "=");
            if (bits.size() != 2) {
                std::cout << "Failed parsing argument: "<< arg << ", = number missmatch!\n" ;
                continue;
            }
            args[bits[0]] = bits[1];
        }
    }
    return args;
}

void createDirectories(const std::string& dir) {
    if (std::error_code ec; !std::filesystem::exists(dir, ec)) {
        if (const bool created = std::filesystem::create_directories(dir, ec); !created || ec) {
            std::cerr << "Failed to create directories for '" << dir << "': " << ec.message() << '\n';
            return;
        } else {
            std::cout << "Created directories: " << dir << '\n';
        }    }
};
