#ifndef FUNKCJE_H
#define FUNKCJE_H

#include <vector>
#include <string>
#include <memory>

#include "struktury.h"
#include "drzewo.h"


/// Funkcja odczytuje z pliku wejsciowego komendy wraz z parametrami, pomijajac komentarze
/// @param[in,out] Nazwa strumienia pliku wejsciowego
/// @return Ciag komend
std::vector<komenda> odczytaj_komendy_z_pliku (const std::string & nazwa);

/// Testowe wypisanie na ekran nazwy kazdej komendy (i jej parametrow)
/// @param[in,out] ciag_komend Komendy zebrane z pliku wejsciowego
void test_komend (const std::vector<komenda> & ciag_komend);


/// @param argc Pierwszy parametr main
/// @param params Drugi parametr main
/// @param[in,out] nazwa_pliku Jezeli poprawne parametry wywolania programu, to tutaj bedzie odczytana nazwa pliku.
/// @return true -- wszystko OK, parametry poprawne, false -- parametry bledne
bool sprawdz_parametry_programu (int argc, char * params [], std::string & nazwa_pliku);

/// Funkcja wyswietlajaca notke przypominajaca jak uruchomic program
/// @param[in,out] nazwa_pliku_wykonywalnego Nazwa pliku wykonywalnego
void help(const std::string & nazwa_pliku_wykonywalnego);

/// Funkcja wykonujaca komendy zebrane z pliku wejsciowego
/// @param[in,out] pKorzen Korzen drzewa
/// @param[in,out] ciag_komend Komendy zebrane z pliku wejsciowego
void wykonaj_komendy(std::shared_ptr<wezel> & pKorzen, const std::vector<komenda>& ciag_komend);

#endif
