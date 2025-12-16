#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <clocale>
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

class Ksiazka {
public:
    string tytul;
    string autor;
    float cena;
    string id2;
    string id;

    Ksiazka(string tyt, string aut,float cena, string i,string id2) : tytul(tyt), autor(aut),cena(cena), id(i),id2(id2) {}
};

class Urzytkownik {
public:
    string imie;
    string nazwisko;
    string id;

    Urzytkownik(string i, string n, string ID) : imie(i), nazwisko(n), id(ID) {}
};

void zapiszKsiazke(const list<Ksiazka>& ksiazki) {
    ofstream plik("ksiazki.txt");
    for (const auto& k : ksiazki) {
        plik << k.id << ";" << k.tytul << ";" << k.autor<<";" <<k.cena<< ";" <<k.id2<<";"<< endl;
    }
    plik.close();
}

void usunKsiazke(list<Ksiazka>& ksiazki, const string& id) {
    ksiazki.remove_if([&id](const Ksiazka& k) { return k.id == id; });
}

void odczytajKsiazke(list<Ksiazka>& ksiazki) {
    ifstream plik("ksiazki.txt");
    string linia;
    ksiazki.clear();

    while (getline(plik, linia)) {
        stringstream ss(linia);
        string id, tytul, autor,cena,id2;

        if (getline(ss, id, ';') &&
            getline(ss, tytul, ';') &&
            getline(ss, autor, ';')&&
            getline(ss, cena, ';')&&
            getline(ss, id2, ';')) {

            ksiazki.push_back(Ksiazka(tytul, autor,stof(cena), id,id2));
        }
    }
    plik.close();
}

void zapiszUrzytkownika(const list<Urzytkownik>& urzytkownicy) {
    ofstream plik("urzytkownicy.txt");
    for (const auto& u : urzytkownicy) {
        plik << u.id << ";" << u.imie << ";" << u.nazwisko << ";" << endl;
    }
    plik.close();
}

void usunUrzytkownika(list<Urzytkownik>& urzytkownicy, const string& id) {
    urzytkownicy.remove_if([&id](const Urzytkownik& u) { return u.id == id; });
}

void odczytajUrzytkownika(list<Urzytkownik>& urzytkownicy) {
    ifstream plik("urzytkownicy.txt");
    string linia;
    urzytkownicy.clear();

    while (getline(plik, linia)) {
        stringstream ss(linia);
        string id, imie, nazwisko;

        if (getline(ss, id, ';') &&
            getline(ss, imie, ';') &&
            getline(ss, nazwisko, ';')) {

            urzytkownicy.push_back(Urzytkownik(imie, nazwisko, id));
        }
    }
    plik.close();
}

void pobierzDane(string tekst, string& dane) {
  do
    {
      cout << "│ " + tekst + ": ";
      getline(cin, dane);
      if(dane.find(';') != string::npos)
        {
          cout << "│ Tekst nie może zawierać średnika (;)!\n";
          cout << "│ " + tekst + ": ";
          getline(cin, dane);
        }
  } while(dane.find(';') != string::npos);
}

