//Jacek Matulewski, e-mail: jacek@fizyka.umk.pl

#include "Modele.h"

#include <windows.h>
#include <gl\gl.h>
#include "Varia.h"



/*static*/ void Modele::RysujProstopadloscian(float dx,float dy,float dz,bool koloruj,bool wiazTekstury,const unsigned int* indeksTekstury)
{
	if(indeksTekstury==NULL) wiazTekstury=false;

	float dx_2=dx/2.0f;
	float dy_2=dy/2.0f;
	float dz_2=dz/2.0f;

	const float ns=3.0f,nt=3.0f;

	if(wiazTekstury) glBindTexture(GL_TEXTURE_2D,indeksTekstury[0]);
	glBegin(GL_QUADS);   
	if(koloruj) glColor3f(1.0f,0.0f,0.0f);		
	//tylnia   
	glNormal3f(0,0,-1);
	glTexCoord2f(ns,nt); glVertex3f(-dx_2,-dy_2,-dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(-dx_2,dy_2,-dz_2);
	glTexCoord2f(0.0f,0.0f); glVertex3f(dx_2,dy_2,-dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(dx_2,-dy_2,-dz_2);
	//przednia
	glNormal3f(0,0,1);
	glTexCoord2f(0.0f,0.0f); glVertex3f(-dx_2,-dy_2,dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(dx_2,-dy_2,dz_2);
	glTexCoord2f(ns,nt); glVertex3f(dx_2,dy_2,dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(-dx_2,dy_2,dz_2);
	glEnd();
	
	if(wiazTekstury) glBindTexture(GL_TEXTURE_2D,indeksTekstury[1]);
	glBegin(GL_QUADS);   
	if(koloruj) glColor3f(0.0f,1.0f,0.0f);	
	//prawa
	glNormal3f(1,0,0);
	glTexCoord2f(ns,nt); glVertex3f(dx_2,-dy_2,dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(dx_2,dy_2,dz_2);
	glTexCoord2f(0.0f,0.0f); glVertex3f(dx_2,dy_2,-dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(dx_2,-dy_2,-dz_2);
	//lewa
	glNormal3f(-1,0,0);
	glTexCoord2f(ns,nt); glVertex3f(-dx_2,-dy_2,dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(-dx_2,dy_2,dz_2);
	glTexCoord2f(0.0f,0.0f); glVertex3f(-dx_2,dy_2,-dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(-dx_2,-dy_2,-dz_2);
	glEnd();

	if(wiazTekstury) glBindTexture(GL_TEXTURE_2D,indeksTekstury[2]);
	glBegin(GL_QUADS);   
	if(koloruj) glColor3f(0.0f,0.0f,1.0f);	
	//gorna
	glNormal3f(0,1,0);
	glTexCoord2f(ns,nt); glVertex3f(-dx_2,dy_2,dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(dx_2,dy_2,dz_2);
	glTexCoord2f(0.0f,0.0f); glVertex3f(dx_2,dy_2,-dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(-dx_2,dy_2,-dz_2);
	//dolna
	glNormal3f(0,-1,0);
	glTexCoord2f(ns,nt); glVertex3f(-dx_2,-dy_2,dz_2);
	glTexCoord2f(ns,0.0f); glVertex3f(dx_2,-dy_2,dz_2);
	glTexCoord2f(0.0f,0.0f); glVertex3f(dx_2,-dy_2,-dz_2);
	glTexCoord2f(0.0f,nt); glVertex3f(-dx_2,-dy_2,-dz_2);
	glEnd();

	if(wiazTekstury) glBindTexture(GL_TEXTURE_2D,NULL);
}


/*static*/ void Modele::RysujSzescian(float krawedz,bool koloruj,bool wiazTekstury,const unsigned int* indeksTekstury)
{
	RysujProstopadloscian(krawedz,krawedz,krawedz,koloruj,wiazTekstury,indeksTekstury);
}

/*
void Modele::RysujNiszczycielaGwiazd(float x0,float y0,float z0)
{
	//glBegin(GL_TRIANGLES);
	////glBegin(GL_LINE_LOOP);
	//glVertex3f(0,y0,0);
	//glVertex3f(-x0,-0.9f*y0,0);
	//glVertex3f(0,-y0,-0.2f*z0);

	//glVertex3f(0,y0,0);
	//glVertex3f(x0,-0.9f*y0,0);
	//glVertex3f(0,-y0,-0.2f*z0);

	//glVertex3f(0,y0,0);
	//glVertex3f(x0,-0.9f*y0,0);
	//glVertex3f(0,-y0,0.2f*z0);

	//glVertex3f(0,y0,0);
	//glVertex3f(-x0,-0.9f*y0,0);
	//glVertex3f(0,-y0,0.2f*z0);
	//glEnd();

	//kadlub
	glBegin(GL_TRIANGLE_FAN);
	glVertex3f(0,y0,0); //dziub
	glVertex3f(-x0,-0.9f*y0,0); //spod lewy
	glVertex3f(0,-y0,-0.2f*z0);

	glVertex3f(x0,-0.9f*y0,0); //spod prawy
	glVertex3f(0,-y0,0.2f*z0); //gora prawa
	glVertex3f(-x0,-0.9f*y0,0); //gora prawa
	glEnd();

	//rufa
	glBegin(GL_QUADS);
	glVertex3f(-x0,-0.9f*y0,0);
	glVertex3f(0,-y0,0.2f*z0);
	glVertex3f(x0,-0.9f*y0,0);
	glVertex3f(0,-y0,-0.2f*z0);
	glEnd();

	//nadbudowka	
	glTranslatef(0,-0.5f*y0,0.13f*z0);
	RysujProstopadloscian(0.55f*x0,0.55f*y0,0.1f*z0,false,false,NULL);
	glColor3f(1,1,1);

	//mostek
	glTranslatef(0,-0.29f*y0,0.13f*z0);
	RysujProstopadloscian(0.15f*x0,0.3f*y0,0.25f*z0,false,false,NULL);
	glTranslatef(0,0.1f*y0,0.1f*z0);
	RysujProstopadloscian(0.4f*x0,0.1f*y0,0.1f*z0,false,false,NULL);
}
*/

/*static*/ void Modele::RysujNiszczycielaGwiazd(float x0,float y0,float z0)
{
	glShadeModel(GL_FLAT);
	//glShadeModel(GL_SMOOTH);

	float punkt1[3]={0,y0,0};
	float punkt2[3]={-x0,-0.9f*y0,0};
	float punkt3[3]={0,-y0,-0.2f*z0};
	float punkt4[3]={x0,-0.9f*y0,0};
	float punkt5[3]={0,-y0,0.2f*z0};
	
	float normalna[3];
	
	//kadlub
	/*
	glBegin(GL_TRIANGLES);
	JednostkowyWektorNormalny3fv(punkt1,punkt3,punkt2,normalna);
	glNormal3fv(normalna);
	glVertex3fv(punkt1);
	glVertex3fv(punkt2);
	glVertex3fv(punkt3);

	JednostkowyWektorNormalny3fv(punkt1,punkt4,punkt3,normalna);
	glNormal3fv(normalna);
	glVertex3fv(punkt1);
	glVertex3fv(punkt4);
	glVertex3fv(punkt3);

	JednostkowyWektorNormalny3fv(punkt1,punkt5,punkt4,normalna);
	glNormal3fv(normalna);	
	glVertex3fv(punkt1);
	glVertex3fv(punkt4);
	glVertex3fv(punkt5);

	JednostkowyWektorNormalny3fv(punkt1,punkt2,punkt5,normalna);
	glNormal3fv(normalna);	
	glVertex3fv(punkt1);
	glVertex3fv(punkt2);
	glVertex3fv(punkt5);
	glEnd();
	*/	

	//kadlub
	//glBindTexture(GL_TEXTURE_2D,indeksTekstury[0]);
	glBegin(GL_TRIANGLE_FAN);
	JednostkowyWektorNormalny3fv(punkt1,punkt3,punkt2,normalna);
	glNormal3fv(normalna);
	glTexCoord2f(0,10); glVertex3fv(punkt1); //dziub
	glTexCoord2f(5,0); glVertex3fv(punkt2); //spod lewy 
	glTexCoord2f(0,0); glVertex3fv(punkt3);

	JednostkowyWektorNormalny3fv(punkt1,punkt4,punkt3,normalna);
	glNormal3fv(normalna);
	glTexCoord2f(-5,0); glVertex3fv(punkt4); //spod prawy
	JednostkowyWektorNormalny3fv(punkt1,punkt5,punkt4,normalna);
	glNormal3fv(normalna);
	glTexCoord2f(0,0); glVertex3fv(punkt5); //gora prawa
	JednostkowyWektorNormalny3fv(punkt1,punkt2,punkt5,normalna);
	glNormal3fv(normalna);
	glTexCoord2f(5,0); glVertex3fv(punkt2); //gora lewa
	glEnd();	

	//rufa
	glShadeModel(GL_SMOOTH);
	glBegin(GL_TRIANGLE_STRIP);
	JednostkowyWektorNormalny3fv(punkt3,punkt4,punkt2,normalna);
	glNormal3fv(normalna);		
	glVertex3fv(punkt3);
	glVertex3fv(punkt4);
	glVertex3fv(punkt2);
	JednostkowyWektorNormalny3fv(punkt2,punkt4,punkt5,normalna);
	glNormal3fv(normalna);
	glVertex3fv(punkt5);
	glEnd();
	glShadeModel(GL_FLAT);

	//nadbudowka	
	//glBindTexture(GL_TEXTURE_2D,indeksTekstury[1]);
	glTranslatef(0,-0.5f*y0,0.13f*z0);
	RysujProstopadloscian(0.55f*x0,0.55f*y0,0.1f*z0,false,false,NULL);	
	
	//mostek
	glTranslatef(0,-0.29f*y0,0.13f*z0);
	RysujProstopadloscian(0.15f*x0,0.3f*y0,0.25f*z0,false,false,NULL);
	glTranslatef(0,0.1f*y0,0.1f*z0);
	RysujProstopadloscian(0.4f*x0,0.1f*y0,0.1f*z0,false,false,NULL);

	glBindTexture(GL_TEXTURE_2D,NULL);

	//silniki (GLU)
	glRotatef(90,1,0,0);
	glTranslatef(0,-0.35f*y0,0.25f*z0);
	glTranslatef(-0.25f*x0,0,0);
	RysujSilnik(0.1f*x0,0.1f*z0,true);
	glTranslatef(0.25f*x0,0,0);
	RysujSilnik(0.1f*x0,0.1f*z0,true);
	glTranslatef(0.25f*x0,0,0);
	RysujSilnik(0.1f*x0,0.1f*z0,true);
}

/*static*/ void Modele::RysujOsie(float x0,float y0,float z0)
{
	glBegin(GL_LINES);
	glVertex3f(0,0,0); glVertex3f(x0,0,0);
	glVertex3f(0,0,0); glVertex3f(0,y0,0);
	glVertex3f(0,0,0); glVertex3f(0,0,z0);
	glEnd();
}

/*static*/ void Modele::RysujPodloze(float x0,float y0,float z0)
{
	glShadeModel(GL_SMOOTH);

	const int nx=10;
	const int nz=10;

	glNormal3f(0,1,0);	

	for(int ix=-nx+1;ix<nx;ix++)
	{
		glBegin(GL_QUAD_STRIP);
		for(int iz=-nz+1;iz<nz;iz++)
		{
			glColor3f(0,0.2f,0);
			if((ix+iz)%2==0) glColor3f(0.2f,0.3f,0);			
			glVertex3f((ix)*x0,y0,iz*z0);
			glVertex3f((ix+1)*x0,y0,iz*z0);
		}
		glEnd();
	}
}

/*static*/ void Modele::RysujNiebo(float x0,float y0,float z0)
{
	glShadeModel(GL_SMOOTH);

	const int nx=10;
	const int nz=10;

	glNormal3f(0,-1,0);	

	for(int ix=-nx+1;ix<nx;ix++)
	{		
		glBegin(GL_QUAD_STRIP);
		for(int iz=-nz+1;iz<nz;iz++)
		{
			glColor3f(0,0.8f,1);
			if((ix+iz)%2==0) glColor3f(0,1,1);			
			glVertex3f((ix)*x0,y0,iz*z0);
			glVertex3f((ix+1)*x0,y0,iz*z0);
		}
		glEnd();
	}
}

/*static*/ void Modele::RysujOcean(float x0,float y0,float z0)
{
	glShadeModel(GL_SMOOTH);

	const int nx=10;
	const int nz=10;

	glNormal3f(0,1,0);	

	for(int ix=-nx+1;ix<nx;ix++)
	{		
		glBegin(GL_QUAD_STRIP);
		for(int iz=-nz+1;iz<nz;iz++)
		{
			glColor4f(0,0.8f,1,0.5f);
			if((ix+iz)%2==0) glColor4f(0,1,1,0.5f);
			glVertex3f((ix)*x0,y0,iz*z0);
			glVertex3f((ix+1)*x0,y0,iz*z0);
		}
		glEnd();
	}
}

/*static*/ void Modele::RysujSzescian_UsrednianieNormalnych(float krawedz)
{
   const float a=krawedz;

   glBegin(GL_QUADS);
   //tylnia
   glNormal3f(-1,-1,-1); glVertex3f(-a,-a,-a);
   glNormal3f(-1,1,-1); glVertex3f(-a,a,-a);
   glNormal3f(1,1,-1); glVertex3f(a,a,-a);
   glNormal3f(1,-1,-1); glVertex3f(a,-a,-a);
   //przednia
   glNormal3f(-1,-1,1); glVertex3f(-a,-a,a);
   glNormal3f(1,-1,1); glVertex3f(a,-a,a);
   glNormal3f(1,1,1); glVertex3f(a,a,a);
   glNormal3f(-1,1,1); glVertex3f(-a,a,a);

   //prawa
   glNormal3f(1,-1,1); glVertex3f(a,-a,a);
   glNormal3f(1,1,1); glVertex3f(a,a,a);
   glNormal3f(1,1,-1); glVertex3f(a,a,-a);
   glNormal3f(1,-1,-1); glVertex3f(a,-a,-a);
   //lewa
   glNormal3f(-1,-1,1); glVertex3f(-a,-a,a);
   glNormal3f(-1,1,1); glVertex3f(-a,a,a);
   glNormal3f(-1,1,-1); glVertex3f(-a,a,-a);
   glNormal3f(-1,-1,-1); glVertex3f(-a,-a,-a);

   //gorna
   glNormal3f(-1,1,1); glVertex3f(-a,a,a);
   glNormal3f(1,1,1); glVertex3f(a,a,a);
   glNormal3f(1,1,-1); glVertex3f(a,a,-a);
   glNormal3f(-1,1,-1); glVertex3f(-a,a,-a);
   //dolna
   glNormal3f(-1,-1,1); glVertex3f(-a,-a,a);
   glNormal3f(1,-1,1); glVertex3f(a,-a,a);
   glNormal3f(1,-1,-1); glVertex3f(a,-a,-a);
   glNormal3f(-1,-1,-1); glVertex3f(-a,-a,-a);

   glEnd();
}

/* ---- Sortowanie scian ---- */
#pragma region Sortowanie scian
class Czworokat
{
	private:
		float* wierzcholki[4]; //Kolejnosc wierzcholkow: (lewy, dolny), (lewy, gorny), (prawy, gorny), (prawy, dolny)		
		float normalna[3];
		float srodek[3];		
		bool kolorUstalony;
		float kolor[4];

		void ObliczSrodek()
		{
			for(int i=0;i<3;i++)
			{				
				srodek[i]=0.0f;
				for(int j=0;j<4;j++) srodek[i]+=wierzcholki[j][i];
				srodek[i]/=4.0f;
			}
		}			

		
	public:
		void Rysuj()
		{								
			if(kolorUstalony) glColor4fv(kolor);
			glBegin(GL_QUADS);
			glNormal3fv(normalna);				
			for(int i=0;i<4;i++) 
			{				
				switch(i)
				{
					case 0: glTexCoord2f(0.0f,0.0f); break;
					case 1: glTexCoord2f(0.0f,1.0f); break;
					case 2: glTexCoord2f(1.0f,1.0f); break;
					case 3: glTexCoord2f(1.0f,0.0f); break;
				}
				glVertex3fv(wierzcholki[i]);
			}
			glEnd();
		}

		float OdlegloscSrodkaOdPunktu(float* punkt)
		{				
			return OdlegloscPunktow(punkt,srodek);
		}

		Czworokat(float* punktLD,float* punktLG,float* punktPG,float* punktPD)
		{
			wierzcholki[0]=punktLD;
			wierzcholki[1]=punktLG;
			wierzcholki[2]=punktPG;
			wierzcholki[3]=punktPD;
			JednostkowyWektorNormalny3fv(wierzcholki[0],wierzcholki[1],wierzcholki[2],normalna);
			ObliczSrodek();	
			kolorUstalony=false;
		}			

		void Czworokat::UstalKolor(float R,float G,float B,float A)
		{			
			kolor[0]=R;
			kolor[1]=G;
			kolor[2]=B;
			kolor[3]=A;
			kolorUstalony=true;
		}
};

int sgn(float arg) {
	if (arg > 0) return 1;
	else if (arg < 0) return -1;
	else return 0;
}

#include "OknoGL.h"
extern COknoGL* p_okno;

int porownajOdlegloscCzworokatowOdKamery(const void *arg1, const void *arg2)
{
	Czworokat* karg1=(Czworokat*)arg1;
	Czworokat* karg2=(Czworokat*)arg2;
	float polozenieKamery[3];	
	p_okno->PolozenieKamery(polozenieKamery);
	float odleglosc=karg1->OdlegloscSrodkaOdPunktu(polozenieKamery)-karg2->OdlegloscSrodkaOdPunktu(polozenieKamery);
	return sgn(odleglosc);
};

void Modele::RysujProstopadloscian_SortowanieScian(float dx,float dy,float dz,bool koloruj,float alfa)
{
	float dx_2=dx/2.0f;
	float dy_2=dy/2.0f;
	float dz_2=dz/2.0f;

	float punktMMM[3]={-dx_2,-dy_2,-dz_2};
	float punktMPM[3]={-dx_2,dy_2,-dz_2};
	float punktPPM[3]={dx_2,dy_2,-dz_2};
	float punktPMM[3]={dx_2,-dy_2,-dz_2};
	
	float punktMMP[3]={-dx_2,-dy_2,dz_2};
	float punktPMP[3]={dx_2,-dy_2,dz_2};
	float punktPPP[3]={dx_2,dy_2,dz_2};
	float punktMPP[3]={-dx_2,dy_2,dz_2};

	Czworokat scianyProstopadloscianu[6]={
		Czworokat(punktMMM,punktMPM,punktPPM,punktPMM), //tylnia (0)
		Czworokat(punktMMP,punktPMP,punktPPP,punktMPP), //przednia (1)
		Czworokat(punktPMP,punktPMM,punktPPM,punktPPP), //prawa (2)
		Czworokat(punktMMP,punktMPP,punktMPM,punktMMM), //lewa (3)
		Czworokat(punktMPP,punktPPP,punktPPM,punktMPM), //gorna (4)
		Czworokat(punktMMP,punktMMM,punktPMM,punktPMP), //dolna (5)
	};	

	if(koloruj)
	{
		scianyProstopadloscianu[0].UstalKolor(0,0,1,alfa);
		scianyProstopadloscianu[1].UstalKolor(0,1,0,alfa);
		scianyProstopadloscianu[2].UstalKolor(0,1,1,alfa);
		scianyProstopadloscianu[3].UstalKolor(1,0,0,alfa);
		scianyProstopadloscianu[4].UstalKolor(1,0,1,alfa);
		scianyProstopadloscianu[5].UstalKolor(1,1,0,alfa);
	}	
	
	/*
	//rysowanie linii wskazujacej polozenie kamery
	#ifdef _DEBUG
	glColor3f(1,0,0);
	float polozenieKamery[3];
	p_okno->PolozenieKamery(polozenieKamery);
	glBegin(GL_LINES);
	glVertex3fv(polozenieKamery);
	glVertex3f(0,0,0);
	glEnd();

	char strX[256]=""; _gcvt(polozenieKamery[0],10,strX);
	char strY[256]=""; _gcvt(polozenieKamery[1],10,strY);
	char strZ[256]=""; _gcvt(polozenieKamery[2],10,strZ);
	char polozenie[256]="x=";
	strcat(polozenie,strX);
	strcat(polozenie,", y=");
	strcat(polozenie,strY);
	strcat(polozenie,", z=");
	strcat(polozenie,strZ);
	//SetWindowText(uchwytOkna,polozenie);
	#endif
	*/

	//glColor4f(1,1,1,alfa);

	//sortowanie	
	qsort(scianyProstopadloscianu,6,sizeof(Czworokat),porownajOdlegloscCzworokatowOdKamery);

	//rysowanie scian
	for(int i=0;i<6;i++) 
	{
		if(false) //testowanie sortowania (argument koloruj musi byc rowny false)
		{
			switch(i)
			{
				case 0: glColor4f(0,0,1,alfa); break;
				case 1: glColor4f(0,1,0,alfa); break;
				case 2: glColor4f(0,1,1,alfa); break;
				case 3: glColor4f(1,0,0,alfa); break;
				case 4: glColor4f(1,0,1,alfa); break;
				case 5: glColor4f(1,1,0,alfa); break;
				default: glColor4f(1,1,1,alfa); break;
			}
		}
		scianyProstopadloscianu[i].Rysuj();	
	}
}
#pragma endregion


/* ---- Funkcje korzystajace z GLU ---- */

#include <gl\glu.h>

/*static*/ void Modele::RysujSfere(float promien,bool teksturowanie,const unsigned int* indeksTekstury)
{ 	
	if(indeksTekstury==NULL) teksturowanie=false;
        glPushMatrix();
	glRotatef(-90,1,0,0); //bieguny na osi OY	
	GLUquadricObj* kwadryka=gluNewQuadric(); //tworzenie obiektu kwadryki 
	gluQuadricDrawStyle(kwadryka,GLU_FILL); //ustalenie stylu rysowania
	//gluQuadricDrawStyle(kwadryka,GLU_SILHOUETTE);
	//gluQuadricDrawStyle(kwadryka,GLU_POINT);		
	if(teksturowanie) 
	{
		glBindTexture(GL_TEXTURE_2D,indeksTekstury[0]);
		glEnable(GL_TEXTURE_2D);
		gluQuadricTexture(kwadryka,GL_TRUE);
	}
	gluSphere(kwadryka,promien,100,100); //rysowanie 
	//gluSphere(kwadryka,promien,20,20); //rysowanie 	
	if(teksturowanie) 
	{
		gluQuadricTexture(kwadryka,GL_FALSE);
		glDisable(GL_TEXTURE_2D);
	}
	gluDeleteQuadric(kwadryka); //usuwanie obiektu 
        glPopMatrix();
}

/*static*/ void Modele::RysujLatarnie(float rozmiar)
{ 
	GLUquadricObj* kwadryka=gluNewQuadric(); //tworzenie obiektu kwadryki 
	gluQuadricDrawStyle(kwadryka,GLU_FILL); //ustalenie stylu rysowania


	//latarnia
	glPushMatrix();
	gluQuadricDrawStyle(kwadryka,GLU_FILL);
	glColor3f(1,1,1);
	glRotatef(90,1,0,0);	
	gluCylinder(kwadryka,0.1*rozmiar,0.2*rozmiar,rozmiar,100,1); //wieza		
	glTranslatef(0,0,rozmiar);
	gluDisk(kwadryka,0,0.2*rozmiar,100,1); //podstawa
	glTranslatef(0,0,-rozmiar);
	//balkon
	glColor3f(1,0,0);
	gluDisk(kwadryka,0.05*rozmiar,0.2*rozmiar,100,1); //balkon - podloga
	gluQuadricDrawStyle(kwadryka,GLU_SILHOUETTE);
	glLineWidth(3);
	glTranslatef(0,0,-0.1f*rozmiar);	
	gluCylinder(kwadryka,0.2*rozmiar,0.2*rozmiar,0.1*rozmiar,30,1); //barierka
	//laterna
	gluQuadricDrawStyle(kwadryka,GLU_SILHOUETTE);
	glColor3f(1,0,0);
	glTranslatef(0,0,-0.1f*rozmiar);		
	gluCylinder(kwadryka,0.1*rozmiar,0.1*rozmiar,0.2*rozmiar,5,1); //laterny - konstrukcja 
	gluQuadricDrawStyle(kwadryka,GLU_FILL);
	glTranslatef(0,0,-0.1f*rozmiar);
	gluCylinder(kwadryka,0,0.1*rozmiar,0.1*rozmiar,100,1); //laterna - daszek
	glColor3f(1,1,0);
	glTranslatef(0,0,0.15f*rozmiar);	
	gluSphere(kwadryka,0.05*rozmiar,30,30); //swiatlo w laternie
	glColor4f(0,1,1,0.5);
	glTranslatef(0,0,-0.05f*rozmiar);	
	COknoGL::GladkiMaterial(true);
	gluCylinder(kwadryka,0.1*rozmiar,0.1*rozmiar,0.2*rozmiar,30,1); //laterna - szyba	
	COknoGL::GladkiMaterial(false);
	glPopMatrix();

	gluDeleteQuadric(kwadryka); //usuwanie obiektu 

	//dom przy latarni
	glPushMatrix();
	glColor3f(1,1,1);
	glTranslatef(0.35f*rozmiar,-0.85f*rozmiar,0);
	RysujProstopadloscian(0.5f*rozmiar,0.3f*rozmiar,0.3f*rozmiar,false,false,NULL); //sciany
	glColor3f(1,0,0);	
	glShadeModel(GL_FLAT);
	glBegin(GL_QUAD_STRIP);
	glNormal3f(0,0.7071f,0.7071f);	
	glVertex3f(0.25f*rozmiar,0.15f*rozmiar,0.15f*rozmiar);
	glVertex3f(-0.25f*rozmiar,0.15f*rozmiar,0.15f*rozmiar);
	glVertex3f(0.25f*rozmiar,0.3f*rozmiar,0);
	glVertex3f(-0.25f*rozmiar,0.3f*rozmiar,0);
	glNormal3f(0,0.7071f,-0.7071f);
	glVertex3f(0.25f*rozmiar,0.15f*rozmiar,-0.15f*rozmiar);
	glVertex3f(-0.25f*rozmiar,0.15f*rozmiar,-0.15f*rozmiar);
	glEnd();
	glBegin(GL_TRIANGLES);
	glNormal3f(1,0,0);
	glVertex3f(0.25f*rozmiar,0.15f*rozmiar,0.15f*rozmiar);
	glVertex3f(0.25f*rozmiar,0.3f*rozmiar,0);
	glVertex3f(0.25f*rozmiar,0.15f*rozmiar,-0.15f*rozmiar);
	glEnd();
	glPopMatrix();	

	//snop swiatla
	glPushMatrix();
	glShadeModel(GL_SMOOTH);
	glTranslatef(0,0.15f*rozmiar,0);
	glBegin(GL_QUADS);
	glNormal3f(0,0,1);
	for(int i=-1;i<=1;i+=2)
	{
		glColor4f(1,1,0,1);
		glVertex3f(0,-0.01f*rozmiar,0);
		glColor4f(1,1,0,0);
		glVertex3f(i*rozmiar,-0.1f*rozmiar,0);
		glColor4f(1,1,0,0);
		glVertex3f(i*rozmiar,0.1f*rozmiar,0);
		glColor4f(1,1,0,1);
		glVertex3f(0,0.01f*rozmiar,0);	
	}
	glEnd();
	glPopMatrix();
}

/*static*/ void Modele::RysujSilnik(float x0,float z0,bool silnikWlaczony)
{
	glPushMatrix();
	GLUquadricObj* kwadryka=gluNewQuadric(); //tworzenie obiektu kwadryki 
	gluQuadricDrawStyle(kwadryka,GLU_FILL); //ustalenie stylu rysowania
	glColor4f(1,1,1,1);
	gluCylinder(kwadryka,x0,0.8f*x0,z0,100,100); //wylot 
	if (silnikWlaczony) 
	{
		glTranslatef(0,0,1.5f*z0);
		glColor4f(0.3f,0.3f,1,0.25f);
		gluSphere(kwadryka,0.2f*x0,100,100);
		glTranslatef(0,0,-z0);
		glColor4f(0.5f,0.5f,1,0.5f);
		gluCylinder(kwadryka,0.75f*x0,0,4*z0,100,100); //smuga
	}
	gluDeleteQuadric(kwadryka); //usuwanie obiektu 
	glPopMatrix();
}

/*static*/ void Modele::RysujKij(float x0)
{
	glPushMatrix();
	GLUquadricObj* kwadryka=gluNewQuadric();
	gluQuadricDrawStyle(kwadryka,GLU_FILL);
	glColor3f(0.75f,0.25f,0.25f);
	gluCylinder(kwadryka,0.02*x0,0.005*x0,x0,10,10);
	gluSphere(kwadryka,0.02*x0,10,10);
	glTranslatef(0,0,x0);
	glColor3f(1,0.5f,0.5f);
	gluDisk(kwadryka,0,0.005*x0,10,10);
	gluDeleteQuadric(kwadryka);
	glPopMatrix();
}