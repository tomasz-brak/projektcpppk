#include "Uzytkownik.h"
#include <string>
#include <fstream>
#include <list>
#include <sstream>

Uzytkownik::Uzytkownik(string i, string n, string ID, string h) : imie(i), nazwisko(n), id(ID), haslo(h) {}

void Uzytkownik::zapiszUzytkownika(const list<Uzytkownik>& uzytkownicy) {
    ofstream plik("urzytkownicy.txt");
    for (const auto& u : uzytkownicy) {
        plik << u.id << ";" << u.imie << ";" << u.nazwisko << ";" << u.haslo << ";" << endl;
    }
    plik.close();
}

void Uzytkownik::usunUzytkownika(list<Uzytkownik>& uzytkownicy, const string& id) {
    uzytkownicy.remove_if([&id](const Uzytkownik& u) { return u.id == id; });
}

bool Uzytkownik::czyPoprawneDaneLogowania(const string& login, const string& haslo) const {
    return (id == login && this->haslo == haslo);
}