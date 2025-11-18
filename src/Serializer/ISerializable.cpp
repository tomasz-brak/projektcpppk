#include "ISerializable.h"

#include <format>
#include <iostream>
#include "storeClassList.h"

std::string ISerializable::serialize() const
{
  std::string result;
  result += "{\n";
  for(auto it = converters.begin(); it != converters.end(); ++it)
    {
      if(!it->second.to_str().has_value())
        {
          std::cerr << "Conversion failuire for: " << it->first << std::endl;
          continue;
        }
      result += "\"" + it->first + "\"" + ": \"" + it->second.to_str().value()
                + "\"";
      if(std::next(it) != converters.end())
        {
          result += ",\n";
          continue;
        }
      result += "\n";
    }
  result += "}";
  return result;
}
