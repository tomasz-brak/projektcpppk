//
// Created by Tomasz on 11/04/2025.
//

#ifndef PROJEKT_ISERIALIZABLE_H
#define PROJEKT_ISERIALIZABLE_H

#include <any>
#include <format>
#include <functional>
#include <iostream>
#include <memory>
#include <string>
#include <vector>
#include <optional>

struct ConverterEntry {
    std::function<std::optional<std::string>()> to_str;
    std::function<std::optional<std::any>(const std::string&)> from_str;
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    [[nodiscard]] std::string serialize() const;

    template<typename T, typename F>
    void registerField(const std::string &field_name, T &value, F converter) {
        std::cout << "Registering field: " << field_name << std::endl;

        auto conv_ptr = std::make_shared<F>(std::move(converter));
        ConverterEntry entry;
        T *ptr = std::addressof(value);

        entry.to_str = [conv_ptr, ptr]() -> std::optional<std::string> {
            return conv_ptr->to_string(*ptr);
        };

        entry.from_str = [conv_ptr, ptr](const std::string &s) -> std::optional<std::any> {
            auto opt = conv_ptr->from_string(s);
            if (!opt) return std::nullopt;
            *ptr = *opt;
            return std::any{*opt};
        };

        converters[field_name] = std::move(entry);
    }

    virtual std::string getPrimaryKey() = 0;

    std::unordered_map<std::string, ConverterEntry> converters;

    std::string name;
};


#endif //PROJEKT_ISERIALIZABLE_H

// filepath: ISerializable.h
