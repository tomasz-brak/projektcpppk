//
// serializable.cpp
//

#include "serializable.h"

#include <iostream>


std::vector<std::string> serializable::serialize() const {
    std::vector<std::string> result;
    for (const auto &field: this->fields) {
        result.push_back(field());
    }
    return result;
}
