//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef UKLADYPUNKTOWMATERIALNYCH2_H
#define UKLADYPUNKTOWMATERIALNYCH2_H

#include "PunktMaterialny.h"

class Pudlo : public ObszarZabroniony
{
	private:
		double minX,maxX,minY,maxY,minZ,maxZ;

	public:
		Wektor PobierzSrodek()
		{
			return Wektor((minX+maxX)/2.0,(minY+maxY)/2.0,(minZ+maxZ)/2.0);
		}

		Wektor PobierzRozmiar()
		{
			return Wektor(maxX-minX,maxY-minY,maxZ-minZ);
		}

		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{			
			double _minX=minX+margines,_maxX=maxX-margines;
			double _minY=minY+margines,_maxY=maxY-margines;
			double _minZ=minZ+margines,_maxZ=maxZ-margines;
			bool wynik=polozenie.X<_minX || polozenie.X>_maxX || polozenie.Y<_minY || polozenie.Y>_maxY || polozenie.Z<_minZ || polozenie.Z>_maxZ;
			if(wynik && normalna!=NULL)
			{	
				//Wektor przemieszczenie=polozenie-poprzedniePolozenie;
				if(polozenie.X<_minX) normalna->X=1;
				if(polozenie.X>_maxX) normalna->X=-1;
				if(polozenie.Y<_minY) normalna->Y=1;
				if(polozenie.Y>_maxY) normalna->Y=-1;
				if(polozenie.Z<_minZ) normalna->Z=1;
				if(polozenie.Z>_maxZ) normalna->Z=-1;
			}
			return wynik;
		}

		Pudlo(double wspolczynnikOdbicia,double wspolczynnikTarcia,double minX,double maxX,double minY,double maxY,double minZ,double maxZ)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia),
			 minX(minX),maxX(maxX),minY(minY),maxY(maxY),minZ(minZ),maxZ(maxZ)
		{	
		}
};

class ZbiorPunktowMaterialnychZeZderzeniami : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		double e;

	public:
		ZbiorPunktowMaterialnychZeZderzeniami(int ilosc,double wspolczynnikRestytucji)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(ilosc),
			 e(wspolczynnikRestytucji)
		{			
		}

	private:
		void KolizjeCentralne(double krokCzasowy)
		{
			for(int i=0;i<ilosc;i++)
			{
				PunktMaterialny* punktI=PobierzPunktMaterialny(i);
				double mI=punktI->Masa();
				for(int j=i+1;j<ilosc;j++)
				{										
					PunktMaterialny* punktJ=PobierzPunktMaterialny(j);

					double nastepnaOdleglosc=(punktI->NastepnePolozenie()-punktJ->NastepnePolozenie()).Dlugosc();
					if(nastepnaOdleglosc<=(punktI->Promien()+punktJ->Promien()))
					{						
						double mJ=punktJ->Masa();
						double odwSumyMas=1/(mI+mJ);
			
						Wektor uI=((e+1)*mJ*punktJ->Predkosc()+punktI->Predkosc()*(mI-e*mJ))*odwSumyMas;
						Wektor uJ=((e+1)*mI*punktI->Predkosc()+punktJ->Predkosc()*(mJ-e*mI))*odwSumyMas;
						punktI->UstawPredkosc(uI);
						punktJ->UstawPredkosc(uJ);						
						punktI->PrzygotujRuch(Sila(i),krokCzasowy,algorytmEulera);
						punktJ->PrzygotujRuch(Sila(j),krokCzasowy,algorytmEulera);
					}
				}
			}
		}

		void Kolizje(double krokCzasowy)
		{
			for(int i=0;i<ilosc;i++)
			{
				PunktMaterialny* punktI=PobierzPunktMaterialny(i);
				double mI=punktI->Masa();
				for(int j=i+1;j<ilosc;j++)
				{										
					PunktMaterialny* punktJ=PobierzPunktMaterialny(j);

					double nastepnaOdleglosc=(punktI->NastepnePolozenie()-punktJ->NastepnePolozenie()).Dlugosc();
					if(nastepnaOdleglosc<=(punktI->Promien()+punktJ->Promien()))
					{						
						Wektor n=punktI->Polozenie()-punktJ->Polozenie();
						n.Normuj();

						double mJ=punktJ->Masa();
						double masaZred=1/(1/mI+1/mJ);

						double Dvn=(punktI->Predkosc()-punktJ->Predkosc())*n;

						double J=-masaZred*(e+1)*Dvn;
			
						Wektor uI=punktI->Predkosc()+n*J/mI;
						Wektor uJ=punktJ->Predkosc()-n*J/mJ;
						punktI->UstawPredkosc(uI);
						punktJ->UstawPredkosc(uJ);						
						punktI->PrzygotujRuch(Sila(i),krokCzasowy,algorytmEulera);
						punktJ->PrzygotujRuch(Sila(j),krokCzasowy,algorytmEulera);
					}
				}
			}
		}

	protected:
		void PoPrzygotowaniuRuchu(double krokCzasowy)
		{
			//KolizjeCentralne(krokCzasowy);
			Kolizje(krokCzasowy);
		}
};

