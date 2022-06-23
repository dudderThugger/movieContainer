#include "movieContainer.hpp"
#include <string>
#include "memtrace.h"
#include <fstream>
#include <sstream>

MovieContainer::MovieContainer(Movie* m){
    if(m == NULL){
        movies = NULL;
        siz = 0;
    }else{
        movies = new Movie*[1];
        movies[0] = m;
        siz = 1;
    }
}

MovieContainer::MovieContainer():movies(0),siz(0){}

void MovieContainer::add(Movie* movie){
    Movie** moviesNew = new Movie*[siz + 1];
    int i = 0;
    for(iterator iter = begin(); iter != end(); ++iter){
        moviesNew[i++] = (*iter);
    }
    siz++;
    moviesNew[i] = movie;
    delete[] movies;
    movies = moviesNew;
}

std::string Title_functor::operator()(Movie* movie){
    if(movie->getTitle().compare(0,title.size(),title) == 0){
        return movie->getFilmData();
    }
    return "";
}

std::string Year_functor::operator()(Movie* movie){
    if(movie->getYear() == year){
        return movie->getFilmData();
    }
    return "";
}

std::string Rating_functor::operator()(Movie* movie){
    if(movie->getRating() > rating || almost_eq(movie->getRating(),rating)){
        return movie->getFilmData();
    }
    return "";
}



Movie* MovieContainer::operator[](int idx){
    if(idx < 0 || idx >= siz) throw std::out_of_range("Tulindexeles");
    return movies[idx];
}

template <typename T, class S, class Func>
void list_if(T first, T last, S& os, Func func) {
    for(T iter = first; iter != last; ++iter){
        os << func((*iter));
    }
}

std::string MovieContainer::listFilms(const char* label) {
    std::stringstream ss;
    Title_functor func = Title_functor(label);
    list_if(begin(), end(), ss, func);
    return ss.str();
}

std::string MovieContainer::listFilms(int label) {
    std::stringstream ss;
    Year_functor func = Year_functor(label);
    list_if(begin(), end(), ss, func);
    return ss.str();
}

std::string MovieContainer::listFilms(double label) {
    std::stringstream ss;
    std::string type = typeid(label).name();
    Rating_functor func = Rating_functor(label);
    list_if(begin(), end(), ss, func);
    return ss.str();
}

bool which_film(const std::string& attribute){
    if(isInteger(attribute)){   //Ha csak integer az azt jelenti, hogy nem dokumentum film
            return false;
    } else if(attribute == "nincs"){ //Ha a szöveg "nincs" van az szintén azt jelenti, hogy nem dokumentum film
            return false;
    }
    return true;
}

double rate_conversion(std::string rating){
    if (rating == "-" ){    //Ha a rating még nincs hozzáadva, akkor -1-et állítunk-be
        return -1; std::cout<< "ide jött!"<< std::endl;
    } else{
        double temp = std::stod(rating);
        if(temp < 0 || temp > 10) throw(std::out_of_range("Hibás értékelés!"));
        return temp;
    }
}

std::fstream& operator>>(std::fstream& file, MovieContainer& container){
    if(file.rdbuf()->in_avail() <= 0){
        throw std::invalid_argument("Üres fájl vagy nem megfelelő fajlnév!");
    }
    std::string txt;    // A film adatait ebbe a stringbe olvassuk be
    std::string title;
    int year;
    std::string attribute;
    std::string rating; // Először egy stringbe olvassuk az értékelést, mert megengedtük, hogy '-' legyen, ha még nem adtunk hozzá értékelést
    while(std::getline(file, title, '\n')){
        bool documentary = true; // Dokumentum, vagy családi film lesz-e a beolvasott film, alapból családi film
        std::getline(file,txt,'\n');
        std::stringstream buffer(txt);
        buffer >> year >> rating;
        buffer.ignore(1,' ');   //Először ignoráljuk az elválasztó ' '-t
        getline(buffer,attribute,'\n');
        Movie* movie;
        documentary = which_film(attribute);
        double rateConv = rate_conversion(rating);
        if(documentary){
            Documentary* add = new Documentary(title,year,rateConv,attribute);
            movie = dynamic_cast<Movie*>(add);
        } else {
            if(attribute == "nincs"){
                attribute = "0";
            }
            Family* add;
            int limit = std::stoi(attribute);
            if(limit > 18 || limit < 0){
                throw std::out_of_range("Hibás korhatár!");
                }else{
                add = new Family(title,year,rateConv,std::stoi(attribute));
            }
            movie = dynamic_cast<Movie*>(add);
        }
        container.add(movie);
    }
    return file;
}

std::fstream& operator<<(std::fstream& file, MovieContainer& container) {
    for(MovieContainer::iterator iter = container.begin(); iter != container.end(); ++iter){
        file << (*iter)->getFilmData();
    }
    return file;
}

void MovieContainer::addRating(std::string title, double newRating){
    for(MovieContainer::iterator iter = begin(); iter != end(); ++iter){
        if(title == (*iter)->getTitle()){
            (*iter)->setRating(newRating);
            return;
        }
    }
    throw std::out_of_range("Nem található a film, amihez az értékelést adná");
}

void MovieContainer::clear(){
    for(iterator iter = begin(); iter != end(); ++iter) {
        delete (*iter);
    }
    delete[] movies;
    movies = NULL;
    siz = 0;
}

MovieContainer::~MovieContainer(){
    for(iterator iter = begin(); iter != end(); ++iter) {
        delete (*iter);
    }
    delete[] movies;
}