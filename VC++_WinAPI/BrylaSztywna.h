//Jacek Matulewski (2009), e-mail: jacek@fizyka.umk.pl

#ifndef BRYLASZTYWNA_H
#define BRYLASZTYWNA_H

#include "Wektor.h"
#include "Macierz.h"
#include "PunktMaterialny.h"

#define KWATERNIONY_ZAMIAST_MACIERZY

#ifdef KWATERNIONY_ZAMIAST_MACIERZY
#include "Kwaternion.h"
#endif

/********************************************************/
/*                    Moment sily                       */
/********************************************************/

/*
template<typename T> struct TMomentSily
{
	private:
		TWektor<T> ramie;
		TWektor<T> sila;

	public:
		TMomentSily(TWektor<T> ramie,TWektor<T> sila)
		{
			this->ramie=ramie;
			this->sila=sila;
		}

		TMomentSily()
		{
			this->ramie=WektorZero;
			this->sila=WektorZero;
		}

		TWektor<T> Sila()
		{
			return sila;
		}

		TWektor<T> MomentSily()
		{
			return ramie^sila;
		}
};
*/

/********************************************************/
/*                   Bry³a sztywna                      */
/********************************************************/

template<typename T> struct TBrylaSztywna
{
	protected: 
		TPunktMaterialny<T>* srodekMasy;
		
		#ifndef KWATERNIONY_ZAMIAST_MACIERZY
		TMacierz<T> macierzObrotu;
		TMacierz<T> poprzedniaMacierzObrotu;
		TMacierz<T> nastepnaMacierzObrotu;
		#else
		TKwaternion<T> kwaternion;
		TKwaternion<T> poprzedniKwaternion;
		TKwaternion<T> nastepnyKwaternion;
		#endif
		
		TWektor<T> predkoscKatowa;
		TWektor<T> nastepnaPredkoscKatowa;
		TMacierz<T> tensorMomentuBezwladnosci;
	public:
		TMacierz<T> odwroconyTensorMomentuBezwladnosci;		
	private:	
		void _TBrylaSztywna(TWektor<T> polozenie,TWektor<T> predkosc,TMacierz<T> macierzObrotu,TWektor<T> predkoscKatowa,TMacierz<T> tensorMomentuBezwladnosci,float kolor[4]);

	public:
		TBrylaSztywna(TWektor<T> polozenie,TWektor<T> predkosc,TMacierz<T> macierzObrotu,TWektor<T> predkoscKatowa,TMacierz<T> tensorMomentuBezwladnosci,float kolor[4]);
		TBrylaSztywna();
		~TBrylaSztywna();
		TWektor<T> PolozenieSrodkaMasy() const;
		TWektor<T> PredkoscSrodkaMasy() const;
		TMacierz<T> MacierzObrotu() const;
		TWektor<T> PredkoscKatowa() const;
		float* Kolor() const;		

		TMacierz<T> NastepnaMacierzObrotu() const; //!!!NOWE-TEST
		TWektor<T> NastepnePolozenieSrodkaMasy() const;

		void UstawPolozenieSrodkaMasy(TWektor<T> polozenie);
		void UstawOrientacje(TMacierz<T> macierzObrotu);
		void UstawPredkoscSrodkaMasy(TWektor<T> predkosc);
		void UstawPredkoscKatowa(TWektor<T> predkoscKatowa);

		//dynamika
	private:		
		//void ObliczMomentBezwladnosci();		
		int numerKroku;
		void PrzygotujObrot_Euler(TWektor<T> momentSily,T krokCzasowy); 
		void PrzygotujObrot_Verlet(TWektor<T> przyspieszenie,T krokCzasowy); 
	public:		
		void PrzygotujRuch(TWektor<T> sila,TWektor<T> momentySily,T krokCzasowy,Algorytm algorytm=algorytmVerleta); 
		void WykonajRuch(); 
		int NumerKroku() const;
};

/********************************************************/
/*               Zbior bryl sztywnych                   */
/********************************************************/

template<typename T> class TZbiorBrylSztywnych //menedzer bryl sztywnych
{	
	protected:
		int ilosc;
		TBrylaSztywna<T>** bryly;
		bool* wiezy;				
		
		virtual void ObliczSileIMomentSily(int indeks,TWektor<T>& sila,TWektor<T>& momentSily) const =0;
		virtual void PrzedKrokiemNaprzod(T krokCzasowy){};
		virtual void PoPrzygotowaniuRuchu(T krokCzasowy){};
		virtual void PoKrokuNaprzod(T krokCzasowy){};

	public:
		bool TestObecnosciZBS() const; //zglasza wyjatek w razie braku zbioru 'bryly'

		TZbiorBrylSztywnych(int ilosc);
		virtual ~TZbiorBrylSztywnych();		
		virtual void PrzygotujRuch(T krokCzasowy,Algorytm algorytm=algorytmEulera);
		void WykonajRuch();
		void KrokNaprzod(T krokCzasowy,Algorytm algorytm=algorytmEulera);

		int LiczbaBryl() const {return ilosc;};
		TBrylaSztywna<T>* PobierzBryleSztywna(int indeks) const;

		void UstawWiezy(int indeks,bool ustalonaPozycja);
};

//typedef TMomentSily<double> MomentSily;
typedef TBrylaSztywna<double> BrylaSztywna;

typedef TZbiorBrylSztywnych<double> ZbiorBrylSztywnych;

#include "BrylaSztywna.cpp"

#endif