#define _USE_MATH_DEFINES
#include <math.h>
#include <time.h>

class ZderzajaceSiePunkty  : public ZbiorPunktowMaterialnychZeZderzeniami
{
	public:
		ZderzajaceSiePunkty(int ilosc,double wspolczynnikRestytucji,Wektor polozenieMin,Wektor polozenieMax,double predkoscMin,double predkoscMax,double masaMin,double masaMax)
			:ZbiorPunktowMaterialnychZeZderzeniami(ilosc,wspolczynnikRestytucji)
		{
			obszarZabroniony=new Pudlo(1,0,1.1*polozenieMin.X,1.1*polozenieMax.X,1.1*polozenieMin.Y,1.1*polozenieMax.Y,-1,1);
			
			srand((unsigned)time(NULL));
			for(int i=0;i<ilosc;++i)
			{
				bool wynikTestuNakrywania=false;
				do
				{
					double polozenieX=(double)rand()/(RAND_MAX+1)*(polozenieMax.X-polozenieMin.X)+polozenieMin.X;
					double polozenieY=(double)rand()/(RAND_MAX+1)*(polozenieMax.Y-polozenieMin.Y)+polozenieMin.Y;
					double polozenieZ=(double)rand()/(RAND_MAX+1)*(polozenieMax.Z-polozenieMin.Z)+polozenieMin.Z;				
					PunktMaterialny* punkt=PobierzPunktMaterialny(i);				
					punkt->UstawPolozenie(Wektor(polozenieX,polozenieY,polozenieZ));
					double predkoscKatPhi=(double)rand()/(RAND_MAX+1)*2*M_PI;					
					double szybkosc=(double)rand()/(RAND_MAX+1)*(predkoscMax-predkoscMin)+predkoscMin;
					punkt->UstawPredkosc(szybkosc*Wektor(cos(predkoscKatPhi),sin(predkoscKatPhi),0));
					double masa=(double)rand()/(RAND_MAX+1)*(masaMax-masaMin)+masaMin;
					punkt->UstawMase(masa);				
					punkt->UstawKolor(i/(float)ilosc,1-i/(float)ilosc,1);

					//test nakladania sfer
					wynikTestuNakrywania=false;
					for(int j=0;j<i;++j)
					{
						PunktMaterialny* punktJ=PobierzPunktMaterialny(j);
						if((punkt->Polozenie()-punktJ->Polozenie()).Dlugosc()<=(punkt->Promien()+punktJ->Promien()))
							wynikTestuNakrywania=true;
					}
				}
				while(wynikTestuNakrywania);
			}						
		}

	protected:
		Wektor Sila(int indeks) const
		{
			return Wektor(0,0,0);
		}
};

