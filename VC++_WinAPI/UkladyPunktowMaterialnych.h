//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef UKLADYPUNKTOWMATERIALNYCH_H
#define UKLADYPUNKTOWMATERIALNYCH_H

#include "PunktMaterialny.h"

#pragma region "Uklady 1D"
class Oscylator : public ZbiorPunktowMaterialnych
{
	private:
		double k; //wspolczynnik sprezystosci

	public:
		Oscylator(double k)
			:ZbiorPunktowMaterialnych(1),
			k(k)
		{	
			PunktMaterialny* punkt=PobierzPunktMaterialny(0);
			punkt->UstawPolozenie(Wektor(-1,0,0));
			//punkt->UstawPredkosc(Wektor(0,0,0));
			punkt->UstawPredkosc(Wektor(1,1,0));
			punkt->UstawKolor(0,1,0.5);
		};

	private:
		Wektor Sila(int indeks) const
		{		
			return -k*PobierzPunktMaterialny(0)->Polozenie();			
		};
};

//class OscylatorySprzezone : public ZbiorPunktowMaterialnych
class OscylatorySprzezone : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	protected:
		double k; //wspolczynnik sprezystosci
		double l; //odleglosc spoczynkowa
		double t,tt; //wspolczynniki tlumienia (sily oporu i tlumienie oscylacji)

	protected:
		bool sprezystoscTylkoPrzyRozciaganiu;

	public:
		OscylatorySprzezone(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double dlugosc)
			//:ZbiorPunktowMaterialnych(ilosc),
			:ZbiorPunktowMaterialnychZObszaremZabronionym(ilosc),
			k(wspolczynnikSprezystosci),t(wspolczynnikTlumienia),tt(wspolczynnikTlumieniaOscylacji),
			sprezystoscTylkoPrzyRozciaganiu(false),
			l(dlugosc/(ilosc-1))
		{									
			for(int i=0;i<ilosc;++i)
			{
				PunktMaterialny* punkt=PobierzPunktMaterialny(i);
				punkt->UstawPolozenie(Wektor(-dlugosc/2+i*l,0,0));
				punkt->UstawPredkosc(Wektor(0,0,0)); //spoczywajace				
				punkt->UstawKolor(0,(float)i/(ilosc-1),1);				
			}			

			//1D
			//PobierzPunktMaterialny(0)->UstawPredkosc(Wektor(0.3,0,0));			

			//3D
			//PobierzPunktMaterialny(0)->UstawPredkosc(Wektor(0.3,0.1,0));
			//PobierzPunktMaterialny(ilosc-1)->UstawPredkosc(Wektor(-0.3,-0.1,0));

			//3D - poprzeczne
			PobierzPunktMaterialny(ilosc/2-1)->UstawPredkosc(Wektor(0,0.1,0));

			//poczatkowa predkosc srednia ma byc rowna zero
			//ZerujPredkoscSrednia();
		}

	protected:
		Wektor Sila(int indeks) const
		{	
			//1D
			//double silaZLewej=0,silaZPrawej=0;
			//if (indeks>0) silaZLewej=-k*(PobierzPunktMaterialny(indeks)->Polozenie().X-PobierzPunktMaterialny(indeks-1)->Polozenie().X-l);
			//if (indeks<LiczbaPunktow()-1) silaZPrawej=k*(PobierzPunktMaterialny(indeks+1)->Polozenie().X-PobierzPunktMaterialny(indeks)->Polozenie().X-l);
			//return Wektor(silaZLewej+silaZPrawej,0,0);

			//3D			
			Wektor silaZLewej=Wektor(0,0,0),silaZPrawej=Wektor(0,0,0);
			if (indeks>0) 
			{
				Wektor doLewego=PobierzPunktMaterialny(indeks-1)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
				double wychylenie=doLewego.Dlugosc()-l;
				if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
				{
					doLewego.Normuj();
					silaZLewej=k*wychylenie*doLewego;
					Wektor roznicaPredkosci=PobierzPunktMaterialny(indeks-1)->Predkosc()-PobierzPunktMaterialny(indeks)->Predkosc();
					silaZLewej+=tt*(doLewego*roznicaPredkosci)*doLewego;
				}
			}
			if (indeks<LiczbaPunktow()-1) 
			{
				Wektor doPrawego=PobierzPunktMaterialny(indeks+1)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
				double wychylenie=doPrawego.Dlugosc()-l;
				if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
				{
					doPrawego.Normuj();
					silaZPrawej=k*wychylenie*doPrawego;
					Wektor roznicaPredkosci=PobierzPunktMaterialny(indeks+1)->Predkosc()-PobierzPunktMaterialny(indeks)->Predkosc();
					silaZPrawej+=tt*(doPrawego*roznicaPredkosci)*doPrawego;
				}
			}
			Wektor sila=silaZLewej+silaZPrawej;					
			if (t!=0) sila-=2.0*PobierzPunktMaterialny(indeks)->Predkosc()*t; //tlumienie
			return sila;
		}
};

class UsztywnioneOscylatorySprzezone : public OscylatorySprzezone
{
	private:
		double s; //wspolczynnik sztywnosci
		Wektor* silySztywnosci;

