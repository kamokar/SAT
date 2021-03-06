#ifndef MACIERZ_H
#define MACIERZ_H

//wersja 1.0

#include "Wektor.h"
#include <stdexcept>

template<typename T> struct TMacierz
{
    private:
        //pola
		static const int rozmiarKolumnyWiersza=3;
		static const int rozmiar=rozmiarKolumnyWiersza*rozmiarKolumnyWiersza;		
		T elementy[rozmiar]; //przechowujemy jak w OpenGL (column-majored odrder, a nie row-majored order) m00, m01, m02, m10, m11, ... (wiersze,kolumny)
		/*
		wiersz,kolumna
		00 01 02
		10 11 12
		20 21 22

		wiersz+kolumna*rozmiarKolumnyWiersza (column-majored order)
		0 3 6
		1 4 7
		2 5 8

		GL:
		0 3 6
		1 4 7
		2 5 8
		*/

		//dostep do elementow macierz
		T& KontrolowanyDostepDoElementu(int indeks)
		{
			if(indeks>=0 && indeks<rozmiar) return elementy[indeks];
			else throw std::invalid_argument("Indeks spoza zakresu");
		}
		T& KontrolowanyDostepDoElementu(int wiersz,int kolumna)
		{	
			//tu latwo zmienic zapis kolumnami na wierszami (jedyne miejsce)
			if(kolumna>=0 && kolumna<rozmiarKolumnyWiersza && wiersz>=0 && wiersz<rozmiarKolumnyWiersza) return elementy[wiersz+rozmiarKolumnyWiersza*kolumna];
			else throw std::invalid_argument("Indeks spoza zakresu");			
		}

	public:
		//operator indeksowania
		T& operator[](const int indeks)
		{
			return KontrolowanyDostepDoElementu(indeks);
		};

		T PobierzElement(int indeks) const
		{
			return const_cast<TMacierz*>(this)->KontrolowanyDostepDoElementu(indeks);
		}
		T PobierzElement(int wiersz,int kolumna) const
		{
			return const_cast<TMacierz*>(this)->KontrolowanyDostepDoElementu(wiersz,kolumna);
		}
		/*T& UstawElement(int wiersz,int kolumna)
		{
			return KontrolowanyDostepDoElementu(wiersz,kolumna);
		}*/
		void UstawElement(int indeks,T wartosc)
		{
			KontrolowanyDostepDoElementu(indeks)=wartosc;
		}
		void UstawElement(int wiersz,int kolumna,T wartosc)
		{
			KontrolowanyDostepDoElementu(wiersz,kolumna)=wartosc;
		}

		TWektor<T> KolumnaX() const
		{
			return TWektor<T>(this->PobierzElement(0,0),this->PobierzElement(1,0),this->PobierzElement(2,0));
		}
		TWektor<T> KolumnaY() const
		{
			return TWektor<T>(this->PobierzElement(0,1),this->PobierzElement(1,1),this->PobierzElement(2,1));
		}
		TWektor<T> KolumnaZ() const
		{
			return TWektor<T>(this->PobierzElement(0,2),this->PobierzElement(1,2),this->PobierzElement(2,2));
		}

		T* KopiaElementow(T bufor[rozmiar]) const
		{
			for(int i=0;i<rozmiar;++i) bufor[i]=PobierzElement(i);
			return bufor;
		}
		T* KopiaElementow4x4(T bufor[16]) const
		{
			for(int kolumna=0;kolumna<rozmiarKolumnyWiersza+1;++kolumna)
				for(int wiersz=0;wiersz<rozmiarKolumnyWiersza+1;++wiersz)
				{
					//int i=kolumna+(rozmiarKolumnyWiersza+1)*wiersz;
					int i=wiersz+(rozmiarKolumnyWiersza+1)*kolumna;
					if(kolumna<3 && wiersz<3) bufor[i]=PobierzElement(wiersz,kolumna);
					else bufor[i]=((wiersz==kolumna)?1:0);
				}
			return bufor;
		}

		//konstruktor
        TMacierz()
        {
            for(int i=0;i<rozmiar;++i) UstawElement(i,0);
        };
		TMacierz(const T elementy[9])
		{
			for(int i=0;i<rozmiar;++i) UstawElement(i,elementy[i]);
		};


		//operatory relacji
		bool operator==(const TMacierz& m) const
		{
			for(int i=0;i<rozmiar;++i) 
				if(PobierzElement(i)!=m.PobierzElement(i))
					return false;
			return true;
		}
		bool operator!=(const TMacierz& w) const 
		{
			return !(*this == w);
		}
		bool Rowna(const TMacierz& m,T tolerancjaBledu = 0) const
		{
			for(int i=0;i<rozmiar;++i) 
				if(fabs(PobierzElement(i)-m.PobierzElement(i))>tolerancjaBledu)
					return false;
			return true;
		}

		//operatory arytmetyczne
		TMacierz& operator+=(const TMacierz& m)
		{
			for(int i=0;i<rozmiar;++i) KontrolowanyDostepDoElementu(i)+=m.PobierzElement(i);
			return *this;
		}
		TMacierz operator+(const TMacierz& m) const
        {
            return TMacierz(*this) += m;
        };
		TMacierz& operator-=(const TMacierz& m)
		{
			for(int i=0;i<rozmiar;++i) KontrolowanyDostepDoElementu(i)-=m.PobierzElement(i);
			return *this;
		}
		TMacierz operator-(const TMacierz& m) const
        {
            return TMacierz(*this) -= m;
        };
		TMacierz& operator*=(const T a)
        {
			for(int i=0;i<rozmiar;++i) KontrolowanyDostepDoElementu(i)*=a;
            return *this;
        };
        TMacierz operator*(const T a) const
        {
            return TMacierz(*this) *= a;
        };
		TMacierz& operator/=(const T a)
        {
			for(int i=0;i<rozmiar;++i) KontrolowanyDostepDoElementu(i)/=a;
            return *this;
        };
        TMacierz operator/(const T a) const
        {
            return TMacierz(*this) /= a;
        };
		TMacierz& operator*=(const TMacierz& m)
        {
			TMacierz kopia=*this;
			this->ZerujElementy();
			for(int kolumna=0;kolumna<rozmiarKolumnyWiersza;++kolumna)
				for(int wiersz=0;wiersz<rozmiarKolumnyWiersza;++wiersz)
					for(int i=0;i<rozmiarKolumnyWiersza;++i)
						KontrolowanyDostepDoElementu(wiersz,kolumna)+=kopia.PobierzElement(wiersz,i)*m.PobierzElement(i,kolumna);
			return *this;
        };
		TMacierz operator*(const TMacierz m) const
        {
            return TMacierz(*this) *= m;
        };
		TWektor<T> operator*(const TWektor<T> w) const
        {
			TWektor<T> wynik(0,0,0);
			for(int wiersz=0;wiersz<rozmiarKolumnyWiersza;++wiersz)				
				wynik[wiersz]=this->PobierzElement(wiersz,0)*w.X+this->PobierzElement(wiersz,1)*w.Y+this->PobierzElement(wiersz,2)*w.Z;
			return wynik;
        };

		//metody		
		void ZerujElementy()
		{
			for(int i=0;i<rozmiar;++i) KontrolowanyDostepDoElementu(i)=0;
		}

		T Slad() const
		{
			T slad=0;
			for(int kolumna=0;kolumna<rozmiarKolumnyWiersza;++kolumna)
				slad+=PobierzElement(kolumna,kolumna);
			return slad;
		}
		T Wyznacznik() const
		{
			//nie uzywam metody Minor, aby przyspieszyc obliczenia
			/*
			00*11*22+
			01*12*20+
			02*10*21-
			20*11*02-
			21*12*00-
			22*10*01
			*/
			/*
			return PobierzElement(0,0)*PobierzElement(1,1)*PobierzElement(2,2)+
				   PobierzElement(0,1)*PobierzElement(1,2)*PobierzElement(2,0)+
				   PobierzElement(0,2)*PobierzElement(1,0)*PobierzElement(2,1)-
				   PobierzElement(2,0)*PobierzElement(1,1)*PobierzElement(0,2)-
				   PobierzElement(2,1)*PobierzElement(1,2)*PobierzElement(0,0)-
				   PobierzElement(2,2)*PobierzElement(1,0)*PobierzElement(0,1);			
		   */
			return  PobierzElement(0,0)*(PobierzElement(1,1)*PobierzElement(2,2)-PobierzElement(2,1)*PobierzElement(1,2))
				   -PobierzElement(0,1)*(PobierzElement(1,0)*PobierzElement(2,2)-PobierzElement(2,0)*PobierzElement(1,2))
				   +PobierzElement(0,2)*(PobierzElement(1,0)*PobierzElement(2,1)-PobierzElement(2,0)*PobierzElement(1,1));
		}
		TMacierz Transponowana() const
		{
			TMacierz m;
			m.ZerujElementy();
			for(int kolumna=0;kolumna<rozmiarKolumnyWiersza;++kolumna)
				for(int wiersz=0;wiersz<rozmiarKolumnyWiersza;++wiersz)
					m.KontrolowanyDostepDoElementu(wiersz,kolumna)=PobierzElement(kolumna,wiersz);
			return m;
		}
		T Minor(int wiersz,int kolumna) const //kolumna i wiersz do wyciecia
		//wyznacznik macierzy 2x2 bez kolumny i wiersza
		{
			//elementy macierz 2x2
			int k0=0; if(kolumna==0) k0=1;
			int k1=1; if(kolumna<2) k1=2;
			int w0=0; if(wiersz==0) w0=1;
			int w1=1; if(wiersz<2) w1=2;
			return PobierzElement(w0,k0)*PobierzElement(w1,k1)-PobierzElement(w1,k0)*PobierzElement(w0,k1);
		}
		TMacierz Odwrotna() const
		{
			//gdy mamy pewnosc, ze macierz obrotu, lepiej do policzenia macierzy odwrotnej uzyc metody Transponowana
			T wyznacznik=Wyznacznik();
			if(wyznacznik==0) throw std::invalid_argument("Nie mozna odwrocic macierzy osobliwej");
			
			TMacierz m;
			m.ZerujElementy();
			for(int kolumna=0;kolumna<rozmiarKolumnyWiersza;++kolumna)
				for(int wiersz=0;wiersz<rozmiarKolumnyWiersza;++wiersz)
				{
					m.KontrolowanyDostepDoElementu(wiersz,kolumna)=(((wiersz+kolumna)%2==0)?1:-1)*this->Minor(kolumna,wiersz);
				}
			
			return m/wyznacznik;
		}

		static TMacierz OperatorGwiazdki(TWektor<T> w)
		{
			//zapis transponowany w wierszach kodu sa kolumny macierzy
			T e[rozmiar]={
				0,w.Z,-w.Y,
				-w.Z,0,w.X,
				w.Y,-w.X,0};
			return TMacierz<T>(e);
		}

		static TMacierz Jednostkowa()
		{
			const T elementyMacierzyJednostkowej[9]={1,0,0,  0,1,0,  0,0,1};
			return TMacierz<T>(elementyMacierzyJednostkowej);
		}
		static TMacierz Zerowa()
		{
			const T elementyMacierzyJednostkowej[9]={0,0,0,  0,0,0,  0,0,0};
			return TMacierz<T>(elementyMacierzyJednostkowej);
		}

		//static 
};

typedef TMacierz<double> Macierz;

const Macierz MacierzZero();
//const double elementyMacierzyJednostkowej[9]={1,0,0,  0,1,0,  0,0,1};
//const Macierz MacierzJednostkowa(elementyMacierzyJednostkowej);

#endif