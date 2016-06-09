//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef VARIA_H
#define VARIA_H

#include <windows.h>

#define SQR(x) ((x)*(x))

//void ShowMessage(LPCSTR Text); //a la Borland
char* DoubleToStr(double liczba,char* lancuch,int rozmiar);

char* trim(char* dest,const char* src);
char* trim(char* src);

BOOL PozycjaKursoraMyszyWOknie(HWND hWnd,POINT* pozycjaKursoraMyszyWOknie);

double DegToRad(double deg);
double RadToDeg(double rad);
double sinDeg(double deg);
double cosDeg(double deg);
float sinDegf(float deg);
float cosDegf(float deg);

float DlugoscWektora3fv(float wektor[3]);
float* IloczynWektorowy3fv(float a[3],float b[3],float wynik[3]);
float* NormujWektor3fv(float wektor[3]);
float* JednostkowyWektorNormalny3fv(float punkt1[3],float punkt2[3],float punkt3[3],float wynik[3]);
float OdlegloscPunktow(float* punkt1,float* punkt2);

bool filetest(const char* filename);
unsigned long* WczytajTeksture(HWND uchwytOkna,char* nazwaPliku,int& teksturaSzer,int& teksturaWys,bool zZasobow=false,unsigned char alfa=255);

unsigned int StworzCzcionke(bool czyCzcionka3D,HWND uchwytOkna,char* NazwaCzcionki,int wysokoscWPikselach,bool Pogrubiona,bool Kursywa,int kodPierwszegoZnaku=32,int kodOstatniegoZnaku=255);
void Pisz(char* napis,int iloscLiter,unsigned int czcionka,int kodPierwszegoZnaku);
void Pisz(unsigned int czcionka, int kodPierwszegoZnaku, const char *napis, ...);

float* TransformujPikselDoPrzestrzeniSceny(POINT pozycjaMyszy,float glebokosc,float pozycja[3]);
POINT TransformujPunktScenyDoPrzestrzeniEkranu(double pozycja[3],POINT& pozycjaPiksela,float& glebokosc);
#endif

