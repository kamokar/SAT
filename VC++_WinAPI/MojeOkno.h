#ifndef MOJEOKNOGL_H
#define MOJEOKNOGL_H

#include "OknoGL.h"
#include "Wektor.h"
#include "PunktMaterialny.h"

#include "UkladyPunktowMaterialnych.h"
#include "UkladyBrylSztywnych.h"

#include <typeinfo.h>

class CMojeOknoGL : public COknoGL
{
	private: 
		void RysujAktorow();		
	//zrodla swiatla
	private:
		void ZrodlaSwiatla();
		void MlecznaZarowka(float jasnosc);
		void ZoltaIZielonaMleczneZarowki();
		void Reflektor(float jasnoscRozblysk=1.0f,float jasnoscRozproszone=0.3f);
	//teksturowanie
	private:
		unsigned int indeksTekstury[3];
		void PrzygotujTekstury();
	public:
		CMojeOknoGL();
		~CMojeOknoGL();
	private:	
		//ZBS
		ZbiorBrylSztywnych* zbs;
		void RysujZbiorProstopadloscianow(ZbiorProstopadloscianow* zp,const double jednostkaDlugosci,bool koloruj) const;

		//timer animacji		
		LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam);
		Algorytm algorytm;
		double krokCzasowy;
		int iloscKrokowWSerii;
		long poprzedniCzas;
		//bool pauza;
		bool typRzutowania; //false=perspektywa,true=izometryczne
} okno;

COknoGL* p_okno=&okno;

#endif