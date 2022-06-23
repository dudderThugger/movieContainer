#include "family.h"

std::string Family::getFilmData() const {
    std::string ret;
    char rate[13];
    sprintf(rate,"%g",getRating());
    ret = getTitle() + "\n" + std::to_string(getYear()) + " " + rate + ' ' + getAttribute() +'\n';
    return ret;
}