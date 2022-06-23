#include "documentary.h"

std::string Documentary::getFilmData() const {
        char rate[13];
        sprintf(rate,"%g",getRating());
        std::string ret = getTitle() + "\n" + std::to_string(getYear()) + " " + rate + ' ' + getAttribute() + '\n';
        return ret;
}