	public:
		UsztywnioneOscylatorySprzezone(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:OscylatorySprzezone(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,dlugosc),
			s(wspolczynnikSztywnosci)
		{
			silySztywnosci=new Wektor[ilosc];
			for(int i=0;i<LiczbaPunktow();i++) silySztywnosci[i]=Wektor(0,0,0);

			//*
			PobierzPunktMaterialny(0)->UstawPredkosc(Wektor(0,0.1,0));
			PobierzPunktMaterialny((ilosc-1)/2)->UstawPredkosc(Wektor(0,-0.1,0));
			PobierzPunktMaterialny((ilosc-1)/2+1)->UstawPredkosc(Wektor(0,-0.1,0));
			PobierzPunktMaterialny(ilosc-1)->UstawPredkosc(Wektor(0,0.1,0));
			//*/

			/*
			PobierzPunktMaterialny((ilosc-1)/2)->UstawPredkosc(Wektor(0,-0.1,0));
			PobierzPunktMaterialny((ilosc-1)/2+1)->UstawPredkosc(Wektor(0,-0.1,0));
			UstawWiezy(0,true);
			UstawWiezy(ilosc-1,true);			
			*/
		}

		virtual void PrzedKrokiemNaprzod(double krokCzasowy)
		{
			OscylatorySprzezone::PrzedKrokiemNaprzod(krokCzasowy);
			
			for(int i=0;i<LiczbaPunktow();i++) silySztywnosci[i]=Wektor(0,0,0);
			if(s==0) return;

			for(int i=1;i<LiczbaPunktow()-1;i++)
			{
				/*
				Wektor odLewego=PobierzPunktMaterialny(i)->Polozenie()-PobierzPunktMaterialny(i-1)->Polozenie();
				Wektor doPrawego=PobierzPunktMaterialny(i+1)->Polozenie()-PobierzPunktMaterialny(i)->Polozenie();

				double iloczynDlugosciWektorow=odLewego.Dlugosc()*doPrawego.Dlugosc();
				//double cosKat=odLewego*doPrawego/iloczynDlugosciWektorow;
				//double kat=acos(cosKat);
				
				Wektor normalnaDoPlaszczyznyKata=IloczynWektorowy(odLewego,doPrawego);								
				double sinKat=normalnaDoPlaszczyznyKata.Dlugosc()/iloczynDlugosciWektorow;
				double kat=asin(sinKat);												
				
				if(kat!=0)
				{
					Wektor silaSztywnosci=-odLewego+doPrawego;
					silaSztywnosci.Normuj();
					silaSztywnosci*=s*kat;
					silySztywnosci[i]+=silaSztywnosci;
					silySztywnosci[i-1]-=silaSztywnosci/2;
					silySztywnosci[i+1]-=silaSztywnosci/2;
				}
				*/

				Wektor doLewego=PobierzPunktMaterialny(i-1)->Polozenie()-PobierzPunktMaterialny(i)->Polozenie();
				Wektor doPrawego=PobierzPunktMaterialny(i+1)->Polozenie()-PobierzPunktMaterialny(i)->Polozenie();
				Wektor silaSztywnosci=s*(doLewego+doPrawego)/2;
				silySztywnosci[i]+=silaSztywnosci;
				silySztywnosci[i-1]-=silaSztywnosci/2;
				silySztywnosci[i+1]-=silaSztywnosci/2;
			}
		}

		Wektor Sila(int indeks) const
		{
			Wektor sila=OscylatorySprzezone::Sila(indeks);
			if(s!=0) sila+=silySztywnosci[indeks];			
			return sila;
		}
};

class Lina : public UsztywnioneOscylatorySprzezone
{
	private:
		Wektor g; //przyspieszenie ziemskie

	public:
		Lina(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:UsztywnioneOscylatorySprzezone(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc),
			 g(Wektor(0,-0.1,0))
		{
			sprezystoscTylkoPrzyRozciaganiu=true;
			for(int i=0;i<ilosc;i++) PobierzPunktMaterialny(i)->UstawPredkosc(Wektor(0,0,0));
			UstawWiezy(0,true);
			//UstawWiezy(ilosc-1,true);
			//UstawWiezy(ilosc/2,true);
		}

		Wektor Sila(int indeks) const
		{
			return UsztywnioneOscylatorySprzezone::Sila(indeks)+PobierzPunktMaterialny(indeks)->Masa()*g;
		}
};

class Lina_OddzialywaniaZDalszymiSasiadami : public Lina
{
	private:
		int ileDodatkowychOddzialywan;

