#include "huffman.h"
#include "kolejka_priorytetowa.h"
#include <fstream>
#include <bitset>
#include <sstream>
#include <vector>


//Funkcja rekurencyjna przechodząca drzewo Huffmana w celu wygenerowania kodów binarnych.
void budujKodyHuffmana(Wezel* korzen, std::string kod, std::unordered_map<char, std::string>& kody) {
    if (!korzen) return; // Zabezpieczenie przed pustym wskaźnikiem

    // Jeśli węzeł jest liściem, oznacza to, że znaleźliśmy znak
    if (!korzen->lewy && !korzen->prawy) {
        kody[korzen->znak] = kod;
    }

    //Idąc w lewo dopisujemy "0", idąc w prawo "1"
    budujKodyHuffmana(korzen->lewy, kod + "0", kody);
    budujKodyHuffmana(korzen->prawy, kod + "1", kody);
}


//Buduje drzewo Huffmana na podstawie tekstu wejściowego.
//Zwraca wskaźnik do korzenia gotowego drzewa.

Wezel* budujDrzewoHuffmana(const std::string& tekst) {
    // Zliczanie wystąpień każdego znaku
    std::unordered_map<char, int> czestotliwosci;
    for (char z : tekst) czestotliwosci[z]++;

    // Tworzenie liści drzewa dla każdego znaku i umieszczenie ich w wektorze
    std::vector<Wezel*> wezly;
    for (auto& para : czestotliwosci) wezly.push_back(new Wezel(para.first, para.second));

    // Inicjalizacja kolejki priorytetowej
    KolejkaPriorytetowa kolejka;
    kolejka.budujZTablicy(wezly);

    // Główna pętla algorytmu: łączenie dwóch najmniejszych węzłów w jeden
    while (!kolejka.pusta()) {
        Wezel* lewy = kolejka.usun(); // Pobierz węzeł o najmniejszej częstotliwości
        
        // Jeśli po usunięciu kolejka jest pusta, 'lewy' jest korzeniem całego drzewa
        if (kolejka.pusta()) return lewy;

        Wezel* prawy = kolejka.usun(); // Pobierz drugi najmniejszy węzeł
        
        // Stwórz nowy węzeł wewnętrzny, który łączy oba węzły
        Wezel* polaczony = new Wezel(lewy->czestotliwosc + prawy->czestotliwosc, lewy, prawy);
        kolejka.dodaj(polaczony);
    }

    return nullptr;
}


// Kompresuje plik wejściowy i zapisuje wynik w formacie binarnym.
void kompresuj(const std::string& plikWejsciowy, const std::string& plikWyjsciowy) {
    // Wczytanie całego pliku do jednego ciągu string
    std::ifstream wejscie(plikWejsciowy, std::ios::binary);
    std::string tekst((std::istreambuf_iterator<char>(wejscie)), std::istreambuf_iterator<char>());
    wejscie.close();

    if (tekst.empty()) return;

    // Przygotowanie struktur: drzewo i mapa kodów
    Wezel* korzen = budujDrzewoHuffmana(tekst);
    std::unordered_map<char, std::string> kody;
    budujKodyHuffmana(korzen, "", kody);

    // Zamiana tekstu na jeden długi ciąg zer i jedynek
    std::string zakodowany;
    for (char z : tekst) zakodowany += kody[z];

    std::ofstream wyjscie(plikWyjsciowy, std::ios::binary);

    // ZAPIS NAGŁÓWKA:
    // Liczba bitów danych (potrzebna, by wiedzieć kiedy przestać czytać przy dekompresji)
    uint32_t liczbaBitow = zakodowany.size();
    wyjscie.write(reinterpret_cast<const char*>(&liczbaBitow), sizeof(liczbaBitow));

    // Słownik (liczba wpisów i pary: znak + kod)
    uint16_t ile = kody.size();
    wyjscie.write(reinterpret_cast<const char*>(&ile), sizeof(ile));
    for (auto& para : kody) {
        char znak = para.first;
        uint8_t dl = para.second.size();
        wyjscie.write(&znak, 1); // Zapisz znak (1 bajt)
        wyjscie.write(reinterpret_cast<const char*>(&dl), 1); // Zapisz długość kodu

        // Konwersja stringa "0101" na liczbę 32-bitową do zapisu
        std::bitset<32> bity;
        for (int i = 0; i < dl; ++i)
         bity[31 - i] = (para.second[i] == '1');
        uint32_t kodLiczbowo = bity.to_ulong();
        wyjscie.write(reinterpret_cast<const char*>(&kodLiczbowo), 4);
    }

    // ZAPIS DANYCH (Pakowanie bitów w bajty):
    uint8_t bufor = 0;      // Bufor na 8 bitów
    uint8_t bityWBuforze = 0; // Licznik zapełnienia bufora

    for (char bit : zakodowany) {
        bufor <<= 1;          // Przesuń bity w lewo, robiąc miejsce na nowy bit
        bufor |= (bit == '1'); // Jeśli znak to '1', ustaw ostatni bit bufora na 1
        bityWBuforze++;

        if (bityWBuforze == 8) { // Jeśli mamy pełny bajt, zapisz go do pliku
            wyjscie << bufor;
            bufor = 0;
            bityWBuforze = 0;
        }
    }
    // Jeśli zostały jakieś bity (mniej niż 8), przesuń je do lewej i zapisz ostatni bajt
    if (bityWBuforze > 0) {
        bufor <<= (8 - bityWBuforze);
        wyjscie << bufor;
    }
    wyjscie.close();
}

