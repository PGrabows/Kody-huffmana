#include "kolejka_priorytetowa.h"
#include <algorithm>


// Logika porównywania węzłów.
// Zwraca true, jeśli węzeł 'a' ma większą wartość niż 'b'.
// W strukturze kopca oznacza to, że 'a' powinno być niżej niż 'b'.

bool PorownajWezel::operator()(Wezel* a, Wezel* b) {
    return a->czestotliwosc > b->czestotliwosc; 
}


// Przywraca własność kopca idąc od dołu do góry.
// Używana po dodaniu nowego elementu na koniec tablicy.

void KolejkaPriorytetowa::naprawWGore(int indeks) {
    // Dopóki nie jesteśmy w korzeniu i rodzic jest "większy" niż aktualny węzeł
    while (indeks > 0 && PorownajWezel()(kopiec[(indeks - 1) / 2], kopiec[indeks])) {
        std::swap(kopiec[indeks], kopiec[(indeks - 1) / 2]); // Zamień z rodzicem
        indeks = (indeks - 1) / 2; // Przejdź do pozycji rodzica
    }
}


// Przywraca własność kopca idąc od góry do dołu.
// Sprawdza rodzica oraz jego dzieci i zamienia miejscami tak, aby najmniejszy był na górze.

void KolejkaPriorytetowa::naprawWDol(int indeks) {
    int lewy, prawy, najmniejszy;
    while (true) {
        lewy = 2 * indeks + 1;  // Wzór na indeks lewego dziecka
        prawy = 2 * indeks + 2; // Wzór na indeks prawego dziecka
        najmniejszy = indeks;

        // Sprawdź czy lewe dziecko istnieje i czy jest mniejsze od aktualnego "najmniejszego"
        if (lewy < kopiec.size() && PorownajWezel()(kopiec[najmniejszy], kopiec[lewy]))
            najmniejszy = lewy;
        
        // Sprawdź czy prawe dziecko istnieje i czy jest mniejsze od dotychczasowego "najmniejszego"
        if (prawy < kopiec.size() && PorownajWezel()(kopiec[najmniejszy], kopiec[prawy]))
            najmniejszy = prawy;

        // Jeśli rodzic jest mniejszy od dzieci, kopiec jest już naprawiony
        if (najmniejszy == indeks) break;

        std::swap(kopiec[indeks], kopiec[najmniejszy]); // Zamiana z mniejszym dzieckiem
        indeks = najmniejszy; // Kontynuuj naprawę w dół od nowej pozycji
    }
}

// Dodaje nowy węzeł do kolejki.
void KolejkaPriorytetowa::dodaj(Wezel* wezel) {
    kopiec.push_back(wezel);           // Dodaj na sam koniec tablicy
    naprawWGore(kopiec.size() - 1);    // Przesuń w górę na właściwe miejsce
}

//Wyciąga z kolejki węzeł o najniższej częstotliwości (korzeń kopca).
Wezel* KolejkaPriorytetowa::usun() {
    if (kopiec.empty()) return nullptr;
    
    Wezel* korzen = kopiec[0];      // Zapamiętaj najmniejszy element
    kopiec[0] = kopiec.back();      // Wstaw ostatni element na miejsce korzenia
    kopiec.pop_back();              // Usuń ostatni element
    
    if (!kopiec.empty()) naprawWDol(0); // Przywróć porządek idąc od góry
    return korzen;
}

// Buduje kopiec z gotowej tablicy węzłów w czasie O(n).
void KolejkaPriorytetowa::budujZTablicy(std::vector<Wezel*>& wezly) {
    kopiec = wezly;
    // Naprawiamy wszystkie węzły, które nie są liśćmi, zaczynając od dołu
    for (int i = (int)kopiec.size() / 2 - 1; i >= 0; --i)
        naprawWDol(i);
}

bool KolejkaPriorytetowa::pusta() const {
    return kopiec.empty();
}

//Wyświetla stan kolejki (tablicy) w konsoli.
void KolejkaPriorytetowa::wypisz() const {
    if (kopiec.empty()) {
        std::cout << "Kolejka jest pusta.\n";
        return;
    }
    std::cout << "Elementy w kopcu (indeks: [znak:czestosc]):\n";
    for (int i = 0; i < (int)kopiec.size(); ++i) {
        char c = kopiec[i]->znak;
        if (c == '\0') c = ' '; // Zamiana znaku specjalnego na spację dla czytelności
        std::cout << i << ": [" << c << ":" << kopiec[i]->czestotliwosc << "]  ";
    }
    std::cout << "\n";
}


//Zmienia wartość (priorytet) elementu pod danym indeksem i naprawia strukturę.
void KolejkaPriorytetowa::zmienPriorytet(int indeks, int nowaCzestotliwosc) {
    if (indeks < 0 || indeks >= (int)kopiec.size()) return;
    
    kopiec[indeks]->czestotliwosc = nowaCzestotliwosc;
    naprawWGore(indeks); // Jeśli wartość zmalała, element może chcieć "płynąć" w górę
    naprawWDol(indeks);  // Jeśli wartość wzrosła, element może chcieć "opaść" w dół
}