//
// serializable.h
//

#ifndef PROJEKT_SERIALIZABLE_H
#define PROJEKT_SERIALIZABLE_H
#include <format>
#include <functional>
#include <string>
#include <memory>
#include <sstream>
#include <optional>

#include "ISerializable.h"


template<typename Derived>
class Serializable : public ISerializable {
public:
    ~Serializable() override = default;

    template<typename... Args>
    static std::shared_ptr<Derived> create(Args &&... args) {
        return std::make_shared<Derived>(std::forward<Args>(args)...);
    }
};

template<typename T>
concept StreamExtractable = requires(std::istringstream &is, T &t)
{
    is >> t;
};



template<typename T>
struct defaultString {
    std::optional<std::string> to_string(const T &v) const {
        if constexpr (std::is_same_v<T, std::string>) {
            return v;
        }
        if constexpr (std::is_arithmetic_v<T>) {
            return std::to_string(v);
        }
        std::stringstream ss;
        ss << v;
        return  ss.str();
    }

    std::optional<T> from_string(const std::string &s) const requires StreamExtractable<T> {
        if constexpr (std::is_same_v<T, std::string>) {
            return s;
        }
        std::istringstream iss(s);
        if (T val; !(iss >> val)) {
            return std::nullopt;
        } else {
            return val;
        }
    }
};

#endif //PROJEKT_SERIALIZABLE_H

// filepath: Serializable.h
