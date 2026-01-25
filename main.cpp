#include <cstdio>
#include "Raport.h"
#include <exception>
#include <iostream>
#include <memory>
#include <string>
#include <list>
#include <cstdlib>
#include <sstream>
#include <ctime>
#include <clocale>
#include <vector>
#include "Display.h"
#include "Ksiazka.h"
#include "Uzytkownik.h"
#ifdef _WIN32
#include <windows.h>
#endif

using namespace std;

/**
 * @brief Struktura przechowująca stałe tekstowe opcji menu.
 */
struct MenuOpcje
{
  /// @brief Tekst opcji dodawania książki.
  inline static const std::string DODAJ_KSIAZKE = "Dodaj Książkę";
  /// @brief Tekst opcji usuwania książki.
  inline static const std::string USUN_KSIAZKE = "Usuń książkę";
  /// @brief Tekst opcji wyświetlania wszystkich książek.
  inline static const std::string WYSWIETL_WSZYSTKIE_KSIAZKI
    = "Wyświetl wszystkie książki";
  /// @brief Tekst opcji wyświetlania książek w sklepie.
  inline static const std::string WYSWIETL_KSIAZKI_SKLEP
    = "Wyświetl książki w sklepie";
  /// @brief Tekst opcji wyświetlania sprzedanych książek.
  inline static const std::string WYSWIETL_SPRZEDANE
    = "Wyświetl książki sprzedane";

  /// @brief Tekst opcji dodawania użytkownika.
  inline static const std::string DODAJ_UZYTKOWNIKA = "Dodaj użytkownika";
  /// @brief Tekst opcji usuwania użytkownika.
  inline static const std::string USUN_UZYTKOWNIKA = "Usuń użytkownika";
  /// @brief Tekst opcji wyświetlania użytkowników.
  inline static const std::string WYSWIETL_UZYTKOWNIKOW
    = "Wyświetl użytkowników";

  /// @brief Tekst opcji kupna książki przez użytkownika.
  inline static const std::string KUP_KSIAZKE = "Kup książkę (Skup)";
  /// @brief Tekst opcji sprzedaży książki do sklepu.
  inline static const std::string SPRZEDAJ_KSIAZKE
    = "Sprzedaj książkę (do sklepu)";
  /// @brief Tekst opcji usuwania własnej książki.
  inline static const std::string USUN_SWOJA_KSIAZKE = "Usuń swoją książkę";

  /// @brief Tekst opcji wylogowania.
  inline static const std::string WYLOGUJ = "Wyloguj";

  /**
   * @brief Zwraca wektor wszystkich opcji dostępnych dla administratora.
   * @return Wektor stringów z nazwami opcji.
   */
  static std::vector<string> PobierzOpcjeAdmina()
  {
    return {
      DODAJ_KSIAZKE,      USUN_KSIAZKE,           WYSWIETL_WSZYSTKIE_KSIAZKI,
      DODAJ_UZYTKOWNIKA,  USUN_UZYTKOWNIKA,       WYSWIETL_UZYTKOWNIKOW,
      WYSWIETL_SPRZEDANE, WYSWIETL_KSIAZKI_SKLEP, WYLOGUJ};
  }

  /**
   * @brief Zwraca wektor wszystkich opcji dostępnych dla zwykłego użytkownika.
   * @return Wektor stringów z nazwami opcji.
   */
  static std::vector<string> PobierzOpcjeUzytkownika()
  {
    return {KUP_KSIAZKE, SPRZEDAJ_KSIAZKE, USUN_SWOJA_KSIAZKE,
            WYSWIETL_KSIAZKI_SKLEP, WYLOGUJ};
  }
};

class RolaSystemu
{
public:
  virtual std::vector<std::string> pobierzMenu() const = 0;

  virtual std::string naglowekPowitalny(const std::string &login) const = 0;

  virtual ~RolaSystemu() = default;
};

class RolaAdministrator : public RolaSystemu
{
public:
  std::vector<std::string> pobierzMenu() const override
  {
    return MenuOpcje::PobierzOpcjeAdmina();
  }

  std::string naglowekPowitalny(const std::string &login) const override
  {
    return "Witaj, Administratorze systemu!\n";
  }
};

