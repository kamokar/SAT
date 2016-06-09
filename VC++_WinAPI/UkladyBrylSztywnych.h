//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef UKLADYBRYLSZTYWNYCH_H
#define UKLADYBRYLSZTYWNYCH_H

#include "BrylaSztywna.h"

//#define _USE_MATH_DEFINES
//#include <math.h>

struct Prostopadloscian : public BrylaSztywna
{
	private:
		double a,b,c; //dlugosci krawedzi
		double gestosc;

	public:
		double Masa() const
		{
			double objetosc=a*b*c;
			return gestosc*objetosc;
		}

		Prostopadloscian(double a,double b,double c,double gestosc,Wektor polozenie,Wektor predkosc,Macierz macierzObrotu,Wektor predkoscKatowa,float kolor[4])
			:BrylaSztywna(polozenie,predkosc,macierzObrotu,predkoscKatowa,Macierz::Jednostkowa(),kolor)
			
		{
			this->a=a;
			this->b=b;
			this->c=c;
			this->gestosc=gestosc;
			srodekMasy->UstawMase(Masa());

			tensorMomentuBezwladnosci*=Masa()/12.0;
			tensorMomentuBezwladnosci[0]*=b*b+c*c; //Ixx
			tensorMomentuBezwladnosci[4]*=a*a+c*c; //Iyy
			tensorMomentuBezwladnosci[8]*=a*a+b*b; //Izz
		}

		Wektor Rozmiar()
		{
			return Wektor(a,b,c);
		}

		void PolozeniaWierzcholkowWzgledemSrodkaMasy(Wektor wierzcholki[8],bool nastepnePolozenia)
		{
			//wersja nieekonomiczna
			for(int iz=-1;iz<=1;iz+=2)
				for(int iy=-1;iy<=1;iy+=2)
					for(int ix=-1;ix<=1;ix+=2)
					{
						int i=2*(iz+1)+(iy+1)+(ix+1)/2;
						if(!nastepnePolozenia)
							wierzcholki[i]=MacierzObrotu().Transponowana()*Wektor(ix*a/2,iy*b/2,iz*c/2);
						else
							wierzcholki[i]=NastepnaMacierzObrotu().Transponowana()*Wektor(ix*a/2,iy*b/2,iz*c/2);
					}
		}

		void ProstopadloscianOgraniczajacyAABB(Wektor* min,Wektor* max,bool nastepny=false)
		{
			Wektor polozeniaWierzcholkow[8];
			PolozeniaWierzcholkowWzgledemSrodkaMasy(polozeniaWierzcholkow,nastepny);
			*min=Wektor::Zero();
			for(int i=0;i<8;++i)
			{
				if(polozeniaWierzcholkow[i].X<min->X) min->X=polozeniaWierzcholkow[i].X;
				if(polozeniaWierzcholkow[i].Y<min->Y) min->Y=polozeniaWierzcholkow[i].Y;
				if(polozeniaWierzcholkow[i].Z<min->Z) min->Z=polozeniaWierzcholkow[i].Z;
			}
			*max=-*min;
		}

};