//Odczytuje skompresowany plik binarny i przywraca oryginalną treść.
void dekompresuj(const std::string& plikWejsciowy, const std::string& plikWyjsciowy) {
    std::ifstream wejscie(plikWejsciowy, std::ios::binary);
    if (!wejscie.is_open()) {
        std::cerr << "Nie mozna otworzyc pliku wejsciowego!\n";
        return;
    }

    //Odczyt całkowitej liczby bitów danych
    uint32_t liczbaBitow;
    wejscie.read(reinterpret_cast<char*>(&liczbaBitow), sizeof(liczbaBitow));

    //Odczyt słownika i budowanie mapy odwrotnej (kod -> znak)
    uint16_t ile;
    wejscie.read(reinterpret_cast<char*>(&ile), sizeof(ile));
    std::unordered_map<std::string, char> odwroconeKody;
    
    for (int i = 0; i < ile; ++i) {
        char znak;
        uint8_t dl;
        uint32_t kodLiczbowo;
        wejscie.read(&znak, 1);
        wejscie.read(reinterpret_cast<char*>(&dl), 1);
        wejscie.read(reinterpret_cast<char*>(&kodLiczbowo), 4);

        std::bitset<32> bity(kodLiczbowo);
        std::string kod;
        for (int b = 0; b < dl; ++b)
            kod += bity[31 - b] ? '1' : '0';

        odwroconeKody[kod] = znak;
    }

    // ODCZYT DANYCH (Rozpakowywanie bajtów na bity):
    std::string zakodowany;
    zakodowany.reserve(liczbaBitow); // Rezerwacja pamięci dla wydajności
    char bajt;
    uint32_t wczytanoBitow = 0;

    while (wczytanoBitow < liczbaBitow && wejscie.get(bajt)) {
        uint8_t wartosc = static_cast<uint8_t>(bajt);
        // Sprawdzamy, czy czytamy pełny bajt, czy ostatnie brakujące bity
        uint8_t bityDoOdczytu = (liczbaBitow - wczytanoBitow >= 8) ? 8 : (liczbaBitow - wczytanoBitow);
        
        for (int i = 0; i < bityDoOdczytu; ++i) {
            // Wyciąganie i-tego bitu od lewej
            bool bit = (wartosc >> (7 - i)) & 1;
            zakodowany += bit ? '1' : '0';
        }
        wczytanoBitow += bityDoOdczytu;
    }
    wejscie.close();

    // TŁUMACZENIE: Zamiana ciągu zer i jedynek na znaki przy użyciu słownika
    std::string obecny, odkodowany;
    for (char bit : zakodowany) {
        obecny += bit;
        if (odwroconeKody.count(obecny)) { // Jeśli fragment kodu pasuje do znaku w słowniku
            odkodowany += odwroconeKody[obecny];
            obecny.clear(); // Resetujemy bufor i szukamy kolejnego kodu
        }
    }

    // Zapis odkodowanej treści do pliku wyjściowego
    std::ofstream wyjscie(plikWyjsciowy, std::ios::binary); 
    wyjscie << odkodowany;
    wyjscie.close();
}