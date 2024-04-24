
#ifndef DRZEWO_H
#define DRZEWO_H

#include <vector>
#include <string>
#include <memory>
#include <list>

/// Struktura wezla drzewa
struct wezel
{
    std::list<double> wartosci;   /// Wartosci w wezle
    
    std::list<std::shared_ptr<wezel>> potomki; /// Potomki wezla
 
    std::weak_ptr<wezel>pRodzic; /// Rodzic wezla

    std::shared_ptr<wezel>wskaznik_na_nowy_korzen = nullptr; /// Wskaznik ktory przechowuje adres na nowy korzen, gdy dokonujemy jego zmiany
    
    bool czyLisc {true}; /// Zmienna okreslajaca czy wezel jest lisciem

    bool czyKorzenDoZmiany {false}; /// Zmienna okreslajaca czy dany wezel jest korzeniem ktory musimy zmienic na nowy
    
    static double rzad;  /// Zmienna przechowywujaca rzad drzewa
};

/// Funkcja dodajaca wartosc do B-drzewa.
/// @param[in,out] pKorzen Korzen drzewa (jezeli drzewo jest puste, ma wartosc nullptr)
/// @param wartosc Wartosc do dodania do drzewa
void dodaj (std::shared_ptr<wezel>& pKorzen, const double wartosc);

/// Funkcja wypisujaca graf drzewa na standardowe wyjscie
/// @param[in,out] pKorzen Korzen drzewa
/// @param wciecie Wspolczynnik wciecia
void wypisz(const std::shared_ptr<wezel> & pKorzen, const int wciecie);

/// Funkcja zapisujaca graf drzewa do pliku
/// @param[in,out] plik Strumien wyjsciowy do pliku
/// @param[in,out] pKorzen Korzen drzewa
/// @param wciecie Wspolczynnik wciecia
void zapisz_do_pliku(std::ofstream& plik,const std::shared_ptr <wezel>& pKorzen, const int wciecie);

/// Funkcja dodaje wartosc do listy i ja sortuje
/// @param wartosc Dodawana wartosc
/// @param[in,out] wartosci Lista wartosci do ktorej dodajemy
void wsortuj_wartosc(const double wartosc, std::list<double>& wartosci);

/// Funkcja sprawdza czy w wezle nie ma zbyt wielu wartosci
/// @param pWezel Wezel do sprawdzenia
/// @return true -- za duzo wartosci w wezle, potrzebna modyfikacja, false -- poprawna ilosc wartosci w wezle
bool czy_za_duzo_w_wezle(const std::shared_ptr <wezel> pWezel);

/// Funkcja ktora stanowi posrednik miedzy funkcja dodaj, a funkcjami ktore modyfikuja wezly
/// @param[in,out] pWezel Wezel drzewa
/// @param wartosc Wartosc ktora spowodowala przeciazenie wezla
void gdy_za_duzo_w_wezle(std::shared_ptr<wezel>& pWezel, double wartosc);

/// Funkcja dzieli zbyt duzy wezel na prawa i lewa wartosc oraz mediane i przekazuje do funkcji gdy_za_duzo_w_wezle, ktora decyduje co robic dalej
/// @param[in,out] pWezel Wezel drzewa do podzielenia
/// @return Lewy wezel
/// @return Mediana
/// @return Prawy wezel
std::tuple<std::shared_ptr<wezel>, double, std::shared_ptr<wezel>> podziel_wezel(std::shared_ptr<wezel>& pWezel);

/// Funkcja usuwa zbyt duzy wezel i wklada na jego miejsce 2 nowe wezly (lewy i prawy) oraz mediane do rodzica
/// @param[in,out] pWezel Rozbity wezel
/// @param wartosc Wartosc ktora wywolala rozbicie
/// @param[in,out] pLewy Lewy wezel utworzony po rozbiciu oryginalnego wezla
/// @param mediana Mediana, czyli wartosc srodkowa, ktora idzie to rodzica wezla
/// @param[in,out] pPrawy Prawy wezel utworzony po rozbiciu oryginalnego wezla
void podmien_wartosci(std::shared_ptr<wezel>& pWezel, const double wartosc, std::shared_ptr<wezel>& pLewy, double mediana, std::shared_ptr<wezel>& pPrawy);

/// Funkcja przyjmuje lewy i prawy wezel oraz mediane (po rozbiciu starego korzenia), tworzy nowy korzen, dodaje do niego lewe i prawe potomki, zwraca wskaznik na nowy korzen, ktory podmieniamy z oryginalnym wskaznikiem
/// @param[in,out] pLewy Lewy wezel
/// @param mediana Mediana 
/// @param[in,out] pPrawy Prawy wezel
/// @return Nowy korzen
std::shared_ptr<wezel> utworz_nowy_korzen_drzewa(std::shared_ptr<wezel>& pLewy, const double mediana, std::shared_ptr<wezel>& pPrawy);

/// Funkcja znajduje adres potomka do ktorego kierujemy wartosc
/// @param pWezel Rodzic, wsrod ktorego potomkow szukamy takiego, do ktorego ma trafic wartosc
/// @param wartosc Wartosc dla ktorej szukamy potomka
/// @return Szukany potomek
std::shared_ptr<wezel> znajdz_adres_potomka(const std::shared_ptr<wezel> pWezel,const double wartosc);

/// Funkcja dodaje potomka do listy potomkow, wsadzajac go w odpowiednie miejsce, tak by lista byla posortowana rosnaco wg pierwszych wartosci potomkow
/// @param[in,out] potomek Potomek do wsortowania
/// @param[in,out] lista_potomkow Lista do ktorej wsortujemy potomka
void wsortuj_potomka(const std::shared_ptr<wezel>& potomek, std::list<std::shared_ptr<wezel>>& lista_potomkow);

#endif
