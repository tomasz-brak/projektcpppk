#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <clocale>
#include "Ksiazka.h"
#include "Uzytkownik.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void wyczyscKonsole()
{
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

void odczytajUzytkownikow(list<Uzytkownik> &uzytkownicy)
{
  ifstream plik("uzytkownicy.txt");
  string linia;
  uzytkownicy.clear();

  while(getline(plik, linia))
    {
      stringstream ss(linia);
      string id, imie, nazwisko, haslo;

      if(getline(ss, id, ';') && getline(ss, imie, ';')
         && getline(ss, nazwisko, ';') && getline(ss, haslo, ';'))
        {
          uzytkownicy.push_back(Uzytkownik(imie, nazwisko, id, haslo));
        }
    }
  plik.close();
}

void odczytajKsiazki(list<Ksiazka> &ksiazki)
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

void zapiszKsiazki(list<Ksiazka> &ksiazki)
{
  ofstream plik("ksiazki.txt");
  for(auto &k : ksiazki)
    {
      plik << k.daneDoZapisu() << endl;
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

  int k = 0;
  string opcja;
  string temp;
  string login, haslo;
  bool istnieje = false;

  wyczyscKonsole();

  cout << "┌──────────── KSIĘGARNIA ────────────┐" << endl;
  cout << "│ Login: ";
  getline(cin, login);
  cout << "│ Hasło: ";
  getline(cin, haslo);
  cout << "└────────────────────────────────────┘" << endl;
  cout << "---> : ";
  getline(cin, temp);

  for(const auto &u : listaUzytkownikow)
    {
      if(u.czyPoprawneDaneLogowania(login, haslo))
        {
          istnieje = true;
        }
    }

  if(login == "admin" and haslo == "admin")
    {
      while(k == 0)
        {
          wyczyscKonsole();

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
          cout << "│ - wyloguj (z)                      │" << endl;
          cout << "└────────────────────────────────────┘" << endl;
          cout << "---> : ";
          getline(cin, opcja);

          if(opcja.length() != 1)
            {
              opcja = 'x';
            }

          if(opcja[0] == 'w')
            {
              string tytul, autor, cena;

              wyczyscKonsole();

              cout << "┌────────── DODAJ KSIĄŻKĘ ───────────┐" << endl;
              cin.ignore();
              cout << "│ Tytul: ";
              getline(cin, tytul);
              cout << "│ Autor: ";
              getline(cin, autor);
              cout << "│ Cena: ";
              getline(cin, cena);
              cout << "├────────────────────────────────────┤" << endl;

              string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3)
                               + to_string(rand() % 1000);
              listaKsiazek.push_back(
                Ksiazka(tytul, autor, stof(cena), nowy_id, "ksiazka"));
              zapiszKsiazki(listaKsiazek);

              cout << "│ Dodano książkę: " << tytul << endl;
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              getline(cin, temp);
            }
          else if(opcja[0] == 'a')
            {
              wyczyscKonsole();

              cout << "┌─────────── USUŃ KSIĄŻKĘ ───────────┐" << endl;
              for(const auto &k : listaKsiazek)
                {
                  cout << "│ [" << k.id << "] Tytul: " << k.tytul
                       << ", Autor: " << k.autor << ", Cena: " << k.cena
                       << " zł" << endl;
                }
              cout << "├────────────────────────────────────┤" << endl;
              cout << "│ Podaj ID książki do usunięcia: ";
              cin >> temp;
              cout << "├────────────────────────────────────┤" << endl;

              size_t poczatkowy_rozmiar = listaKsiazek.size();
              usunKsiazke(listaKsiazek, temp);

              if(listaKsiazek.size() < poczatkowy_rozmiar)
                {
                  cout << "│ Usunieto książkę o ID: " << temp << endl;
                  zapiszKsiazke(listaKsiazek);
                }
              else
                {
                  cout << "│ Nie znaleziono książki o ID: " << temp << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 's')
            {
              wyczyscKonsole();

              cout << "┌──────── LISTA KSIĄŻEK (" << listaKsiazek.size()
                   << (listaKsiazek.size() > 9 ? "─" : "") << ") ─────────┐"
                   << endl;
              if(listaKsiazek.size() != 0)
                {
                  for(const auto &k : listaKsiazek)
                    {
                      cout << "│ - Tytul: " << k.tytul
                           << ", Autor: " << k.autor << ", Cena: " << k.cena
                           << " zł" << endl;
                    }
                }
              else
                {
                  cout << "│ Brak książek !!!" << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'd')
            {
              string imie, nazwisko;

              wyczyscKonsole();

              cout << "┌──────── DODAJ UŻYTKOWNIKA ─────────┐" << endl;

              cin.ignore();
              cout << "│ Imie: ";
              getline(cin, imie);
              cout << "│ Nazwisko: ";
              getline(cin, nazwisko);
              cout << "│ Hasło: ";
              getline(cin, haslo);
              cout << "├────────────────────────────────────┤" << endl;

              string nowy_id = imie.substr(0, 3) + nazwisko.substr(0, 3)
                               + to_string(rand() % 1000);
              listaUzytkownikow.push_back(
                Urzytkownik(imie, nazwisko, nowy_id, haslo));

              zapiszUrzytkownika(listaUzytkownikow);

              cout << "│ Dodano: " << imie << " " << nazwisko << endl;
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              getline(cin, temp);
            }
          else if(opcja[0] == 'q')
            {
              wyczyscKonsole();

              cout << "┌──────── USUŃ URZYTKOWNIKA ─────────┐" << endl;
              for(const auto &u : listaUzytkownikow)
                {
                  cout << "│ [" << u.id << "] Imie: " << u.imie
                       << ", Nazwisko: " << u.nazwisko << endl;
                }
              cout << "├────────────────────────────────────┤" << endl;
              cout << "│ Podaj ID urzytkownika do usunięcia: ";
              cin >> temp;
              cout << "├────────────────────────────────────┤" << endl;

              size_t poczatkowy_rozmiar = listaUzytkownikow.size();

              usunUrzytkownika(listaUzytkownikow, temp);

              if(listaUzytkownikow.size() < poczatkowy_rozmiar)
                {
                  cout << "│ Usunieto uzytkownika o ID: " << temp << endl;
                  zapiszUrzytkownika(listaUzytkownikow);
                }
              else
                {
                  cout << "│ Nie znaleziono uzytkownika o ID: " << temp
                       << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'e')
            {
              wyczyscKonsole();

              cout << "┌──────── LISTA KSIĄŻEK (" << listaKsiazek.size()
                   << (listaKsiazek.size() > 9 ? "─" : "") << ") ─────────┐"
                   << endl;
              cout << "┌────── LISTA UŻYTKOWNIKÓW ("
                   << listaUzytkownikow.size() << ") ─────────┐" << endl;
              if(listaUzytkownikow.size() != 0)
                {
                  for(const auto &u : listaUzytkownikow)
                    {
                      cout << "│ [" << u.id << "] Imie: " << u.imie
                           << ", Nazwisko: " << u.nazwisko << endl;
                    }
                }
              else
                {
                  cout << "│ Brak urzytkowników !!!" << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'z')
            {
              k = 1;
            }
        }
    }
  else if(istnieje == true)
    {
      while(k == 0)
        {
          wyczyscKonsole();

          cout << "┌──────────── KSIĘGARNIA ────────────┐" << endl;
          cout << "│ Wybierz opcje:                     │" << endl;
          cout << "│ - kup książkę (r)                  │" << endl;
          cout << "│ - usuń książkę (a)                 │" << endl;
          cout << "│ - sprzedaj książkę (f)             │" << endl;
          cout << "│ - wyswietl liste książek (s)       │" << endl;
          cout << "├────────────────────────────────────┤" << endl;
          cout << "│ - wyloguj (z)                      │" << endl;
          cout << "└────────────────────────────────────┘" << endl;
          cout << "---> : ";
          cin >> opcja;

          if(opcja.length() != 1)
            {
              opcja = 'x';
            }

          if(opcja[0] == 'w')
            {
              string tytul, autor, cena;

              wyczyscKonsole();

              cout << "┌────────── DODAJ KSIĄŻKĘ ───────────┐" << endl;
              cin.ignore();
              cout << "│ Tytul: ";
              getline(cin, tytul);
              cout << "│ Autor: ";
              getline(cin, autor);
              cout << "│ Cena: ";
              getline(cin, cena);
              cout << "├────────────────────────────────────┤" << endl;

              string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3)
                               + to_string(rand() % 1000);
              listaKsiazek.push_back(
                Ksiazka(tytul, autor, stof(cena), nowy_id, "ksiazka"));
              zapiszKsiazke(listaKsiazek);

              cout << "│ Dodano książkę: " << tytul << endl;
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              getline(cin, temp);
            }
          else if(opcja[0] == 'a')
            {
              wyczyscKonsole();

              cout << "┌─────────── USUŃ KSIĄŻKĘ ───────────┐" << endl;
              for(const auto &k : listaKsiazek)
                {
                  cout << "│ [" << k.id << "] Tytul: " << k.tytul
                       << ", Autor: " << k.autor << ", Cena: " << k.cena
                       << " zł" << endl;
                }
              cout << "├────────────────────────────────────┤" << endl;
              cout << "│ Podaj ID książki do usunięcia: ";
              cin >> temp;
              cout << "├────────────────────────────────────┤" << endl;

              size_t poczatkowy_rozmiar = listaKsiazek.size();
              usunKsiazke(listaKsiazek, temp);

              if(listaKsiazek.size() < poczatkowy_rozmiar)
                {
                  cout << "│ Usunieto książkę o ID: " << temp << endl;
                  zapiszKsiazke(listaKsiazek);
                }
              else
                {
                  cout << "│ Nie znaleziono książki o ID: " << temp << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 's')
            {
              wyczyscKonsole();

              cout << "┌──────── LISTA KSIĄŻEK (" << listaKsiazek.size()
                   << (listaKsiazek.size() > 9 ? "─" : "") << ") ─────────┐"
                   << endl;
              if(listaKsiazek.size() != 0)
                {
                  for(const auto &k : listaKsiazek)
                    {
                      if(k.id2 == login)
                        {
                          cout << "│ - Tytul: " << k.tytul
                               << ", Autor: " << k.autor
                               << ", Cena: " << k.cena << " zł" << endl;
                        }
                    }
                }
              else
                {
                  cout << "│ Brak książek !!!" << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'd')
            {
              string imie, nazwisko;

              wyczyscKonsole();

              cout << "┌──────── DODAJ UŻYTKOWNIKA ─────────┐" << endl;
              cin.ignore();
              cout << "│ Imie: ";
              getline(cin, imie);
              cout << "│ Nazwisko: ";
              getline(cin, nazwisko);
              cout << "├────────────────────────────────────┤" << endl;

              string nowy_id = imie.substr(0, 3) + nazwisko.substr(0, 3)
                               + to_string(rand() % 1000);
              listaUzytkownikow.push_back(
                Urzytkownik(imie, nazwisko, nowy_id, haslo));

              zapiszUrzytkownika(listaUzytkownikow);

              cout << "│ Dodano: " << imie << " " << nazwisko << endl;
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              getline(cin, temp);
            }
          else if(opcja[0] == 'q')
            {
              wyczyscKonsole();

              cout << "┌──────── USUŃ URZYTKOWNIKA ─────────┐" << endl;
              for(const auto &u : listaUzytkownikow)
                {
                  cout << "│ [" << u.id << "] Imie: " << u.imie
                       << ", Nazwisko: " << u.nazwisko << endl;
                }
              cout << "├────────────────────────────────────┤" << endl;
              cout << "│ Podaj ID urzytkownika do usunięcia: ";
              cin >> temp;
              cout << "├────────────────────────────────────┤" << endl;

              size_t poczatkowy_rozmiar = listaUzytkownikow.size();

              usunUrzytkownika(listaUzytkownikow, temp);

              if(listaUzytkownikow.size() < poczatkowy_rozmiar)
                {
                  cout << "│ Usunieto uzytkownika o ID: " << temp << endl;
                  zapiszUrzytkownika(listaUzytkownikow);
                }
              else
                {
                  cout << "│ Nie znaleziono uzytkownika o ID: " << temp
                       << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'e')
            {
              wyczyscKonsole();

              cout << "┌────── LISTA UŻYTKOWNIKÓW ("
                   << listaUzytkownikow.size() << ") ─────────┐" << endl;
              if(listaUzytkownikow.size() != 0)
                {
                  for(const auto &u : listaUzytkownikow)
                    {
                      cout << "│ [" << u.id << "] Imie: " << u.imie
                           << ", Nazwisko: " << u.nazwisko << endl;
                    }
                }
              else
                {
                  cout << "│ Brak urzytkowników !!!" << endl;
                }
              cout << "└────────────────────────────────────┘" << endl;
              cout << "<--- : ";
              cin.ignore();
              getline(cin, temp);
            }
          else if(opcja[0] == 'z')
            {
              k = 1;
            }
        }
    }

  return 0;
}