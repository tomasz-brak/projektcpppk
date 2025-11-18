// main.cpp

#include <iostream>

#include "util.h"
#include "Serializer/Serializable.h"
#include "Serializer/ISerializable.h"
#include "Serializer/storeClassList.h"
#include <windows.h>

#pragma execution_character_set("utf-8")
int main(const int argc, char **argv)
{
  /* --------------------- */
  /* --- PROGRAM SETUP --- */
  /* --------------------- */
  SetConsoleOutputCP(65001);
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
}
