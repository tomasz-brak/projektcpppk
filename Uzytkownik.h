#ifndef Uzytkownik_H
#define Uzytkownik_H
#include <string>
#include <list>
#include <fstream>
#include <list>

class Uzytkownik
{
public:
  Uzytkownik(std::string i, std::string n, std::string ID, std::string h);
  bool czyPoprawneDaneLogowania(const std::string &login,
                                const std::string &haslo) const;

  std::string imie;
  std::string nazwisko;
  std::string id;
  std::string haslo;

  static void zapiszUzytkownika(const std::list<Uzytkownik> &uzytkownicy)
  {
    std::ofstream plik("urzytkownicy.txt");
    for(const auto &u : uzytkownicy)
      {
        plik << u.id << ";" << u.imie << ";" << u.nazwisko << ";" << u.haslo
             << ";" << std::endl;
      }
    plik.close();
  }

  static void
  usunUzytkownika(std::list<Uzytkownik> &uzytkownicy, const std::string &id)
  {
    uzytkownicy.remove_if([&id](const Uzytkownik &u) { return u.id == id; });
  }
};

#endif