	protected:
		Wektor Sila(int indeks) const
		{	
			Wektor sila=Lina::Sila(indeks);

			for(int n=2;n<2+ileDodatkowychOddzialywan;n++)
			{
				Wektor silaZLewej=Wektor(0,0,0),silaZPrawej=Wektor(0,0,0);
				if (indeks>=n) 
				{
					Wektor odLewego=PobierzPunktMaterialny(indeks)->Polozenie()-PobierzPunktMaterialny(indeks-n)->Polozenie();
					double wychylenie=odLewego.Dlugosc()-n*l;
					if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
					{
						odLewego.Normuj();
						silaZLewej=-k*wychylenie*odLewego;
					}
				}
				if (indeks<LiczbaPunktow()-n) 
				{
					Wektor doPrawego=PobierzPunktMaterialny(indeks+n)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
					double wychylenie=doPrawego.Dlugosc()-n*l;
					if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
					{
						doPrawego.Normuj();
						silaZPrawej=k*wychylenie*doPrawego;
					}
				}
				sila+=silaZLewej+silaZPrawej;								
			}
			
			return sila;
		}

	public:
		Lina_OddzialywaniaZDalszymiSasiadami(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc,int ileDodatkowychOddzialywan)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc),
			ileDodatkowychOddzialywan(ileDodatkowychOddzialywan)
		{	
		}
};

//podloze bez klasy obszaru zabronionego
class LinaZPodlozem : public Lina
{
	protected:
		double poziomPodlozaY;

	protected:
		virtual void PoKrokuNaprzod(double krokCzasowy)
		{	
			for(int i=0;i<LiczbaPunktow();i++)
			{
				PunktMaterialny* punkt=PobierzPunktMaterialny(i);
				Wektor polozenie=punkt->Polozenie();
				if(polozenie.Y<=poziomPodlozaY)
				{
					polozenie.Y=poziomPodlozaY;
					punkt->UstawPolozenie(polozenie);
					
					Wektor predkosc=punkt->Predkosc();
					predkosc.Y=0;
					punkt->UstawPredkosc(predkosc);
				}
			}
		}

	public:
		LinaZPodlozem(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc,double poziomPodloza)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc),
			poziomPodlozaY(poziomPodloza)
		{			
		}

		Wektor Sila(int indeks) const
		{			
			Wektor sila=Lina::Sila(indeks);		

			const double sp=0.1; //wspolczynnik tarcia podloza
			PunktMaterialny* punkt=PobierzPunktMaterialny(indeks);
			if (punkt->Polozenie().Y<=poziomPodlozaY) 
				sila+=-2*sp*punkt->Predkosc();

			return sila;
		}

		double PobierzPoziomPodloza()
		{
			return poziomPodlozaY;
		}
};

class Wlos : public Lina
{
	public:
		Wlos(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc)
		{
			UstawWiezy(0,true);
			UstawWiezy(1,true);			
		}
};
#pragma endregion

#pragma region "Obszary zabronione"
class Podloze : public ObszarZabroniony
{
	private:
		double poziomY;

	public:
		double PobierzPoziom()
		{
			return poziomY;
		}

		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{
			bool wynik=(polozenie.Y<poziomY+margines);
			if(wynik && normalna!=NULL) *normalna=Wektor(0,1,0);
			return wynik;
		}

		Podloze(double wspolczynnikOdbicia,double wspolczynnikTarcia,double poziomY)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia),
			 poziomY(poziomY)
		{
		}
};

class Kula : public ObszarZabroniony
{
	protected:
		Wektor srodek;
		double promien;

	public:
		Wektor PobierzSrodek()
		{
			return srodek;
		}

		double PobierzPromien()
		{
			return promien;
		}

		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{
			Wektor wektorPromienia=polozenie-srodek;
			bool wynik=wektorPromienia.Dlugosc()<promien+margines;
			if(wynik && normalna!=NULL)
			{
				*normalna=wektorPromienia;				
				normalna->Normuj();				
			}
			return wynik;
		}

		Kula(double wspolczynnikOdbicia,double wspolczynnikTarcia,Wektor srodek,double promien)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia),
		     srodek(srodek),promien(promien)			 
		{	
		}
};

class ProstopadloscianNieograniczonyWKierunkuZ : public ObszarZabroniony
{
	private:
		double minX,maxX,minY,maxY;

	public:
		Wektor PobierzSrodek()
		{
			return Wektor((minX+maxX)/2.0,(minY+maxY)/2.0,0);
		}

