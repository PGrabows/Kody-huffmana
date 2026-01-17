#include <iostream>
#include <vector>
#include <string>
#include "kolejka_priorytetowa.h"
#include "huffman.h"

// Test kopca
void menuKolejki() {
    KolejkaPriorytetowa kp; // Tworzenie obiektu kolejki
    int opcja = 0;

    while (opcja != 5) {
        std::cout << "\n--- TEST KOLEJKI PRIORYTETOWEJ ---\n";
        kp.wypisz(); // Wyświetla aktualny stan kopca
        std::cout << "1. Dodaj wezel\n";
        std::cout << "2. Usun minimum (korzen)\n";
        std::cout << "3. Zmien priorytet (indeks)\n";
        std::cout << "4. Zbuduj nowa kolejke z tablicy\n";
        std::cout << "5. Koniec testu i przejdz do Huffmana\n";
        std::cout << "Wybor: ";
        std::cin >> opcja;

        if (opcja == 1) {
            char z; int f;
            std::cout << "Znak: "; std::cin >> z;
            std::cout << "Czestotliwosc: "; std::cin >> f;
            // Tworzymy nowy węzeł na stercie i dodajemy do kolejki
            kp.dodaj(new Wezel(z, f));
        } 
        else if (opcja == 2) {
            // Usuwa element z góry kopca
            Wezel* w = kp.usun();
            if (w) std::cout << "Usunieto: " << w->znak << " o czestotliwosci " << w->czestotliwosc << "\n";
        } 
        else if (opcja == 3) {
            int idx, f;
            std::cout << "Podaj indeks: "; std::cin >> idx;
            std::cout << "Nowa czestotliwosc: "; std::cin >> f;
            // Zmiana wartości wewnątrz kopca wymaga przywrócenia jego właściwości
            kp.zmienPriorytet(idx, f);
        }
        else if (opcja == 4) {
            int ile;
            std::cout << "Ile elementow dodac do tablicy? "; std::cin >> ile;
            std::vector<Wezel*> wezly;
            for(int i=0; i<ile; ++i) {
                char z; int f;
                std::cout << "Znak " << i+1 << ": "; std::cin >> z;
                std::cout << "Czest. " << i+1 << ": "; std::cin >> f;
                wezly.push_back(new Wezel(z, f));
            }
            // Buduje kopiec efektywniej niż dodawanie elementów jeden po drugim
            kp.budujZTablicy(wezly);
        }
    }
}

//Główna funkcja programu
int main() {
    // Uruchomienie interaktywnego testera kolejki
    menuKolejki();
    // Główna pętla programu kompresującego
    int wybor;
    while (true) {
        std::cout << "\n--- KOMPRESOR HUFFMANA ---\n";
        std::cout << "1 - Kompresja\n2 - Dekompresja\n0 - Wyjscie\nWybor: ";
        std::cin >> wybor;
        // Czyścimy bufor strumienia, aby std::getline poniżej nie wczytał znaku nowej linii '\n'
        std::cin.ignore();

        if (wybor == 0) break;
        std::string plikWejsciowy, plikWyjsciowy;
        if (wybor == 1) {
            std::cout << "Plik do kompresji: ";
            std::getline(std::cin, plikWejsciowy); // Używamy getline, by móc wpisać spacje w nazwie pliku
            std::cout << "Plik wynikowy (np. out.huf): ";
            std::getline(std::cin, plikWyjsciowy);
            
            kompresuj(plikWejsciowy, plikWyjsciowy);
            std::cout << "Zakonczono kompresje.\n";
        } 
        else if (wybor == 2) {
            std::cout << "Plik skompresowany: ";
            std::getline(std::cin, plikWejsciowy);
            std::cout << "Plik wynikowy (np. dekompresowany.txt): ";
            std::getline(std::cin, plikWyjsciowy);

            dekompresuj(plikWejsciowy, plikWyjsciowy);
            std::cout << "Zakonczono dekompresje.\n";
        }
    }
    return 0;
}