class RolaKlient : public RolaSystemu
{
public:
  std::vector<std::string> pobierzMenu() const override
  {
    return MenuOpcje::PobierzOpcjeUzytkownika();
  }

  std::string naglowekPowitalny(const std::string &login) const override
  {
    return "Witaj, " + login + "! (Klient)\n";
  }
};

/**
 * @brief Klasa zarządzająca interfejsem użytkownika i logiką biznesową
 * księgarni. Korzysta z klasy Display do wyświetlania danych.
 */
class KsiegarniaUI
{
private:
  list<Ksiazka> &listaKsiazek;
  list<Uzytkownik> &listaUzytkownikow;
  string current_user_id;

  /**
   * @brief Wyświetla sformatowany komunikat w ramce i oczekuje na
   * potwierdzenie.
   * @param tytul Tytuł sekcji.
   * @param tresc Treść komunikatu.
   */
  void pokazKomunikat(const string &tytul, const string &tresc)
  {
    Raport::printf("Wyświeltam komunikat: {}", tytul);
    Display::clear();
    Display::isBox = true;
    Display::add(make_unique<string>(tytul + "\n"));
    Display::sectionBreak();
    Display::add(make_unique<string>(tresc + "\n"));
    Display::ask({"Ok"});
    Display::show();
  }

  /**
   * @brief Tworzy sformatowany ciąg znaków reprezentujący książkę.
   * @param k Obiekt książki.
   * @return Sformatowany string.
   */
  string formatujKsiazke(const Ksiazka &k)
  {
    Raport::printf("Sformatowano książkę");
    stringstream ss;
    ss << "[" << k.id << "] " << k.tytul << " - " << k.autor << " ("
       << (k.id2 == "sklep" ? "sklep" : "użytkownik " + k.id2) << ") - "
       << k.cena << " zł";
    return ss.str();
  }

  /**
   * @brief Wyświetla listę opcji i pozwala użytkownikowi wybrać jedną z nich.
   * @param tytul Tytuł ekranu.
   * @param opcje Lista dostępnych opcji do wyboru.
   * @param prompt Pytanie zadawane użytkownikowi.
   * @return Wybrana opcja jako string lub pusty string, jeśli wybrano powrót.
   */
  string wybierzZListy(const string &tytul, const vector<string> &opcje,
                       const string &prompt)
  {
    Raport::printf("Wybrano z listy");
    Display::clear();
    Display::isBox = true;
    Display::add(make_unique<string>(tytul + "\n"));

    if(opcje.empty())
      {
        return "";
      }

    Display::Question q;
    q.prompt = prompt + "\n";
    q.options = opcje;
    q.options.push_back("Wróć");

    auto result = Display::multiQuestion({q});
    if(result.empty() || result[0] == "Wróć")
      return "";
    return result[0];
  }

public:
  /**
   * @brief Konstruktor klasy KsiegarniaUI.
   * @param ksiazki Referencja do listy książek.
   * @param uzytkownicy Referencja do listy użytkowników.
   */
  KsiegarniaUI(list<Ksiazka> &ksiazki, list<Uzytkownik> &uzytkownicy)
      : listaKsiazek(ksiazki), listaUzytkownikow(uzytkownicy)
  {
    Raport::printf("Zainicjalizowano UI");
  }

  /**
   * @brief Ustawia ID aktualnie zalogowanego użytkownika.
   * @param id ID użytkownika.
   */
  void ustawZalogowanegoUzytkownika(const string &id) { current_user_id = id; }

  /**
   * @brief Obsługuje proces dodawania nowej książki przez administratora.
   */
  void dodajKsiazke()
  {
    Display::clear();
    Display::isBox = true;
    Display::add(make_unique<string>("DODAJ KSIĄŻKĘ\n"));

    vector<Display::Question> questions
      = {{"Podaj tytuł", {}}, {"Podaj autora", {}}, {"Podaj cenę", {}}};

    auto answers = Display::multiQuestion(questions);
    if(answers.size() < 3)
      return;

    string tytul = answers[0];
    string autor = answers[1];
    string cena_str = answers[2];
    float cena = 0.0f;

    try
      {
        cena = stof(cena_str);
      }
    catch(...)
      {
        pokazKomunikat("Błąd", "Błędna cena!");
        return;
      }

    string nowy_id
      = tytul.substr(0, 3) + autor.substr(0, 3) + to_string(rand() % 1000);
    listaKsiazek.push_back(Ksiazka(tytul, autor, cena, nowy_id, "sklep"));
    Ksiazka::zapiszKsiazki(listaKsiazek);

    pokazKomunikat("Sukces", "Dodano książkę do sklepu!");
    Raport::printf("Dodano książkę o tytule: {}", tytul);
  }