class ProblemBilardzisty : public ZbiorPunktowMaterialnychZeZderzeniami
{
	public:
		ProblemBilardzisty(double promien,double parametrZderzenia)
			:ZbiorPunktowMaterialnychZeZderzeniami(2,1)
		{			
			PunktMaterialny* punkt1=PobierzPunktMaterialny(0);
			punkt1->UstawPolozenie(Wektor(-2,parametrZderzenia,0));
			punkt1->UstawPredkosc(Wektor(1,0,0));
			punkt1->UstawMase(1);
			punkt1->UstawPromien(promien);
			punkt1->UstawKolor(0,1,0);

			PunktMaterialny* punkt2=PobierzPunktMaterialny(1);
			punkt2->UstawPolozenie(Wektor(0,0,0));
			punkt2->UstawPredkosc(Wektor(0,0,0));
			punkt2->UstawMase(1);
			punkt2->UstawPromien(promien);
			punkt2->UstawKolor(1,0,0);
		}

	private:
		Wektor Sila(int indeks) const
		{
			return Wektor(0,0,0);
		}
};

class PunktyPoruszajaceSiePionowoIPoziomo : public ZbiorPunktowMaterialnychZeZderzeniami
{
	public:
		PunktyPoruszajaceSiePionowoIPoziomo(int ilosc1,int ilosc2,Wektor polozenieMin,Wektor polozenieMax,double predkoscMin,double predkoscMax,double masaMin,double masaMax)
			:ZbiorPunktowMaterialnychZeZderzeniami(ilosc1+ilosc2,1)
		{
			obszarZabroniony=new Pudlo(1,0,1.1*polozenieMin.X,1.1*polozenieMax.X,1.1*polozenieMin.Y,1.1*polozenieMax.Y,-1,1);

			srand((unsigned)time(NULL));
			for(int grupa=1;grupa<=2;grupa++)
			{				
				int offset=0;
				int ilosc=ilosc1;
				if (grupa==2) 
				{
					offset=ilosc1;
					ilosc=ilosc2;
				}
				for(int i=0;i<ilosc;++i)
				{
					bool wynikTestuNakrywania=false;
					do
					{
						double polozenieX=(double)rand()/(RAND_MAX+1)*(polozenieMax.X-polozenieMin.X)+polozenieMin.X;
						double polozenieY=(double)rand()/(RAND_MAX+1)*(polozenieMax.Y-polozenieMin.Y)+polozenieMin.Y;
						double polozenieZ=(double)rand()/(RAND_MAX+1)*(polozenieMax.Z-polozenieMin.Z)+polozenieMin.Z;						
						PunktMaterialny* punkt=PobierzPunktMaterialny(i+offset);
						punkt->UstawPolozenie(Wektor(polozenieX,polozenieY,polozenieZ));
						double predkoscKatPhi=(double)rand()/(RAND_MAX+1)*2*M_PI;						
						if (grupa==1) predkoscKatPhi=0;
						if (grupa==2) predkoscKatPhi=M_PI/2;
						double szybkosc=(double)rand()/(RAND_MAX+1)*(predkoscMax-predkoscMin)+predkoscMin;
						punkt->UstawPredkosc(szybkosc*Wektor(cos(predkoscKatPhi),sin(predkoscKatPhi),0));
						double masa=(double)rand()/(RAND_MAX+1)*(masaMax-masaMin)+masaMin;
						punkt->UstawMase(masa);				
						if(grupa==1) punkt->UstawKolor(1,1,0); else punkt->UstawKolor(0,1,0);

						//test nakladania sfer
						wynikTestuNakrywania=false;
						for(int j=0;j<i;++j)
						{
							PunktMaterialny* punktJ=PobierzPunktMaterialny(j);
							if((punkt->Polozenie()-punktJ->Polozenie()).Dlugosc()<=(punkt->Promien()+punktJ->Promien()))
								wynikTestuNakrywania=true;
						}
					}
					while(wynikTestuNakrywania);
				}						
			}
		}

	private:
		Wektor Sila(int indeks) const
		{
			return Wektor(0,0,0);
		}
};

