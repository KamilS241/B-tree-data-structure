#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <list>
#include <algorithm>

#include "funkcje.h"
#include "struktury.h"
#include "drzewo.h"


#define debug(x)   std::cerr << __FILE__ << " (" << __LINE__ << ") " << #x << " == " << (x) << std::endl

std::vector<komenda> odczytaj_komendy_z_pliku(const std::string& nazwa)
{
    std::vector<komenda>  kontenerdane;
    std::ifstream dane(nazwa);
    double liczba;
    if (dane)
    {
        std::string linia;
        while (getline(dane, linia))
        {
            size_t m = linia.find_first_of("%");
            std::string linia_bez_komentarza = linia.substr(0, m);
            std::stringstream ss;
            ss << linia_bez_komentarza;
            double liczba;
            std::string nazwa_polecenia;
            ss >> nazwa_polecenia;
            if (nazwa_polecenia == "add")
            {
                std::vector<double> params;
                while (ss >> liczba)
                    params.push_back(liczba);

                kontenerdane.push_back({ typ::add, params, std::string() });
            }
            else if (nazwa_polecenia == "graph")
            {
                std::vector<double> params;
                kontenerdane.push_back({ typ::graph, params, std::string() });
            }
            else if (nazwa_polecenia == "print")
            {
                std::string nazwa_pliku;
                std::string linia_po_poleceniu;
                std::vector<double> params;
                if (ss >> linia_po_poleceniu)
                {
                    if (linia_po_poleceniu == "+")
                    {
                        ss >> nazwa_pliku;
                        kontenerdane.push_back({ typ::append, params, nazwa_pliku });
                    }
                    else
                    {
                        ss >> nazwa_pliku;
                        kontenerdane.push_back({ typ::print, params, linia_po_poleceniu });
                    }
                }
                else
                {
                    std::string nazwa_pliku = "brak_pliku";
                    kontenerdane.push_back({ typ::print, params, nazwa_pliku});
                }
            }
            else if (nazwa_polecenia == "rzad")
            {
                std::vector<double> params;
                if (ss >> liczba)
                {
                    params.push_back(liczba);
                    kontenerdane.push_back({ typ::rzad, params, std::string() });
                }
            }
        }
        dane.close();
        return kontenerdane;
    }   
    return kontenerdane;  
}

std::string to_string (const typ & t)
{
    switch (t)
    {
        case typ::add : return std::string ("add");
        case typ::rzad: return std::string("rzad");
        case typ::print: return std::string("print");
        case typ::append: return std::string("append");
        case typ::graph: return std::string("graph");
        default:       return std::string("???");
    }
}

void test_komend(const std::vector<komenda>& ciag_komend)
{
    for (int i = 0; i < ciag_komend.size(); i++)                //indeks komendy
    {
        std::string nazwa_komendy=to_string(ciag_komend[i].rodzaj);
        std::cout << nazwa_komendy<<" ";
        
        for (int j = 0; j < ciag_komend[i].params.size(); j++)  //indeks parametru w komendzie
        {
            std::cout << ciag_komend[i].params[j] << " ";
        }
        std::cout<<ciag_komend[i].nazwa_pliku;
        std::cout << std::endl;
    }
}

void help(const std::string& nazwa_pliku_wykonywalnego)
{
    std::cout << "Uruchomienie programu:\n" << nazwa_pliku_wykonywalnego << " -i" << std::endl;
}

bool sprawdz_parametry_programu(int argc, char* params[], std::string& nazwa_pliku)
{
    if (argc != 3)
    {
       return false;
    }
    if ((std::string)params[1]!="-i")
    {
       return false;
    }
    nazwa_pliku = params[2];
    return true;
}

void wykonaj_komendy(std::shared_ptr<wezel>& pKorzen,const std::vector<komenda>& ciag_komend)
{
    wezel korzen;
    std::list <double> lista_wartosci;
    pKorzen = std::make_shared<wezel>(korzen); 

    for (const auto komenda : ciag_komend)
    {
        switch (komenda.rodzaj)
        {
        case typ::rzad:
        {
            wezel::rzad = komenda.params[0];
            continue;
        }
        case typ::add:
        {
            for (auto wartosc : komenda.params)
            {
                dodaj(pKorzen, wartosc);
                lista_wartosci.push_back(wartosc);
                lista_wartosci.sort();
            }
           continue;
        }
        case typ::graph:
        {
            std::cout << "Graf drzewa:" << std::endl;
            std::cout << "==================================" << std::endl;
            wypisz(pKorzen, 0);
            std::cout << "==================================" << std::endl;
            continue;
        }
        case typ::print:
        {
            if (komenda.nazwa_pliku == "brak_pliku")
            {
                std::cout << "Dodane wartosci: (";
                for (auto wartosc : lista_wartosci)
                {
                    std::cout << wartosc<<", ";
                }
                std::cout << ")" << std::endl;
            }
            else
            {
                std::string nazwa_pliku = komenda.nazwa_pliku;
                std::ofstream plik(nazwa_pliku);
                if (plik)
                {
                    plik << "Graf drzewa:" << std::endl << "========================" << std::endl;
                    zapisz_do_pliku(plik, pKorzen, 0);
                    plik << "========================";
                }
                plik.close();
            }
            continue;
        }
        case typ::append:
        {
            std::string nazwa_pliku = komenda.nazwa_pliku;
            std::ofstream plik;
            plik.open(nazwa_pliku, std::ofstream::app);
            if (plik)
            {
                plik <<std::endl<< "Zaktualizowany graf drzewa:" << std::endl << "========================" << std::endl;
                zapisz_do_pliku(plik, pKorzen, 0);
                plik << "========================";
            }
            plik.close();
            continue;
        }
        }
    }
}
