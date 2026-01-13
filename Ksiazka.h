#ifndef Ksiazka_H
#define Ksiazka_H

using namespace std;

class Ksiazka {
public:
    Ksiazka(string tyt, string aut, float cena, string i, string id2);
    void usunKsiazke(list<Ksiazka>& ksiazki, const string& id);
    string daneDoZapisu();

private:
    string tytul;
    string autor;
    float cena;
    string id2;
    string id;
};

#endif