  /**
   * @brief Obsługuje proces usuwania książki z systemu.
   */
  void usunKsiazke()
  {
    vector<string> opcje;
    vector<string> ids;
    for(const auto &k : listaKsiazek)
      {
        opcje.push_back(formatujKsiazke(k));
        ids.push_back(k.id);
      }

    if(opcje.empty())
      {
        pokazKomunikat("Usuń książkę", "Brak książek do usunięcia.");
        return;
      }

    string wybor
      = wybierzZListy("USUŃ KSIĄŻKĘ", opcje, "Wybierz książkę do usunięcia:");
    if(wybor.empty())
      return;

    string id_to_remove = "";
    int index = 0;
    for(const auto &opt : opcje)
      {
        if(opt == wybor)
          {
            id_to_remove = ids[index];
            break;
          }
        index++;
      }

    if(!id_to_remove.empty())
      {
        size_t stary_rozmiar = listaKsiazek.size();
        Ksiazka::usunKsiazke(listaKsiazek, id_to_remove);
        if(listaKsiazek.size() < stary_rozmiar)
          {
            Ksiazka::zapiszKsiazki(listaKsiazek);
            pokazKomunikat("Sukces", "Usunięto książkę!");
          }
      }

    Raport::printf("Usunięto {}", id_to_remove);
  }

  /**
   * @brief Wyświetla listę książek, filtrując je według typu.
   * @param filterType Typ filtrowania: "all", "sklep", "sprzedane".
   */
  void wyswietlKsiazki(const string &filterType = "all")
  {
    Display::clear();
    Display::isBox = true;

    string title = "LISTA KSIĄŻEK\n";
    if(filterType == "sklep")
      title = "KSIĄŻKI W SKLEPIE\n";
    if(filterType == "sprzedane")
      title = "KSIĄŻKI SPRZEDANE (U UŻYTKOWNIKÓW)\n";

    Display::add(make_unique<string>(title));

    bool found = false;
    for(const auto &k : listaKsiazek)
      {
        bool show = false;
        if(filterType == "all")
          show = true;
        else if(filterType == "sklep" && k.id2 == "sklep")
          show = true;
        else if(filterType == "sprzedane" && k.id2 != "sklep")
          show = true;

        if(show)
          {
            Display::add(make_unique<string>(formatujKsiazke(k) + "\n"));
            found = true;
          }
      }

    if(!found)
      {
        Display::add(make_unique<string>("Brak książek w tej kategorii.\n"));
      }

    Display::ask({"Wróć"});
    Display::show();

    Raport::printf("Zakończono operację listy książek");
  }

  /**
   * @brief Obsługuje proces dodawania nowego użytkownika.
   */
  void dodajUzytkownika()
  {
    Display::clear();
    Display::isBox = true;
    Display::add(make_unique<string>("DODAJ UŻYTKOWNIKA\n"));

    vector<Display::Question> questions
      = {{"Podaj imię", {}}, {"Podaj nazwisko", {}}, {"Podaj hasło", {}}};

    auto answers = Display::multiQuestion(questions);
    if(answers.size() < 3)
      return;

    string imie = answers[0];
    string nazwisko = answers[1];
    string haslo = answers[2];

    string nowy_id
      = imie.substr(0, 3) + nazwisko.substr(0, 3) + to_string(rand() % 1000);
    listaUzytkownikow.push_back(Uzytkownik(imie, nazwisko, nowy_id, haslo));
    Uzytkownik::zapiszUzytkownika(listaUzytkownikow);

    pokazKomunikat("Sukces", "Dodano użytkownika (" + nowy_id + ")!");

    Raport::printf("Dodano użytkownika {}", nowy_id);
  }

