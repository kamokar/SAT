#ifndef BRYLASZTYWNA_CPP
#define BRYLASZTYWNA_CPP

#include "BrylaSztywna.h"

/********************************************************/
/*                   Bry³a sztywna                      */
/********************************************************/

template<typename T>
void TBrylaSztywna<T>::_TBrylaSztywna(TWektor<T> polozenie,TWektor<T> predkosc,TMacierz<T> macierzObrotu,TWektor<T> predkoscKatowa,TMacierz<T> tensorMomentuBezwladnosci,float kolor[4])
{
	srodekMasy=new PunktMaterialny(polozenie,predkosc,1,0,kolor);
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	this->macierzObrotu=macierzObrotu;
	#else
	this->kwaternion=TKwaternion<T>::TworzZMacierzyObrotu(macierzObrotu);
	#endif
	this->predkoscKatowa=predkoscKatowa;
	this->tensorMomentuBezwladnosci=tensorMomentuBezwladnosci;
	odwroconyTensorMomentuBezwladnosci=tensorMomentuBezwladnosci.Odwrotna();	
	numerKroku=0;
}

template<typename T>
TBrylaSztywna<T>::TBrylaSztywna(TWektor<T> polozenie,TWektor<T> predkosc,TMacierz<T> macierzObrotu,TWektor<T> predkoscKatowa,TMacierz<T> tensorMomentuBezwladnosci,float kolor[4])
	:srodekMasy(NULL)
{
	_TBrylaSztywna(polozenie,predkosc,macierzObrotu,predkoscKatowa,tensorMomentuBezwladnosci,kolor);
}

template<typename T>
TBrylaSztywna<T>::TBrylaSztywna()
	:srodekMasy(NULL)
{
	float kolor[4]={1,1,1,1};
	_TBrylaSztywna(WektorZero,WektorZero,Macierz(elementyMacierzyJednostkowej),WektorZero,Macierz(elementyMacierzyJednostkowej),kolor);
}

template<typename T>
TBrylaSztywna<T>::~TBrylaSztywna()
{
	delete srodekMasy;
}

/*
template<typename T>
void TBrylaSztywna<T>::ObliczMomentBezwladnosci() //nieuzywane
{
	//I*n
	TWektor<T> In;			
	In.X=tensorMomentuBezwladnosci[0]*kierunekOsiObrotu;
	In.Y=tensorMomentuBezwladnosci[1]*kierunekOsiObrotu;
	In.Z=tensorMomentuBezwladnosci[2]*kierunekOsiObrotu;
	momentBezwladnosci=kierunekOsiObrotu*In;
}
*/

template<typename T>
void TBrylaSztywna<T>::PrzygotujObrot_Euler(TWektor<T> wypadkowyMomentSily,T krokCzasowy)
{
	//przyspieszenie
	TWektor<T> pochodnaPredkosciKatowej=odwroconyTensorMomentuBezwladnosci*(wypadkowyMomentSily-(predkoscKatowa^(tensorMomentuBezwladnosci*predkoscKatowa)));
	nastepnaPredkoscKatowa=predkoscKatowa+pochodnaPredkosciKatowej*krokCzasowy;

	//predkosc
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	TMacierz<T> pochodnaMacierzyObrotu=Macierz::OperatorGwiazdki(predkoscKatowa)*macierzObrotu;
	nastepnaMacierzObrotu=macierzObrotu+pochodnaMacierzyObrotu*krokCzasowy;
	#else
	TKwaternion<T> pochodnaKwaternionu=TKwaternion<T>(0,predkoscKatowa)*kwaternion*0.5; //0.5 na koncu ze wzgledu na zdefiniowany operator *(Kwaternion<T>,T), a brak odwrotnego
	nastepnyKwaternion=kwaternion+pochodnaKwaternionu*krokCzasowy;
	nastepnyKwaternion.Normuj();
	#endif
}

template<typename T>
void TBrylaSztywna<T>::PrzygotujRuch(TWektor<T> wypadkowaSila,TWektor<T> wypadkowyMomentSily,T krokCzasowy,Algorytm algorytm)
{	
	srodekMasy->PrzygotujRuch(wypadkowaSila,krokCzasowy,algorytm);
	switch(algorytm)
	{
		case algorytmEulera:
		case algorytmVerleta:
			PrzygotujObrot_Euler(wypadkowyMomentSily,krokCzasowy);
			break;
	}
}

template<typename T>
void TBrylaSztywna<T>::WykonajRuch()
{
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	poprzedniaMacierzObrotu=macierzObrotu;	
	macierzObrotu=nastepnaMacierzObrotu;
	#else
	poprzedniKwaternion=kwaternion;
	kwaternion=nastepnyKwaternion;
	#endif
	predkoscKatowa=nastepnaPredkoscKatowa;
	srodekMasy->WykonajRuch();	
	++numerKroku;
}

template<typename T>
int TBrylaSztywna<T>::NumerKroku() const
{
	return numerKroku;
}

template<typename T>
float* TBrylaSztywna<T>::Kolor() const
{
	return srodekMasy->Kolor();	
}

