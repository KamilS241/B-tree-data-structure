#include <iostream>
#include <fstream>
#include "funkcje.h"
#include "struktury.h"
#include "drzewo.h"
#include <tuple>


#define debug(x)  std::cerr << __FILE__ << " (" << __LINE__ << ") " << #x << " == " << (x) << std::endl

double wezel::rzad;

void dodaj(std::shared_ptr<wezel>& pKorzen, const double wartosc)
{
    if (pKorzen == nullptr)
    {
        pKorzen = std::shared_ptr<wezel>(new wezel);
        pKorzen->wartosci.push_back(wartosc);
        pKorzen->czyLisc = true;
        return;
    }
    else
    {
        if (pKorzen->czyLisc)
        {
            wsortuj_wartosc(wartosc, pKorzen->wartosci);
            bool za_duzo_w_wezle = czy_za_duzo_w_wezle(pKorzen);
            if (za_duzo_w_wezle)
            {             
                gdy_za_duzo_w_wezle(pKorzen, wartosc);
            }
        }
        else
        {
            std::shared_ptr<wezel> pPotomek = znajdz_adres_potomka(pKorzen, wartosc);
            dodaj(pPotomek, wartosc); 
            bool za_duzo_w_wezle = czy_za_duzo_w_wezle(pKorzen);
            if (za_duzo_w_wezle)
            {
                gdy_za_duzo_w_wezle(pKorzen, wartosc);
            }
            
        }
    }
    if (pKorzen->czyKorzenDoZmiany == true)
    {
        pKorzen = pKorzen->wskaznik_na_nowy_korzen;
        return;
    }
}

std::tuple<std::shared_ptr<wezel>, double, std::shared_ptr<wezel>> podziel_wezel(std::shared_ptr<wezel>&pWezel)
{
    std::shared_ptr<wezel> pLewy(new wezel);
    std::shared_ptr<wezel> pPrawy(new wezel);
    double mediana;
    std::list<double> LL;
    std::list<double> LP;
    auto it = pWezel->wartosci.begin();
    std::advance(it, (pWezel->wartosci.size() / 2));
    //lista_docelowa.splice(miejsce gdzie maja isc wartosci, dawca, poczatek skad zabrac, koniec skad zabrac)
    LL.splice(LL.begin(), pWezel->wartosci, pWezel->wartosci.begin(), it);
    std::advance(it, 1);
    LP.splice(LP.begin(), pWezel->wartosci, it, pWezel->wartosci.end());
    pLewy->wartosci = LL;
    pPrawy->wartosci = LP;
    mediana = *(pWezel->wartosci.begin());
    return { pLewy, mediana, pPrawy };
}

void podmien_wartosci(std::shared_ptr<wezel>& pWezel, const double wartosc, std::shared_ptr<wezel>& pLewy, double mediana, std::shared_ptr<wezel>& pPrawy)
{
    auto rodzic = pWezel->pRodzic.lock();
    wsortuj_wartosc(mediana, rodzic->wartosci);
    wsortuj_potomka(pLewy, rodzic->potomki);
    wsortuj_potomka(pPrawy, rodzic->potomki);
    pLewy->pRodzic = rodzic;
    pPrawy->pRodzic = rodzic;
    auto it = rodzic->potomki.begin();
    while (it != rodzic->potomki.end())
    {
        if ((*it)==pWezel)
        {
            rodzic->potomki.erase(it);
            break;
        }
        it++;
    }   
    bool za_duzo_w_wezle = czy_za_duzo_w_wezle(pWezel->pRodzic.lock());
    if (za_duzo_w_wezle)
    {
       gdy_za_duzo_w_wezle(rodzic, mediana);
    }
    pWezel->pRodzic = rodzic;
}

void gdy_za_duzo_w_wezle(std::shared_ptr<wezel>& pWezel, double wartosc)
{
    auto rodzic =pWezel->pRodzic.lock();
    auto [pLewy, mediana, pPrawy] = podziel_wezel(pWezel);
    if (pWezel->czyLisc)
    {
        pLewy->czyLisc = true;
        pPrawy->czyLisc = true;
        if (rodzic!=nullptr)
        {
            podmien_wartosci(pWezel, wartosc, pLewy, mediana, pPrawy);
            pWezel->pRodzic = rodzic;
        }
        else
        {
            auto nowy_korzen = utworz_nowy_korzen_drzewa(pLewy, mediana, pPrawy);
            pWezel->wskaznik_na_nowy_korzen = nowy_korzen;
            pWezel->czyKorzenDoZmiany = true;
        }
        pWezel->czyLisc = false;
    }
    else
    {
        pLewy->czyLisc = false;
        pPrawy->czyLisc = false;
        for (auto potomek : pWezel->potomki)
        {
            if (potomek->wartosci.front()<mediana)
            {
                wsortuj_potomka(potomek, pLewy->potomki);
                potomek->pRodzic = pLewy;
            }
            else if (potomek->wartosci.front() >= mediana)
            {
                if (potomek->wartosci.front() == mediana and potomek->wartosci.back() == mediana and pLewy->potomki.size() != pWezel->potomki.size() / 2)
                {
                    wsortuj_potomka(potomek, pLewy->potomki);
                }
                else
                {
                    wsortuj_potomka(potomek, pPrawy->potomki);
                    potomek->pRodzic = pPrawy;
                }
            }
        }
        if (rodzic!=nullptr)
        {
            podmien_wartosci(pWezel, wartosc, pLewy, mediana, pPrawy);
            pWezel->pRodzic = rodzic;
        }
        else
        {
            auto nowy_korzen=utworz_nowy_korzen_drzewa(pLewy, mediana, pPrawy);
            pWezel->wskaznik_na_nowy_korzen = nowy_korzen;
            pWezel->czyKorzenDoZmiany=true;
        }
    }
}

