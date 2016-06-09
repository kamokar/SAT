//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef WEKTOR_H
#define WEKTOR_H

//wersja 2.3

#include <stdexcept>
#include <math.h>

template<typename T> struct TWektor
{
    public:
        //pola
		T X,Y,Z,W;

		/*
		union 
		{
			struct 
			{
				T X,Y,Z,W;
			};
			T wspolrzedne[4];
		};
		*/
		
		//konstruktor
        TWektor()
			:X(0),Y(0),Z(0),W(1)
        {
        };
        TWektor(T X,T Y,T Z,T W=1)
			:X(X),Y(Y),Z(Z),W(W)
        {
        }

		//operator indeksowania
		T& operator[](const int indeks)
		{
			switch(indeks)
			{
				case 0: return X; break;
				case 1: return Y; break;
				case 2: return Z; break;
				default: throw std::invalid_argument("Indeks spoza zakresu"); break;
			}
			//return wspolrzedne[indeks];
		};

		//operatory relacji
		bool operator==(const TWektor& w) const
		{
			return X==w.X && Y==w.Y && Z==w.Z && W==w.W;			
		}
		bool operator!=(const TWektor& w) const 
		{
			return !(*this == w);
		}
		bool Rownowazny(const TWektor& w) const
		{
			if(W==0 || w.W==0) throw std::exception("Jeden lub oba punkty znajduj¹ siê w nieskoñczonoœci (W=0)");
			return X/W==w.X/w.W && Y/W==w.Y/w.W && Z/W==w.Z/w.W;
		}

		//operatory arytmetyczne
		TWektor& operator+=(const TWektor& w)
        {            
			if(W==1)
			{
				X+=w.X;
				Y+=w.Y;
				Z+=w.Z;
			}
			else
			{
				X=X*w.W+w.X*W;
				Y=Y*w.W+w.Y*W;
				Z=Z*w.W+w.Z*W;
				W*=w.W;
			}
			return *this;
        };
		TWektor operator+(const TWektor& w) const
        {
            return TWektor(*this) += w;
        };		
		
		TWektor& operator-=(const TWektor& w)
        {
			if(W==1)
			{
				X-=w.X;
				Y-=w.Y;
				Z-=w.Z;				
			}
			else
			{
				X=X*w.W-w.X*W;
				Y=Y*w.W-w.Y*W;
				Z=Z*w.W-w.Z*W;
				W*=w.W;
			}
			return *this;
        };
        TWektor operator-(const TWektor& w) const
        {
            return TWektor(*this) -= w;
        };
		TWektor& operator*=(const T a)
        {
            X*=a;
            Y*=a;
            Z*=a;
			//wspolrzedna W pozostawiamy bez zmian
            return *this;
        };
        TWektor operator*(const T a) const
        {
            return TWektor(*this) *= a;
        };
		TWektor& operator/=(const T a)
        {
			if(a==0.0) throw std::invalid_argument("Dzielenie przez zero");
            X/=a;
            Y/=a;
            Z/=a;
			//wspolrzedna W pozostawiamy bez zmian
            return *this;
        };		
		TWektor operator/(const T a) const
        {
            return TWektor(*this) /= a;
        };

		TWektor operator-() const
		{
			return TWektor(-X,-Y,-Z,W);
		}
		TWektor operator+() const
        {
            return *this;
        };

		//iloczyn skalarny
		T operator*(const TWektor& w) const 
        {
			return (X*w.X+Y*w.Y+Z*w.Z)/(W*w.W); //wartosc z przestrzeni trojwymiarowej (to nie jest iloczyn skalarny w rozumieniu przestrzeni 4D)
        };

		//iloczyn wektorowy
		TWektor<T> operator^(const TWektor& w) const //operator ^ ma niski priorytet
		{
			if(W!=1 || w.W!=1) throw std::exception("Iloczyn wektorowy nie jest okreœlony we wspó³rzêdnych jednorodnych (wspó³rzêdna W musi byæ równa 1)");
			TWektor wynik;
			wynik.X=  Y*w.Z-Z*w.Y;
			wynik.Y=-(X*w.Z-Z*w.X);
			wynik.Z=  X*w.Y-Y*w.X;
			return wynik;
		}
		TWektor<T> operator%(const TWektor& w) const //operator % ma taki sam priorytet jak *
		{
			return *this^w;
		}

		//metody
		T KwadratDlugosci() const
		{
			if(W==0) throw std::exception("Punkt znajduje siê w nieskoñczonoœci (W=0)");
            return (X*X+Y*Y+Z*Z)/(W*W);
		}
		T Dlugosc() const
        {			
			return sqrt(KwadratDlugosci());
			//return (T)sqrt((long double)KwadratDlugosci());
        }
		void Normuj() //normalizacja w przestrzeni trojwymiarowej
		{
			T dlugosc=Dlugosc();
			if (dlugosc!=0) *this/=dlugosc;
			else throw std::invalid_argument("Nie mozna unormowac zerowego wektora");				
		}
		TWektor<T> Unormowany() const
		{
			TWektor<T> w(*this);
			w.Normuj();
			return w;
		}

		T* ZapiszWTablicy(T tablica[3]) const //przeksztalca do wspolrzednych trojwymiarowych
		{			
			if(W==0) throw std::exception("Punkt znajduje siê w nieskoñczonoœci (W=0)");
			tablica[0]=X/W;
			tablica[1]=Y/W;
			tablica[2]=Z/W;
			return tablica;
		}
		T* ZapiszWTablicy4(T tablica[4]) const
		{
			tablica[0]=X;
			tablica[1]=Y;
			tablica[2]=Z;
			tablica[3]=W;
			return tablica;
		}

		static TWektor<T> Zero()
		{
			return TWektor(0,0,0);
		}
		static TWektor<T> WersorX()
		{
			return TWektor(1,0,0);
		}
		static TWektor<T> WersorY()
		{
			return TWektor(0,1,0);
		}
		static TWektor<T> WersorZ()
		{
			return TWektor(0,0,1);
		}
		static TWektor<T> Jeden()
		{
			return TWektor(1,1,1);
		}
};

template<typename T> TWektor<T> inline operator*(const T a,const TWektor<T> &w)
{
	return w*a;
    //return TWektor<T>(a*w.X,a*w.Y,a*w.Z);	
}

template<typename T> TWektor<T> IloczynWektorowy(TWektor<T> a,TWektor<T> b)
{
	return a^b;
}

//specjalizacja dla int
template<> int TWektor<int>::Dlugosc() const
{
	return (int)sqrt((long double)KwadratDlugosci());        
}

typedef TWektor<double> Wektor;

//inicjacja wykonywana po teœcie
//static const Wektor WektorZero(0,0,0,1);
//Wektor WersorX(1,0,0);
//Wektor WersorY(0,1,0);
//Wektor WersorZ(0,0,1);

#endif