  /**
   * @brief Obsługuje proces usuwania użytkownika.
   */
  void usunUzytkownika()
  {
    vector<string> opcje;
    vector<string> ids;
    for(const auto &u : listaUzytkownikow)
      {
        opcje.push_back("[" + u.id + "] " + u.imie + " " + u.nazwisko);
        ids.push_back(u.id);
      }

    if(opcje.empty())
      {
        pokazKomunikat("Usuń użytkownika", "Brak użytkowników do usunięcia.");
        return;
      }

    string wybor
      = wybierzZListy("USUŃ UŻYTKOWNIKA", opcje, "Wybierz użytkownika:");
    if(wybor.empty())
      return;

    string id_to_remove = "";
    int index = 0;
    for(const auto &opt : opcje)
      {
        if(opt == wybor)
          {
            id_to_remove = ids[index];
            break;
          }
        index++;
      }

    size_t stary_rozmiar = listaUzytkownikow.size();
    Uzytkownik::usunUzytkownika(listaUzytkownikow, id_to_remove);

    if(listaUzytkownikow.size() < stary_rozmiar)
      {
        Uzytkownik::zapiszUzytkownika(listaUzytkownikow);
        pokazKomunikat("Sukces", "Usunięto użytkownika.");
      }
    else
      {
        pokazKomunikat("Błąd", "Nie znaleziono użytkownika.");
      }
    Raport::printf("Usunięto użytkownika: {}", id_to_remove);
  }

  /**
   * @brief Wyświetla listę wszystkich zarejestrowanych użytkowników.
   */
  void wyswietlUzytkownikow()
  {
    Display::clear();
    Display::isBox = true;
    Display::add(make_unique<string>("LISTA UŻYTKOWNIKÓW\n"));

    if(listaUzytkownikow.empty())
      {
        Display::add(make_unique<string>("Brak użytkowników!\n"));
      }
    else
      {
        for(const auto &u : listaUzytkownikow)
          {
            Display::add(make_unique<string>("[" + u.id + "] " + u.imie + " "
                                             + u.nazwisko + "\n"));
          }
      }
    Display::ask({"Wróć"});
    Display::show();

    Raport::printf("Zakończono wyświetlanie użytkowników");
  }

  /**
   * @brief Obsługuje proces kupna książki przez zalogowanego użytkownika.
   */
  void kupKsiazke()
  {
    vector<string> opcje;
    vector<string> ids;

    for(const auto &k : listaKsiazek)
      {
        if(k.id2 == "sklep")
          {
            opcje.push_back(formatujKsiazke(k));
            ids.push_back(k.id);
          }
      }

    if(opcje.empty())
      {
        pokazKomunikat("Kup książkę", "Brak książek w sklepie.");
        return;
      }

    string wybor = wybierzZListy("DOSTĘPNE KSIĄŻKI W SKLEPIE", opcje,
                                 "Wybierz książkę do kupienia:");
    if(wybor.empty())
      return;

    string id_ksiazki = "";
    int index = 0;
    for(const auto &opt : opcje)
      {
        if(opt == wybor)
          {
            id_ksiazki = ids[index];
            break;
          }
        index++;
      }

    bool kupiono = false;
    for(auto &k : listaKsiazek)
      {
        if(k.id == id_ksiazki && k.id2 == "sklep")
          {
            k.id2 = current_user_id;
            kupiono = true;
            break;
          }
      }

    if(kupiono)
      {
        Ksiazka::zapiszKsiazki(listaKsiazek);
        pokazKomunikat("Sukces", "Pomyślnie kupiono książkę!");
      }
    else
      {
        pokazKomunikat("Błąd", "Książka nie jest już dostępna.");
      }
    Raport::printf("Użytkownik: {}, kupił: {}", current_user_id, id_ksiazki);
  }

