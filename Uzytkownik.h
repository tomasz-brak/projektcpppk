#ifndef Uzytkownik_H
#define Uzytkownik_H

using namespace std;

class Uzytkownik {
public:
    Uzytkownik(string i, string n, string ID, string h);
    void zapiszUzytkownika(const list<Uzytkownik>& uzytkownicy);
    void usunUzytkownika(list<Uzytkownik>& uzytkownicy, const string& id);
    bool czyPoprawneDaneLogowania(const string& login, const string& haslo) const;

private:
    string imie;
    string nazwisko;
    string id;
    string haslo;
};

#endif