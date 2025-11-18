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
    std::function<std::optional<std::any>(const std::string&)> from_str;
};

class ISerializable {
public:
    virtual ~ISerializable() = default;
    [[nodiscard]] std::string serialize() const;



    template<typename T, typename F, typename Derived>
    void registerField(const std::string &field_name, T &value, F converter) {
        std::cout << "Registering field: " << field_name << std::endl;

        auto conv_ptr = std::make_shared<F>(std::move(converter));
        ConverterEntry entry;
        T *ptr = std::addressof(value);

        entry.to_str = [conv_ptr, ptr]() -> std::optional<std::string> {
            return conv_ptr->to_string(*ptr);
        };

        Loader fieldLoader = [conv_ptr, ptr, this](std::shared_ptr<ISerializable> obj, const std::string &s) -> bool {
            auto derived_obj = std::dynamic_pointer_cast<Derived>(obj);
            if (!derived_obj) {
                return false;
            }
            auto opt = conv_ptr->from_string(s);
            if (!opt) return false;

            T* original_ptr = ptr;
            auto base_address = reinterpret_cast<char*>(this);
            auto field_offset = reinterpret_cast<char*>(original_ptr) - base_address;

            T* target_ptr = reinterpret_cast<T*>(reinterpret_cast<char*>(derived_obj.get()) + field_offset);
            *target_ptr = *opt;

            return true;
        };

        registerLoader(this->name, fieldLoader, field_name);

        converters[field_name] = std::move(entry);
    }

    virtual std::string getPrimaryKey() = 0;

    std::unordered_map<std::string, ConverterEntry> converters;

    std::string name;
};



#endif //PROJEKT_ISERIALIZABLE_H