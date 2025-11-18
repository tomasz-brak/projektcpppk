//
// Created by WIEiK-student on 18.11.2025.
//

#ifndef KLIENT_H
#define KLIENT_H
#include "Serializer/Serializable.h"

#include <string>

using namespace std;



class Klient:public Serializable<Klient>{
  private:
public:
  string imie;
  string nazwisko;
  static void daneKlienta() {}
    Klient();
  static const std::string &static_name()
  {
    static const std::string name="Klient";
    return name;
  }
};




#endif //KLIENT_H
