//
// Created by tomasz on 11/16/2025.
//

#include "Token.h"
#include <string>

Token::Token()
    : parent(nullptr)
      , type(tokenType::UNKNOWN) {
}

std::string Token::contentAsString() const {
    std::string out;
    for (const auto &ch: this->content) {
        out += ch->ch;
    }
    return out;
}

std::string Token::toString(const int scope) const {
    std::string out;
    out.append(scope*2, ' ');
    switch (this->type) {
        case tokenType::UNKNOWN:
            out += "<Token> UNKNOWN ";
            break;
        case tokenType::STRING:
            out +=  std::format("<Token> STRING");
            break;
        case tokenType::ARRAY:
            out += "<Token> ARRAY ";
            break;
        case tokenType::DICT:
            out += "TOKEN DICT ";
            break;
        case tokenType::KEY:
            out += "<Token> KEY ";
            out += " value: " + this->value->toString(scope);
            break;
    }
    // for (const auto &child: this->children) {
    //     out += std::format(" {}", this->contentAsString());
    //     out += '\n';
    //     out += child->toString(scope + 1);
    // }
    out += std::to_string(this->type) + ". " + contentAsString();
    if (this->parent != nullptr) {
        out += " parent: " + std::to_string(this->parent->type);
    }else {
        out += " parent: NULL";
    }
    out += "\n";
    for (const auto &child: this->children) {
        out += child->toString(scope+1);
    }
    return out;
}
