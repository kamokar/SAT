#ifndef PUNKTMATERIALNY_CPP
#define PUNKTMATERIALNY_CPP

#include "PunktMaterialny.h"

#pragma region Przechowywanie danych punktu
template<typename T>
void TPunktMaterialny<T>::_TPunktMaterialny(TWektor<T> polozenie,TWektor<T> predkosc,T masa,T promien,float kolor[4])
{
	numerKroku=0;
	this->polozenie=polozenie;
	this->poprzedniePolozenie=polozenie;
	this->predkosc=predkosc;
	this->masa=masa;
	this->promien=promien;
	UstawKolor(kolor);
}

template<typename T>
TPunktMaterialny<T>::TPunktMaterialny(TWektor<T> polozenie,TWektor<T> predkosc,T masa,T promien,float kolor[4])
{
	_TPunktMaterialny(polozenie,predkosc,masa,promien,kolor);
}

template<typename T>
TPunktMaterialny<T>::TPunktMaterialny(void)	
{	
	float kolor[4]={1,1,1,1};
	_TPunktMaterialny(TWektor<T>(0,0,0),TWektor<T>(0,0,0),1,0.1,kolor);
}

template<typename T>
TWektor<T> TPunktMaterialny<T>::Polozenie() const
{
	return polozenie;
}

template<typename T>
TWektor<T> TPunktMaterialny<T>::PoprzedniePolozenie() const
{
	return poprzedniePolozenie;
}

template<typename T>
TWektor<T> TPunktMaterialny<T>::NastepnePolozenie() const
{
	return nastepnePolozenie;
}

template<typename T>
TWektor<T> TPunktMaterialny<T>::Predkosc() const
{
	return predkosc;
}

template<typename T>
T TPunktMaterialny<T>::Masa() const
{
	return masa;
}

template<typename T>
T TPunktMaterialny<T>::Promien() const
{
	return promien;
}

template<typename T>
float* TPunktMaterialny<T>::Kolor() const
{
	return const_cast<float*>(kolor);
}

template<typename T>
void TPunktMaterialny<T>::UstawPolozenie(TWektor<T> polozenie)
{
	this->polozenie=polozenie;
	this->poprzedniePolozenie=polozenie;
}

template<typename T>
void TPunktMaterialny<T>::UstawPoprzedniePolozenie(TWektor<T> poprzedniePolozenie)
{
	this->poprzedniePolozenie=poprzedniePolozenie;
}

template<typename T>
void TPunktMaterialny<T>::UstawPredkosc(TWektor<T> predkosc)
{
	this->predkosc=predkosc;
}

template<typename T>
void TPunktMaterialny<T>::UstawMase(T masa)
{
	this->masa=masa;
}

template<typename T>
void TPunktMaterialny<T>::UstawPromien(T promien)
{
	this->promien=promien;
}

template<typename T>
void TPunktMaterialny<T>::UstawKolor(float kolor[4])
{
	for(int i=0;i<4;++i) this->kolor[i]=kolor[i];
}

template<typename T>
void TPunktMaterialny<T>::UstawKolor(float r,float g,float b,float a)
{
	float kolor[4]={r,g,b,a};
	UstawKolor(kolor);
}
#pragma endregion

#pragma region Dynamika (metody numeryczne)
template<typename T>
int TPunktMaterialny<T>::NumerKroku()
{
	return numerKroku;
}

template<typename T>
void TPunktMaterialny<T>::PrzygotujRuch_Euler(TWektor<T> przyspieszenie,T krokCzasowy)
{
	nastepnaPredkosc=predkosc+przyspieszenie*krokCzasowy;
	nastepnePolozenie=polozenie+nastepnaPredkosc*krokCzasowy;
}

#define SQR(x) ((x)*(x))

template<typename T>
void TPunktMaterialny<T>::PrzygotujRuch_Verlet(TWektor<T> przyspieszenie,T krokCzasowy)
{
	if(numerKroku==0) PrzygotujRuch_Euler(przyspieszenie,krokCzasowy);
	else
	{
		nastepnePolozenie=2.0*polozenie-poprzedniePolozenie+przyspieszenie*SQR(krokCzasowy);		
		nastepnaPredkosc=(nastepnePolozenie-poprzedniePolozenie)/(2*krokCzasowy);
	}
}

