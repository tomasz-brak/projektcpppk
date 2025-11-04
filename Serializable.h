//
// serializable.h
//

#ifndef PROJEKT_SERIALIZABLE_H
#define PROJEKT_SERIALIZABLE_H
#include <format>
#include <functional>
#include <string>
#include <iostream>
#include <memory>
#include <vector>

#include "ISerializable.h"


template<typename Derived>
class Serializable : public ISerializable {
public:
    ~Serializable() override = default;
    template <typename... Args>
    static std::shared_ptr<Derived> create(Args&&... args) {
        return std::make_shared<Derived>(std::forward<Args>(args)...);
    }
};

template <typename T>
T identity(T a) {
    return a;
}

template <typename T>
std::string defaultToString(T a) {
    return std::to_string(a);
}


#endif //PROJEKT_SERIALIZABLE_H

// filepath: Serializable.h
