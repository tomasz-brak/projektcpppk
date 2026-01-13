#ifndef Ksiazka_H
#define Ksiazka_H
#include <fstream>
#include <string>
#include <list>

class Ksiazka
{
public:
  Ksiazka(std::string tyt, std::string aut, float cena, std::string i,
          std::string id2);
  std::string daneDoZapisu();

  std::string tytul;
  std::string autor;
  float cena;
  std::string id2;
  std::string id;

  static void zapiszKsiazki(std::list<Ksiazka> &ksiazki)
  {
    std::ofstream plik("ksiazki.txt");
    for(auto &k : ksiazki)
      {
        plik << k.daneDoZapisu() << std::endl;
      }
    plik.close();
    std::string tytul;
    std::string autor;
    float cena;
    std::string id2;
    std::string id;
  };

  static void usunKsiazke(std::list<Ksiazka> &ksiazki, std::string ksiazka);

#endif
};