std::shared_ptr<wezel> utworz_nowy_korzen_drzewa(std::shared_ptr<wezel>& pLewy, const double mediana, std::shared_ptr<wezel>& pPrawy)
{
    std::shared_ptr<wezel> nowy_korzen(new wezel);
    nowy_korzen->czyLisc = false;
    wsortuj_potomka(pLewy, nowy_korzen->potomki);
    wsortuj_potomka(pPrawy, nowy_korzen->potomki);
    pLewy->pRodzic = nowy_korzen;
    pPrawy->pRodzic = nowy_korzen;
    nowy_korzen->wartosci.push_back(mediana);
    return nowy_korzen;
}

std::shared_ptr<wezel> znajdz_adres_potomka(const std::shared_ptr<wezel> pWezel, const double wartosc)
{
    auto it = pWezel->wartosci.begin();
    auto itp = pWezel->potomki.begin();
    while (it != pWezel->wartosci.end())
    {
        if (*it > wartosc)
        {
            return *itp;
        }
        else if ((*it <= wartosc) and (std::next(it, 1) == pWezel->wartosci.end()))
        {
            itp++;
            return *itp;
        }
        else
        {
            it++;
            itp++;
        }
    }
    auto itb = pWezel->potomki.back();
    return itb;
}

bool czy_za_duzo_w_wezle(const std::shared_ptr <wezel> pWezel)
{
    return pWezel->wartosci.size() > (pWezel->rzad * 2); 
}

void wsortuj_potomka(const std::shared_ptr<wezel>& potomek, std::list<std::shared_ptr<wezel>>& lista_potomkow)
{
    if (lista_potomkow.size() == 0)
    {
        lista_potomkow.push_back(potomek);
    }
    else
    {
        auto it = lista_potomkow.begin();
        while (it != lista_potomkow.end())
        {
            if (potomek->wartosci.front() < (*it)->wartosci.front())
            {
                lista_potomkow.insert(it, potomek);
                return;
            }
            it++;
        }
        lista_potomkow.push_back(potomek);
        return;
    }
}

void wsortuj_wartosc(const double wartosc, std::list<double>& wartosci)
{
    wartosci.push_back(wartosc);
    wartosci.sort();
    return;
}

void wypisz(const std::shared_ptr<wezel> & pKorzen, const int wciecie)
{
    const int N { 4 } ;
    if (pKorzen)
    {
        if (pKorzen->czyLisc==false)
        {
            auto iPot = pKorzen->potomki.begin();
            auto iWar = pKorzen->wartosci.begin();
            for (;
                iWar != pKorzen->wartosci.end();
                iPot++, iWar++)
            {
                    wypisz(*iPot, wciecie + 1);
                    for (int i = 0; i < wciecie * N; i++) std::cout << ' ';
                    std::cout << *iWar << " " << std::endl;
            }
                wypisz(*iPot, wciecie + 1);
        }
        else
        {
            for (auto wartosc : pKorzen->wartosci)
            {
                for (int i = 0; i < wciecie * N; i++) std::cout << ' ';
                std::cout << wartosc << std::endl;
            }
        }
    }
}

void zapisz_do_pliku(std::ofstream& plik, const std::shared_ptr <wezel>& pKorzen, const int wciecie)
{  
    const int N{ 4 };
    if (pKorzen)
    {
        if (pKorzen->czyLisc == false)
        {
            auto iPot = pKorzen->potomki.begin();
            auto iWar = pKorzen->wartosci.begin();
            for (;
                iWar != pKorzen->wartosci.end();
                iPot++, iWar++)
            {
                zapisz_do_pliku(plik,*iPot, wciecie + 1);
                for (int i = 0; i < wciecie * N; i++) plik << ' ';
                plik << *iWar << " " << std::endl;
            }
            zapisz_do_pliku(plik, *iPot, wciecie + 1);
        }
        else
        {
            for (auto wartosc : pKorzen->wartosci)
            {
                for (int i = 0; i < wciecie * N; i++) plik << ' ';
                plik << wartosc << std::endl;
            }
        }
    }
}
