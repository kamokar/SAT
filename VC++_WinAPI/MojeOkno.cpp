#include "MojeOkno.h"
#include "Varia.h" //JednostkowyWektorNormalny3fv
#include "Modele.h" //Rysuj...
//#include "glext.h"
#include "resource.h"

//#include "UkladyPunktowMaterialnych.h"
//#include "UkladyPunktowMaterialnych2.h"

#define _USE_MATH_DEFINES
#include <cmath>

#include "UkladyBrylSztywnych.h"

#include "Kwaternion.h"

CMojeOknoGL::CMojeOknoGL()
:COknoGL(),zbs(NULL),
algorytm(algorytmEulera),krokCzasowy(0.1),iloscKrokowWSerii(1),
//pauza(false),
typRzutowania(false)
{			
	//double testA[9]={0,1,2,3,4,5,6,7,8};
	//double testA[9]={1,2,3,4,5,6,7,8,9};
	double testA[9]={1,4,7,2,5,8,3,6,9};
	double test0[9]={3,56,7,2,5,8,1,6,9};
	Macierz m(testA);
	Macierz m0(test0);
	double testB[16];
	double testC[9];
	m.KopiaElementow(testC);
	m.KopiaElementow4x4(testB);

	double w=m.Wyznacznik();
	Macierz i=Macierz::Jednostkowa();
	Macierz m1=i+m;
	double w1=m1.Wyznacznik();
	Macierz m2=i*m1;
	Macierz m3=m1*m;
	Macierz m4=m*m1;
	Macierz m5=m0*m1;
	Macierz m6=m1*m0;
	double w5=m5.Wyznacznik();
	double w6=m6.Wyznacznik();

	Macierz gw=Macierz::OperatorGwiazdki(Wektor(1,2,3));

	Wektor v=Wektor(1,2,3);
	Wektor v1=m*v;

	double ea[9]={1,0,0,0,1,4,1,0,0};
	Macierz a=Macierz(ea);
	Wektor v2=a*v;

	Macierz m7=TworzMacierzObrotuZKataIOsiObrotu(3.14159265358979323846/4.0,v);	

	zbs=new ZbiorProstopadloscianow(5,1,0.5,1.5,1); krokCzasowy=0.1;

	#ifdef _DEBUG
	if(zbs==NULL) MessageBox(uchwytOkna,"Brak zbioru punktów materialnych","Debug info",MB_OK);
	#endif
}

CMojeOknoGL::~CMojeOknoGL()
{
	delete zbs;
	zbs=NULL;
}

