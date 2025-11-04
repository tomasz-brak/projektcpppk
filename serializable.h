//
// serializable.h
//

#ifndef PROJEKT_SERIALIZABLE_H
#define PROJEKT_SERIALIZABLE_H
#include <functional>
#include <string>
#include <iostream>
#include <vector>



class serializable {
public:
    virtual ~serializable() = default;


    template <typename T, typename  F>
    void registerField(const std::string& name, const T& value, F converter) {
        std::cout << "registerField(" << name << ")" << std::endl;
        fields.push_back([&value, converter, name]() {
            return name + ":" + converter(value);
        });
    }

    std::vector<std::string> serialize() const;

private:
    std::vector<std::function<std::string()>> fields;
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