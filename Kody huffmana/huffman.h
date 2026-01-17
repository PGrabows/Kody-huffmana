#pragma once
#include <string>
#include <unordered_map>


struct Wezel;

//Przechodzi drzewo Huffmana i przypisuje każdemu znakowi jego unikalny kod binarny.
void budujKodyHuffmana(Wezel* korzen, std::string kod, std::unordered_map<char, std::string>& kody);

//Analizuje tekst, zlicza wystąpienia znaków i buduje drzewo Huffmana.
Wezel* budujDrzewoHuffmana(const std::string& tekst);

//Główna funkcja kompresująca. Odczytuje plik tekstowy i zapisuje jego binarną wersję.
void kompresuj(const std::string& plikWejsciowy, const std::string& plikWyjsciowy);

//Funkcja odwracająca proces kompresji. Odczytuje bity i zamienia je z powrotem na tekst.
void dekompresuj(const std::string& plikWejsciowy, const std::string& plikWyjsciowy);