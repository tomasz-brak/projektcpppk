#ifndef PROJEKT_STORECLASSLIST_H
#define PROJEKT_STORECLASSLIST_H

#include <memory>
#include <stack>
#include <vector>
#include "Serializable.h"

struct textStatus {
    int string_since = -1;
    int dict_scope = 0;
    int array_scope = 0;
    std::optional<std::size_t> opened_by = std::nullopt;
    std::optional<std::size_t> closed_by = std::nullopt;
    std::string::const_iterator ch;
};

struct charData {
    char ch{};
    std::string::const_iterator ch_P;


    std::shared_ptr<textStatus> status;

    [[nodiscard]] bool isToken() const {
        static constexpr std::string_view t = "{}[]\"";
        return t.find(ch) != std::string_view::npos and status->string_since > 0;
    }
};

bool storeClass(const std::vector<std::shared_ptr<ISerializable>>&, const std::string &dataDir);
bool readClass (const char* name, std::vector<std::shared_ptr<ISerializable>> &data, const std::string& dataDir);

std::optional<std::size_t> ParseToken(std::string::const_iterator ch,
                                      const std::shared_ptr<textStatus> &status,
                                      std::string::const_iterator stringBegin, std::stack<std::size_t> &tokenStack);
std::string removeWhitespacePreserveStrings(const std::string &s);
#endif

// filepath: storeClassList.h