template<typename T>
void TPunktMaterialny<T>::PrzygotujRuch(TWektor<T> sila,T krokCzasowy,Algorytm algorytm)
{
	TWektor<T> przyspieszenie=sila/masa;
	switch(algorytm)
	{
		case algorytmEulera:
			PrzygotujRuch_Euler(przyspieszenie,krokCzasowy);
			break;
		case algorytmVerleta:
			PrzygotujRuch_Verlet(przyspieszenie,krokCzasowy);
			break;
	}
}

template<typename T>
void TPunktMaterialny<T>::WykonajRuch()
{
	poprzedniePolozenie=polozenie;
	polozenie=nastepnePolozenie;		
	predkosc=nastepnaPredkosc;
	++numerKroku;
}
#pragma endregion

/********************************************************/
/*            Zbior punktow materialnych                */
/********************************************************/

#pragma region Zbior punktow materialnych
template<typename T> 
TZbiorPunktowMaterialnych<T>::TZbiorPunktowMaterialnych(int ilosc)	
{	
	this->ilosc=ilosc;
	this->punkty=new TPunktMaterialny<T>[ilosc];	
	this->wiezy=new bool[ilosc];
	for(int i=0;i<ilosc;i++) wiezy[i]=false;	
}

template<typename T> 
TZbiorPunktowMaterialnych<T>::~TZbiorPunktowMaterialnych()
{
	delete [] punkty;
	delete [] wiezy;	
}

template<typename T>
void TZbiorPunktowMaterialnych<T>::PrzygotujRuch(T krokCzasowy,Algorytm algorytm)
{		
	for(int i=0;i<ilosc;++i) 
		if(!wiezy[i]) 
			this->punkty[i].PrzygotujRuch(Sila(i),krokCzasowy,algorytm);
}

template<typename T>
void TZbiorPunktowMaterialnych<T>::WykonajRuch()
{		
	for(int i=0;i<ilosc;++i) 
		if(!wiezy[i]) 
			this->punkty[i].WykonajRuch();					
}

template<typename T>
void TZbiorPunktowMaterialnych<T>::KrokNaprzod(T krokCzasowy,Algorytm algorytm)
{	
	PrzedKrokiemNaprzod(krokCzasowy);
	PrzygotujRuch(krokCzasowy,algorytm);
	PoPrzygotowaniuRuchu(krokCzasowy);
	WykonajRuch();
	PoKrokuNaprzod(krokCzasowy);
}

template<typename T>
TWektor<T> TZbiorPunktowMaterialnych<T>::SrodekMasy() const
{
	TWektor<T> srodekMasy(0.0,0.0,0.0);
    for(int indeks=0;indeks<ilosc;indeks++)
        srodekMasy+=punkty[indeks].Polozenie();
    srodekMasy/=(T)ilosc;
    return srodekMasy;
}

template<typename T>
TPunktMaterialny<T>* TZbiorPunktowMaterialnych<T>::PobierzPunktMaterialny(int indeks) const
{
	if(indeks<0 || indeks>=ilosc) return NULL;
	return &punkty[indeks];
}

template<typename T>
void TZbiorPunktowMaterialnych<T>::UstawWiezy(int indeks,bool ustalonaPozycja)
{
	if(indeks>=0 && indeks<ilosc) this->wiezy[indeks]=ustalonaPozycja;
}

template<typename T>
void TZbiorPunktowMaterialnych<T>::ZerujPredkoscSrednia()
{
	int iloscZWiezami=0;
	TWektor<T> predkoscSrednia=TWektor<T>(0,0,0);
	for(int i=0;i<ilosc;++i)
	{
		if(!wiezy[i])
			predkoscSrednia+=PobierzPunktMaterialny(i)->Predkosc();			
		else
			iloscZWiezami++;
	}
	predkoscSrednia/=ilosc-iloscZWiezami;
	for(int i=0;i<ilosc;++i)
		if(!wiezy[i])
			punkty[i].UstawPredkosc(punkty[i].Predkosc()-predkoscSrednia);
}
#pragma endregion

/********************************************************/
/*                 Obszar zabroniony                    */
/********************************************************/

#pragma region Obszar zabroniony
template<typename T>
TObszarZabroniony<T>::TObszarZabroniony(T wspolczynnikOdbicia,T wspolczynnikTarcia)
	:wspolczynnikOdbicia(wspolczynnikOdbicia),wspolczynnikTarcia(wspolczynnikTarcia)
{
}

template<typename T>
T TObszarZabroniony<T>::PobierzWspolczynnikOdbicia()
{
	return wspolczynnikOdbicia;
}