		Wektor PobierzRozmiar()
		{
			return Wektor(maxX-minX,maxY-minY,0);
		}

		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{
			bool subtelnosciPrzyRogach=true;
			double _minX=minX-margines,_maxX=maxX+margines;
			double _minY=minY-margines,_maxY=maxY+margines;			
			bool wynik=polozenie.X>_minX && polozenie.X<_maxX && polozenie.Y>_minY && polozenie.Y<_maxY;
			if(subtelnosciPrzyRogach && wynik && margines>0)
			{
				if(polozenie.X<minX && polozenie.Y>maxY && (polozenie-Wektor(minX,maxY,polozenie.Z)).Dlugosc()>margines) wynik=false;
				if(polozenie.X<minX && polozenie.Y<minY && (polozenie-Wektor(minX,minY,polozenie.Z)).Dlugosc()>margines) wynik=false;
				if(polozenie.X>maxX && polozenie.Y>maxY && (polozenie-Wektor(maxX,maxY,polozenie.Z)).Dlugosc()>margines) wynik=false;
				if(polozenie.X>maxX && polozenie.Y<minY && (polozenie-Wektor(maxX,minY,polozenie.Z)).Dlugosc()>margines) wynik=false;
			}
			if(wynik && normalna!=NULL)
			{							
				Wektor przemieszczenie=polozenie-poprzedniePolozenie;
				if(przemieszczenie.X!=0)
				{				
					*normalna=Wektor(0,0,0);

					//prosta y=ax+b
					double a=przemieszczenie.Y/przemieszczenie.X;
					double b=polozenie.Y-a*polozenie.X;
					//przeciecie z lewa krawedzia
					double y_minX=a*_minX+b;
					if(y_minX>=_minY && y_minX<=_maxY && poprzedniePolozenie.X<_minX) normalna->X=-1;
					//przeciecie z prawa krawedzia
					double y_maxX=a*_maxX+b;
					if(y_maxX>=_minY && y_maxX<=_maxY && poprzedniePolozenie.X>_maxX) normalna->X=1;
					//przeciecie z gorna krawedzia
					double x_maxY=(_maxY-b)/a;
					if(x_maxY>=_minX && x_maxY<=_maxX && poprzedniePolozenie.Y>_maxY) normalna->Y=1;
					//przeciecie z dolna krawedzia
					double x_minY=(_minY-b)/a;
					if(x_minY>=_minX && x_minY<=_maxX && poprzedniePolozenie.Y<_minY) normalna->Y=-1;
				}
				else 
				{
					if(przemieszczenie.Y>0) *normalna=Wektor(0,-1,0);
					else *normalna=Wektor(0,1,0);
				}				
				if(subtelnosciPrzyRogach && margines>0)
				{
					if(polozenie.X<minX && polozenie.Y>maxY) *normalna=polozenie-Wektor(minX,maxY,polozenie.Z);
					if(polozenie.X<minX && polozenie.Y<minY) *normalna=polozenie-Wektor(minX,minY,polozenie.Z);
					if(polozenie.X>maxX && polozenie.Y>maxY) *normalna=polozenie-Wektor(maxX,maxY,polozenie.Z);
					if(polozenie.X>maxX && polozenie.Y<minY) *normalna=polozenie-Wektor(maxX,minY,polozenie.Z);
					normalna->Normuj();
				}
			}

			return wynik;
		}

		ProstopadloscianNieograniczonyWKierunkuZ(double wspolczynnikOdbicia,double wspolczynnikTarcia,double minX,double maxX,double minY,double maxY)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia),
			 minX(minX),maxX(maxX),minY(minY),maxY(maxY)
		{	
		}
};

class WalecNieograniczonyWKierunkuZ : public ObszarZabroniony
{
	protected:
		Wektor srodek;
		double promien;

	public:
		Wektor PobierzSrodek()
		{
			return srodek;
		}

		double PobierzPromien()
		{
			return promien;
		}

		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{
			Wektor wektorPromienia=polozenie-srodek;
			wektorPromienia.Z=0;
			bool wynik=wektorPromienia.Dlugosc()<promien+margines;
			if(wynik && normalna!=NULL)
			{
				*normalna=wektorPromienia;
				normalna->Normuj();
			}
			return wynik;
		}

		WalecNieograniczonyWKierunkuZ(double wspolczynnikOdbicia,double wspolczynnikTarcia,Wektor srodek,double promien)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia),
			 srodek(srodek),promien(promien)			 
		{	
		}
};

class DwieKule : public ObszarZabroniony
{
	protected:
		Kula *kula1,*kula2;

	public:
		bool CzyWObszarzeZabronionym(Wektor polozenie,Wektor poprzedniePolozenie,double margines,Wektor* normalna)
		{
			Wektor normalna1,normalna2;
			bool wynik1=kula1->CzyWObszarzeZabronionym(polozenie,poprzedniePolozenie,margines,(normalna!=NULL)?&normalna1:NULL);
			bool wynik2=kula2->CzyWObszarzeZabronionym(polozenie,poprzedniePolozenie,margines,(normalna!=NULL)?&normalna2:NULL);
			bool wynik=wynik1 || wynik2;
			if(wynik && normalna!=NULL)
			{
				*normalna=Wektor(0,0,0);
				if(wynik2) *normalna=normalna2;
				if(wynik1) *normalna=normalna1;
			}
			return wynik;
		}

		DwieKule(double wspolczynnikOdbicia,double wspolczynnikTarcia,Wektor srodek,double promien)
			:ObszarZabroniony(wspolczynnikOdbicia,wspolczynnikTarcia)
		{	
			kula1=new Kula(wspolczynnikOdbicia,wspolczynnikTarcia,Wektor(2,0.5,0),1);
			kula2=new Kula(wspolczynnikOdbicia,wspolczynnikTarcia,Wektor(2,-0.5,0),1);
		}
};

#pragma endregion

