#include <iostream>
#include "movieContainer.hpp"
#include "gtest_lite.h"
#include <fstream>
#include "memtrace.h"
#include <sstream>
using namespace std;
/*
void felh(){
    int n;
    MovieContainer tar;
    cout<< "Válaszzon az alábbi opciók közül:\n 1) Filmtár betöltése\n 2) Filmek listázása\n 3) Új film felvétele\n"
        " 4) Értékelés módosítása\n 5) Fájlba írás\n 6) Kilépés\n\n";
    while(cin >> n){
        try{
            if(n == 1){
                if(tar.getSize() != 0){
                    tar.clear();
                }
                cout << "Adja meg a fájl nevét: ";
                std::string fajl; cin >> fajl;
                fstream file;
                file.open(fajl,ios::in);
                file >> tar;
                cout<< "\n Fájl sikeresen betöltve a tárolóba!\n";
            } else if(n == 2){
                cout<< "Adja meg mi alapján szeretne listázni!\n 1) Cím\n 2) Év\n 3) Értékelés\n";
                int m;
                cin >> m;
                if(m == 1){
                    cout << "Adja meg a kereső címkét: ";
                    std::string title;
                    cin >> title;
                    cout << endl << tar.listFilms(title.c_str()) << endl;
                } else if(m == 2){
                    cout << "Adja meg a kiadási évet: ";
                    int year;
                    cin >> year;
                    cout << tar.listFilms(year) << endl;
                }  else if(m == 3){
                    cout << "Adja meg az értékelést: ";
                    double rating;
                    cin >> rating;
                    cout<< endl << tar.listFilms(rating) << endl;
                } else {
                    throw std::invalid_argument("1-3 ig terjedő számot adjon meg!");
                }
            }else if(n == 3){
                cout<< "\nAdja meg milyen típusú filmet szeretne felvenni:\n 0) Családi filmet\n 1) Dokumentum filmet\n\n";
                int m; cin >> m;
                if(m != 0 && m != 1){ cout << "\n\n";
                    throw(invalid_argument("Kérem 0-s vagy 1-es értéket adjon meg!"));
                } else{
                    cout << "  Adja meg a film címét: ";
                    std::string title; cin >> title;
                    cout << "  a kiadás évét: ";
                    int year; cin >> year;
                    cout << "  az értékelést: ";
                    double rate; cin >> rate;
                    if(m == 0){
                        cout << "  a korhatárt: ";
                        int ageLimit; cin >> ageLimit;
                        cout << cin.badbit << "   " << cin.failbit << endl;
                        Family* add = new Family(title,year,rate,ageLimit);
                        cout << "\n\n" << title <<" című film sikeresen hozzáadva a tárolóhoz";
                        tar.add(add);
                    } else {
                        cout << cin.badbit << "   " << cin.failbit << endl;
                        char description[100]; 
                        cout << "  a film rövid leírását: ";
                        cin.ignore(1,'\n');
                        cin.getline(description, 100, '\n');
                        Documentary* add = new Documentary(title,year,rate,description);
                        tar.add(add);
                        cout <<"\n\n"<<title <<"című film sikeresen hozzáadva a tárolóhoz!";
                    }
                }
            }else if(n == 4){
                cout<< " Adja meg a film címét, aminek az értékelését megváltoztatná: ";
                string title; cin >> title;
                cout<< " És az új értékelést: ";
                double newRate; cin>> newRate;
                tar.addRating(title, newRate);
                cout<< " Az új értékelés a(z) "<< title << " című filmhez sikeresen hozzáadva!" << endl;
            } else if(n == 5){
                cout<< "Adja meg hogy milyen néven mentsük le a fájlt: ";
                string fileOut; cin >> fileOut; cout<< endl;
                fstream fileIII;
                fileIII.open(fileOut, ios::out);
                fileIII >> tar;
                cout<< " A fájl sikeresen kiírva!\n";
            } else if(n == 6){
                return;
            }
        } catch(std::out_of_range& e){
            cout << e.what() << endl;
        } catch(std::invalid_argument& e){
            cout << e.what() << endl;
        }
        cout<< endl << endl;
        cout<< "Válaszzon az alábbi opciók közül:\n 1) Filmtár betöltése\n 2) Filmek listázása\n 3) Új film felvétele\n"
        " 4) Értékelés módosítása\n 5) Fájlba írás\n 6) Kilépées\n\n";
    }
}
*/
int main(void){
    GTINIT(std::cin);

    //Ezt a tárolót több tesztnél is használni fogjuk
    MovieContainer tar;

    //Megnézzük, hogy be tudunk-e olvasni fájlból, amiben csak egy film van
    TEST(MovieContainer,file_read){
        std::fstream fileBe;
        fileBe.open("filmtar.txt", ios::in);
        EXPECT_NO_THROW(fileBe >> tar);
        fileBe.close();
    } END

    //Leteszteljük, hogy dob-e kivételt, ha a fájlban nem megfelelő adat szerepel(pl. 10+-os értékelés)
    TEST(MovieContainer, file_exception){
        std::fstream file;
        file.open("rosszForm.txt", ios::in);
        MovieContainer uj;
        EXPECT_THROW(file >> uj,std::out_of_range& e);
        cout << uj.listFilms("").c_str();        
    } END

    //Tudunk-e hozzáadni a listához dokumentum illetve családi filmet
    TEST(MovieContainer,add_to_list){
        Documentary uj("Seaspiracy",2021,8.1,"Passionate about ocean life, a filmmaker sets out to document" 
        "the harm that humans do to marine species - and uncovers alarming global corruption.");
        tar.add(new Documentary(uj));
        Family uj2("Encanto", 2021, 7.2, 6);
        tar.add(new Family(uj2));
        EXPECT_EQ(3, tar.getSize());
    }END

    //Ez a stream csak a tesztelésnél kell
    std::stringstream ss;
    ss << "Arthur és a villangók" << '\n' << 2006 <<" " << 6 <<' ' << 0 << '\n';

    //Cim alapján listázás 1 objektumra
    TEST(MovieContainer, list_by_title1){
        EXPECT_STREQ(ss.str().c_str(), tar.listFilms("Arthur").c_str());
    }END

    //Év alapján listázás (mindent kiir, aminek az éve azonos vagy későbbi mint a megadott év)
    TEST(MovieContainer, list_by_year1){
        EXPECT_STREQ(ss.str().c_str(), tar.listFilms(2006).c_str());
    }END

    //Értékelés alapján listázás (mindent kiir, ami azonos vagy nagyobb értékelés)
    TEST(MovieContainer, list_by_rating1){
        stringstream rateTest;
        rateTest << tar[1]->getFilmData();
        EXPECT_STREQ(rateTest.str().c_str(), tar.listFilms(8.1).c_str());
    }END

    //Működik-e az év és értékelés szerinti listázás több film esetén is
    TEST(MovieContainer, list_multiple_films){
        ss.str("");
        ss << tar[1]->getFilmData() << tar[2]->getFilmData();
        EXPECT_STREQ(ss.str().c_str(), tar.listFilms(2021).c_str());
        EXPECT_STREQ(ss.str().c_str(),tar.listFilms(7.0).c_str());
    }END

    //Fájlba írás és aztán több filmet tartalmazó fájlból olvasás
    TEST(MovieContainer, file_write_file_read_multiple){
        fstream fileOut;
        fileOut.open("kimenet.txt",ios::out);
        fileOut << tar;
        fileOut.close();
        MovieContainer back;
        fstream fileIn;
        fileIn.open("kimenet.txt",ios::in);
        fileIn >> back;
        fileIn.close();
        EXPECT_STREQ(tar.listFilms("").c_str(), back.listFilms("").c_str());
    }END

    //Tudunk-e új értékelést adni
    TEST(MovieContainer, add_rating){
        Family* croodek = new Family("Croodék",2013,0,6);
        tar.add(croodek);
        tar.addRating("Croodék",9.9);
        EXPECT_STREQ("Croodék\n2013 9.9 6\n",tar.listFilms("Croodék").c_str());
    }END

    // Több filmre listázás cím szerint
    TEST(MovieContainer,list_multiple_films){
        Documentary* arthur = new Documentary("Arthur király",2004,-1,"Arthur király legendáíról szóló rövid dokumentumfilm");
        tar.add(arthur);
        stringstream arthurok;
        arthurok << tar[0]->getFilmData() << arthur->getFilmData();
        EXPECT_STREQ(arthurok.str().c_str(), tar.listFilms("Arthur").c_str());
    }END

    TEST(MovieContainer, clear){
        tar.clear();
        EXPECT_EQ(tar.getSize(),0);
        EXPECT_THROW(tar[0],std::out_of_range& e);
    }END

    TEST(MovieContainer, exceptions_other){
        MovieContainer::iterator iter;
        for(iter = tar.begin(); iter != tar.end(); ++iter);
        EXPECT_THROW((*iter), out_of_range& e);
        tar.add(new Documentary("Seaspiracy",2021,8.1,"Passionate about ocean life, a filmmaker sets out to document" 
        "the harm that humans do to marine species - and uncovers alarming global corruption."));
        EXPECT_THROW(tar.addRating("Seaspiracy",21.1),out_of_range& e);
        EXPECT_THROW(tar.addRating("Nem az a film",21.1), std::out_of_range& e);
        fstream error;
        error.open("ilyenfajlnemlesz.xd", ios::in);
        Family* madagaszkar = new Family("Madagaszkár", 2005, 8.5, 12);
        MovieContainer hiba(madagaszkar);
        EXPECT_THROW(error>>hiba,invalid_argument& e);
        error.close();
        error.open("fajlHibaA.txt", ios::in);
        error >> hiba;
    } END

    TEST(MovieContainer, felh){
        //felh(); egyébként ez működik, de írtam teszteseteket, szóval kikkommentelve hagyom
    }END

    GTEND(std::cerr);
    return 0;
}
