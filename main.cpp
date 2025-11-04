// main.cpp

#include <iostream>

#include "Serializable.h"
#include "storeClassList.h"

class Client : public Serializable<Client> {
public:
    std::string name = "client" ;
    int id;

    std::string getPrimaryKey() override {
        return defaultToString(id);
    }
    Client() {
        registerField("name", name, identity<std::string>);
        registerField("id", id, defaultToString<int>);
    }

};


int main(const int argc, const char * argv[]) {
    auto a = Client::create();
    a->id =1;
    a->name = "hello";
    auto b = Client::create();
    b->id =2;
    b->name = "world";
    const std::vector<std::shared_ptr<ISerializable>> c = {a, b};
    storeClass(c);
}

// filepath: main.cpp
