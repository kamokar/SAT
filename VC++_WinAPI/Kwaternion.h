//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef KWATERNION_H
#define KWATERNION_H

//wersja 1.1

#include <stdexcept>
#include "Varia.h"

template<typename T> struct TKwaternion
{
	public:
        //pola
		T s;
		TWektor<T> v;

	public:
		//konstruktor
        TKwaternion(T s,TWektor<T> v)
			:s(s),v(v)
        {
        }
        TKwaternion(T w,T x,T y,T z)
			:s(w),v(TWektor<T>(x,y,z))
        {
        }
		TKwaternion()
			:s(0),v(TWektor<T>::Zero())
		{
		}

		//operatory relacji
		bool operator==(const TKwaternion& q) const
		{
			return s==q.s && v==q.v;
		}
		bool operator!=(const TKwaternion& q) const 
		{
			return !(*this == q);
		}

		//operatory arytmetyczne
		TKwaternion& operator+=(const TKwaternion& q)
        {            
			s+=q.s;
			v+=q.v;
			return *this;
        };
		TKwaternion operator+(const TKwaternion& q) const
        {
            return TKwaternion(*this) += q;
        };		
		TKwaternion& operator-=(const TKwaternion& q)
        {            
			s-=q.s;
			v-=q.v;
			return *this;
        };
		TKwaternion operator-(const TKwaternion& q) const
        {
            return TKwaternion(*this) -= q;
        };
		TKwaternion& operator*=(const T s)
        {			
			//jak mnozenie przez kwaternion z v=0
			this->s*=s;
			v*=s;            
            return *this;
        };
        TKwaternion operator*(const T s) const
        {
            return TKwaternion(*this) *= s;
        };
		TKwaternion& operator/=(const T s)
        {						
			this->s/=s;
			v/=s;            
            return *this;
        };
        TKwaternion operator/(const T s) const
        {
            return TKwaternion(*this) /= s;
        };
		TKwaternion& operator*=(const TKwaternion q)
        {
			//uwaga! mnozenie kwaternionow nie jest przemienne
			T _s=s;
			TWektor<T> _v=v;

			s=_s*q.s-_v*q.v;
			v=_s*q.v+q.s*_v+(q.v^_v);
            
            return *this;
        };
        TKwaternion operator*(const TKwaternion q) const
        {
            return TKwaternion(*this) *= q;
        };
		TKwaternion operator-() const
		{
			return TKwaternion(-s,-v);
		}
		TKwaternion operator+() const
		{
			return *this;
		}

		//metody
		T KwadratNormy() const
		{
			return s*s+v*v;
		}
		T Norma() const
		{
			return sqrt(KwadratNormy());
		}
		void Normuj()
		{
			T norma=Norma();
			if(norma!=0) *this/=norma;
			else throw std::invalid_argument("Nie mozna unormowac zerowego kwaternionu");
		}
		TKwaternion Unormowany() const
		{
			TKwaternion<T> q(*this);
			q.Normuj();
			return q;
		}

		TKwaternion Sprzezony() const
		{
			return TKwaternion(s,-v);
		}
		TKwaternion Odwrotny() const
		{
			return Sprzezony()/KwadratNormy();
		}
		
		TKwaternion& operator/=(const TKwaternion q)
        {
			return TKwaternion<T>(*this)*=q.Odwrotny();
        };
        TKwaternion operator/(const TKwaternion q) const
        {
            return TKwaternion(*this) /= q;
        };

		T KatObrotu() const
		{
			//w=c=cos(theta/2)
			return 2*acos(s);
		}
		TWektor<T> OsObrotu() const
		{			
			T katObrotu=KatObrotu();
			if(katObrotu!=0) return v/sin(katObrotu/2);
			else throw std::exception("Nie mozna obliczyc osi obrotu dla zerowego kata");
		}

		TMacierz<T> MacierzObrotu() const
		{
			//elementy macierzy ulozone kolumnami
			TMacierz<T> m;
			//kolumna x
			m[0]=1-2*SQR(v.Y)-2*SQR(v.Z);
			m[1]=2*v.X*v.Y-2*v.Z*s;
			m[2]=2*v.X*v.Z+2*v.Y*s;
			//kolumna y
			m[3]=2*v.X*v.Y+2*v.Z*s;
			m[4]=1-2*SQR(v.X)-2*SQR(v.Z);
			m[5]=2*v.Y*v.Z-2*v.X*s;
			//kolumna z
			m[6]=2*v.X*v.Z-2*v.Y*s;
			m[7]=2*v.Y*v.Z+2*v.X*s;
			m[8]=1-2*SQR(v.X)-2*SQR(v.Y);
			return m;

		}

		static TKwaternion TworzZKataIOsiObrotu(const T katObrotu,const TWektor<T> osObrotu)
		{
			return TKwaternion(cos(katObrotu/2),osObrotu*sin(katObrotu/2));
		}
		static TKwaternion TworzZMacierzyObrotu(TMacierz<T> m)
		{
			/*
			0 3 6  00 01 02
			1 4 7  10 11 12
			2 5 8  20 21 22
			*/

			T slad=1+m[0]+m[4]+m[8];
			if(slad>=0)
			{
				T s=sqrt(slad)/2;
				TWektor<T> v(m[7]-m[5],m[2]-m[6],m[3]-m[1]);
				v/=(4*s);
				return TKwaternion(s,v);
			}
			else
			{
				//wybor najwiekszego elementu
				int i=0; //m00
				if(m[4]>m[0]) i=1; //m11
				if(m[8]>m[4]) i=2; //m22

				TWektor<T> v;
				T s=0,a;

				switch(i)
				{
					case 0:
						a=sqrt((m[0]-(m[4]+m[8]))+1)/2;
						v.X=a;
						a=1/(4*a);
						v.Y=(m[1]+m[3])*a;
						v.Z=(m[6]+m[2])*a;
						s=  (m[7]-m[5])*a;
						break;
					case 1:
						a=sqrt((m[4]-(m[8]+m[0]))+1)/2;
						v.Y=a;
						a=1/(4*a);
						v.X=(m[1]+m[3])*a;
						v.Z=(m[5]+m[7])*a;						
						s = (m[2]-m[6])*a;
						break;
					case 2:
						a=sqrt((m[8]-(m[0]+m[4]))+1)/2;
						v.Z=a;
						a=1/(4*a);
						v.X=(m[6]+m[2])*a;
						v.Y=(m[5]+m[7])*a;
						s = (m[3]-m[1])*a;
						break;
				}
				return TKwaternion(s,v);
			}			
		}

		static TKwaternion<T> Zero()
		{
			return TKwaternion(0,TWektor<T>::Zero());
		}
		static TKwaternion<T> Jeden()
		{
			return TKwaternion(1,TWektor<T>::Zero());
		}
};

template<typename T> TKwaternion<T> inline operator*(const T a,const TKwaternion<T> &q)
{
	return q*a;
}

//jak glRotated, ale dla radianow
template<typename T> 
TMacierz<T> TworzMacierzObrotuZKataIOsiObrotu(const T katObrotu,const TWektor<T> osObrotu)
{	
	return TKwaternion<T>::TworzZKataIOsiObrotu(katObrotu,osObrotu/osObrotu.Dlugosc()).MacierzObrotu();
}

typedef TKwaternion<double> Kwaternion;

//const Kwaternion KwaternionZero(0,0,0,0);

#endif