class RuchyBrowna  : public ZderzajaceSiePunkty
{
	public:
		RuchyBrowna(int ilosc,int iloscKropliTluszczu,Wektor polozenieMin,Wektor polozenieMax,double predkoscMin,double predkoscMax,double masaMin,double masaMax)
			:ZderzajaceSiePunkty(ilosc+iloscKropliTluszczu,1,polozenieMin,polozenieMax,predkoscMin,predkoscMax,masaMin,masaMax)
		{	
			for(int i=0;i<ilosc;i++) PobierzPunktMaterialny(i)->UstawPromien(0.05);

			for(int i=0;i<iloscKropliTluszczu;i++)
			{
				//nie sprawdzam czy nie pokrywaja sie z innymi punktami i kroplami (zakladam, ze w rzeczywistosci moga)
				PunktMaterialny* ostatniPunkt=PobierzPunktMaterialny(ilosc+i);
				ostatniPunkt->UstawKolor(1,1,0,0.75);
				ostatniPunkt->UstawMase(10);			
				ostatniPunkt->UstawPromien(0.3);			
			}
		}
};


//Sterowanie - klawiatura
class Poduszkowce : public ZderzajaceSiePunkty
{
	private:
		PunktMaterialny* sterowanyPunkt;
		Wektor przyspieszeniePoduszkowca;
		double minX,maxX,minY,maxY;

	public:
		Poduszkowce(int ilosc,Wektor polozenieMin,Wektor polozenieMax,double predkoscMin,double predkoscMax,double masaMin,double masaMax)
		  :ZderzajaceSiePunkty(ilosc,1,polozenieMin,polozenieMax,predkoscMin,predkoscMax,masaMin,masaMax),
		  przyspieszeniePoduszkowca(Wektor::Zero())
		{
			sterowanyPunkt=PobierzPunktMaterialny(0);
			sterowanyPunkt->UstawKolor(0,1,0);
			sterowanyPunkt->UstawPolozenie(Wektor(polozenieMin.X,0,0));
			sterowanyPunkt->UstawPredkosc(Wektor(0,0,0));
		}

		Wektor PobierzPrzyspieszeniePoduszkowca()
		{
			return przyspieszeniePoduszkowca;
		}

		void UstawPrzyspieszeniePoduszkowca(Wektor przyspieszenie)
		{
			przyspieszeniePoduszkowca=przyspieszenie;			
		}

	private:
		Wektor SilaDzialajacaNaPoduszkowiec() const
		{
			return sterowanyPunkt->Masa()*przyspieszeniePoduszkowca;
		}

		Wektor Sila(int indeks) const
		{
			Wektor sila=ZderzajaceSiePunkty::Sila(indeks);
			if(indeks==0) sila+=SilaDzialajacaNaPoduszkowiec();
			return sila;
		}
};

class Bilard : public ZderzajaceSiePunkty
{
	private:
		double t;
		int indeksUderzanejBili;
		Wektor przyspieszenieUderzanejBili;	
		bool trybKontroliKija; //true - kontrola kija, false - bile w ruchu

		void UstawKoloryBil_Osemka()
		//1 biala, 7 czerwonych, 7 zoltych, 1 czarna
		{
			PunktMaterialny* bila;

			//biala
			bila=PobierzPunktMaterialny(0);
			bila->UstawKolor(1,1,1);		
			//czerwone
			for(int i=1;i<8;++i)
			{
				bila=PobierzPunktMaterialny(i);
				bila->UstawKolor(1,0,0);				
			}
			//zolte
			for(int i=8;i<15;++i)
			{
				bila=PobierzPunktMaterialny(i);
				bila->UstawKolor(1,1,0);				
			}
			//czarna
			bila=PobierzPunktMaterialny(15);
			bila->UstawKolor(0,0,0);
		}

