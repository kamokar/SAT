//Jacek Matulewski (2009), e-mail: jacek@fizyka.umk.pl

#ifndef PUNKTMATERIALNY_H
#define PUNKTMATERIALNY_H

#include "Wektor.h"

enum Algorytm {algorytmEulera, algorytmVerleta};

/********************************************************/
/*                  Punkt materialny                    */
/********************************************************/

template<typename T> class TPunktMaterialny
{
	private:		
		TWektor<T> polozenie;
		TWektor<T> predkosc;
		TWektor<T> poprzedniePolozenie;
		TWektor<T> nastepnePolozenie;
		TWektor<T> nastepnaPredkosc;
		T masa;		
		T promien;
		float kolor[4];
		void _TPunktMaterialny(TWektor<T> polozenie,TWektor<T> predkosc,T masa,T promien,float kolor[4]);
	public:
		TPunktMaterialny(TWektor<T> polozenie,TWektor<T> predkosc,T masa,T promien,float kolor[4]);
		TPunktMaterialny(void);		
		TWektor<T> Polozenie() const;
		TWektor<T> PoprzedniePolozenie() const;
		TWektor<T> NastepnePolozenie() const;
		TWektor<T> Predkosc() const;
		T Masa() const;
		T Promien() const;
		float* Kolor() const;		
		void UstawPolozenie(TWektor<T> polozenie);
		void UstawPoprzedniePolozenie(TWektor<T> poprzedniePolozenie);
		void UstawPredkosc(TWektor<T> predkosc);		
		void UstawMase(T masa);
		void UstawPromien(T promien);
		void UstawKolor(float kolor[4]);
		void UstawKolor(float r,float g,float b,float a=1);		
	private:
		int numerKroku;
		void PrzygotujRuch_Euler(TWektor<T> przyspieszenie,T krokCzasowy); 
		void PrzygotujRuch_Verlet(TWektor<T> przyspieszenie,T krokCzasowy); 
	public:		
		void PrzygotujRuch(TWektor<T> sila,T krokCzasowy,Algorytm algorytm=algorytmVerleta); 
		void WykonajRuch(); 
		int NumerKroku();
};

/********************************************************/
/*            Zbior punktow materialnych                */
/********************************************************/

template<typename T> class TZbiorPunktowMaterialnych //menedzer punktow materialnych
{	
	protected:
		int ilosc;
		TPunktMaterialny<T>* punkty;
		bool* wiezy;		

		virtual TWektor<T> Sila(int indeks) const =0;
		virtual void PrzedKrokiemNaprzod(T krokCzasowy){};
		virtual void PoPrzygotowaniuRuchu(T krokCzasowy){};
		virtual void PoKrokuNaprzod(T krokCzasowy){};

	public:
		TZbiorPunktowMaterialnych(int ilosc);
		virtual ~TZbiorPunktowMaterialnych();		
		virtual void PrzygotujRuch(T krokCzasowy,Algorytm algorytm=algorytmVerleta);
		void WykonajRuch();
		void KrokNaprzod(T krokCzasowy,Algorytm algorytm=algorytmVerleta);

		int LiczbaPunktow() const {return ilosc;};
		TPunktMaterialny<T>* PobierzPunktMaterialny(int indeks) const;
		TWektor<T> SrodekMasy() const;

		void UstawWiezy(int indeks,bool ustalonaPozycja);

	protected:
		void ZerujPredkoscSrednia();
};

/********************************************************/
/*                Obszar zabroniony                     */
/********************************************************/

template<typename T> class TObszarZabroniony
{
	private:
		T wspolczynnikOdbicia;
		T wspolczynnikTarcia;
	public:
		TObszarZabroniony(T wspolczynnikOdbicia,T wspolczynnikTarcia);
		T PobierzWspolczynnikOdbicia();
		T PobierzWspolczynnikTarcia();
		virtual bool CzyWObszarzeZabronionym(TWektor<T> polozenie,TWektor<T> poprzedniePolozenie,T margines,TWektor<T>* normalna) = 0;
};

/********************************************************/
/*  Zbior punktow materialnych z obszarem zabronionym   */
/********************************************************/

template<typename T> class TZbiorPunktowMaterialnychZObszaremZabronionym : public TZbiorPunktowMaterialnych<T>
{
	private:
		static const bool zaznaczajKontaktPunktuZObszaremZabronionym = false;

	protected:		
		TObszarZabroniony<T>* obszarZabroniony;
	private:		
		void PrzygotujRuchPrzyKontakcieZObszaremZabronionym(int indeks,T krokCzasowy);
	public:
		TZbiorPunktowMaterialnychZObszaremZabronionym(int ilosc);
		virtual ~TZbiorPunktowMaterialnychZObszaremZabronionym();
		TObszarZabroniony<T>* PobierzObszarZabroniony();
		void PrzygotujRuch(T krokCzasowy,Algorytm algorytm);
};

typedef TPunktMaterialny<double> PunktMaterialny;
typedef TZbiorPunktowMaterialnych<double> ZbiorPunktowMaterialnych;
typedef TObszarZabroniony<double> ObszarZabroniony;
typedef TZbiorPunktowMaterialnychZObszaremZabronionym<double> ZbiorPunktowMaterialnychZObszaremZabronionym;

#include "PunktMaterialny.cpp" //szablon wymaga dostepu do calej klasy w momencie kompilacji

#endif