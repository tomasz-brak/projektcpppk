//
// Created by tomasz on 11/11/2025.
//

#ifndef PROJEKT_UTIL_H
#define PROJEKT_UTIL_H
#include <string>
#include <unordered_map>
#include <vector>

std::vector<std::string> splitString(std::string s, const std::string &d);

typedef std::unordered_map<std::string, std::string> argMap;

argMap argParser(int argc, char *argv[]);

void createDirectories(const std::string &dir);

#endif // PROJEKT_UTIL_H
