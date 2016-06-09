#include "Varia.h"

//---------------------------------

#include <windows.h>

void ShowMessage(LPCSTR Text)
{
	MessageBox(NULL,Text,"",NULL);
}

BOOL PozycjaKursoraMyszyWOknie(HWND hWnd,POINT* pozycjaKursoraMyszyWOknie)
{
	if (!GetCursorPos(pozycjaKursoraMyszyWOknie)) return FALSE;
	BOOL wynik=ScreenToClient(hWnd,pozycjaKursoraMyszyWOknie);
	return wynik;
}

//---------------------------------
//operacje na lancuchach

char* trim(char* dest,const char* src)
{
    int len=strlen(src);
    char* tmp=new char[len+1]; //miejsce na ew. dodatkowy znak \0
    strcpy(tmp,src);
    int indexOfFirstNonSpaceCharacter=0;
    while(tmp[indexOfFirstNonSpaceCharacter]==' ')
    {
        indexOfFirstNonSpaceCharacter++;
    }
    int indexOfLastNonSpaceCharacter=len-1;
    while(tmp[indexOfLastNonSpaceCharacter]==' ')
    {
        indexOfLastNonSpaceCharacter--;
    }
    tmp[indexOfLastNonSpaceCharacter+1]='\0'; //to moze wydluzyc lancuch (potrzebny dodatkowy znak)
    strcpy(dest,tmp+indexOfFirstNonSpaceCharacter);
    delete [] tmp;
    return dest;
}

char* trim(char* src)
{
    char* tmp=new char[strlen(src)+1]; //trim moze zwrocic lancuch o jeden znak (\0) dluzszy 
    trim(tmp,src);
    strcpy(src,tmp);
    delete [] tmp;
    return src;
}

//---------------------------------

#define _USE_MATH_DEFINES
#include <math.h>

double DegToRad(double deg)
{
	return M_PI*deg/180.0f;
}

double RadToDeg(double rad)
{
	return 180.0f*rad/M_PI;
}

double sinDeg(double deg)
{
	return sin(DegToRad(deg));
}

double cosDeg(double deg)
{
	return cos(DegToRad(deg));
}

float sinDegf(float deg)
{
	return (float)sinDeg(deg);
}

float cosDegf(float deg)
{
	return (float)cosDeg(deg);
}

//---------------------------------

float* Roznica3fv(float punkt1[3],float punkt2[3],float wynik[3])
{
	for(int i=0;i<3;i++) wynik[i]=punkt2[i]-punkt1[i];
	return wynik;
}

float* IloczynWektorowy3fv(float a[3],float b[3],float wynik[3])
{
	const int x=0;
	const int y=1;
	const int z=2;

	wynik[x]=  a[y]*b[z]-a[z]*b[y];
	wynik[y]=-(a[x]*b[z]-a[z]*b[x]);
	wynik[z]=  a[x]*b[y]-a[y]*b[x];
	return wynik;
}

#include <math.h>  //sqrt

float DlugoscWektora3fv(float wektor[3])
{
	float dlugosc=0;
	for(int i=0;i<3;i++) dlugosc+=SQR(wektor[i]);
	return sqrt(dlugosc);
}

float* NormujWektor3fv(float wektor[3])
{
	float wsp=DlugoscWektora3fv(wektor);
	for(int i=0;i<3;i++) wektor[i]/=wsp;
	return wektor;
}

float* JednostkowyWektorNormalny3fv(float punkt1[3],float punkt2[3],float punkt3[3],float wynik[3])
{
	float wektor12[3],wektor13[3];
	return NormujWektor3fv(IloczynWektorowy3fv(Roznica3fv(punkt1,punkt2,wektor12),Roznica3fv(punkt1,punkt3,wektor13),wynik));
}

float OdlegloscPunktow(float* punkt1,float* punkt2)
{
	float wektor[3]={punkt2[0]-punkt1[0],punkt2[1]-punkt1[1],punkt2[2]-punkt1[2]};
	float wynik=0.0f;
	for(int i=0;i<3;i++) wynik+=SQR(wektor[i]);
	return sqrt(wynik);	
}

//---------------------------------

#include <stdio.h>

bool filetest(const char* filename)
{
	FILE* plik_test;
	if((plik_test=fopen(filename,"r"))==NULL) return false;
	else fclose(plik_test);
	return true;
} 

#include <stdexcept>

