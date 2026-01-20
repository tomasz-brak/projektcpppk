#include "Uzytkownik.h"
#include <string>
#include <fstream>
#include <sstream>
#include <list>
#include <sstream>

using namespace std;

const string KLUCZ_SZYFROWANIA = "klucz";

string xorString(const string &input, const string &key)
{
  string output = input;
  size_t key_len = key.length();
  for(size_t i = 0; i < input.length(); ++i)
    {
      output[i] = input[i] ^ key[i % key_len];
    }
  return output;
}

Uzytkownik::Uzytkownik(string i, string n, string ID, string h)
    : imie(i), nazwisko(n), id(ID), haslo(h)
{}

bool Uzytkownik::czyPoprawneDaneLogowania(const string &login,
                                          const string &haslo) const
{
  return (id == login && this->haslo == haslo);
}

void Uzytkownik::zapiszUzytkownika(const list<Uzytkownik> &uzytkownicy)
{
  ofstream plik("uzytkownicy.txt");
  if(!plik.is_open())
    return;

  for(const auto &u : uzytkownicy)
    {
      string haslo_zaszyfrowane = xorString(u.haslo, KLUCZ_SZYFROWANIA);
      plik << u.id << ";" << u.imie << ";" << u.nazwisko << ";"
           << haslo_zaszyfrowane << endl;
    }
  plik.close();
}

void Uzytkownik::usunUzytkownika(list<Uzytkownik> &uzytkownicy,
                                 const string &id)
{
  uzytkownicy.remove_if([id](const Uzytkownik &u) { return u.id == id; });
}
void Uzytkownik::odczytajUzytkownikow(std::list<Uzytkownik> &uzytkownicy)
{
  ifstream plik("uzytkownicy.txt");
  string linia;
  uzytkownicy.clear();

  while(getline(plik, linia))
    {
      stringstream ss(linia);
      string id, imie, nazwisko, haslo;

      if(getline(ss, id, ';') && getline(ss, imie, ';')
         && getline(ss, nazwisko, ';') && getline(ss, haslo, ';'))
        {
          uzytkownicy.push_back(Uzytkownik(imie, nazwisko, id, haslo));
        }
    }
  plik.close();
}
