//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#ifndef MODELE_H
#define MODELE_H

class Modele
{
	public: 
		static void RysujProstopadloscian(float dx,float dy,float dz,bool koloruj,bool wiazTekstury,const unsigned int* indeksTekstury);
		static void RysujSzescian(float krawedz,bool koloruj,bool wiazTekstury,const unsigned int* indeksTekstury);
		static void RysujNiszczycielaGwiazd(float x0,float y0,float z0);
		static void RysujOsie(float x0,float y0,float z0);
		static void RysujPodloze(float x0,float y0,float z0);
		static void RysujNiebo(float x0,float y0,float z0);
		static void RysujOcean(float x0,float y0,float z0);
		static void RysujSzescian_UsrednianieNormalnych(float krawedz);
		static void RysujSfere(float promien,bool wiazTekstury,const unsigned int* indeksTekstury);
		static void RysujLatarnie(float rozmiar);
		static void RysujSilnik(float x0,float z0,bool silnikWlaczony);
		static void RysujProstopadloscian_SortowanieScian(float dx,float dy,float dz,bool koloruj,float alfa);
		static void RysujKij(float x0);
};



#endif