  /**
   * @brief Obsługuje proces sprzedaży książki przez użytkownika do sklepu.
   */
  void sprzedajKsiazke()
  {
    vector<string> opcje;
    vector<string> ids;

    for(const auto &k : listaKsiazek)
      {
        if(k.id2 == current_user_id)
          {
            opcje.push_back(formatujKsiazke(k));
            ids.push_back(k.id);
          }
      }

    if(opcje.empty())
      {
        pokazKomunikat("Sprzedaj książkę", "Nie posiadasz żadnych książek.");
        return;
      }

    string wybor = wybierzZListy("TWOJE KSIĄŻKI", opcje,
                                 "Wybierz książkę do sprzedaży sklepowi:");
    if(wybor.empty())
      return;

    string id_ksiazki = "";
    int index = 0;
    for(const auto &opt : opcje)
      {
        if(opt == wybor)
          {
            id_ksiazki = ids[index];
            break;
          }
        index++;
      }

    bool sprzedano = false;
    for(auto &k : listaKsiazek)
      {
        if(k.id == id_ksiazki && k.id2 == current_user_id)
          {
            k.id2 = "sklep";
            sprzedano = true;
            break;
          }
      }

    if(sprzedano)
      {
        Ksiazka::zapiszKsiazki(listaKsiazek);
        pokazKomunikat("Sukces", "Pomyślnie sprzedano książkę sklepowi!");
      }
    else
      {
        pokazKomunikat("Błąd", "Nie posiadasz takiej książki.");
      }

    Raport::printf("Odsprzedano książkę: {}", id_ksiazki);
  }

  /**
   * @brief Obsługuje proces trwałego usuwania książki przez użytkownika.
   */
  void usunSwojaKsiazke()
  {
    vector<string> opcje;
    vector<string> ids;

    for(const auto &k : listaKsiazek)
      {
        if(k.id2 == current_user_id)
          {
            opcje.push_back(formatujKsiazke(k));
            ids.push_back(k.id);
          }
      }

    if(opcje.empty())
      {
        pokazKomunikat("Usuń swoją książkę", "Nie posiadasz żadnych książek.");
        return;
      }

    string wybor = wybierzZListy("TWOJE KSIĄŻKI", opcje,
                                 "Wybierz książkę do trwałego usunięcia:");
    if(wybor.empty())
      return;

    string id_ksiazki = "";
    int index = 0;
    for(const auto &opt : opcje)
      {
        if(opt == wybor)
          {
            id_ksiazki = ids[index];
            break;
          }
        index++;
      }

    size_t stary_rozmiar = listaKsiazek.size();
    bool wlasciciel = false;
    for(const auto &k : listaKsiazek)
      {
        if(k.id == id_ksiazki && k.id2 == current_user_id)
          {
            wlasciciel = true;
            break;
          }
      }

    if(wlasciciel)
      {
        Ksiazka::usunKsiazke(listaKsiazek, id_ksiazki);
        if(listaKsiazek.size() < stary_rozmiar)
          {
            Ksiazka::zapiszKsiazki(listaKsiazek);
            pokazKomunikat("Sukces", "Usunięto książkę.");
          }
      }
    else
      {
        pokazKomunikat("Błąd", "Nie udało się usunąć książki.");
      }
    Raport::printf("Użytkownik {} niszczy książkę {}", current_user_id,
                   id_ksiazki);
  }
};