#pragma region "Uklady z obszarami zabronionymi"
class PunktyUderzajaceWKule : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		double polozenieYMin,polozenieYMax;
		double polozenieZMin,polozenieZMax;
		double predkoscXMin,predkoscXMax;
		
	public:
		PunktyUderzajaceWKule(int ilosc)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(ilosc),
			 polozenieYMin(-0.75),polozenieYMax(0.75),
			 polozenieZMin(-0.75),polozenieZMax(0.75),
			 predkoscXMin(0.1),predkoscXMax(0.2)
		{			
			obszarZabroniony=new Kula(0,0,Wektor(2,0,0),1);
			for(int i=0;i<ilosc;++i)
			{
				double polozenieY=(double)rand()/(RAND_MAX+1)*(polozenieYMax-polozenieYMin)+polozenieYMin;
				double polozenieZ=(double)rand()/(RAND_MAX+1)*(polozenieZMax-polozenieZMin)+polozenieZMin;
				double predkoscX=(double)rand()/(RAND_MAX+1)*(predkoscXMax-predkoscXMin)+predkoscXMin;
				PunktMaterialny* punkt=PobierzPunktMaterialny(i);
				punkt->UstawPolozenie(Wektor(-2.75,polozenieY,polozenieZ));
				punkt->UstawPredkosc(Wektor(predkoscX,0,0));
				//punkt->UstawKolor(0,1,0.5);
			}			
		}
		
		Wektor Sila(int indeks) const
		{
			return Wektor(0,0,0);
		}
};

class LinaZPodlozem2 : public Lina
{
	public:
		LinaZPodlozem2(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc,double poziomPodloza)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc)			 
		{			
			obszarZabroniony=new Podloze(0,0.1,poziomPodloza);
		}
};

class LinaZProstopadloscianemNieograniczonymWKierunkuZ : public Lina
{
	public:
		LinaZProstopadloscianemNieograniczonymWKierunkuZ(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc)
		{
			obszarZabroniony=new ProstopadloscianNieograniczonyWKierunkuZ(0,0,-1,1,-0.75,-0.25);
			UstawWiezy(0,false);
		}
};


class LinaZWalcemNieograniczonymWKierunkuZ : public Lina
{
	public:
		LinaZWalcemNieograniczonymWKierunkuZ(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc)
			 
		{
			obszarZabroniony=new WalecNieograniczonyWKierunkuZ(0,0,Wektor(0,-1.5,0),1);
			UstawWiezy(0,false);				
		}
};

class LinaZKula : public Lina
{
	/*
	private:
		bool CzyWObszarzeZabronionym(int indeks,Wektor* normalna,double* wspolczynnikOdbicia,double* wspolczynnikTarcia)
		{
			PunktMaterialny* punkt=PobierzPunktMaterialny(indeks);
			Wektor polozenie=punkt->Polozenie();			

			bool wynik=obszarZabroniony->CzyWObszarzeZabronionym(polozenie,punkt->PoprzedniePolozenie(),normalna);
			if(wspolczynnikOdbicia!=NULL) *wspolczynnikOdbicia=0;
			if(wspolczynnikTarcia!=NULL) *wspolczynnikTarcia=0;

			if (wynik) punkt->UstawKolor(1,0,0); else punkt->UstawKolor(0,1,0);
			return wynik;
		}
	*/

	public:
		LinaZKula(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,double dlugosc)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikTlumieniaOscylacji,wspolczynnikSztywnosci,dlugosc)
		{
			obszarZabroniony=new Kula(0,0,Wektor(0,-1.5,0),1);
			UstawWiezy(0,false);				
		}
};

/*
class LinaZObszaremZabronionym_SkosnePodloze : public Lina
{
		//nachylone podloze
		bool CzyWObszarzeZabronionym(int indeks,Wektor* normalna)
		{
			PunktMaterialny* punkt=PobierzPunktMaterialny(indeks);
			Wektor polozenie=punkt->Polozenie();			

			double a=-1,b=-2;
			bool wynik=(polozenie.Y<a*polozenie.X+b); //y=ax+b
			if(wynik && normalna!=NULL)
			{
				*normalna=Wektor(1,1,0); //uzaleznic od a
				normalna->Normuj();
			}
			if (wynik) punkt->UstawKolor(1,0,0); else punkt->UstawKolor(0,1,0);
			return wynik;
		}

	public:
		LinaZObszaremZabronionym_SkosnePodloze(int ilosc,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikSztywnosci,double dlugosc)
			:Lina(ilosc,wspolczynnikSprezystosci,wspolczynnikTlumienia,wspolczynnikSztywnosci,dlugosc)			 
		{
			UstawWiezy(0,false);				
		}
};
*/