int main()
{
    #ifdef _WIN32
        SetConsoleOutputCP(65001);
        SetConsoleCP(65001);
    #endif
        setlocale(LC_ALL, "pl_PL.UTF-8");

    srand(time(0));

    list<Ksiazka> listaKsiazek;
    list<Urzytkownik> listaUrzytkownikow;

    odczytajKsiazke(listaKsiazek);
    odczytajUrzytkownika(listaUrzytkownikow);

    int k = 0;
    string opcja;
    string temp;

    while(k == 0)
    {
        system("cls");
        cout << "┌──────────── KSIĘGARNIA ────────────┐" << endl;
        cout << "│ Wybierz opcje:                     │" << endl;
        cout << "│ - dodaj książkę (w)                │" << endl;
        cout << "│ - usuń książkę (a)                 │" << endl;
        cout << "│ - wyswietl liste książek (s)       │" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ - dodaj urzytkownika (d)           │" << endl;
        cout << "│ - usuń urzytkownika (q)            │" << endl;
        cout << "│ - wyswietl liste urzytkowników (e) │" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ - sprzedarz książek (r)            │" << endl;
        cout << "│ - skup książek (f)                 │" << endl;
        cout << "├────────────────────────────────────┤" << endl;
        cout << "│ - zamknij (z)                      │" << endl;
        cout << "└────────────────────────────────────┘" << endl;
        cout << "---> : ";
        cin>>opcja;

        if (opcja.length()!=1) {
            opcja = 'x';
        }

        if (opcja[0] == 'w')
        {
            string tytul, autor,cena;
            system("cls");
            cout << "┌────────── DODAJ KSIĄŻKĘ ───────────┐" << endl;
            cin.ignore();
            pobierzDane("Tytul", tytul);
            pobierzDane("Autor", autor);
            pobierzDane("Cena", cena);
            cout << "├────────────────────────────────────┤" << endl;

            string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3) + to_string(rand() % 1000);
            listaKsiazek.push_back(Ksiazka(tytul, autor,stof(cena), nowy_id,"ksiazka"));
            zapiszKsiazke(listaKsiazek);

            cout << "│ Dodano książkę: " << tytul <<endl;
            cout << "└────────────────────────────────────┘" << endl;
            cout << "<--- : ";
            getline(cin, temp);
        }
        else if (opcja[0] == 'a')
        {
            system("cls");
            cout << "┌─────────── USUŃ KSIĄŻKĘ ───────────┐" << endl;
            for (const auto& k : listaKsiazek) {
                cout << "│ [" << k.id << "] Tytul: " << k.tytul << ", Autor: " << k.autor <<", Cena: "<<k.cena<<" zł"<< endl;
            }
            cout << "├────────────────────────────────────┤" << endl;
            cout << "│ Podaj ID książki do usunięcia: ";
            cin >> temp;
            cout << "├────────────────────────────────────┤" << endl;

            size_t poczatkowy_rozmiar = listaKsiazek.size();
            usunKsiazke(listaKsiazek, temp);

            if (listaKsiazek.size() < poczatkowy_rozmiar) {
                cout << "│ Usunieto książkę o ID: " << temp << endl;
                zapiszKsiazke(listaKsiazek);
            } else {
                cout << "│ Nie znaleziono książki o ID: " << temp << endl;
            }
            cout << "└────────────────────────────────────┘" << endl;
             cout << "<--- : ";
             cin.ignore(); getline(cin, temp);
        }
        else if (opcja[0] == 's')
        {
            system("cls");
            cout << "┌──────── LISTA KSIĄŻEK (" << listaKsiazek.size() << (listaKsiazek.size()>9?"─":"") << ") ─────────┐" << endl;
            if (listaKsiazek.size()!=0){
                    for (const auto& k : listaKsiazek) {
                        cout << "│ - Tytul: " << k.tytul << ", Autor: " << k.autor <<", Cena: "<<k.cena<<" zł"<< endl;
                }
            }
            else{
                cout << "│ Brak książek !!!" << endl;
            }
            cout << "└────────────────────────────────────┘" << endl;
            cout << "<--- : ";
            cin.ignore();
            getline(cin, temp);
        }
        else if (opcja[0] == 'd')
        {
            string imie, nazwisko;
            system("cls");
            cout << "┌──────── DODAJ UŻYTKOWNIKA ─────────┐" << endl;

            cin.ignore();
            cout << "│ Imie: ";
            getline(cin, imie);
            cout << "│ Nazwisko: ";
            getline(cin, nazwisko);
            cout << "├────────────────────────────────────┤" << endl;

            string nowy_id = imie.substr(0, 3) + nazwisko.substr(0, 3) + to_string(rand() % 1000);
            listaUrzytkownikow.push_back(Urzytkownik(imie, nazwisko, nowy_id));

            zapiszUrzytkownika(listaUrzytkownikow);

            cout << "│ Dodano: " << imie << " " << nazwisko <<endl;
            cout << "└────────────────────────────────────┘" << endl;
            cout << "<--- : ";
            getline(cin, temp);
        }
        else if (opcja[0] == 'q')
        {
            system("cls");
            cout << "┌──────── USUŃ URZYTKOWNIKA ─────────┐" << endl;
             for (const auto& u : listaUrzytkownikow) {
                cout << "│ [" << u.id << "] Imie: " << u.imie << ", Nazwisko: " << u.nazwisko << endl;
            }
            cout << "├────────────────────────────────────┤" << endl;
            cout << "│ Podaj ID urzytkownika do usunięcia: ";
            cin >> temp;
            cout << "├────────────────────────────────────┤" << endl;

            size_t poczatkowy_rozmiar = listaUrzytkownikow.size();

            usunUrzytkownika(listaUrzytkownikow, temp);

            if (listaUrzytkownikow.size() < poczatkowy_rozmiar) {
                cout << "│ Usunieto uzytkownika o ID: " << temp << endl;
                zapiszUrzytkownika(listaUrzytkownikow);
            } else {
                cout << "│ Nie znaleziono uzytkownika o ID: " << temp << endl;
            }
                cout << "└────────────────────────────────────┘" << endl;
             cout << "<--- : ";
             cin.ignore(); getline(cin, temp);
        }
        else if (opcja[0] == 'e')
        {
            system("cls");
            cout << "┌──────── LISTA KSIĄŻEK (" << listaKsiazek.size() << (listaKsiazek.size()>9?"─":"") << ") ─────────┐" << endl;
            cout << "┌────── LISTA UŻYTKOWNIKÓW (" << listaUrzytkownikow.size() << ") ─────────┐" << endl;
            if (listaUrzytkownikow.size()!=0){
                    for (const auto& u : listaUrzytkownikow) {
                        cout << "│ [" << u.id << "] Imie: " << u.imie << ", Nazwisko: " << u.nazwisko << endl;
                    }
            }
            else{
                cout << "│ Brak urzytkowników !!!" << endl;
            }
            cout << "└────────────────────────────────────┘" << endl;
            cout << "<--- : ";
            cin.ignore();
            getline(cin, temp);
        }
        else if (opcja[0] == 'z')
        {
            k = 1;
        }
    }

    return 0;
}