		void UstawKoloryBil()
		{
			PobierzPunktMaterialny(0)->UstawKolor(1,1,1); //0 - biala
			PobierzPunktMaterialny(1)->UstawKolor(1,1,0); //1 - ¿ó³ta
			PobierzPunktMaterialny(2)->UstawKolor(0,0,1); //2 - niebieska
			PobierzPunktMaterialny(3)->UstawKolor(1,0,0); //3 - czerwona
			PobierzPunktMaterialny(4)->UstawKolor(1,0,1); //4 - fioletowa
			PobierzPunktMaterialny(5)->UstawKolor(1,0.5f,0); //5 - pomarañczowa
			PobierzPunktMaterialny(6)->UstawKolor(0,1,0); //6 - zielona
			PobierzPunktMaterialny(7)->UstawKolor(0.75f,0.25f,0.25f); //7 - br¹zowa
			PobierzPunktMaterialny(8)->UstawKolor(0,0,0); //8 - czarna
			PobierzPunktMaterialny(9)->UstawKolor(1,1,0); //9 - ¿ó³ta
			PobierzPunktMaterialny(10)->UstawKolor(0,0,1); //10 - niebieska
			PobierzPunktMaterialny(11)->UstawKolor(1,0,0); //11 - czerwona
			PobierzPunktMaterialny(12)->UstawKolor(1,0,1); //12 - fioletowa
			PobierzPunktMaterialny(13)->UstawKolor(1,0.5f,0); //13 - pomarañczowa
			PobierzPunktMaterialny(14)->UstawKolor(0,1,0); //14 - zielona
			PobierzPunktMaterialny(15)->UstawKolor(0.75f,0.25f,0.25f); //15 - br¹zowa
		}

	public:
		Bilard(double wspolczynnikRestytucji,double wspolczynnikTarcia,Wektor polozenieMin,Wektor polozenieMax,double masa,double promien)
			:ZderzajaceSiePunkty(16,wspolczynnikRestytucji,polozenieMin,polozenieMax,0,0,masa,masa),
			 t(wspolczynnikTarcia),
			 indeksUderzanejBili(-1),przyspieszenieUderzanejBili(Wektor(0,0,0)),
			 trybKontroliKija(true)
		{
			obszarZabroniony=new Pudlo(1,0,polozenieMin.X,polozenieMax.X,polozenieMin.Y,polozenieMax.Y,-promien,promien);
						
			UstawKoloryBil();
	
			//polozenia poczatkowe bil (trojkat)
			//biala
			PunktMaterialny* bila=PobierzPunktMaterialny(0);
			bila->UstawPolozenie(Wektor(-1.25f,0,0));
			//bila->UstawPredkosc(Wektor(3,0,0));
			//trojkat
			int k=1;
			for(int i=1;i<=5;++i)
				for(int j=0;j<i;j++)
				{
					bila=PobierzPunktMaterialny(k);
					bila->UstawPolozenie(Wektor(0.75f+i*2*promien,-(i-1)*promien+j*2*promien,0));
					++k;
				}
		}

		void UderzenieBili(int indeks,Wektor przyspieszenie)
		{
			indeksUderzanejBili=indeks;
			przyspieszenieUderzanejBili=przyspieszenie;
			trybKontroliKija=false;
		}

		bool TrybKontroliKija()
		{
			return trybKontroliKija;
		}

	private:
		Wektor Sila(int indeks) const
		{
			Wektor sila=ZderzajaceSiePunkty::Sila(indeks);			
			PunktMaterialny* bila=PobierzPunktMaterialny(indeks);
			//uderzenie kijem
			if(indeks==indeksUderzanejBili) sila+=bila->Masa()*przyspieszenieUderzanejBili;				
			//sily tarcia o sukno stolu
			sila-=t*bila->Predkosc();
			return sila;			
		}

	protected:
		void PoKrokuNaprzod(double krokCzasowy)
		{
			if(indeksUderzanejBili!=-1)
			{
				indeksUderzanejBili=-1;
				przyspieszenieUderzanejBili=Wektor::Zero();
			}

			const double NUMER_ZERO=1E-3;
			if(!trybKontroliKija)
			{
				if(PobierzPunktMaterialny(0)->Predkosc().Dlugosc()<NUMER_ZERO)
				{
					bool wszystkieStoja=true;
					for(int i=1;i<ilosc;++i)
						if(!wiezy[i] && PobierzPunktMaterialny(i)->Predkosc().Dlugosc()>NUMER_ZERO) 
						{
							wszystkieStoja=false;
							break;
						}
					if(wszystkieStoja) 
					{
						Beep(100,10);
						trybKontroliKija=true;
					}
				}
			}
		}
};

