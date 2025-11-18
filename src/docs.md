### Zapisywalna Klasa
1. Utworzenie klasy która dziedziczy `Serializable`. Sprawia to że "dostaje" ona metody z `Serializable` które pozwalają na zapis danych
```c++
class NazwaKlasy : public Serializable<NazwaKlasy>
```
2. Parametry klasy to co ona "trzyma".
```c++
{
public:
  int id = 0;
  std::string test;
```

3. Każda zapisywalna klasa musi funkcje `static static_name()` zwracającą nazwę funkcji
```c++
  static const std::string &static_name()
  {
    static const std::string name = "NazwaKlasyJakas";
    return name;
  }
```

4. Pierwszą rzeczą, jaką trzeba zrobić w Constructorze Klasy to nazwać parametr name;
```c++
  NazwaKlasy()
  {
    this->name = static_name(); // to jest do zmiany
    [...]
  }
}; 
```
5. Oraz zarejestrować wszystkie parametry które chcemy by były zapisywalne:
```c++
registerField(
    "cos", // Nazwa parametru która będzie kluczem przy zapisie
   &NazwaKlasy::id // Wskaźnik pokazujący jak daleko od początku klasy znajduje się w zapisywany/odczytywany parametr
   defaultString<int>() // Funkcje formatujące, zamieniają wartość na tekst oraz tekst na wartość, defaultString działa dla numerow znaków i stringow.
   // <tu typ zmiennej>
)
```
```c++
registerField("name", &NazwaKlasy::test, defaultString<std::string>());
```