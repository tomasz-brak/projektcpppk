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

class ISerializable;

using FieldName = std::string;
using Loader = std::function<bool(std::shared_ptr<ISerializable>, const std::string&)>;
using Factory = std::function<std::shared_ptr<ISerializable>()>;
using LoaderRegistry = std::unordered_map<std::string, std::unordered_map<FieldName, Loader>>;
using FactoryRegistry = std::unordered_map<std::string, Factory>;

inline LoaderRegistry &loaderRegistry() {
    static LoaderRegistry registry;
    return registry;
}

inline FactoryRegistry &factoryRegistry() {
    static FactoryRegistry registry;
    return registry;
}

inline void registerLoader(const std::string &name, Loader loader, const std::string& field_name) {
    loaderRegistry()[name][field_name] = std::move(loader);
}

inline void registerFactory(const std::string &name, Factory factory) {
    factoryRegistry()[name] = std::move(factory);
}


struct ConverterEntry {
    std::function<std::optional<std::string>()> to_str;
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    [[nodiscard]] std::string serialize() const;

    virtual std::string getPrimaryKey() = 0;

    std::unordered_map<std::string, ConverterEntry> converters;

    std::string name;
};



#endif //PROJEKT_ISERIALIZABLE_H