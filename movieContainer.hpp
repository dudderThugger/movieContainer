#ifndef HEADER_H_MOVIECONTAINER
#define HEADER_H_MOVIECONTAINER

#include <iostream>
#include "movies.h"
#include "memtrace.h"
#include <sstream>
#include "family.h"
#include "documentary.h"
/**
 * @file movieContainer.hpp
 * A nagyházi lényegét adó filmtár definícióját és a hozzá köthető
 * függvényeket illetve osztályokat tartalmazó fejléc
 */
/**
 * @brief Filmtár osztály filmek tárolására
 * A filmek heterogén kollekciója
 */
class MovieContainer{
    /**
     * Movie pointereket tároló tömb, annak mérete
     */
    Movie** movies;
    int siz;
    ///Másoló konstruktor tiltása
    MovieContainer(const MovieContainer&){}
    void operator=(const MovieContainer&);
public:
    /**
     * @brief Konstruktor
     * @param first Az első filmelem, ha nincs megadva nullt állítunk be alapértelmezettnek
     */
    MovieContainer(Movie* first);
    MovieContainer();
    /**Iterator és a begin end függvények */
    class iterator;
    iterator begin() {return iterator(*this);}
    iterator end() {return iterator(*this, siz);}
    /**
     * A filmeket listázó függvények a paraméter szerint overload-olva
     */
    std::string listFilms(const char*);
    std::string listFilms(int);
    std::string listFilms(double);
    /**
     * @brief Új film felvétele
     * @param movie az új film, amit hozzáadunk a listához
     */
    void add(Movie* movie);
    /**
     * @brief Értékelést változtató függvény
     * @param title a film címe aminek az értékelését megváltoztatjuk
     * @param neewRating új értékelés
     */
    void addRating(std::string title, double newRating);
    /**
     * indexelő operátor, igazából csak a tesztelésnél használjuk 
     */
    Movie* operator[](int);
    /**
     * méret getter függvénye
     */
    int getSize() const {return siz;}

    void clear();
    /**
     * Az iterátor osztály definíciója
     */
    class iterator{
        Movie **actual, **last;
    public:
        iterator() :actual(0), last(0) {}
        iterator(MovieContainer& mc, int ix = 0) :actual(mc.movies + ix), last(mc.movies + mc.siz) {}
        iterator& operator++(){
            if(actual != last) ++actual;
            return *this;
        }
        bool operator!=(const iterator &i){return(actual != i.actual);}
        Movie*& operator*() {
            if(actual != last) return *actual;
            else throw std::out_of_range("Hibas indirekcio");
        }
    };
    /**
     * @brief Filmtar destruktora
     * Felszabadítja a tárhoz hozzáadott movie mutatókat
     */
    ~MovieContainer();
};

/**
 * @brief Feltételes listázást végrehajtó függvény
 * Generikus algoritmus
 * @tparam T A tároló iterátora
 * @tparam S Az ostream, amire kiírjuk a kilistázott filmeket
 * @tparam Func A funktor
 */
template<typename T, class S, class Func>
void list_if(T,T,S&,Func);

/**
 * @brief A cím alapján listázás funktora
 */
class Title_functor{
    /**
     * A cím ami alapján keresünk
     */
    std::string title;
public:
    Title_functor(const std::string& title) :title(title) {}
    /**
     * @brief A zárójel operátor overloadja
     * Visszaadja a zárójelben lévő függvény adatait stringben, ha a funktor konstruktorában megadott címmel 
     * megegyező a film eleje (strng::combare függvényt használunk)
     * @param movie
     * @return A film adatai vagy egy üres string, ha nem teljesül a filmre a feltétel
     */
    std::string operator()(Movie* movie);
};

/**
 * @brief Év alapján listázás funktora
 */
class Year_functor{
    /// Év aminél későbbi dolgokat ki fog írni a funktor
    int year;
public:
    Year_functor(int year) :year(year) {}
    /**
     * @brief A zárójel operátor overloadja
     * Visszaadja a zárójelben lévő függvény adatait stringben, ha a funktorban ,egadott évvel megegyező
     * a film kiadási éve
     * @param movie
     * @return A film adatai vagy egy üres string, ha nem teljesül a filmre a feltétel
     */
    std::string operator()(Movie* movie);
};

/**
 * @brief Az értékelés alapján listázás funktora
 */
class Rating_functor{
    /**
     * Az értékelés, aminél nagyobb értékelésű filmeket keresünk
     */
    double rating;
public:
    Rating_functor(double r) :rating(r) {}
    /**
     * @brief Zárójel operátor overloadja
     * Megnézi, hogy a film értékelése nagyobb vagy megegyező(almost_eq(double,double) függvény dönti el),
     * mint a konstruktorban megadott értékelés
     * @param movie A zárójelben lévő film
     * @return movie->getFilmData(), ha teljesülnek a feltételek, egyébként üres string
     */
    std::string operator()(Movie* movie);
};


/**
 * @brief A fájlból filmtárba olvasást végzi
 * 
 * @param file A file stream amiből olvasunk
 * @param container A filmtar amibe beolvasunk
 * @return std::fstream& Visszaadjuk az file streamet
 */
std::fstream& operator<<(std::fstream& file, MovieContainer& container);

/**
 * @brief FIlmtárból fájlba írás
 * 
 * @param file A fájl stream amibe írunk
 * @param container A filmtár objektuma, aminek az adatai kiírjuk
 * @return std::fstream& A paraméterként megadott file stream
 */
std::fstream& operator>>(std::fstream& file,MovieContainer& container);

/**
 * @brief Eldönti egy stringről, hogy integer-e
 * Az algoritmust stackoverflow-n találtam paercebal felhasználó válaszában
 */
inline bool isInteger(const std::string & s){
   if(s.empty() || ((!isdigit(s[0])) && (s[0] != '-') && (s[0] != '+'))) return false;

   char * p;
   strtol(s.c_str(), &p, 10);

   return (*p == 0);
}

/**
 * @brief Eldönti a paraméterben kapott double-ökről, hogy nagyjából egyeznek-e
 */
inline bool almost_eq(double a, double b){
    return fabs(a - b) <= ((a+b)/10000000); 
}

/**
 * @brief Eldönti milyen típusú film attribútum alapján
 * @param attribute 
 * @return true Dokumentum film
 * @return false Családi film
 */
bool which_film(const std::string& attribute);

/**
 * @brief rating stringet átalakítja double típusúvá
 * 
 * @param rating 
 * @return double értékelés
 */
double rate_conversion(std::string rating);

#endif
