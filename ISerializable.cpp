//
// Created by Tomasz on 11/04/2025.
//

#include "ISerializable.h"

#include <format>
#include <iostream>
#include "storeClassList.h"

template<typename T, typename F>
void ISerializable::registerField(const std::string &name, const T &value, F converter) {
    std::cout << "Registered field: " << name << std::endl;
    fields.push_back([&value, converter, name]() {
        return std::format(R"("{}": "{}")", name, converter(value));
    });
}

std::string ISerializable::serialize() const {
    std::string result;
    for (const auto &field: this->fields) {
        result += field();
        return result;
    }
    return "";
}

// filepath: ISerializable.cpp
