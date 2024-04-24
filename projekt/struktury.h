
#ifndef STRUKTURY_H
#define STRUKTURY_H

#include <vector>
#include <string>
#include <memory>
#include <list>

/// Typ wyliczeniowy okreslajacy rodzaje komend
enum class typ
{
    rzad,   /// Rzad drzewa
    add,    /// Komenda dodaj
    graph,  /// Komenda wypisz drzewo na standardowe urzadzenie wyjsciowe
    print,  /// Komenda wypisz wartosci drzewa, lub zapisz graf do pliku
    append  /// Inaczej print +, czyli dopisanie do pliku
};

/// Struktura komendy
struct komenda
{
    typ rodzaj;                  ///  rodzaj  komendy 
    std::vector<double> params;  /// parametry liczbowe
    std::string nazwa_pliku;    /// nazwa pliku do wypisywania
};

#endif
