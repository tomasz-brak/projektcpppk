//
// Created by WIEiK-student on 18.11.2025.

#include "Klient.h"

#include "Serializer/Serializable.h"
Klient::Klient()
{
  this->name = static_name();

  registerField("imie", &Klient::imie, defaultString<std::string>());
}