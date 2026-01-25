#include <format>
#include <fstream>
#include <ostream>
#include <stdexcept>
#include <string_view>
class Raport
{
  inline static std::ofstream out;

public:
  inline static void init() { out.open("raport.txt"); }
  template <typename... Args>
  inline static void printf(std::string_view formatString, Args &&...args)
  {
    if(!out.is_open())
      {
        throw std::runtime_error("File is not open!");
      }

    out << std::vformat(formatString, std::make_format_args(args...))
        << std::endl;
  }
};
