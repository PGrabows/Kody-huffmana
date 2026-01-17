#pragma once // Zabezpieczenie przed wielokrotnym dołączaniem pliku
#include <vector>
#include <iostream>


// Struktura reprezentująca pojedynczy element drzewa Huffmana.
// Może być liściem (przechowuje znak) lub węzłem wewnętrznym (łączy dwójke dzieci).
 
struct Wezel {
    char znak;            // Przechowywany znak (tylko w liściach)
    int czestotliwosc;    // Priorytet węzła (liczba wystąpień znaku)
    Wezel* lewy;          // Wskaźnik na lewe dziecko
    Wezel* prawy;         // Wskaźnik na prawe dziecko

    // KONSTRUKTOR LIŚCI: Tworzy węzeł dla konkretnego znaku
    // Używa listy nicjalizacyjnej dla optymalnej wydajności.
    Wezel(char c, int f) : znak(c), czestotliwosc(f), lewy(nullptr), prawy(nullptr) {}
    
    // KONSTRUKTOR WĘZŁÓW WEWNĘTRZNYCH: Łączy dwa poddrzewa w jedno
    // znak('\0') oznacza, że ten węzeł nie reprezentuje konkretnej litery.
    Wezel(int f, Wezel* l, Wezel* p) : znak('\0'), czestotliwosc(f), lewy(l), prawy(p) {}
};



struct PorownajWezel {
    bool operator()(Wezel* a, Wezel* b); // Deklaracja operatora porównania
};


//Klasa implementująca kolejkę priorytetową opartą na kopcu binarnym

class KolejkaPriorytetowa {
private:
    // Dynamiczna tablica przechowująca wskaźniki do węzłów (struktura kopca)
    std::vector<Wezel*> kopiec;

    // Metody pomocnicze do utrzymania porządku w kopcu
    void naprawWDol(int indeks);  // Przesuwa element w dół (gdy jest za duży)
    void naprawWGore(int indeks); // Przesuwa element w górę (gdy jest za mały)

public:
    // Dodaje nowy węzeł do kolejki
    void dodaj(Wezel* wezel);

    // Zdejmuje i zwraca węzeł o najmniejszej częstotliwości (szczyt kopca)
    Wezel* usun();

    // Przekształca gotowy wektor węzłów w poprawny kopiec
    void budujZTablicy(std::vector<Wezel*>& wezly);

    // Sprawdza, czy w kolejce są jeszcze jakieś elementy
    bool pusta() const;

    // Wyświetla stan kolejki (pomocne przy debugowaniu)
    void wypisz() const;

    // Zmienia wagę elementu i przesuwa go w odpowiednie miejsce w kopcu
    void zmienPriorytet(int indeks, int nowaCzestotliwosc);
    
    // Zwraca aktualną liczbę elementów w kolejce
    int rozmiar() const { return (int)kopiec.size(); }

    // Daje bezpośredni dostęp do węzła pod danym indeksem
    Wezel* pobierz(int indeks) { return kopiec[indeks]; }
};