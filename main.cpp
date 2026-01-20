#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <cstdlib>
#include <ctime>
#include <clocale>
#include "Display.h"
#include "Ksiazka.h"
#include "Uzytkownik.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

void czekajNaEnter()
{
  cout << "\nNaciśnij ENTER, aby kontynuować...";
  string tmp;
  getline(cin, tmp);
}

bool jest_uzytkownik = false;
bool jest_admin = false;

class Opcje
{
public:
  Opcje() = delete;

  inline static const std::string DODAJ_KSIAZKE = "Dodaj Książkę";
  inline static const std::string USUN_KSIAZKE = "Usuń książke";
  inline static const std::string WYSWIETL_KSIAZKI = "Wyświetl książki";
  inline static const std::string DODAJ_UZYTKOWNIKA = "Dodaj użytkownika";
  inline static const std::string USUN_UZYTKOWNIKA = "Usuń użytkownika";
  inline static const std::string WYSWIETL_UZYTKOWNIKOW
    = "Wyświetl użytkowników";
  inline static const std::string SPRZEDAZ_KSIAZKI = "Sprzedarz książki";
  inline static const std::string SKUP_KSIAZKI = "Skup książki";
  inline static const std::string ZAMKNIJ = "Zamknij";

  static std::vector<string> PobierzWszystkie()
  {
    return {DODAJ_KSIAZKE,     USUN_KSIAZKE,     WYSWIETL_KSIAZKI,
            DODAJ_UZYTKOWNIKA, USUN_UZYTKOWNIKA, WYSWIETL_UZYTKOWNIKOW,
            SPRZEDAZ_KSIAZKI,  SKUP_KSIAZKI,     ZAMKNIJ};
  }
};

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

  Ksiazka::odczytajKsiazki(listaKsiazek);
  Uzytkownik::odczytajUzytkownikow(listaUzytkownikow);

  string login, haslo, opcja, user_input;
  string current_user_id;

  while(true)
    {
      int k = 0;
      string opcja;
      string login, haslo;
      bool istnieje = false;
      jest_uzytkownik = false;
      jest_admin = false;

      auto display = make_unique<Display>();
      display->box();
      display->add(make_unique<string>("System Księgarni"));
      display->show();

      login = Display::singleQuestion("Podaj login");
      haslo = Display::singleQuestion("Podaj hasło");

      cout << "Loged in with" << login << haslo << "\n";

      if(login == "admin" && haslo == "admin")
        {
          jest_admin = true;
        }

      for(const auto &u : listaUzytkownikow)
        {
          if(u.czyPoprawneDaneLogowania(login, haslo))
            {
              jest_uzytkownik = true;
              current_user_id = u.id;
              break;
            }
        }

      if(!jest_admin && !jest_uzytkownik)
        {
          auto errDisplay = make_unique<Display>();
          errDisplay->box();
          errDisplay->add(make_unique<string>("Błędne dane logowania!"));
          errDisplay->show();

          czekajNaEnter();
          continue;
        }

      bool wylogowano = false;
      while(!wylogowano)
        if(jest_admin) // Simplified check
          {
            while(k == 0)
              {
                Display::clearConsole();
                display->clear();
                display->box();
                display->add(make_unique<string>("[Księgarnia - Admin]"));
                display->sectionBreak();
                display->add(make_unique<string>("Wybierz czynność"));
                display->sectionBreak();
                display->ask(Opcje::PobierzWszystkie());

                display->show();
                if(Opcje::PobierzWszystkie().size() > 0)
                  {
                    opcja = Opcje::PobierzWszystkie()[display->current_anwser];
                  }

                if(opcja == Opcje::DODAJ_KSIAZKE)
                  {
                    Display::clearConsole();
                    string tytul = Display::singleQuestion("Podaj Tytul");
                    string autor = Display::singleQuestion("Podaj Autor");
                    string cena = Display::singleQuestion("Podaj Cena");

                    string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3)
                                     + to_string(rand() % 1000);
                    listaKsiazek.push_back(
                      Ksiazka(tytul, autor, stof(cena), nowy_id, "ksiazka"));
                    Ksiazka::zapiszKsiazki(listaKsiazek);

                    display->clear();
                    display->box();
                    display->add(
                      make_unique<string>("Dodano książkę: " + tytul));
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(opcja == Opcje::USUN_KSIAZKE)
                  {
                    Display::clearConsole();
                    display->clear();
                    display->box();
                    display->add(make_unique<string>("USUŃ KSIĄŻKĘ"));
                    display->sectionBreak();

                    if(!listaKsiazek.empty())
                      {
                        for(const auto &k : listaKsiazek)
                          {
                            display->add(make_unique<string>(
                              "- Tytul: " + k.tytul + ", Autor: " + k.autor
                              + ", Cena: " + to_string(k.cena) + " zł\n"));
                          }
                      }
                    display->show();

                    string user_input = Display::singleQuestion(
                      "Podaj ID książki do usunięcia");

                    size_t poczatkowy_rozmiar = listaKsiazek.size();
                    Ksiazka::usunKsiazke(listaKsiazek, user_input);

                    display->clear();
                    display->box();
                    if(listaKsiazek.size() < poczatkowy_rozmiar)
                      {
                        display->add(make_unique<string>(
                          "Usunieto książkę o ID: " + user_input));
                        Ksiazka::zapiszKsiazki(listaKsiazek);
                      }
                    else
                      {
                        display->add(make_unique<string>(
                          "Nie znaleziono książki o ID: " + user_input));
                      }
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(opcja == Opcje::WYSWIETL_KSIAZKI)
                  {
                    Display::clearConsole();
                    display->clear();
                    display->box();
                    display->add(make_unique<string>(
                      "LISTA KSIĄŻEK (" + to_string(listaKsiazek.size())
                      + ")"));
                    display->sectionBreak();

                    if(!listaKsiazek.empty())
                      {
                        for(const auto &k : listaKsiazek)
                          {
                            display->add(make_unique<string>(
                              "- Tytul: " + k.tytul + ", Autor: " + k.autor
                              + ", Cena: " + to_string(k.cena) + " zł"));
                          }
                      }
                    else
                      {
                        display->add(make_unique<string>("Brak książek !!!"));
                      }
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(opcja == Opcje::DODAJ_UZYTKOWNIKA)
                  {
                    Display::clearConsole();
                    string imie = Display::singleQuestion("Podaj Imie");
                    string nazwisko
                      = Display::singleQuestion("Podaj Nazwisko");
                    string haslo_usr = Display::singleQuestion("Podaj Hasło");

                    string nowy_id = imie.substr(0, 3) + nazwisko.substr(0, 3)
                                     + to_string(rand() % 1000);
                    listaUzytkownikow.push_back(
                      Uzytkownik(imie, nazwisko, nowy_id, haslo_usr));

                    Uzytkownik::zapiszUzytkownika(listaUzytkownikow);

                    display->clear();
                    display->box();
                    display->add(
                      make_unique<string>("Dodano: " + imie + " " + nazwisko));
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(opcja == Opcje::USUN_UZYTKOWNIKA)
                  {
                    Display::clearConsole();
                    display->clear();
                    display->box();
                    display->add(make_unique<string>("USUŃ UŻYTKOWNIKA"));
                    display->sectionBreak();
                    for(const auto &u : listaUzytkownikow)
                      {
                        display->add(make_unique<string>(
                          "[" + u.id + "] " + u.imie + " " + u.nazwisko));
                      }
                    display->show();

                    string user_input = Display::singleQuestion(
                      "Podaj ID użytkownika do usunięcia");

                    size_t poczatkowy_rozmiar = listaUzytkownikow.size();
                    Uzytkownik::usunUzytkownika(listaUzytkownikow, user_input);

                    display->clear();
                    display->box();
                    if(listaUzytkownikow.size() < poczatkowy_rozmiar)
                      {
                        display->add(make_unique<string>(
                          "Usunieto uzytkownika o ID: " + user_input));
                        Uzytkownik::zapiszUzytkownika(listaUzytkownikow);
                      }
                    else
                      {
                        display->add(make_unique<string>(
                          "Nie znaleziono uzytkownika o ID: " + user_input));
                      }
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(opcja == Opcje::ZAMKNIJ)
                  {
                    k = 1;
                    wylogowano = true;
                  }
              }
          }
        else if(jest_uzytkownik)
          {
            while(k == 0)
              {
                Display::clearConsole();
                display->clear();
                display->box();
                display->add(make_unique<string>("[Księgarnia - Użytkownik]"));
                display->sectionBreak();
                display->add(make_unique<string>("Wybierz opcje:"));
                display->sectionBreak();

                vector<string> user_options
                  = {"Dodaj książkę", "Usuń książkę", "Wyświetl moje książki",
                     "Wyloguj"};
                display->ask(user_options);
                display->show();

                int user_choice = display->current_anwser;

                if(user_choice == 0)
                  {
                    Display::clearConsole();
                    string tytul = Display::singleQuestion("Podaj Tytul");
                    string autor = Display::singleQuestion("Podaj Autor");
                    string cena = Display::singleQuestion("Podaj Cena");

                    string nowy_id = tytul.substr(0, 3) + autor.substr(0, 3)
                                     + to_string(rand() % 1000);
                    listaKsiazek.push_back(
                      Ksiazka(tytul, autor, stof(cena), nowy_id, "ksiazka"));
                    Ksiazka::zapiszKsiazki(listaKsiazek);

                    display->clear();
                    display->box();
                    display->add(
                      make_unique<string>("Dodano książkę: " + tytul));
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(user_choice == 1)
                  {
                    Display::clearConsole();
                    display->clear();
                    display->box();
                    display->add(make_unique<string>("USUŃ KSIĄŻKĘ"));
                    display->sectionBreak();
                    if(!listaKsiazek.empty())
                      {
                        for(const auto &k : listaKsiazek)
                          {
                            display->add(make_unique<string>(
                              "- Tytul: " + k.tytul + ", Autor: " + k.autor
                              + ", Cena: " + to_string(k.cena) + " zł\n"));
                          }
                      }
                    display->show();
                    string uid = Display::singleQuestion(
                      "Podaj ID książki do usunięcia");

                    size_t poczatkowy_rozmiar = listaKsiazek.size();
                    Ksiazka::usunKsiazke(listaKsiazek, uid);

                    display->clear();
                    display->box();
                    if(listaKsiazek.size() < poczatkowy_rozmiar)
                      {
                        display->add(make_unique<string>(
                          "Usunieto książkę o ID: " + uid));
                        Ksiazka::zapiszKsiazki(listaKsiazek);
                      }
                    else
                      {
                        display->add(make_unique<string>(
                          "Nie znaleziono książki o ID: " + uid));
                      }
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(user_choice == 2)
                  {
                    Display::clearConsole();
                    display->clear();
                    display->box();
                    display->add(make_unique<string>("MOJE KSIĄŻKI"));
                    display->sectionBreak();
                    bool found = false;
                    for(const auto &k : listaKsiazek)
                      {
                        if(k.id2 == login)
                          {
                            display->add(make_unique<string>(
                              "- " + k.tytul + " " + k.autor + " "
                              + to_string(k.cena) + " zł"));
                            found = true;
                          }
                      }
                    if(!found)
                      display->add(make_unique<string>("Brak książek"));
                    display->show();
                    Display::singleQuestion("Wciśnij Enter, aby kontynuować");
                  }
                else if(user_choice == 3)
                  {
                    k = 1;
                    wylogowano = true;
                  }
              }
          }
        else
          {
            // Should not be reachable if logic is correct, but break loop just
            // in case
            wylogowano = true;
          }
    }
}
