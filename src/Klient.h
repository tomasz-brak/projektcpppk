//
// Created by WIEiK-student on 18.11.2025.
//

#ifndef KLIENT_H
#define KLIENT_H
#include "Serializer/Serializable.h"

#include <string>


class Klient:public Serializable<Klient>{
  public:
    std::string imie;
    std::string nazwisko;
    static void daneKlienta() {}
      Klient();
    static const std::string &static_name()
    {
      static std::string name="klient";
      return name;
    }
};




#endif //KLIENT_H
