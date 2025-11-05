//
// Created by Tomasz on 11/04/2025.
//

#ifndef PROJEKT_ISERIALIZABLE_H
#define PROJEKT_ISERIALIZABLE_H
#include <format>
#include <functional>
#include <iostream>
#include <string>
#include <vector>


class ISerializable {
public:
    virtual ~ISerializable() = default;
    [[nodiscard]] std::string serialize() const;

    template<typename T, typename F>
    void registerField(const std::string &name, const T &value, F converter) {
        std::cout << "Registered field: " << name << std::endl;
        fields.push_back([&value, converter, name]() {
            return std::format(R"("{}": "{}")", name, converter(value));
        });
    }

    virtual std::string getPrimaryKey() = 0;

    std::vector<std::function<std::string()>> fields;
    std::string name;
};


#endif //PROJEKT_ISERIALIZABLE_H

// filepath: ISerializable.h
