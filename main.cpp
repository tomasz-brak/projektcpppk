#include <cstdio>
#include <iostream>
#include <fstream>
#include <memory>
#include <string>
#include <list>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <clocale>
#include "Display.h"
#include "Ksiazka.h"
#include "Uzytkownik.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;


void wyczyscKonsole()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void czekajNaEnter()
{
    cout << "\nNaciśnij ENTER, aby kontynuować...";
    string tmp;
    getline(cin, tmp);
}

void odczytajUzytkownikow(list<Uzytkownik>& uzytkownicy)
{
    ifstream plik("uzytkownicy.txt");
    string linia;
    uzytkownicy.clear();

    while (getline(plik, linia))
    {
        stringstream ss(linia);
        string id, imie, nazwisko, haslo_zaszyfrowane;

        if (getline(ss, id, ';') && getline(ss, imie, ';') &&
            getline(ss, nazwisko, ';') && getline(ss, haslo_zaszyfrowane, ';'))
        {
            string haslo = xorString(haslo_zaszyfrowane, KLUCZ_SZYFROWANIA);
            uzytkownicy.push_back(Uzytkownik(imie, nazwisko, id, haslo));
        }
    }
    plik.close();
}

void odczytajKsiazki(list<Ksiazka>& ksiazki)
{
    ifstream plik("ksiazki.txt");
    string linia;
    ksiazki.clear();

    while (getline(plik, linia))
    {
        stringstream ss(linia);
        string id, tytul, autor, cena_str, id2;

        if (getline(ss, id, ';') && getline(ss, tytul, ';') &&
            getline(ss, autor, ';') && getline(ss, cena_str, ';') &&
            getline(ss, id2, ';'))
        {
            float cena = stof(cena_str);
            ksiazki.push_back(Ksiazka(tytul, autor, cena, id, id2));
        }
    }
    plik.close();
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
    list<Uzytkownik> listaUzytkownikow;

    odczytajKsiazki(listaKsiazek);
    odczytajUzytkownikow(listaUzytkownikow);

    string login, haslo, opcja, user_input;
    string current_user_id;

    while (true)
    {
        wyczyscKonsole();

        cout << "┌──────────────── KSIĘGARNIA ────────────────┐" << endl;
        cout << "│ Login: ";
        getline(cin, login);
        cout << "│ Hasło: ";
        getline(cin, haslo);

        bool jest_admin = (login == "admin" && haslo == "admin");
        bool jest_uzytkownik = false;

        for (const auto& u : listaUzytkownikow)
        {
            if (u.czyPoprawneDaneLogowania(login, haslo))
            {
                jest_uzytkownik = true;
                current_user_id = u.id;
                break;
            }
        }

        if (!jest_admin && !jest_uzytkownik)
        {
            cout << "├────────────────────────────────────────────┤" << endl;
            cout << "│ Błędne dane logowania!                     │" << endl;
            cout << "└────────────────────────────────────────────┘" << endl;
            czekajNaEnter();
            continue;
        }
        else{
          cout << "└────────────────────────────────────────────┘" << endl;
        }

        bool wylogowano = false;
        while (!wylogowano)
        {
            wyczyscKonsole();

            if (jest_admin)
            {
                // === MENU ADMINA ===
                cout << "┌──────────────── KSIĘGARNIA ────────────────┐" << endl;
                cout << "│ Witaj, Admin!" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ w - dodaj książkę do sklepu                │" << endl;
                cout << "│ a - usuń książkę                           │" << endl;
                cout << "│ s - wyświetl wszystkie książki             │" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ d - dodaj użytkownika                      │" << endl;
                cout << "│ q - usuń użytkownika                       │" << endl;
                cout << "│ e - wyświetl użytkowników                  │" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ r - książki sprzedane (u użytkowników)     │" << endl;
                cout << "│ f - książki w sklepie                      │" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ z - wyloguj                                │" << endl;
                cout << "└────────────────────────────────────────────┘" << endl;
                cout << "---> : ";
                getline(cin, opcja);
                char ch = opcja.empty() ? 0 : tolower(opcja[0]);

                switch (ch)
                {
                    case 'w':  // dodaj książkę (tylko do sklepu)
                    {
                        string tytul, autor, cena_str;
                        float cena = 0.0f;

                        wyczyscKonsole();
                        
                        cout << "┌─────────────── DODAJ KSIĄŻKĘ ──────────────┐" << endl;
                        cout << "│ Tytuł: ";
                        getline(cin, tytul);
                        cout << "│ Autor: ";
                        getline(cin, autor);
                        cout << "│ Cena: ";
                        getline(cin, cena_str);

                        try { cena = stof(cena_str); }
                        catch (...) {
                            cout << "│ Błędna cena!" << endl;
                            czekajNaEnter();
                            break;
                        }

                        string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3) + to_string(rand() % 1000);
                        listaKsiazek.push_back(Ksiazka(tytul, autor, cena, nowy_id, "sklep"));
                        Ksiazka::zapiszKsiazki(listaKsiazek);
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Dodano książkę do sklepu!                  │" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'a':  // usuń dowolną książkę
                    {
                        wyczyscKonsole();
                        
                        cout << "┌─────────────── USUŃ KSIĄŻKĘ ───────────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            ma = true;
                            cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor
                                 << " (" << (k.id2 == "sklep" ? "sklep" : "użytkownik " + k.id2) << ") - " << k.cena << " zł" << endl;
                        }
                        if (!ma)
                        {
                            cout << "│ Brak książek do usunięcia.                 │" << endl;
                            cout << "└────────────────────────────────────────────┘" << endl;
                            czekajNaEnter();
                            break;
                        }
                        
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Podaj ID książki do usunięcia: ";
                        getline(cin, user_input);

                        size_t stary_rozmiar = listaKsiazek.size();
                        Ksiazka::usunKsiazke(listaKsiazek, user_input);
                        cout << "├────────────────────────────────────────────┤" << endl;
                        if (listaKsiazek.size() < stary_rozmiar)
                        {
                            Ksiazka::zapiszKsiazki(listaKsiazek);
                            cout << "│ Usunięto książkę !                         │" << endl;
                        }
                        else
                        {
                            cout << "│ Nie znaleziono książki.                    │" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 's':  // wszystkie książki
                    {
                        wyczyscKonsole();
                        
                        cout << "┌──────────── WSZYSTKIE KSIĄŻKI ─────────────┐" << endl;
                        if (listaKsiazek.empty())
                            cout << "│ Brak książek!                              │" << endl;
                        for (const auto& k : listaKsiazek)
                        {
                            cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor
                                 << " (" << (k.id2 == "sklep" ? "sklep" : "użytk. " + k.id2) << ") - " << k.cena << " zł" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;

                        czekajNaEnter();
                        break;
                    }
                    case 'd':  // dodaj użytkownika
                    {
                        string imie, nazwisko, haslo_u;
                        wyczyscKonsole();
                        
                        cout << "┌───────────── DODAJ UŻYTKOWNIKA ────────────┐" << endl;
                        cout << "│ Imię: ";
                        getline(cin, imie);
                        cout << "│ Nazwisko: ";
                        getline(cin, nazwisko);
                        cout << "│ Hasło: ";
                        getline(cin, haslo_u);

                        string nowy_id = imie.substr(0, 3) + nazwisko.substr(0, 3) + to_string(rand() % 1000);
                        listaUzytkownikow.push_back(Uzytkownik(imie, nazwisko, nowy_id, haslo_u));
                        Uzytkownik::zapiszUzytkownika(listaUzytkownikow);

                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Dodano użytkownika ("<<nowy_id<<")!" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'q':  // usuń użytkownika
                    {
                        wyczyscKonsole();
                        cout << "┌──────────── USUŃ UŻYTKOWNIKA ──────────────┐" << endl;
                        bool ma = false;
                        for (const auto& u : listaUzytkownikow)
                        {
                            ma = true;
                            cout << "│ [" << u.id << "] " << u.imie << " " << u.nazwisko << endl;
                        }
                        if (!ma)
                        {
                            cout << "│ Brak użytkownków do usunięcia.             │" << endl;
                            cout << "└────────────────────────────────────────────┘" << endl;
                            czekajNaEnter();
                            break;
                        }
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Podaj ID użytkownika do usunięcia: ";
                        getline(cin, user_input);

                        size_t stary_rozmiar = listaUzytkownikow.size();
                        Uzytkownik::usunUzytkownika(listaUzytkownikow, user_input);
                        cout << "├────────────────────────────────────────────┤" << endl;
                        if (listaUzytkownikow.size() < stary_rozmiar)
                        {
                            Uzytkownik::zapiszUzytkownika(listaUzytkownikow);
                            cout << "│ Usunięto użytkownika.                      │" << endl;
                        }
                        else
                        {
                            cout << "│ Nie znaleziono użytkownika.                │" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'e':  // lista użytkowników
                    {
                        wyczyscKonsole();
                        cout << "┌──────────── LISTA UŻYTKOWNIKÓW ────────────┐" << endl;
                        if (listaUzytkownikow.empty())
                            cout << "│ Brak użytkowników!                         │" << endl;
                        for (const auto& u : listaUzytkownikow)
                        {
                            cout << "│ [" << u.id << "] " << u.imie << " " << u.nazwisko << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'r':  // książki sprzedane (u użytkowników)
                    {
                        wyczyscKonsole();
                        
                        cout << "┌──── KSIĄŻKI SPRZEDANE (U UŻYTKOWNIKÓW) ────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 != "sklep")
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - u użytkownika " << k.id2 << endl;
                            }
                        }
                        if (!ma) cout << "│ Brak sprzedanych książek.                  │" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'f':  // książki w sklepie
                    {
                        wyczyscKonsole();
                        
                        cout << "┌──────────── KSIĄŻKI W SKLEPIE ─────────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 == "sklep")
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor << " - " << k.cena << " zł" << endl;
                            }
                        }
                        if (!ma) cout << "│ Brak książek w sklepie.                    │" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'z':
                        wylogowano = true;
                        break;
                }
            }
            else  // === MENU ZWYKŁEGO UŻYTKOWNIKA ===
            {
                cout << "┌──────────────── KSIĘGARNIA ────────────────┐" << endl;
                cout << "│ Witaj, " << login << "!" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ r - kup książkę                            │" << endl;
                cout << "│ f - sprzedaj książkę sklepowi              │" << endl;
                cout << "│ a - usuń swoją książkę                     │" << endl;
                cout << "│ s - wyświetl książki w sklepie             │" << endl;
                cout << "├────────────────────────────────────────────┤" << endl;
                cout << "│ z - wyloguj                                │" << endl;
                cout << "└────────────────────────────────────────────┘" << endl;
                cout << "---> : ";
                getline(cin, opcja);
                char ch = opcja.empty() ? 0 : tolower(opcja[0]);

                switch (ch)
                {
                    case 'r':  // kup książkę
                    {
                        wyczyscKonsole();
    
                        cout << "┌──────── DOSTĘPNE KSIĄŻKI W SKLEPIE ────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 == "sklep")
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor << " - " << k.cena << " zł" << endl;
                            }
                        }
                        if (!ma)
                        {
                            cout << "│ Brak książek w sklepie.                    │" << endl;
                            cout << "└────────────────────────────────────────────┘" << endl;
                            czekajNaEnter();
                            break;
                        }
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Podaj ID książki do kupna: ";
                        getline(cin, user_input);

                        bool kupiono = false;
                        for (auto& k : listaKsiazek)
                        {
                            if (k.id == user_input && k.id2 == "sklep")
                            {
                                k.id2 = current_user_id;
                                kupiono = true;
                                break;
                            }
                        }
                        
                        cout << "├────────────────────────────────────────────┤" << endl;
                        if (kupiono)
                        {
                            Ksiazka::zapiszKsiazki(listaKsiazek);
                            cout << "│ Pomyślnie kupiono książkę!                 │" << endl;
                        }
                        else
                        {
                            cout << "│ Książka nie jest dostępna w sklepie.       │" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'f':  // sprzedaj/s kup do sklepu
                    {
                        wyczyscKonsole();
                        
                        cout << "┌────────────── TWOJE KSIĄŻKI ───────────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 == current_user_id)
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor << " - " << k.cena << " zł" << endl;
                            }
                        }
                        if (!ma)
                        {
                            cout << "│ Nie posiadasz żadnych książek.             │" << endl;
                            cout << "└────────────────────────────────────────────┘" << endl;
                            czekajNaEnter();
                            break;
                        }
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Podaj ID książki do sprzedaży sklepowi: ";
                        getline(cin, user_input);

                        bool sprzedano = false;
                        for (auto& k : listaKsiazek)
                        {
                            if (k.id == user_input && k.id2 == current_user_id)
                            {
                                k.id2 = "sklep";
                                sprzedano = true;
                                break;
                            }
                        }
                        cout << "├────────────────────────────────────────────┤" << endl;
                        if (sprzedano)
                        {
                            Ksiazka::zapiszKsiazki(listaKsiazek);
                            cout << "│ Pomyślnie sprzedano książkę sklepowi!      │" << endl;
                        }
                        else
                        {
                            cout << "│ Nie posiadasz takiej książki.              │" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'a':  // usuń swoją książkę
                    {
                        wyczyscKonsole();
                        
                        cout << "┌────────────── TWOJE KSIĄŻKI ───────────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 == current_user_id)
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor << " - " << k.cena << " zł" << endl;
                            }
                        }
                        if (!ma)
                        {
                            cout << "│ Nie posiadasz żadnych książek.             │" << endl;
                            cout << "└────────────────────────────────────────────┘" << endl;
                            czekajNaEnter();
                            break;
                        }
                        cout << "├────────────────────────────────────────────┤" << endl;
                        cout << "│ Podaj ID książki do usunięcia:";
                        getline(cin, user_input);

                        bool jest_wlasna = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id == user_input && k.id2 == current_user_id)
                            {
                                jest_wlasna = true;
                                break;
                            }
                        }
                        
                        cout << "├────────────────────────────────────────────┤" << endl;
                        if (jest_wlasna)
                        {
                            size_t stary_rozmiar = listaKsiazek.size();
                            Ksiazka::usunKsiazke(listaKsiazek, user_input);
                            if (listaKsiazek.size() < stary_rozmiar)
                            {
                                Ksiazka::zapiszKsiazki(listaKsiazek);
                                cout << "│ Usunięto książkę.                          │" << endl;
                            }
                        }
                        else
                        {
                            cout << "│ Nie posiadasz takiej książki.              │" << endl;
                        }
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 's':  // książki w sklepie
                    {
                        wyczyscKonsole();
                        cout << "┌──────── DOSTĘPNE KSIĄŻKI W SKLEPIE ────────┐" << endl;
                        bool ma = false;
                        for (const auto& k : listaKsiazek)
                        {
                            if (k.id2 == "sklep")
                            {
                                ma = true;
                                cout << "│ [" << k.id << "] " << k.tytul << " - " << k.autor << " - " << k.cena << " zł" << endl;
                            }
                        }
                        if (!ma) cout << "│ Brak książek w sklepie.                    │" << endl;
                        cout << "└────────────────────────────────────────────┘" << endl;
                        czekajNaEnter();
                        break;
                    }
                    case 'z':
                        wylogowano = true;
                        break;
                    default:
                        cout << "Nieznana opcja!" << endl;
                        czekajNaEnter();
                }
            }
        }
    }

    return 0;
}