template<typename T>
TWektor<T> TBrylaSztywna<T>::PolozenieSrodkaMasy() const
{
	return srodekMasy->Polozenie();
}

template<typename T>
TWektor<T> TBrylaSztywna<T>::PredkoscSrodkaMasy() const
{
	return srodekMasy->Predkosc();
}

template<typename T>
TWektor<T> TBrylaSztywna<T>::NastepnePolozenieSrodkaMasy() const
{
	return srodekMasy->NastepnePolozenie();
}


template<typename T>
TMacierz<T> TBrylaSztywna<T>::MacierzObrotu() const
{
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	return macierzObrotu;
	#else
	return kwaternion.MacierzObrotu();
	#endif 
}

template<typename T>
TMacierz<T> TBrylaSztywna<T>::NastepnaMacierzObrotu() const
{
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	return nastepnaMacierzObrotu;
	#else
	return nastepnyKwaternion.MacierzObrotu();
	#endif 
}

template<typename T>
TWektor<T> TBrylaSztywna<T>::PredkoscKatowa() const
{
	return predkoscKatowa;
}

template<typename T>
void TBrylaSztywna<T>::UstawPolozenieSrodkaMasy(TWektor<T> polozenie)
{
	srodekMasy->UstawPolozenie(polozenie);
}

template<typename T>
void TBrylaSztywna<T>::UstawOrientacje(TMacierz<T> macierzObrotu)
{
	#ifndef KWATERNIONY_ZAMIAST_MACIERZY
	this->macierzObrotu=macierzObrotu;
	#else
	this->kwaternion=TKwaternion<T>::TworzZMacierzyObrotu(macierzObrotu);
	#endif
}

template<typename T>
void TBrylaSztywna<T>::UstawPredkoscSrodkaMasy(TWektor<T> predkosc)
{
	srodekMasy->UstawPredkosc(predkosc);
}

template<typename T>
void TBrylaSztywna<T>::UstawPredkoscKatowa(TWektor<T> predkoscKatowa)
{
	this->predkoscKatowa=predkoscKatowa;
}

/********************************************************/
/*               Zbior bryl sztywnych                   */
/********************************************************/

template<typename T> 
bool TZbiorBrylSztywnych<T>::TestObecnosciZBS() const
{
	bool wynik=(this->bryly!=NULL);
	//if (!wynik) throw std::exception("Brak zbioru bryl sztywnych. Nadpisz klase ZbiorBrylSztywnych okreslajac ten zbior");
	return wynik;
}

template<typename T> 
TZbiorBrylSztywnych<T>::TZbiorBrylSztywnych(int ilosc)	
	:bryly(NULL),ilosc(ilosc)
	//trzeba okreslic konkretne bryly w klasie potomnej
{	
	this->wiezy=new bool[ilosc];
	for(int i=0;i<ilosc;i++) wiezy[i]=false;	
}

template<typename T> 
TZbiorBrylSztywnych<T>::~TZbiorBrylSztywnych()
{
	/*
	if(bryly!=NULL) 
	{
		for(int i=0;i<ilosc;++i) delete bryly[i];			
		delete [] bryly;
	}
	*/
	delete [] wiezy;	
}

template<typename T>
TBrylaSztywna<T>* TZbiorBrylSztywnych<T>::PobierzBryleSztywna(int indeks) const
{
	if(!TestObecnosciZBS()) return NULL;
	if(indeks<0 || indeks>=ilosc) return NULL;
	return bryly[indeks];
}

template<typename T>
void TZbiorBrylSztywnych<T>::UstawWiezy(int indeks,bool ustalonaPozycja)
{
	if(indeks>=0 && indeks<ilosc) this->wiezy[indeks]=ustalonaPozycja;
}

template<typename T>
void TZbiorBrylSztywnych<T>::PrzygotujRuch(T krokCzasowy,Algorytm algorytm)
{			
	TWektor<T> wypadkowaSila,wypadkowyMomentSily;
	if(!TestObecnosciZBS()) return;
	for(int i=0;i<ilosc;++i) 
		if(!wiezy[i]) 
		{
			ObliczSileIMomentSily(i,wypadkowaSila,wypadkowyMomentSily);
			this->bryly[i]->PrzygotujRuch(wypadkowaSila,wypadkowyMomentSily,krokCzasowy,algorytm);
		}
}

template<typename T>
void TZbiorBrylSztywnych<T>::WykonajRuch()
{	
	if(!TestObecnosciZBS()) return;
	for(int i=0;i<ilosc;++i) 
		if(!wiezy[i]) 
			this->bryly[i]->WykonajRuch();					
}

template<typename T>
void TZbiorBrylSztywnych<T>::KrokNaprzod(T krokCzasowy,Algorytm algorytm)
{	
	if(!TestObecnosciZBS()) return;
	PrzedKrokiemNaprzod(krokCzasowy);
	PrzygotujRuch(krokCzasowy,algorytm);
	PoPrzygotowaniuRuchu(krokCzasowy);
	WykonajRuch();
	PoKrokuNaprzod(krokCzasowy);
}

#endif