unsigned long* WczytajTeksture(HWND uchwytOkna,char* nazwaPliku,int& teksturaSzer,int& teksturaWys,bool zZasobow,unsigned char alfa)
{
	//niezbyt elegancko jest tworzyc new w funkcji (zapomina sie o delete)
	//ale dzieki temu nie trzeba rozbijac na dwie czesci (pobieranie obrazu i czytanie)
	//nie trzeba zwracac informacji o obrazie, bo zawsze zwracamy tablice RGBA
	try
	{
		//czytanie bitmapy z pliku lub z zasobow
		if (!zZasobow && !filetest(nazwaPliku)) throw std::invalid_argument("Brak pliku");
		HBITMAP uchwytObrazu = (HBITMAP)LoadImage(GetModuleHandle(NULL),nazwaPliku,IMAGE_BITMAP,0,0,(zZasobow?0:LR_LOADFROMFILE) | LR_CREATEDIBSECTION);
		
		//informacje o bitmapie
		BITMAP obraz;		
		GetObject(uchwytObrazu,sizeof(BITMAP),&obraz);
		teksturaSzer=obraz.bmWidth;
		teksturaWys=obraz.bmHeight;

		//informacja o ilosci bitow na piksel
		//char bufor[256];
		//ShowMessage(_gcvt(obraz.bmBitsPixel,10,bufor));		

		unsigned long* tekstura=new unsigned long[teksturaSzer*teksturaWys];
		switch(obraz.bmBitsPixel)
		{
			case 24:
				{
				unsigned char* tekstura24bppBRG=new unsigned char[obraz.bmWidthBytes*teksturaWys];
				memcpy(tekstura24bppBRG,obraz.bmBits,teksturaSzer*teksturaWys*3);
				//gdybym zwrocil tekstura24bppBRG zrzutowane na unsigned long*, to powinienem uzywac do wyswietlania GL_BGR_EXT
				
				//konwersja na RGBA
				for(int ih=0;ih<teksturaWys;ih++)	//kolejnosc i tak dowolna, bo tablica jednowymiarowa	
					for(int iw=0;iw<teksturaSzer;iw++)		
					{
						int i=3*iw+(ih*obraz.bmWidthBytes); //uwzglednia uzupelnianie do WORD
						unsigned char A=alfa;
						unsigned char B=tekstura24bppBRG[i];
						unsigned char G=tekstura24bppBRG[i+1];
						unsigned char R=tekstura24bppBRG[i+2];

						/*
						//jezeli konwersja na BW
						unsigned char jasnosc=(R+G+B)/3;
						R=jasnosc;
						B=jasnosc;
						G=jasnosc;
						*/

						tekstura[iw+(ih*teksturaSzer)]=(A << 24) + (B << 16) + (G << 8) + (R);
					}
				delete [] tekstura24bppBRG;
				}
				break;

			case 1:	//monochromatyczne
				{
				unsigned char* tekstura1bppMono=new unsigned char[obraz.bmWidthBytes*teksturaWys];
				memcpy(tekstura1bppMono,obraz.bmBits,obraz.bmWidthBytes*teksturaWys);				
				
				//konwersja na RGBA				
				for(int ih=0;ih<teksturaWys;ih++)		
					for(int iw=0;iw<teksturaSzer;iw++)
					{
						int i=iw/8+(ih*obraz.bmWidthBytes); //uwzglednia uzupelnianie do WORD
						int numerBitu=iw % 8;
						unsigned char A=alfa;
						bool bitZapalony=((tekstura1bppMono[i] << numerBitu) & 128)==128;
						//ignorujemy palete i tworzymy obraz czarno-bialy
						unsigned char B=bitZapalony?255:0;
						unsigned char G=bitZapalony?255:0;
						unsigned char R=bitZapalony?255:0;
						tekstura[iw+(ih*teksturaSzer)]=(A << 24) + (B << 16) + (G << 8) + (R);
					}
				delete [] tekstura1bppMono;
				}
				break;

			case 8: //256 kolorow, wymaga palety barw (tabeli kolorow)
				{
					unsigned char* tekstura8bppPalette=new unsigned char[obraz.bmWidthBytes*teksturaWys];
					memcpy(tekstura8bppPalette,obraz.bmBits,obraz.bmWidthBytes*teksturaWys);				
					//memcpy(tekstura,tekstura8bppPalette,obraz.bmWidthBytes*teksturaWys);				

					//pobranie tabeli kolorow (pomijamy czytanie BITMAPINFO)
					HDC uchwyt=CreateCompatibleDC(GetDC(uchwytOkna));
					SelectObject(uchwyt,uchwytObrazu);
					RGBQUAD tabelaKolorow[256];
					GetDIBColorTable(uchwyt,0,256,tabelaKolorow);
								
					/*					
					//standardowa paleta kolorow z systemu
					//HPALETTE uchwytPalety=(HPALETTE)GetStockObject(DEFAULT_PALETTE); 
					PALETTEENTRY tabelaKolorow[256];
					GetSystemPaletteEntries(GetDC(uchwytOkna),0,256,tabelaKolorow);
					*/				
							
					for(int ih=0;ih<teksturaWys;ih++)	//kolejnosc i tak dowolna, bo tablica jednowymiarowa	
						for(int iw=0;iw<teksturaSzer;iw++)		
						{
							int i=iw+(ih*obraz.bmWidthBytes); //uwzglednia uzupelnianie do WORD
							unsigned char A=alfa;
							unsigned char R=tabelaKolorow[tekstura8bppPalette[i]].rgbRed;
							unsigned char G=tabelaKolorow[tekstura8bppPalette[i]].rgbGreen;
							unsigned char B=tabelaKolorow[tekstura8bppPalette[i]].rgbBlue;
							tekstura[iw+(ih*teksturaSzer)]=(A << 24) + (B << 16) + (G << 8) + (R);						
						}
					delete [] tekstura8bppPalette;
				}
				break;

			case 4: //16 kolorow (paleta barw jak w Paint)
				{
					unsigned char* tekstura4bppPalette=new unsigned char[obraz.bmWidthBytes*teksturaWys];
					memcpy(tekstura4bppPalette,obraz.bmBits,obraz.bmWidthBytes*teksturaWys);				
					
					//pobranie tabeli kolorow (pomijamy czytanie BITMAPINFO)
					HDC uchwyt=CreateCompatibleDC(GetDC(uchwytOkna));
					SelectObject(uchwyt,uchwytObrazu);
					RGBQUAD tabelaKolorow[16];
					GetDIBColorTable(uchwyt,0,16,tabelaKolorow);
								
					for(int ih=0;ih<teksturaWys;ih++)	//kolejnosc i tak dowolna, bo tablica jednowymiarowa	
						for(int iw=0;iw<teksturaSzer;iw++)		
						{
							int i=iw/2+(ih*obraz.bmWidthBytes); //uwzglednia uzupelnianie do WORD
							bool pierwszaPolowaBajtu=!(iw % 2);
							unsigned char A=alfa;
							int numerKoloruZPalety=(pierwszaPolowaBajtu)?((tekstura4bppPalette[i] & 0xF0)>>4):(tekstura4bppPalette[i] & 0x0F);
							unsigned char R=tabelaKolorow[numerKoloruZPalety].rgbRed;
							unsigned char G=tabelaKolorow[numerKoloruZPalety].rgbGreen;
							unsigned char B=tabelaKolorow[numerKoloruZPalety].rgbBlue;
							tekstura[iw+(ih*teksturaSzer)]=(A << 24) + (B << 16) + (G << 8) + (R);						
					}
					delete [] tekstura4bppPalette;
				}
				break;
			
			default: throw std::exception("Nieobslugiwany format bitmapy"); break;
		}
		
		DeleteObject(uchwytObrazu);

		return tekstura;
   	}
	catch(const std::exception& exc)
    {
		char komunikat[256]="B³¹d podczas pobierania tekstury:\n";
		strcat(komunikat,exc.what());
		MessageBox(NULL,komunikat,"B³¹d teksturowania",NULL); 
		return NULL;
	}
}

