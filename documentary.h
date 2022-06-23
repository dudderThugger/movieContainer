#ifndef DOCUMENTARY_HEADER_INCLUDED
#define DOCUMENTARY_HEADER_INCLUDED
#include "movies.h"
#include <iostream>
#include "memtrace.h"
/**
 * @file documentary.hpp
 * Dokumentum film osztályát definiálja
 */
/**
 * @brief Dokumentum osztály dokumentum film tárolására 
 */
class Documentary :public Movie {
    /**
     * A film rövid leírása 
     */
    std::string description;
public:
    /**
     * @brief Construct a new Documentary object
     * @param description Filmről rövid leírás
     */
    Documentary(const std::string& title, int year, double rating, const std::string& description) :Movie(title,year,rating), description(description) {}
    ///getter függvények 
    std::string getAttribute() const {return description;}
    std::string getFilmData() const;
};
#endif