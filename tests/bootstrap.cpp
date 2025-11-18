//
// Created by tomasz on 11/14/2025.
//

#include <windows.h>

#include "../src/util.h"
#include "./test_util.h"

RUN_TEST(util_splitString)
{
  std::string s = "hello world";
  std::string d = " ";
  std::vector<std::string> v = {"hello", "world"};
  T_ASSERT(splitString(s, d) == v);
}

int main()
{
  SetConsoleOutputCP(65001);

  for(const auto &test : get_test_registry())
    {
      test.func();
    }
}