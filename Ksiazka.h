#ifndef KSIAZKA_H
#define KSIAZKA_H

#include <string>
#include <list>

class Ksiazka
{
public:
  std::string tytul;
  std::string autor;
  float cena;
  std::string id;
  std::string id2;

  Ksiazka(std::string tyt, std::string aut, float c, std::string i,
          std::string id2);

  // DODANE const
  std::string daneDoZapisu() const;

  static void usunKsiazke(std::list<Ksiazka> &ksiazki, std::string id);

  static void zapiszKsiazki(const std::list<Ksiazka> &ksiazki);

  static void odczytajKsiazki(std::list<Ksiazka> &ksiazki);
};

#endif // KSIAZKA_H
