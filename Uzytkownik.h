#ifndef UZYTKOWNIK_H
#define UZYTKOWNIK_H

#include <string>
#include <list>

class Uzytkownik
{
public:
    std::string imie;
    std::string nazwisko;
    std::string id;
    std::string haslo;

    Uzytkownik(std::string i, std::string n, std::string ID, std::string h);

    bool czyPoprawneDaneLogowania(const std::string &login,
                                  const std::string &haslo) const;

    static void zapiszUzytkownika(const std::list<Uzytkownik>& uzytkownicy);
    static void usunUzytkownika(std::list<Uzytkownik>& uzytkownicy, const std::string& id);
};

extern const std::string KLUCZ_SZYFROWANIA;

std::string xorString(const std::string& input, const std::string& key);

#endif // UZYTKOWNIK_H
