#ifndef Ksiazka_H
#define Ksiazka_H
#include <string>
#include <list>

class Ksiazka {
public:
    Ksiazka(std::string tyt, std::string aut, float cena, std::string i, std::string id2);
    void usunKsiazke(std::list<Ksiazka>& ksiazki, const std::string& id);
    std::string daneDoZapisu();

    std::string tytul;
    std::string autor;
    float cena;
    std::string id2;
    std::string id;
};

#endif
