//
// Created by Tomasz on 11/04/2025.
//

#ifndef PROJEKT_ISERIALIZABLE_H
#define PROJEKT_ISERIALIZABLE_H
#include <functional>
#include <memory>
#include <string>
#include <vector>


class ISerializable {
public:
    virtual ~ISerializable() = default;
    std::string serialize() const;

    template<class T, class F>
    void registerField(const std::string &name, const T &value, F converter);

    virtual std::string getPrimaryKey() = 0;

    std::vector<std::function<std::string()>> fields;
    std::string name;
};


#endif //PROJEKT_ISERIALIZABLE_H

// filepath: ISerializable.h
