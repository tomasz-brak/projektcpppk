#include "Ksiazka.h"
#include <sstream>

using namespace std;

Ksiazka::Ksiazka(string tyt, string aut, float cena, string i, string id2)
    : tytul(tyt), autor(aut), cena(cena), id(i), id2(id2)
{}

string Ksiazka::daneDoZapisu()
{
  stringstream dane;
  dane << id << ";" << tytul << ";" << autor << ";" << cena << ";" << id2
       << ";";
  return dane.str();
}

void Ksiazka::usunKsiazke(std::list<Ksiazka> &ksiazki, string id){
  ksiazki.remove_if([id](Ksiazka current_book){
    return current_book.id == id;
  });
}
