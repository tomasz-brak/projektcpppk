#include "Uzytkownik.h"
#include <string>
#include <fstream>
#include <list>

using namespace std;

Uzytkownik::Uzytkownik(string i, string n, string ID, string h)
    : imie(i), nazwisko(n), id(ID), haslo(h)
{}

bool Uzytkownik::czyPoprawneDaneLogowania(const string &login,
                                          const string &haslo) const
{
  return (id == login && this->haslo == haslo);
}
