# 🧬 Conway's Game of Life - SFML Edition

Interaktywna symulacja automatu komórkowego "Gra w Życie" stworzona w C++ z wykorzystaniem biblioteki graficznej SFML. Projekt oferuje zaawansowane możliwości manipulacji siatką, dynamiczne wprowadzanie danych oraz osadzone zasoby binarne.

## 🚀 Kluczowe Cechy (Technical Highlights)

* **W pełni Statyczne EXE:** Aplikacja została skompilowana z linkowaniem statycznym bibliotek SFML, co oznacza brak konieczności posiadania plików `.dll`.
* **Embedded Resources:** Czcionka `arial.ttf` została osadzona bezpośrednio w kodzie binarnym (`openFromMemory`), dzięki czemu program jest pojedynczym, niezależnym plikiem wykonywalnym.
* **Wydajność:** Wykorzystanie algorytmu Double Buffering (`std::vector::swap`) oraz optymalizacja obliczeń sąsiedztwa komórek.
* **Interaktywne UI:** Dynamiczne pole tekstowe do zmiany rozmiaru siatki oraz system gotowych wzorów (Glider, Gosper Gun itp.).

## 🛠 Technologie

* **Język:** C++17
* **Biblioteki:** SFML 3.0.0
* **Środowisko:** Visual Studio 2022

## 🎮 Sterowanie

| Klawisz | Akcja |
| :--- | :--- |
| `Spacja` | Start/Pauza symulacji |
| `R` | Losowe wypełnienie planszy |
| `C` | Wyczyszczenie siatki |
| `S` | Skok o jedną generację (Krok) |
| `Enter` | Zatwierdzenie rozmiaru siatki |
| `Strzałki` | Zmiana prędkości / wybór wzoru |
| `Myszka` | Ręczne przełączanie stanów komórek |

## 🏗 Architektura Systemu

### 1. Zarządzanie Czasem (Game Loop)
Program wykorzystuje `sf::Clock` do precyzyjnego odmierzania czasu ewolucji niezależnie od liczby klatek na sekundę (FPS), co zapobiega "zamrażaniu" interfejsu podczas dużych obliczeń.

### 2. Detekcja Kolizji UI
Zaimplementowano system AABB (*Axis-Aligned Bounding Box*) do obsługi przycisków, przeliczając współrzędne myszy na koordynaty świata gry za pomocą `mapPixelToCoords`.



### 3. Logika Ewolucji (Grid Logic)
Algorytm `step()` operuje na dwóch buforach danych. Zapobiega to błędom logicznym, w których nowo narodzona komórka wpływa na obliczenia sąsiadów w tej samej turze.



## 📦 Uruchomienie

W sekcji [Releases](../../releases) znajdziesz gotowy plik `.exe`. Dzięki osadzeniu zasobów, wystarczy go pobrać i uruchomić – nie wymaga instalacji żadnych dodatkowych bibliotek.
