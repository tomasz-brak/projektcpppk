# Sprawozdanie z projektu: System obsługi księgarni

## 1. Temat pracy
Tematem pracy jest zaprojektowanie i zaimplementowanie konsolowej aplikacji w języku C++ służącej do zarządzania księgarnią. System umożliwia obsługę dwóch typów użytkowników: administratora (zarządzanie asortymentem i użytkownikami) oraz klienta (kupno i sprzedaż książek).

## 2. Nazwiska autorów
*   Tomasz Brak
*   Kamil Kwiecień
*   Przemysław Niemczyński

## 3. Wstęp teoretyczny
Projekt został zrealizowany w paradygmacie programowania obiektowego przy użyciu języka C++.

W projekcie wykorzystano następujące zagadnienia i technologie:
*   **Programowanie Obiektowe:** Zastosowano klasy do reprezentacji głównych części systemu (`Ksiazka`, `Uzytkownik`) oraz do obsługi interfejsu (`Display`). Wykorzystano mechanizm hermetyzacji.
*   **Obsługa plików (I/O):** Trwałość danych zapewniono poprzez zapis i odczyt z plików tekstowych (`ksiazki.txt`, `uzytkownicy.txt`) przy użyciu biblioteki `<fstream>`.
*   **Algorytmy:** Wykorzystano funkcje lambda w połączeniu z metodą `remove_if` do usuwania obiektów z list.

## 4. Opis implementacji z fragmentami kodu źródłowego

Aplikacja składa się z trzech głównych modułów: zarządzania danymi (`Ksiazka`, `Uzytkownik`), silnika interfejsu (`Display`) oraz logiki sterującej (`main.cpp`, `KsiegarniaUI`).

### 4.1. Klasa `Ksiazka`
Odpowiada za reprezentację pojedynczej książki. Posiada pola takie jak tytuł, autor, cena, ID oraz ID właściciela (może to być "sklep" lub ID użytkownika). Metoda `daneDoZapisu` przygotowuje string w formacie CSV.

```cpp
// Fragment pliku Ksiazka.cpp
string Ksiazka::daneDoZapisu() const
{
  stringstream dane;
  // Format: ID;Tytuł;Autor;Cena;ID_Właściciela
  dane << id << ";" << tytul << ";" << autor << ";" << cena << ";" << id2;
  return dane.str();
}
```

### 4.2. Klasa `Uzytkownik` i bezpieczeństwo
Klasa przechowuje dane użytkowników. Ciekawym elementem jest funkcja `xorString`, która szyfruje hasło kluczem statycznym przed zapisem do pliku, co zwiększa podstawowe bezpieczeństwo danych.

```cpp
// Fragment pliku Uzytkownik.cpp
string xorString(const string &input, const string &key)
{
  string output = input;
  size_t key_len = key.length();
  for(size_t i = 0; i < input.length(); ++i)
    {
      output[i] = input[i] ^ key[i % key_len];
    }
  return output;
}
```
*Kontynuując rozwój aplikacji należało by zastąpić tą opcje bardziej zaawansowanym algorytmem kryptograficznym*

### 4.3. Klasa `Display`
Klasa odpowiadająca za renderowanie interfejsu w konsoli. Obsługuje rysowanie ramek, menu wyboru (nawigacja strzałkami) oraz pobieranie znaków.

Metoda `show()` zawiera główną pętlę rysującą interfejs:

```cpp
// Fragment pliku Display.cpp - obsługa nawigacji w menu
if(k == UP)
  {
    current_anwser++;
  }
else if(k == DOWN)
  {
    current_anwser--;
  }
else if(k == ENTER)
  {
    anwsered = true;
  }
```

### 4.4. Logika biznesowa (`KsiegarniaUI`)
Klasa ta łączy dane z interfejsem. Metody takie jak `kupKsiazke` zmieniają właściciela książki (pole `id2`) z "sklep" na ID zalogowanego użytkownika.

```cpp
// Fragment logiki kupna książki w main.cpp (metoda kupKsiazke)
if(k.id == id_ksiazki && k.id2 == "sklep")
  {
    k.id2 = current_user_id; // Przypisanie książki do użytkownika
    kupiono = true;
    break;
  }
```

## 5. Przykłady działania ze zrzutami ekranu

Poniżej przedstawiono scenariusz działania aplikacji, obejmujący logowanie, widok administratora oraz operacje użytkownika.

### 5.1. Ekran logowania
![](https://i.imgur.com/Ain6bN8.png)

### 5.2. Menu Administratora
Po zalogowaniu jako administrator, dostępny jest pełny wachlarz opcji, w tym dodawanie książek i zarządzanie użytkownikami. 
![](https://i.imgur.com/5g3MvOq.png)
*Możemy zmieniać opcje z użyciem strzałek.*
![](https://i.imgur.com/HrGcLk1.png)

### 5.3. Dodawanie nowej książki
Administrator może dodać nową pozycję do sklepu, podając tytuł, autora i cenę. ID książki jest generowane automatycznie.
![](https://i.imgur.com/vhsuxhQ.png)
![](https://i.imgur.com/msmAdY8.png)

### 5.4. Menu Użytkownika i zakup książki
Zwykły użytkownik posiada ograniczone menu. Może przeglądać ofertę sklepu i dokonać zakupu. Zakupiona książka znika z oferty sklepu, a pojawia się w ekwipunku użytkownika ("Twoje książki").
![](https://i.imgur.com/ZAHnb2d.png)
![](https://i.imgur.com/8nEgQJO.png)

### 5.5. Wyświetlanie listy użytkowników (Opcja Administratora)
Administrator ma wgląd w listę zarejestrowanych klientów.
![](https://i.imgur.com/Hhn5WAM.png)
## 6. Wnioski

Zrealizowany projekt spełnia założenia systemu obsługi księgarni. Udało się stworzyć aplikację, która:
1.  **Jest trwała:** Dane nie znikają po wyłączeniu programu dzięki zastosowaniu plików tekstowych.
2.  **Jest interaktywna:** Zastosowanie własnej klasy `Display` pozwoliło na stworzenie menu sterowanego strzałkami, co znacznie poprawia User Experience (UX) w porównaniu do standardowego wpisywania numerów opcji.
3.  **Zapewnia separację uprawnień:** Rozdzielono logikę dla administratora i zwykłego użytkownika.

Możliwe kierunki rozwoju aplikacji to:
*   Wdrożenie bazy danych SQL zamiast plików tekstowych.
*   Dodanie hashowania haseł (np. bcrypt) zamiast prostego XOR.
*   Rozbudowa systemu o kategorie książek i historię transakcji.