//---------------------------------

#include <gl\gl.h>

//pierwszy znak np. pozycja spacji w zestawie znakow ASCII = 32
//ostatniZnak, pozycja ostatniego znaku w zestawie znakow ASCII = 128 lub 256

unsigned int StworzCzcionke(bool czyCzcionka3D,HWND uchwytOkna,char* NazwaCzcionki,int wysokoscWPikselach,bool Pogrubiona,bool Kursywa,int kodPierwszegoZnaku,int kodOstatniegoZnaku)
{   
	unsigned int indeksPierwszejListy = glGenLists(kodOstatniegoZnaku+1-kodPierwszegoZnaku);   //Tworzy liste na pelen zestaw czcionek

	//funkcja WinAPI (GDI); tworzy obiekt czcionki
	HFONT uchwytCzcionki = CreateFont(
		                    wysokoscWPikselach, //wysokosc czcionki
			                0,  //uzywam szerokosci czcionki proporcjonalnej do wysokosci
							0,  //nachylenie czcionek
							0,  //kat
							Pogrubiona?FW_BOLD:FALSE, //pogrubionie
							Kursywa?TRUE:FALSE,       //kursywa
							FALSE,                //podkreslenie
							FALSE,                //przekreslenie
							ANSI_CHARSET,         //zbior liter ANSI
							OUT_TT_PRECIS,        //czcionki true type
							CLIP_DEFAULT_PRECIS,  //domyslna precyzja przycinania rozmiaru
							ANTIALIASED_QUALITY,  //antyaliasing przy tworzeniu obrazow
							FF_DONTCARE|DEFAULT_PITCH,  //styl i dekoracja czcionki
							NazwaCzcionki);     //nazwa czcionki

	HDC uchwytDC=GetDC(uchwytOkna);
	HFONT uchwytCzcionkiDC=(HFONT)SelectObject(uchwytDC,uchwytCzcionki);   //zwiazanie z naszym kontekstem okna
	//Funkcja WGL (specyficzna dla Windows); 32-spacja; ilosc: 96 - alfabet lacinski, 256 - gdy tez polskie znaki
	if(!czyCzcionka3D)
	{
		wglUseFontBitmaps(uchwytDC,kodPierwszegoZnaku,kodOstatniegoZnaku+1-kodPierwszegoZnaku,indeksPierwszejListy);
	}
	else
	{
		wglUseFontOutlines(uchwytDC,kodPierwszegoZnaku,kodOstatniegoZnaku+1-kodPierwszegoZnaku,indeksPierwszejListy,
							0.0f, //precyzja budowania czcionki
							0.2f, //glebokosc czcionki
							WGL_FONT_POLYGONS,
							NULL); //informacje o rozmiarach czcionek
	}
	SelectObject(uchwytDC,uchwytCzcionkiDC);   //Wybor czcionki, ktora stworzylismy
	DeleteObject(uchwytCzcionki);        //Usuwanie pomocniczego

   return indeksPierwszejListy; //=indeks tablicy list
}

