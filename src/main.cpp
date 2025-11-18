// main.cpp

#include <iostream>

#include "util.h"
#include "Serializer/Serializable.h"
#include "Serializer/ISerializable.h"
#include "Serializer/storeClassList.h"
#include <windows.h>

#pragma execution_character_set( "utf-8" )

class Client : public Serializable<Client> {
public:
    int id = 0;
    std::string test;


    std::string getPrimaryKey() override {
        return std::to_string(id);
    }

    explicit Client() {
        this->name = "Client";
        registerField<std::string, defaultString<std::string>, Client>("name", test, defaultString<std::string>());
        registerField<int, defaultString<int>, Client>("id", id, defaultString<int>());
        registerFactory(this->name, [](){ return std::make_shared<Client>(); });
    }

};

int main(const int argc, char **argv) {
    SetConsoleOutputCP( 65001 );
    // Parse argv
    auto args = argParser(argc, argv);
    std::string dataDir;
    if (const auto it = args.find("dataDir"); it != args.end()) {
        dataDir = it->second;
    } else {
        dataDir = "./";
    }
    std::cout << "Program will use " << dataDir << " for storing data" << std::endl ;

    const auto a = Client::create();
    a->id =1;
    a->test = "hello";
    auto b = Client::create();
    b->id =2;
    b->test = "world";
    const std::vector<std::shared_ptr<ISerializable>> c = {a, b};
    storeClass(c, dataDir);
    b->test = "mars";
    storeClass(c, dataDir);
    std::vector<std::shared_ptr<ISerializable>> fromFileVec;
    readClass("Client", fromFileVec, dataDir);
    for (const auto &item: fromFileVec) {
        auto client = std::dynamic_pointer_cast<Client>(item);
        if (client) {
            std::cout << "Client id: " << client->id << ", test: " << client->test << std::endl;
        }
    }

}

// filepath: main.cpp