void CMojeOknoGL::RysujZbiorProstopadloscianow(ZbiorProstopadloscianow* zp,const double jednostkaDlugosci,bool koloruj) const
{
	if(zp==NULL || !zp->TestObecnosciZBS()) return;

	glPushMatrix();

	for(int indeks=0;indeks<zp->LiczbaBryl();++indeks)
	{	
		//if(indeks>0) break;
		Prostopadloscian* prostopadloscian=(Prostopadloscian*)zp->PobierzBryleSztywna(indeks);
        Wektor polozenieProstopadloscianu=jednostkaDlugosci*prostopadloscian->PolozenieSrodkaMasy();
		Wektor rozmiarProstopadloscianu=jednostkaDlugosci*prostopadloscian->Rozmiar();
		
		double elementyMacierzyObrotuProstopadloscianu[16];
		//prostopadloscian->MacierzObrotu().Transponowana().KopiaElementow4x4(elementyMacierzyObrotuProstopadloscianu);		
		prostopadloscian->MacierzObrotu().KopiaElementow4x4(elementyMacierzyObrotuProstopadloscianu);		
		for(int i=0;i<16;++i) elementyMacierzyObrotuProstopadloscianu[i]*=jednostkaDlugosci;				
  
        if(!koloruj) glColor4fv(prostopadloscian->Kolor());

		glPushMatrix();	
		glTranslated(polozenieProstopadloscianu.X,polozenieProstopadloscianu.Y,polozenieProstopadloscianu.Z); //translacja		

		//oryginalna bryla
		glPushMatrix();
		glMultMatrixd(elementyMacierzyObrotuProstopadloscianu); //obrot		
		Modele::RysujProstopadloscian((float)rozmiarProstopadloscianu.X,(float)rozmiarProstopadloscianu.Y,(float)rozmiarProstopadloscianu.Z,koloruj,false,NULL);
		glPopMatrix();

		//rysowanie wierzcholkow oryginalnej figury
		/*
		Wektor polozeniaWierzcholkow[8];
		prostopadloscian->PolozeniaWierzcholkowWzgledemSrodkaMasy(polozeniaWierzcholkow);
		glPointSize(10);
		glColor3f(0,0,0);
		glBegin(GL_POINTS);
		for(int i=0;i<8;++i)
		{
			glVertex3d(polozeniaWierzcholkow[i].X,polozeniaWierzcholkow[i].Y,polozeniaWierzcholkow[i].Z);
		}
		glEnd();
		*/

		//AABB
		/*
		Wektor min,max;
		prostopadloscian->ProstopadloscianOgraniczajacyAABB(&min,&max);
		glColor4f(0.5f,1,1,0.2f);
		Modele::RysujProstopadloscian(float(max.X-min.X),float(max.Y-min.Y),float(max.Z-min.Z),false,false,NULL);
		*/

		//BS (niewydajna implementacja - ponowne liczenie promienia)
		/*
		glPushMatrix();
		glMultMatrixd(elementyMacierzyObrotuProstopadloscianu); //obrot		
		glColor3f(0,0,0);
		glRotatef(-90,1,0,0); //bieguny na osi OY	
		GLUquadricObj* kwadryka=gluNewQuadric(); //tworzenie obiektu kwadryki 	
		gluQuadricDrawStyle(kwadryka,GLU_SILHOUETTE);
		gluSphere(kwadryka,prostopadloscian->Rozmiar().Dlugosc()/2.0,10,10); //rysowanie 		
		gluDeleteQuadric(kwadryka); //usuwanie obiektu 
		glPopMatrix();
		*/

		glPopMatrix();						
    }

	glPopMatrix();
}

LRESULT CMojeOknoGL::WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam)
{	
	const int IDT_TIMER_OBLICZEN=3;
	const int IDT_TIMER_RYSOWANIA=4;

	long wynik=COknoGL::WndProc(hWnd,message,wParam,lParam);		

	switch (message)
	{
		case WM_CREATE: //pole uchwytOkna nie jest zainicjowane w WM_CREATE -> nalezy uzywac hWnd
			if (SetTimer(hWnd,IDT_TIMER_OBLICZEN,USER_TIMER_MINIMUM,NULL)==0)
				MessageBox(hWnd,"Nie uda³o siê ustawiæ timera obliczen","",MB_OK | MB_ICONERROR);
			if (SetTimer(hWnd,IDT_TIMER_RYSOWANIA,50,NULL)==0)
				MessageBox(hWnd,"Nie uda³o siê ustawiæ timera rysowania","",MB_OK | MB_ICONERROR);
			poprzedniCzas=GetTickCount();
			break;
		case WM_TIMER:
			if (pauza) break;
			switch(wParam)
			{
				case IDT_TIMER_OBLICZEN:
					if (zbs!=NULL)
						for(int i=0;i<iloscKrokowWSerii;i++) 
							zbs->KrokNaprzod(krokCzasowy,algorytm);					
					break;
				case IDT_TIMER_RYSOWANIA:
					RysujScene();
					break;
			}
			wynik=0;

			break;
		case WM_DESTROY:
			KillTimer(uchwytOkna,IDT_TIMER_OBLICZEN);
			KillTimer(uchwytOkna,IDT_TIMER_RYSOWANIA);
			break;

		case WM_KEYDOWN:
			bool wywolacRysujScene=true;
			switch(wParam)
			{
				case 'C':
					typRzutowania=!typRzutowania;
					UstawienieSceny(typRzutowania);
					break;
				case VK_SPACE:
					pauza=!pauza;
					wywolacRysujScene=false;
					break;
			}
			if (pauza && wywolacRysujScene) RysujScene();
			break;
	}
	
	return wynik;
}

