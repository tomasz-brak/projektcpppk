#include "Ksiazka.h"
#include <fstream>
#include <sstream>
#include <list>

using namespace std;

Ksiazka::Ksiazka(string tyt, string aut, float cena, string i, string id2) : tytul(tyt), autor(aut), cena(cena), id(i), id2(id2) {}

void Ksiazka::usunKsiazke(list<Ksiazka>& ksiazki, const string& id) {
    ksiazki.remove_if([&id](const Ksiazka& k) { return k.id == id; });
}

string Ksiazka::daneDoZapisu() {
    stringstream dane;
    dane << id << ";" << tytul << ";" << autor << ";" << cena << ";" << id2 << ";";
	return dane.str();
}