class PunktyUderzajaceWDwieKule : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		double polozenieYMin,polozenieYMax;
		double polozenieZMin,polozenieZMax;
		double predkoscXMin,predkoscXMax;
		
	public:
		PunktyUderzajaceWDwieKule(int ilosc)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(ilosc),
			 polozenieYMin(-0.75),polozenieYMax(0.75),
			 polozenieZMin(-0.75),polozenieZMax(0.75),
			 predkoscXMin(0.1),predkoscXMax(0.2)
		{			
			obszarZabroniony=new DwieKule(1,0,Wektor(2,0,0),1);
			for(int i=0;i<ilosc;++i)
			{
				double polozenieY=(double)rand()/(RAND_MAX+1)*(polozenieYMax-polozenieYMin)+polozenieYMin;
				double polozenieZ=(double)rand()/(RAND_MAX+1)*(polozenieZMax-polozenieZMin)+polozenieZMin;
				double predkoscX=(double)rand()/(RAND_MAX+1)*(predkoscXMax-predkoscXMin)+predkoscXMin;
				PunktMaterialny* punkt=PobierzPunktMaterialny(i);
				punkt->UstawPolozenie(Wektor(-2.75,polozenieY,polozenieZ));
				punkt->UstawPredkosc(Wektor(predkoscX,0,0));
				//punkt->UstawKolor(0,1,0.5);
			}			
		}
		
		Wektor Sila(int indeks) const
		{
			return Wektor(0,0,0);
		}
};
#pragma endregion

//------------------------------------------------------------------

#pragma region "Uklasy 2D"
typedef TWektor<int> Wersor;

//nowe zastosowanie szablonu klasy Wektor
Wersor wersoryKierunkowe2D[8]={
	Wersor(0,1,0),Wersor(1,1,0),
	Wersor(1,0,0),Wersor(1,-1,0),
	Wersor(0,-1,0),Wersor(-1,-1,0),
	Wersor(-1,0,0),Wersor(-1,1,0)};

class Siatka : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		static const int coKtorySasiad=1;  //1 - takze po przekatnych (8), 2 - tylko na siatce pion/poziom (4)

	private:
		double k; //wspolczynnik sprezystosci		
		double t; //wspolczynnik tlumienia (sily oporu)	
		double tt; //wspolczynnik tlumienia oscylacji
		double s; //wspolczynnik sztywnosci
		double lx,ly; //odleglosc spoczynkowa
		int Nx,Ny;
		bool sprezystoscTylkoPrzyRozciaganiu;
		Wektor* silySztywnosci;
		Wektor g; //przyspieszenie ziemskie

		virtual void PrzedKrokiemNaprzod(double krokCzasowy)
		{
			ZbiorPunktowMaterialnych::PrzedKrokiemNaprzod(krokCzasowy);
			
			for(int i=0;i<LiczbaPunktow();i++) silySztywnosci[i]=Wektor(0,0,0);
			if(s==0) return;

			for(int ix=1;ix<Nx-1;++ix)
				for(int iy=1;iy<Ny-1;++iy)
				{
					int indeks=ix+Nx*iy;
					
					Wektor wektorWypadkowySasiadow=Wektor(0,0,0);
					for(int kierunek=0;kierunek<8;kierunek+=coKtorySasiad)
					{
						int nxSasiada=ix+wersoryKierunkowe2D[kierunek].X;
						int nySasiada=iy+wersoryKierunkowe2D[kierunek].Y;				
						if(nxSasiada>=0 && nxSasiada<Nx && nySasiada>=0 && nySasiada<Ny)
						{
							Wektor doSasiada=PobierzPunktMaterialny(indeks+wersoryKierunkowe2D[kierunek].X+Nx*wersoryKierunkowe2D[kierunek].Y)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
							wektorWypadkowySasiadow+=doSasiada;
						}						
					}
					wektorWypadkowySasiadow/=(8.0/coKtorySasiad);
					Wektor silaSztywnosci=s*wektorWypadkowySasiadow;

					silySztywnosci[indeks]+=silaSztywnosci;
					for(int kierunek=0;kierunek<8;kierunek+=coKtorySasiad)
						silySztywnosci[indeks+wersoryKierunkowe2D[kierunek].X+Nx*wersoryKierunkowe2D[kierunek].Y]-=silaSztywnosci/(8.0/coKtorySasiad);
				}
		}		

	public:
		Siatka(int Nx,int Ny,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,double wspolczynnikSztywnosci,Wektor przyspieszenieZiemskie,double dlugoscX,double dlugoscY,ObszarZabroniony* obszarZabroniony=NULL)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(Nx*Ny),Nx(Nx),Ny(Ny),
			k(wspolczynnikSprezystosci),t(wspolczynnikTlumienia),tt(wspolczynnikTlumieniaOscylacji),
			s(wspolczynnikSztywnosci),sprezystoscTylkoPrzyRozciaganiu(true),
			lx(dlugoscX/(Nx-1)),ly(dlugoscX/(Ny-1)),
			g(przyspieszenieZiemskie)
		{			
			this->obszarZabroniony=obszarZabroniony;
			silySztywnosci=new Wektor[LiczbaPunktow()];
			
			for(int ix=0;ix<Nx;++ix)
				for(int iy=0;iy<Ny;++iy)
				{
					int i=ix+Nx*iy;
					silySztywnosci[i]=Wektor(0,0,0);
					PunktMaterialny* punkt=PobierzPunktMaterialny(i);
					punkt->UstawPolozenie(Wektor(-dlugoscX/2+ix*lx,-dlugoscY/2+iy*ly,0));
					punkt->UstawPredkosc(Wektor(0,0,0)); //spoczywajace										
					punkt->UstawKolor(1,(float)ix/(Nx-1),(float)iy/(Ny-1));
				}			

			
			//PobierzPunktMaterialny(Nx/2+Nx*Ny/2-2)->UstawPredkosc(Wektor(0,0,0.1));

			//poczatkowa predkosc srednia ma byc rowna zero
			ZerujPredkoscSrednia();

			/*
			//linka
			for(int ix=0;ix<Nx;++ix)
				for(int iy=0;iy<Ny;++iy)
				{
					int i=ix+Nx*iy;
					if(iy==Ny-1) UstawWiezy(i,true);
				}
			*/

			/*
			//obrecz
			for(int ix=0;ix<Nx;++ix)
				for(int iy=0;iy<Ny;++iy)
				{
					int i=ix+Nx*iy;
					if(ix==0 || ix==Nx-1 || iy==0 || iy==Ny-1) UstawWiezy(i,true);
				}
			*/

			/*
			//rogi
			UstawWiezy(0,true);
			UstawWiezy(Nx-1,true);
			UstawWiezy(Nx*(Ny-1),true);
			UstawWiezy(Nx*Ny-1,true);
			PobierzPunktMaterialny(0)->UstawKolor(0,1,0);
			PobierzPunktMaterialny(Nx-1)->UstawKolor(0,1,0);
			PobierzPunktMaterialny(Nx*(Ny-1))->UstawKolor(0,1,0);
			PobierzPunktMaterialny(Nx*Ny-1)->UstawKolor(0,1,0);
			*/
		}

		int PobierzNx()
		{
			return Nx;
		}
		int PobierzNy()
		{
			return Ny;
		}

	protected:
		virtual Wektor Sila(int indeks) const
		{	
			int nx=indeks%Nx;
			int ny=(indeks-nx)/Nx;
			
			Wektor sila=Wektor(0,0,0);
			for(int kierunek=0;kierunek<8;kierunek+=coKtorySasiad)
			{
				int nxSasiada=nx+wersoryKierunkowe2D[kierunek].X;
				int nySasiada=ny+wersoryKierunkowe2D[kierunek].Y;				
				if(nxSasiada>=0 && nxSasiada<Nx && nySasiada>=0 && nySasiada<Ny)
				{
					Wektor doSasiada=PobierzPunktMaterialny(indeks+wersoryKierunkowe2D[kierunek].X+Nx*wersoryKierunkowe2D[kierunek].Y)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
					double odlegloscSpoczynkowa=sqrt(SQR(wersoryKierunkowe2D[kierunek].X*lx)+SQR(wersoryKierunkowe2D[kierunek].Y*ly));
					double wychylenie=doSasiada.Dlugosc()-odlegloscSpoczynkowa;
					if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
					{
						doSasiada.Normuj();
						sila+=k*wychylenie*doSasiada;
						Wektor roznicaPredkosci=PobierzPunktMaterialny(indeks+wersoryKierunkowe2D[kierunek].X+Nx*wersoryKierunkowe2D[kierunek].Y)->Predkosc()-PobierzPunktMaterialny(indeks)->Predkosc();
						sila+=tt*(doSasiada*roznicaPredkosci)*doSasiada;
					}
				}
			}
			if (t!=0) sila-=PobierzPunktMaterialny(indeks)->Predkosc()*t; //tlumienie
			if (s!=0) sila+=silySztywnosci[indeks];
			sila+=PobierzPunktMaterialny(indeks)->Masa()*g;
			return sila;
		}
};
#pragma endregion