void CMojeOknoGL::RysujAktorow()
{	
	/*
	glMatrixMode(GL_MODELVIEW_MATRIX);
	glLoadIdentity();
	glRotatef(45,1,2,3);
	//glTranslatef(1,2,3);
	float macierz[16];
	glGetFloatv(GL_MODELVIEW_MATRIX,macierz);	
	*/

	const double jednostkaDlugosci=1;

	//srodek ukladu wspolrzednych
	glColor3f(0,0,0);
	glPointSize(1);
	glBegin(GL_POINTS);
	glVertex3f(0,0,0);
	glEnd();	

	RysujZbiorProstopadloscianow((ZbiorProstopadloscianow*)zbs,jednostkaDlugosci,true);

	if(zbs!=NULL)
	{
		char bufor[256];
		int numerKroku=zbs->PobierzBryleSztywna(0)->NumerKroku();
		_itoa_s(numerKroku,bufor,256,10);
		SetWindowText(uchwytOkna,bufor);
	}
}

#pragma region Zrodla swiatla
//zrodla swiatla
void CMojeOknoGL::ZrodlaSwiatla()
{
	natezenie_swiatla_tla=0.5f;
	MlecznaZarowka(0.5f);
}


void CMojeOknoGL::MlecznaZarowka(float jasnosc)
{	
	const float kolor[4]={jasnosc,jasnosc,jasnosc,1.0f};
	const float pozycja[4]={5.0f,0.0f,5.0f,1.0f};	
	glLightfv(GL_LIGHT1,GL_POSITION,pozycja);
	glLightfv(GL_LIGHT1,GL_DIFFUSE,kolor);
	glEnable(GL_LIGHT1);
}

void CMojeOknoGL::ZoltaIZielonaMleczneZarowki()
{
	//zolta mleczna zarowka
	const float kolor_zolta[4]={1.0f,1.0f,0.0f,1.0f};
	const float pozycja_zolta[4]={-2.0f,0.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT2,GL_POSITION,pozycja_zolta);
	glLightfv(GL_LIGHT2,GL_DIFFUSE,kolor_zolta);
	//glEnable(GL_LIGHT2);

	//zielona mleczna zarowka
	const float kolor_zielony[4]={0.0f,1.0f,0.0f,1.0f};
	const float pozycja_zielony[4]={2.0f,0.0f,1.0f,1.0f};
	glLightfv(GL_LIGHT3,GL_POSITION,pozycja_zielony);
	glLightfv(GL_LIGHT3,GL_DIFFUSE,kolor_zielony);
	//glEnable(GL_LIGHT3);
}

void CMojeOknoGL::Reflektor(float jasnoscRozblysk,float jasnoscRozproszone)
{
	const float kolor_rozproszone[4]={jasnoscRozproszone,jasnoscRozproszone,jasnoscRozproszone,1.0f};
	const float kolor_rozblysk[4]={jasnoscRozblysk,jasnoscRozblysk,jasnoscRozblysk,1.0};
	const float pozycja[4]={-10.0f,-10.0f,10.0f,1.0f};	
	const float kierunek[4]={1.0,1.0,-1.0,1.0};   
	const float szerokosc_wiazki=30.0f; //w stopniach
	const float wygaszanie=1.0f;
   
	glLightfv(GL_LIGHT4,GL_POSITION,pozycja);
	glLightfv(GL_LIGHT4,GL_DIFFUSE,kolor_rozproszone);

	glLightfv(GL_LIGHT4,GL_SPECULAR,kolor_rozblysk);
	glLightfv(GL_LIGHT4,GL_SPOT_DIRECTION,kierunek);
	glLightf(GL_LIGHT4,GL_SPOT_CUTOFF,szerokosc_wiazki);
	glLightf(GL_LIGHT4,GL_SPOT_EXPONENT,wygaszanie);
	//glEnable(GL_LIGHT4);
}
#pragma endregion
