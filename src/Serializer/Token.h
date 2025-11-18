//
// Created by tomasz on 11/16/2025.
//

#ifndef PROJEKT_TOKEN_H
#define PROJEKT_TOKEN_H
#include <memory>
#include <vector>

#include "storeClassList.h"


enum tokenType {
    UNKNOWN,

    STRING, // 1
    KEY, // 2
    DICT, // 3
    ARRAY, // 4
    ROOT_FILE // 5
};

class Token {
    public:
        std::string FileName;
        std::shared_ptr<Token> parent;
        std::vector<std::shared_ptr<Token> > children;
        tokenType type;
        std::vector<std::shared_ptr<charData> > content;
        Token();
        [[nodiscard]] std::string contentAsString() const;
        [[nodiscard]] std::string toString(int scope) const;
        std::shared_ptr<Token> value;
        std::shared_ptr<Token> key;
};



#endif //PROJEKT_TOKEN_H