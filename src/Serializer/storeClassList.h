#ifndef PROJEKT_STORECLASSLIST_H
#define PROJEKT_STORECLASSLIST_H

#include <memory>
#include <stack>
#include <vector>
#include "Serializable.h"

struct textStatus {
    int string_since = -1;

    std::optional<std::size_t> closed_by = std::nullopt;
    std::string::const_iterator ch;
};

struct charData {
    char ch{};

    std::shared_ptr<textStatus> status;
};

bool storeClass(const std::vector<std::shared_ptr<ISerializable>>&, const std::string &dataDir);
bool readClass (const char* name, std::vector<std::shared_ptr<ISerializable>> &data, const std::string& dataDir);

std::optional<std::size_t> ParseToken(std::string::const_iterator ch,
                                      const std::shared_ptr<textStatus> &status,
                                      std::string::const_iterator stringBegin, std::stack<std::size_t> &tokenStack);
std::string removeWhitespacePreserveStrings(const std::string &s);
#endif

// filepath: storeClassList.h
