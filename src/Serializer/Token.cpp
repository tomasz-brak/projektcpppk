//
// Created by tomasz on 11/16/2025.
//

#include "Token.h"
#include <string>

Token::Token() : parent(nullptr), type(tokenType::UNKNOWN) {}

std::string Token::contentAsString() const
{
  if(this->content.empty())
    {
      return "";
    }
  std::string out;
  for(const auto &ch : this->content)
    {
      out += ch->ch;
    }
  return out;
}

std::string Token::toString(const int scope) const
{
  std::string out;
  out.append(scope * 2, ' ');
  switch(this->type)
    {
    case tokenType::UNKNOWN: out += "<Token t='UNKNOWN'>"; break;
    case tokenType::STRING: out += "<Token t='String'>"; break;
    case tokenType::ARRAY: out += "<Token t='Array'>"; break;
    case tokenType::DICT: out += "<Token t='Dict'>"; break;
    case tokenType::KEY: out += "<Token t='Key'>"; break;
    }
  if(this->value != nullptr)
    {
      out += " value: '" + this->value->contentAsString() + '\'';
    }
  if(this->key != nullptr)
    {
      out += " key : '" + this->key->contentAsString() + '\'';
    }

  out += "\n";
  for(const auto &child : this->children)
    {
      out += child->toString(scope + 1);
    }

  return out;
}
