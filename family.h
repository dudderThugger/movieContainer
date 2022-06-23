#ifndef FAMILY_HEADER_INCLUDED
#define FAMILY_HEADER_INCLUDED
#include "movies.h"
#include <iostream>
#include "memtrace.h"
/**
 * @file family.h
 * A családi film osztályát definiáló fájl
 */
/**
 * @brief Családi filmek
 */
class Family :public Movie {
    /**
     * Korhatár
     */
    int ageLimit;
public:
    /**
     * @brief A családi filmek konstruktora
     * @param ageLimit korhatár
     */
    Family(std::string title, int year, double rating, int ageLimit) :Movie(title,year,rating), ageLimit(ageLimit) {}
    /**
     * Getter függvények
     */
    std::string getAttribute() const {return std::to_string(ageLimit);}
    std::string getFilmData() const;
};

#endif
