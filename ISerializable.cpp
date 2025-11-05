//
// Created by Tomasz on 11/04/2025.
//

#include "ISerializable.h"

#include <format>
#include <iostream>
#include "storeClassList.h"



std::string ISerializable::serialize() const {
    std::string result;
    result += "{\n";
    for (auto it  = fields.begin(); it != fields.end(); ++it) {

        result += (*it)();
        if (it != fields.end()-1) {
            result += ",\n";
            continue;
        }
        result += "\n";
    }
    result += "}";
    return result;
}


// filepath: ISerializable.cpp
