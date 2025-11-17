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

    STRING,
    DICT,
    ARRAY,
};

class Token {
    public:
        std::shared_ptr<Token> parent;
        std::vector<std::shared_ptr<Token> > children;
        tokenType type;
        std::vector<std::shared_ptr<charData> > content;
        std::vector<std::shared_ptr<charData> > key;
        Token();
        [[nodiscard]] std::string contentAsString() const;
        [[nodiscard]] std::string toString(int scope) const;
};



#endif //PROJEKT_TOKEN_H