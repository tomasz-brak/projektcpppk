//
// Created by tomasz on 10/21/2025.
//

#ifndef PROJEKT_SERIALIZABLE_H
#define PROJEKT_SERIALIZABLE_H
#include <string>
#include <unordered_map>
#include <vector>

typedef std::string data;
typedef std::string field_name;

class Serializable {
public:
    virtual ~Serializable() = default;
    char separator = ';';
    virtual std::unordered_map<field_name, data> serialize()=0;
    virtual void load()=0;
    void write();
    void read();
    virtual std::string storageLocation()=0;
};


#endif //PROJEKT_SERIALIZABLE_H