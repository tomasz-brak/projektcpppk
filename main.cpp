// main.cpp

#include <iostream>

#include "Serializable.h"
#include "ISerializable.h"
#include "storeClassList.h"

class Client : public Serializable<Client> {
public:
    int id = 0;
    std::string test;


    std::string getPrimaryKey() override {
        return defaultToString(id);
    }
    Client() {
        this->name = "Client";
        registerField("name", test, identity<std::string>);
        registerField("id", id, defaultToString<int>);
    }

};


int main(const int argc, const char * argv[]) {
    auto a = Client::create();
    a->id =1;
    a->test = "hello";
    auto b = Client::create();
    b->id =2;
    b->test = "world";
    const std::vector<std::shared_ptr<ISerializable>> c = {a, b};
    storeClass(c);
}

// filepath: main.cpp