void Pisz(char* napis,int iloscLiter,unsigned int czcionka,int kodPierwszegoZnaku)
{
   if (napis==NULL || napis=="") return;

   glPushAttrib(GL_LIST_BIT);   //Odklada na stos atrybuty wyswietlania
   glListBase(czcionka-kodPierwszegoZnaku);    //Ustawia podstawe znakow
   glCallLists(iloscLiter,GL_UNSIGNED_BYTE,napis);
                                //Wyswietla kolejno listy liter (napis)
   glPopAttrib();               //Przywraca ze stosu atrybuty wyswietlania
}

void Pisz(unsigned int czcionka, int kodPierwszegoZnaku, const char *napis, ...)
{
	char	tekst[2048];						
	va_list	ap;								

	if (napis == NULL || napis == "") return;	

	va_start(ap, napis);							
		vsprintf(tekst, napis, ap);				
	va_end(ap);

	Pisz(tekst, strlen(tekst), czcionka, kodPierwszegoZnaku);
	return;
}

//---------------------------------

#include <gl/glu.h>

float* TransformujPikselDoPrzestrzeniSceny(POINT pozycjaPiksela,float glebokosc,float pozycjaPunktu3D[3])
{
	//viewport i macierze
	int viewport[4]; glGetIntegerv(GL_VIEWPORT,viewport);
	double macierzModelWidok[16]; glGetDoublev(GL_MODELVIEW_MATRIX,macierzModelWidok);
	double macierzRzutowania[16]; glGetDoublev(GL_PROJECTION_MATRIX,macierzRzutowania);

	//wspolrzedne sceny
	double wspolrzednePunktu3D[3];

	//transformacja
	gluUnProject(
		pozycjaPiksela.x,viewport[3]-pozycjaPiksela.y,glebokosc,
		macierzModelWidok,macierzRzutowania,viewport,
		&wspolrzednePunktu3D[0],&wspolrzednePunktu3D[1],&wspolrzednePunktu3D[2]);

	//konwersja z double na float
	for(int i=0;i<3;i++) pozycjaPunktu3D[i]=(float)wspolrzednePunktu3D[i];
	return pozycjaPunktu3D;
}

POINT TransformujPunktScenyDoPrzestrzeniEkranu(double pozycja[3],POINT& pozycjaPiksela,float& glebokosc)
{
	//viewport i macierze
	int viewport[4]; glGetIntegerv(GL_VIEWPORT,viewport);
	double macierzModelWidok[16]; glGetDoublev(GL_MODELVIEW_MATRIX,macierzModelWidok);
	double macierzRzutowania[16]; glGetDoublev(GL_PROJECTION_MATRIX,macierzRzutowania);

	//bufor na wynik
	double pozycjaPiksela2D[3]={pozycjaPiksela.x,pozycjaPiksela.y,glebokosc};

	//transformacja
	gluProject(
		pozycja[0],pozycja[1],pozycja[2],
		macierzModelWidok,macierzRzutowania,viewport,
		&pozycjaPiksela2D[0],&pozycjaPiksela2D[1],&pozycjaPiksela2D[2]);

	//konwersja
	pozycjaPiksela.x=(long)pozycjaPiksela2D[0];
	pozycjaPiksela.y=(long)pozycjaPiksela2D[1];
	glebokosc=(float)pozycjaPiksela2D[2];

	return pozycjaPiksela;
}