#pragma region "Uklasy 3D"
class Szescian : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		double k; //wspolczynnik sprezystosci		
		double t,tt; //wspolczynniki tlumienia (sily oporu i tlumienia stycznego)
		double lx,ly,lz; //odleglosc spoczynkowa
		int Nx,Ny,Nz;
		bool sprezystoscTylkoPrzyRozciaganiu;		
		Wektor g; //przyspieszenie ziemskie

	public:
		Szescian(int Nx,int Ny,int Nz,double wspolczynnikSprezystosci,double wspolczynnikTlumienia,double wspolczynnikTlumieniaOscylacji,Wektor przyspieszenieZiemskie,double dlugoscX,double dlugoscY,double dlugoscZ,ObszarZabroniony* obszarZabroniony=NULL)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(Nx*Ny*Nz),Nx(Nx),Ny(Ny),Nz(Nz),
			k(wspolczynnikSprezystosci),t(wspolczynnikTlumienia),tt(wspolczynnikTlumieniaOscylacji),
			sprezystoscTylkoPrzyRozciaganiu(false),
			lx(dlugoscX/(Nx-1)),ly(dlugoscX/(Ny-1)),lz(dlugoscZ/(Nz-1)),
			g(przyspieszenieZiemskie)			
		{						
			this->obszarZabroniony=obszarZabroniony;

			for(int ix=0;ix<Nx;++ix)
				for(int iy=0;iy<Ny;++iy)
					for(int iz=0;iz<Nz;++iz)
					{
						int i=PobierzIndeksPunktu(ix,iy,iz);
						PunktMaterialny* punkt=PobierzPunktMaterialny(i);
						punkt->UstawPolozenie(Wektor(-dlugoscX/2+ix*lx,-dlugoscY/2+iy*ly,-dlugoscZ/2+iz*lz));
						punkt->UstawPredkosc(Wektor(0,0,0)); //spoczywajace										
						punkt->UstawKolor((float)iz/(Nz-1),(float)ix/(Nx-1),(float)iy/(Ny-1));
					}
					
			//ZerujPredkoscSrednia();
		}

		int PobierzNx() const
		{
			return Nx;
		}
		int PobierzNy() const
		{
			return Ny;
		}
		int PobierzNz() const
		{
			return Nz;
		}
		int PobierzIndeksPunktu(int nx,int ny,int nz) const
		{
			return nx+Nx*ny+Nx*Ny*nz;
		}
		void PobierzWspolrzednePunktu(int indeks,int& nx,int& ny,int& nz) const
		{
			nx=indeks%Nx;
			ny=((indeks-nx)/Nx)%Ny;
			nz=(indeks-nx*ny)/(Nx*Ny);
		}

	protected:
		virtual Wektor Sila(int indeks) const
		{		
			int nx=0,ny=0,nz=0;
			PobierzWspolrzednePunktu(indeks,nx,ny,nz);			

			Wektor sila=Wektor(0,0,0);
			for(int kierunekX=-1;kierunekX<=1;kierunekX++)
				for(int kierunekY=-1;kierunekY<=1;kierunekY++)
					for(int kierunekZ=-1;kierunekZ<=1;kierunekZ++)
					{
						if(kierunekX==0 && kierunekY==0 && kierunekZ==0) continue;

						int nxSasiada=nx+kierunekX;
						int nySasiada=ny+kierunekY;
						int nzSasiada=nz+kierunekZ;
						if(nxSasiada>=0 && nxSasiada<Nx && nySasiada>=0 && nySasiada<Ny && nzSasiada>=0 && nzSasiada<Nz)
						{
							Wektor doSasiada=PobierzPunktMaterialny(indeks+kierunekX+Nx*kierunekY+Nx*Nz*kierunekZ)->Polozenie()-PobierzPunktMaterialny(indeks)->Polozenie();
							double odlegloscSpoczynkowa=sqrt(SQR(kierunekX*lx)+SQR(kierunekY*ly)+SQR(kierunekZ*lz));
							double wychylenie=doSasiada.Dlugosc()-odlegloscSpoczynkowa;
							if (!sprezystoscTylkoPrzyRozciaganiu || wychylenie>0)
							{
								doSasiada.Normuj();
								sila+=k*wychylenie*doSasiada;								
								Wektor roznicaPredkosci=PobierzPunktMaterialny(indeks+kierunekX+Nx*kierunekY+Nx*Nz*kierunekZ)->Predkosc()-PobierzPunktMaterialny(indeks)->Predkosc();
								sila+=tt*(doSasiada*roznicaPredkosci)*doSasiada;
							}							
						}
					}
			if (t!=0) sila-=PobierzPunktMaterialny(indeks)->Predkosc()*t; //tlumienie
			sila+=PobierzPunktMaterialny(indeks)->Masa()*g;
			return sila;
		}
};
#pragma endregion