class ZbiorProstopadloscianow : public ZbiorBrylSztywnych
{
	public:
		ZbiorProstopadloscianow(int ilosc,double a,double b,double c,double gestosc)
			:ZbiorBrylSztywnych(ilosc)
		{
			typedef BrylaSztywna* PBrylaSztywna;
			bryly=new PBrylaSztywna[ilosc];
			srand(0);
			float kolor[4]={0,1,0,1}; //zielony
				//bryly[i]=new Prostopadloscian(a,b,c,gestosc,Wektor(-1+2*i,0,0), Wektor::Zero(), Macierz::Jednostkowa(),Wektor(0,0.01*i,0.1),kolor);
				//bryly[i]=new Prostopadloscian(a,b,c,gestosc,Wektor(-1+2*i,0,0),Wektor(0,0,0), Macierz::Jednostkowa(), Wektor(0,0,0),kolor);
				//bryly[i]=new Prostopadloscian(a,b,c,gestosc,Wektor(-1+2*i,0,0),WektorZero,Macierz(elementyMacierzyJednostkowej),Wektor(0,0.01*i,0.1),kolor);
				//bryly[i]=new Prostopadloscian(a,b,c,gestosc,Wektor(-1+2.5*i,0,0),WektorZero,Macierz(elementyMacierzyJednostkowej),Wektor(0,0.01*i,0.1),kolor);
				//bryly[i]=new Prostopadloscian(a,b,c,gestosc,Wektor(-1+2.5*i,0,0),WektorZero,Macierz(elementyMacierzyJednostkowej),WektorZero,kolor);
				//bryly[0] = new Prostopadloscian(a, b, c, gestosc, Wektor(-1 + 2.5*i, 0, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0.2*rand() / (RAND_MAX + 1.0), 0.2*rand() / (RAND_MAX + 1.0), 0.2*rand() / (RAND_MAX + 1.0)), kolor);

			bryly[0] = new Prostopadloscian(0.5, 2, 3, gestosc, Wektor(0.5, -1, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0, 0, 0), kolor);
			bryly[1] = new Prostopadloscian(1, 4, 6, gestosc, Wektor(1, 0, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0, 0, 0), kolor);
			bryly[2] = new Prostopadloscian(1, 2, 3, gestosc, Wektor(666, 0, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0, 0, 0), kolor);
			bryly[3] = new Prostopadloscian(1, 2, 3, gestosc, Wektor(666, 0, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0, 0, 0), kolor);
			bryly[4] = new Prostopadloscian(1, 2, 3, gestosc, Wektor(666, 0, 0), Wektor::Zero(), Macierz::Jednostkowa(), Wektor(0, 0, 0), kolor);
			
			//bryly[0]->UstawPredkoscKatowa(Wektor(0, 0, 0));
			//bryly[0]->UstawPredkoscSrodkaMasy(Wektor(0, 0, 0));

		}

		~ZbiorProstopadloscianow()
		{
			for(int i=0;i<ilosc;++i) delete bryly[i];
			delete [] bryly;
			bryly=NULL;
		}

			virtual void PoPrzygotowaniuRuchu(double krokCzasowy)
		{			
			//detekcja BS+OBB+reakcja na zderzenie
			for(int i=0;i<ilosc;++i)
			{
				Wektor polozenieSferyI=bryly[i]->NastepnePolozenieSrodkaMasy();
				for(int j=i+1;j<ilosc;++j)
				{
					
						//detekcja OBB
						Prostopadloscian* pI=(Prostopadloscian*)bryly[i];
						Prostopadloscian* pJ=(Prostopadloscian*)bryly[j];
	
						if(TestNakrywaniaDwochProstopadloscianow(pI,pJ,true))
						{
							Beep(100,1);
										
							Wektor wypadkowaSila,wypadkowyMomentSily;
							ObliczSileIMomentSily(i,wypadkowaSila,wypadkowyMomentSily);
							pI->PrzygotujRuch(wypadkowaSila,wypadkowyMomentSily,krokCzasowy,algorytmEulera);
							ObliczSileIMomentSily(j,wypadkowaSila,wypadkowyMomentSily);
							pJ->PrzygotujRuch(wypadkowaSila,wypadkowyMomentSily,krokCzasowy,algorytmEulera);
						}
					
									}
			}
					

		}


	protected:
		void ObliczSileIMomentSily(int indeks,Wektor& sila,Wektor& momentSily) const		
		{
			sila=Wektor::Zero();
			momentSily=Wektor::Zero();
			//if(indeks==0) momentSily=Wektor(0,0,0.01);
		}

		bool TestNakrywaniaDwochProstopadloscianow(Prostopadloscian* pI, Prostopadloscian* pJ, bool dummy)
		{
			return dummy;
		}

};
#endif
