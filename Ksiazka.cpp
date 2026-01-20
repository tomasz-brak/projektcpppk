#include "Ksiazka.h"
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

Ksiazka::Ksiazka(string tyt, string aut, float cena, string i, string id2)
    : tytul(tyt), autor(aut), cena(cena), id(i), id2(id2)
{}

// DODANE const
string Ksiazka::daneDoZapisu() const
{
  stringstream dane;
  dane << id << ";" << tytul << ";" << autor << ";" << cena << ";" << id2;
  return dane.str();
}

void Ksiazka::usunKsiazke(std::list<Ksiazka> &ksiazki, string id)
{
  ksiazki.remove_if(
    [id](Ksiazka current_book) { return current_book.id == id; });
}

void Ksiazka::zapiszKsiazki(const std::list<Ksiazka> &ksiazki)
{
  ofstream plik("ksiazki.txt");
  if(!plik.is_open())
    return;

  for(const auto &k : ksiazki)
    {
      plik << k.daneDoZapisu() << endl; // teraz działa, bo metoda jest const
    }
  plik.close();
}
void Ksiazka::odczytajKsiazki(list<Ksiazka> &ksiazki)
{
  ifstream plik("ksiazki.txt");
  string linia;
  ksiazki.clear();

  while(getline(plik, linia))
    {
      stringstream ss(linia);
      string id, tytul, autor, cena, id2;

      if(getline(ss, id, ';') && getline(ss, tytul, ';')
         && getline(ss, autor, ';') && getline(ss, cena, ';')
         && getline(ss, id2, ';'))
        {
          ksiazki.push_back(Ksiazka(tytul, autor, stof(cena), id, id2));
        }
    }
  plik.close();
}