class BilardZLuzami : public Bilard
{
	private:
		bool* bilaWLuzie;
		int iloscBilWLuzie;
		Wektor luza[6];
		const double promienLuzy;
		Wektor kieszen;

	public:
		BilardZLuzami(double wspolczynnikRestytucji,double wspolczynnikTarcia,Wektor polozenieMin,Wektor polozenieMax,double masa,double promien)
			:Bilard(wspolczynnikRestytucji,wspolczynnikTarcia,polozenieMin,polozenieMax,masa,promien),
			 promienLuzy(3*punkty[0].Promien()),iloscBilWLuzie(0)
		{
			bilaWLuzie=new bool[ilosc];
			for(int i=0;i<ilosc;++i) bilaWLuzie[i]=false;

			luza[0]=Wektor(polozenieMin.X,polozenieMin.Y,polozenieMin.Z);
			luza[1]=Wektor((polozenieMax.X+polozenieMin.X)/2.0,polozenieMin.Y,polozenieMin.Z);
			luza[2]=Wektor(polozenieMax.X,polozenieMin.Y,polozenieMin.Z);
			luza[3]=Wektor(polozenieMax.X,polozenieMax.Y,polozenieMin.Z);
			luza[4]=Wektor((polozenieMax.X+polozenieMin.X)/2.0,polozenieMax.Y,polozenieMin.Z);
			luza[5]=Wektor(polozenieMin.X,polozenieMax.Y,polozenieMin.Z);			

			kieszen=polozenieMax+1.5*promienLuzy*Wektor(1,1,0);
		}

		~BilardZLuzami()
		{
			delete [] bilaWLuzie;
		}

		Wektor PobierzPolozenieLuzy(int indeks)
		{
			if(indeks<0 || indeks>=6) throw std::exception("Nieprawid³owy numer ³uzy");
			Wektor polozenieIPromienLuzy=luza[indeks];
			polozenieIPromienLuzy.Z=promienLuzy;
			return polozenieIPromienLuzy;
		}		

	protected:
		void PoKrokuNaprzod(double krokCzasowy)
		{
			Bilard::PoKrokuNaprzod(krokCzasowy);

			for(int i=0;i<ilosc;++i)
			{
				PunktMaterialny* bila=PobierzPunktMaterialny(i);
				for(int j=0;j<6;++j)
				{
					Wektor odlegloscOdLuzy=bila->Polozenie()-luza[j];
					odlegloscOdLuzy.Z=0;
					if(odlegloscOdLuzy.Dlugosc()<promienLuzy)
					{						
						bila->UstawPredkosc(Wektor::Zero());
						bilaWLuzie[i]=true;
						iloscBilWLuzie++;
						bila->UstawPolozenie(kieszen-Wektor(0,(iloscBilWLuzie-1)*2.5*bila->Promien(),0));
						UstawWiezy(i,true);																		
						continue;
					}
				}
			}

			if(bilaWLuzie[0] && TrybKontroliKija())
			{
				PunktMaterialny* bialaBila=PobierzPunktMaterialny(0);
				bool bialaBilaUstawiona=true;
				Wektor propozycjaPolozenia(-1.25f,0,0);								
				do
				{										
					bialaBilaUstawiona=true;
					for(int k=1;k<ilosc;++k)
					{
						PunktMaterialny* bilaK=PobierzPunktMaterialny(k);
						if( (propozycjaPolozenia-bilaK->Polozenie()).Dlugosc() < 1.1*(bialaBila->Promien()+bilaK->Promien()) )
						{
							bialaBilaUstawiona=false;										
							propozycjaPolozenia.X-=bilaK->Promien();
							break;
						}
					}
				} while(!bialaBilaUstawiona);
				bilaWLuzie[0]=false;
				UstawWiezy(0,false);
				iloscBilWLuzie--;
				bialaBila->UstawPolozenie(propozycjaPolozenia); 
			}
		}
};

#endif