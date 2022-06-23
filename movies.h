#ifndef HEADER_H_MOVIES
#define HEADER_H_MOVIES

#include <iostream>
#include <math.h>

/**
 * @brief movies.h
 * Ebben a fájlban filmek alaposztálya deklarálva
 */

/**
 * @brief Movies class, absztarkt alaposztálya a különböző filmtípusoknak 
 */
class Movie{
    /**
     * A filmek közös attribútumai: cím, kiadás éve és az értékelés
     */
    std::string title;
    int year;
    double rating;
public:
    /**
     * A Movies alaposztály egyetlen konstruktora, nincs default konstruktor, úgyis minden filmet dinamikusan készítünk
     * @brief Konstruktor
     * 
     * @param title A film címe
     * @param year A kiadás éve
     * @param rating Az értékelés
     */
    Movie(const std::string& title, int year, double rating) :title(title), year(year), rating(rating) {}
    /**
     * Getter függvények
     */
    std::string getTitle() const {return title;}
    int getYear() const {return year;}
    double getRating() const {return rating;}
    /**
     * @brief Az értékelés setter függvénye
     * std::out_of_range kivételt dob, ha nincs a 0-10-es határ között az új értékelés
     * @param newRating
     */
    void setRating(double newRating) {
        if (newRating < 0 || newRating > 10){
            throw(std::out_of_range("Nem megfelelő értékelés!"));
        }        
        rating = newRating;
    }
    /**
     * @brief Film adatait tartalmazó stringet készít
     * Virtuális függvény
     * @return A film adatai
     */
    virtual std::string getFilmData() const = 0;
    /**
     * @brief Az egyéni attribútum getterje
     * Virtuális függvény
     * @return egyéni attribútum
     */
    virtual std::string getAttribute() const = 0;

    /**
     * Virtuális destruktor 
     */
    virtual ~Movie(){}
};
#endif