template<typename T>
T TObszarZabroniony<T>::PobierzWspolczynnikTarcia()
{
	return wspolczynnikTarcia;
}
#pragma endregion

/********************************************************/
/*  Zbior punktow materialnych z obszarem zabronionym   */
/********************************************************/

template<typename T>
TZbiorPunktowMaterialnychZObszaremZabronionym<T>::TZbiorPunktowMaterialnychZObszaremZabronionym(int ilosc)
	:TZbiorPunktowMaterialnych<T>(ilosc),
	 obszarZabroniony(NULL)
{
}

template<typename T>
TZbiorPunktowMaterialnychZObszaremZabronionym<T>::~TZbiorPunktowMaterialnychZObszaremZabronionym()
{
	if(obszarZabroniony!=NULL) delete obszarZabroniony;
}

template<typename T>
TObszarZabroniony<T>* TZbiorPunktowMaterialnychZObszaremZabronionym<T>::PobierzObszarZabroniony()
{
	return obszarZabroniony;
}

template<typename T>
void TZbiorPunktowMaterialnychZObszaremZabronionym<T>::PrzygotujRuch(T krokCzasowy,Algorytm algorytm)
{			
	for(int i=0;i<ilosc;++i) 
		if(!wiezy[i]) 
		{			
			this->punkty[i].PrzygotujRuch(Sila(i),krokCzasowy,algorytm);
			if(obszarZabroniony!=NULL && obszarZabroniony->CzyWObszarzeZabronionym(this->punkty[i].NastepnePolozenie(),this->punkty[i].Polozenie(),this->punkty[i].Promien(),NULL))
			{
				if(zaznaczajKontaktPunktuZObszaremZabronionym) this->punkty[i].UstawKolor(1,0,0);
				PrzygotujRuchPrzyKontakcieZObszaremZabronionym(i,krokCzasowy);
			}
			else 
			{
				if(zaznaczajKontaktPunktuZObszaremZabronionym) this->punkty[i].UstawKolor(0,1,0);				
			}
		}
}

template<typename T>
void TZbiorPunktowMaterialnychZObszaremZabronionym<T>::PrzygotujRuchPrzyKontakcieZObszaremZabronionym(int indeks,T krokCzasowy)
{
	if(obszarZabroniony==NULL) return;	

	PunktMaterialny* punkt=PobierzPunktMaterialny(indeks);	
	TWektor<T> normalna=TWektor<T>(0,0,0);

	//zakladamy, ze nastepne polozenie jest juz obliczone
	if(obszarZabroniony->CzyWObszarzeZabronionym(punkt->NastepnePolozenie(),punkt->Polozenie(),punkt->Promien(),&normalna))
	{							
		//eliminacja skladowej normalnej sily
		TWektor<T> sila=Sila(indeks);
		double skladowaNormalnaSily=normalna*sila;
		if (skladowaNormalnaSily<0) //usuwanie skladowej sily skierowanej do powierzchni
			sila-=normalna*(skladowaNormalnaSily); //dzialanie sily kontaktowej
		
		//uwzglednienie odbicia (zmiana aktualnego wektora predkosci)
		TWektor<T> predkosc=punkt->Predkosc();		
		//wspolczynnikOdbicia: 0.0 - odbicie doskonale niesprezyste (tlumienie predkosci), 1.0 - //odbicie doskonale sprezyste
		double skladowaNormalnaPredkosci=normalna*predkosc;
		if (skladowaNormalnaPredkosci<0) //usuwanie skladowej predkosci skierowanej do powierzchni
			predkosc-=normalna*((obszarZabroniony->PobierzWspolczynnikOdbicia()+1)*(skladowaNormalnaPredkosci));
		punkt->UstawPredkosc(predkosc); 
		
		//tarcie
		if (skladowaNormalnaSily<0 && predkosc.Dlugosc()>0)
		{
			TWektor<T> tarcie=-predkosc;
			tarcie.Normuj();
			tarcie*=fabs(obszarZabroniony->PobierzWspolczynnikTarcia()*skladowaNormalnaSily);
			sila+=tarcie;
		}
		
		//ponowne przygotowanie ruchu z nowymi sila i predkoscia
		punkt->PrzygotujRuch(sila,krokCzasowy,algorytmEulera); //musi byc metoda Eulara, bo widzi zmiane predkosci
	}
	else throw std::exception("Ta funkcja nie powinna byæ nigdy wywo³ywana, gdy nie jesteœmy w obszarze zabronionym");
}
#endif