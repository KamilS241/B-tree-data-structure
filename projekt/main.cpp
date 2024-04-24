#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <memory>
#include <algorithm>  // std::shuffle
#include <numeric> // iota
#include <random>

#include "funkcje.h"
#include "struktury.h"
#include "drzewo.h"

#define debug(x) std::cerr << __FILE__ << " (" << __LINE__ << ") " << #x << " == " << (x) << std::endl

void test() 
{
    wezel::rzad = 2;
    std::shared_ptr<wezel> pKorzen = nullptr;
    const int N { 150 }; // rozmiar wektora
    std::vector<int> liczby (N); 
    std::iota(liczby.begin(), liczby.end(), 1); // wypelniam wektor kolejnymi liczbami od 1 do N
    std::random_device los;
    std::mt19937 generator{los()};
    
    //std::ranges::shuffle(liczby.begin(), liczby.end(), generator); // losowa permutacja 

    std::cout << "dodaje do drzewa wartosci" << std::endl;
    for (const auto n : liczby)
    {
       dodaj(pKorzen, n);
       std::cout << n << " " << std::flush;
    }
    std::cout << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << "drzewo:" << std::endl;
    wypisz(pKorzen, 0);
    std::cout << "======================" << std::endl; 
}

int main(int ile, char * params[])
{
//     test();  // testowanie dodawania
    
//     return 0;

    std::string nazwa_pliku;
    if (sprawdz_parametry_programu(ile, params, nazwa_pliku))
    {
        const std::string nazwa_strumienia(nazwa_pliku);
        std::vector<komenda>komendy = odczytaj_komendy_z_pliku(nazwa_strumienia);
        //test_komend(komendy);
        std::shared_ptr<wezel> pKorzen = nullptr;
        wykonaj_komendy(pKorzen, komendy);
    }
     else 
       help(params[0]);
    return 0;
}
