// main.cpp

#include <bits/monostate.h>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

#include "util.h"
#include "Serializer/Serializable.h"
#include "Serializer/ISerializable.h"
#include "Serializer/storeClassList.h"
// #include <windows.h>

#pragma execution_character_set("utf-8")

class Foo : public Serializable<Foo>
{
public:
  int a = 0;
  std::string b = "Something!";

  static const std::string &static_name()
  {
    static const std::string name = "Foo";
    return name;
  }

  explicit Foo(){
    registerField("A", &Foo::a, defaultString<int>());
    registerField("B", &Foo::b, defaultString<std::string>());
  }
};

class Bar : public Foo {
  public:
  int c =6;

  static const std::string &static_name()
{
    static const std::string name = "Bar";
    return name;
  }
  explicit Bar(){
    registerField("C", &Bar::c, defaultString<int>());
  }
};

int main(const int argc, char **argv)
{
  /* --------------------- */
  /* --- PROGRAM SETUP --- */
  /* --------------------- */
  // SetConsoleOutputCP(65001);
  // Parse argv
  auto args = argParser(argc, argv);
  std::string dataDir;
  if(const auto it = args.find("dataDir"); it != args.end())
    {
      dataDir = it->second;
    }
  else
    {
      dataDir = "./";
    }
  std::cout << "Program will use " << dataDir << " for storing data"
            << std::endl;

  std::cout << "Testing! " << std::endl;
  auto foo = Foo::create();
  foo->a = 5;
  foo->b = "Hello World";

  auto bar = std::make_shared<Bar>();
  bar->c = 6;

  std::vector<std::shared_ptr<ISerializable>> list = {foo, bar};
  storeClass(list, dataDir);
  
  std::vector<std::shared_ptr<ISerializable>> loaded;
  readClass("Foo", loaded, dataDir);
  readClass("Bar", loaded, dataDir);

}


