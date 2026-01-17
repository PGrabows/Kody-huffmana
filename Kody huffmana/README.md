Opis struktury projektu

1. kolejka_priorytetowa.h / .cpp To fundament całego projektu. Pliki zawierają implementację klasy KolejkaPriorytetowa oraz strukturę Wezel.

    Wezel: Struktura przechowująca pojedynczy znak, liczbę jego wystąpień (częstotliwość) oraz wskaźniki na lewe i prawe dziecko. Jest to niezbędne do późniejszego zbudowania drzewa Huffmana.

    Kopiec binarny (Min-Heap): Kolejka działa na zasadzie kopca minimalnego. Oznacza to, że element o najmniejszej częstotliwości zawsze znajduje się w korzeniu (na początku tablicy).

    Działanie: Metody naprawWGore i naprawWDol odpowiadają za przywrócenie właściwości kopca (sortowanie) po każdej operacji dodania lub usunięcia węzła.

2. huffman.h / .cpp Moduł odpowiedzialny za właściwą logikę kompresji i dekompresji plików.

    budujDrzewoHuffmana: Funkcja analizuje tekst wejściowy, zlicza wystąpienia każdego znaku i za pomocą kolejki priorytetowej łączy węzły w drzewo binarne.

    kompresuj: Program generuje kody bitowe dla znaków na podstawie drzewa, a następnie zapisuje treść w formacie binarnym. Do pliku wynikowego dodawany jest nagłówek (słownik), który jest konieczny, aby później odczytać dane.

    dekompresuj: Funkcja odczytuje nagłówek, odbudowuje kody znaków i na ich podstawie zamienia ciąg bitów z powrotem na oryginalny tekst.

3. glowny.cpp Główny plik programu integrujący wszystkie funkcje. Zawiera menu użytkownika podzielone na dwa etapy: testowanie struktury danych oraz właściwy program kompresujący.

Instrukcja obsługi

Program działa w dwóch trybach. Po uruchomieniu najpierw przechodzimy do modułu testowego kolejki, a po jego zakończeniu do właściwego kompresora plików.
Część 1: Testowanie Kolejki Priorytetowej

Ten etap służy do weryfikacji, czy struktura danych działa poprawnie, zanim zostanie użyta w algorytmie Huffmana.

    Dodaj węzeł: Ręczne wprowadzanie znaku i jego częstotliwości. Program umieszcza element w kopcu zgodnie z priorytetem.

    Usuń minimum (korzeń): Pobiera i usuwa element o najmniejszej wartości. Jest to kluczowa operacja wykorzystywana przez algorytm Huffmana do łączenia drzew.

    Zmień priorytet: Umożliwia edycję częstotliwości dla danego węzła. Po zmianie kolejka automatycznie się naprawia, aby zachować porządek kopca.

    Zbuduj nową kolejkę z tablicy: Służy do szybkiego przetestowania działania na większej liczbie danych (symulacja algorytmu build heap).

    Koniec testu: Zamyka moduł testowy i przechodzi do menu kompresora.

Część 2: Kompresor Huffmana

Główna część programu operująca na plikach.

    1 - Kompresja:

        Program prosi o nazwę pliku źródłowego (np. tekst.txt).

        Następnie prosi o nazwę dla pliku wynikowego (np. wynik.huf).

        Algorytm zlicza znaki, buduje drzewo i zapisuje skompresowane dane wraz ze słownikiem.

    2 - Dekompresja:

        Należy podać nazwę pliku skompresowanego (np. wynik.huf).

        Następnie podajemy nazwę pliku wyjściowego (np. odczyt.txt).

        Program odtwarza treść na podstawie zapisanego słownika i kodów binarnych.

    0 - Wyjście: Zakończenie działania programu.