int main()
{
  Raport::init();
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
  Raport::printf("Odczyt danych zakończony");

  KsiegarniaUI ui(listaKsiazek, listaUzytkownikow);

  Raport::printf("Zakończono incjalizację programu");

  while(true)
    {
      Display::clear();
      Display::isBox = true;
      Display::add(make_unique<string>("KSIĘGARNIA - LOGOWANIE\n"));

      vector<Display::Question> loginForm = {{"Login", {}}, {"Hasło", {}}};

      auto credentials = Display::multiQuestion(loginForm);
      if(credentials.size() < 2)
        continue;

      string login = credentials[0];
      string haslo = credentials[1];

      bool jest_admin = (login == "admin" && haslo == "admin");
      bool jest_uzytkownik = false;
      string user_id = "";

      if(!jest_admin)
        {
          Raport::printf("Następuje próba zalogowania jako użytkownik");
          for(const auto &u : listaUzytkownikow)
            {
              if(u.czyPoprawneDaneLogowania(login, haslo))
                {
                  jest_uzytkownik = true;
                  user_id = u.id;
                  break;
                }
            }
        }

      if(!jest_admin && !jest_uzytkownik)
        {
          Display::clear();
          Display::isBox = true;
          Display::add(make_unique<string>("Błędne dane logowania!\n"));
          Display::ask({"Spróbuj ponownie"});
          Display::show();
          continue;
        }

      ui.ustawZalogowanegoUzytkownika(jest_admin ? "admin" : user_id);

      bool wylogowano = false;

      std::unique_ptr<RolaSystemu> aktualnaRola;

      if(jest_admin)
        {
          aktualnaRola = std::make_unique<RolaAdministrator>();
          Raport::printf("Zalogowano jako administrator");
        }
      else

        aktualnaRola = std::make_unique<RolaKlient>();

      while(!wylogowano)

        {
          Display::clear();
          Display::isBox = true;

          string naglowek = aktualnaRola->naglowekPowitalny(login);

          Display::add(make_unique<string>(naglowek));
          Display::sectionBreak();

          vector<string> opcjeMenu = aktualnaRola->pobierzMenu();
          Display::Question menuQ;
          menuQ.prompt = "Wybierz akcję:\n";
          menuQ.options = opcjeMenu;

          auto response = Display::multiQuestion({menuQ});
          string opcja = response.empty() ? "" : response[0];

          if(opcja == MenuOpcje::WYLOGUJ)
            {
              Raport::printf("Użytkownik wykonuje: WYLOGUJ");
              wylogowano = true;
            }
          else if(opcja == MenuOpcje::DODAJ_KSIAZKE)
            {
              Raport::printf("Użytkownik wykonuje: DODAJ_KSIAZKE");
              ui.dodajKsiazke();
            }
          else if(opcja == MenuOpcje::USUN_KSIAZKE)
            {
              Raport::printf("Użytkownik wykonuje: USUN_KSIAZKE");
              ui.usunKsiazke();
            }
          else if(opcja == MenuOpcje::WYSWIETL_WSZYSTKIE_KSIAZKI)
            {
              Raport::printf(
                "Użytkownik wykonuje: WYSWIETL_WSZYSTKIE_KSIAZKI");
              ui.wyswietlKsiazki("all");
            }
          else if(opcja == MenuOpcje::DODAJ_UZYTKOWNIKA)
            {
              Raport::printf("Użytkownik wykonuje: DODAJ_UZYTKOWNIKA");
              ui.dodajUzytkownika();
            }
          else if(opcja == MenuOpcje::USUN_UZYTKOWNIKA)
            {
              Raport::printf("Użytkownik wykonuje: USUN_UZYTKOWNIKA");
              ui.usunUzytkownika();
            }
          else if(opcja == MenuOpcje::WYSWIETL_UZYTKOWNIKOW)
            {
              Raport::printf("Użytkownik wykonuje: WYSWIETL_UZYTKOWNIKOW");
              ui.wyswietlUzytkownikow();
            }
          else if(opcja == MenuOpcje::WYSWIETL_SPRZEDANE)
            {
              Raport::printf("Użytkownik wykonuje: WYSWIETL_SPRZEDANE");
              ui.wyswietlKsiazki("sprzedane");
            }
          else if(opcja == MenuOpcje::WYSWIETL_KSIAZKI_SKLEP)
            {
              Raport::printf("Użytkownik wykonuje: WYSWIETL_KSIAZKI_SKLEP");
              ui.wyswietlKsiazki("sklep");
            }
          else if(opcja == MenuOpcje::KUP_KSIAZKE)
            {
              Raport::printf("Użytkownik wykonuje: KUP_KSIAZKE");
              ui.kupKsiazke();
            }
          else if(opcja == MenuOpcje::SPRZEDAJ_KSIAZKE)
            {
              Raport::printf("Użytkownik wykonuje: SPRZEDAJ_KSIAZKE");
              ui.sprzedajKsiazke();
            }
          else if(opcja == MenuOpcje::USUN_SWOJA_KSIAZKE)
            {
              Raport::printf("Użytkownik wykonuje: USUN_SWOJA_KSIAZKE");
              ui.usunSwojaKsiazke();
            }
        }
    }

  return 0;
}