//------------------------------------------------------------------

#pragma region "Zadania"
class PunktyOdbijajaceSieOdPodloza : public ZbiorPunktowMaterialnychZObszaremZabronionym
{
	private:
		Wektor g; //przyspieszenie ziemskie
		double polozenieXMin,polozenieXMax;
		double polozenieYMin,polozenieYMax;
		double polozenieZMin,polozenieZMax;
		double predkoscXMin,predkoscXMax;
		
	public:
		PunktyOdbijajaceSieOdPodloza(int ilosc)
			:ZbiorPunktowMaterialnychZObszaremZabronionym(ilosc),
			 g(Wektor(0,-0.1,0)),			 
			 polozenieXMin(-2),polozenieXMax(2),
			 polozenieYMin(-0.75),polozenieYMax(0.75),
			 polozenieZMin(-5),polozenieZMax(1),
			 predkoscXMin(0.1),predkoscXMax(0.2)
		{			
			for(int i=0;i<ilosc;++i)
			{
				double polozenieX=(double)rand()/(RAND_MAX+1)*(polozenieXMax-polozenieXMin)+polozenieXMin;
				double polozenieY=(double)rand()/(RAND_MAX+1)*(polozenieYMax-polozenieYMin)+polozenieYMin;
				double polozenieZ=(double)rand()/(RAND_MAX+1)*(polozenieZMax-polozenieZMin)+polozenieZMin;				
				PunktMaterialny* punkt=PobierzPunktMaterialny(i);
				punkt->UstawPolozenie(Wektor(polozenieX,polozenieY,polozenieZ));
				punkt->UstawPredkosc(Wektor(0,0,0));
				//punkt->UstawKolor(0,1,0.5);

				obszarZabroniony=new Podloze(1,0,-2);
			}			
		}
		
		Wektor Sila(int indeks) const
		{
			return PobierzPunktMaterialny(indeks)->Masa()*g;
		}
};
#pragma endregion

#endif
