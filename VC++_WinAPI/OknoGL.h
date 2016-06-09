#ifndef OPENGL_H
#define OPENGL_H

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>

class COkno
{
	protected:
		long szerokoscObszaruUzytkownika;
		long wysokoscObszaruUzytkownika;
		HWND uchwytOkna;
	public:		
		COkno():uchwytOkna(NULL){};
		bool Init(HINSTANCE uchwytAplikacji,POINT polozenieOkna,POINT rozmiarOkna);
		WPARAM Run();
		virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam);
	private:
		bool ZmianaRozdzielczosci(long szerokosc,long wysokosc,long glebiaKolorow=32) const;
};

//zakomentowanie ponizszej linii powoduje przelaczenie rotacji sferycznej ArcBall
//do rotacji TPP z wyrozniona plaszczyzna
#define ARCBALL

#ifdef ARCBALL
#include "ArcBall.h"
#endif

class COknoGL : public COkno
{
	private:
		HGLRC uchwytRC; //uchwyt kontekstu renderingu
		HDC uchwytDC; //uchwyt prywatnego kontekstu urzadzenia GDI
		bool UstalFormatPikseli(HDC uchwytDC) const;
		bool InitWGL(HWND uchwytOkna);
		void UsunWGL();
	protected:		
		void UstawienieKamery();
		void UstawienieSceny(bool rzutowanieIzometryczne=false);		
		void RysujScene();	
		virtual void RysujAktorow() = 0;
		void WyswietlCzestoscRenderowania();
	public:		
		COknoGL():COkno(),uchwytRC(NULL),uchwytDC(NULL),			
			//kontrola kamery FPP i naiwna TPP
			kontrolaKameryPrzezUzytkownika(true),
			poczatkowaPozycjaKursoraMyszy(POINT()),
			kameraR(10),
			kameraCelPhi(0),kameraCelTheta(0),
			kameraX(0),kameraY(0),kameraZ(0),			
			//ArcBall
			#ifdef ARCBALL
			ArcBall(NULL),
			#else			
			kameraPhi(0),kameraTheta(0),
			#endif
			//oswietlenie
			natezenie_swiatla_tla(0.5f),
			pauza(false)
			{				
				#ifdef ARCBALL
				InicjujArcBall();
				#else
				//Prosty ArcBall
				for(int i=0;i<4;i++) 
					for(int j=0;j<4;j++) 
						korektaMacierzyModelWidok[4*i+j]=((i==j)?1.0f:0.0f);
				#endif
			};					
		virtual LRESULT WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam);		
	
	//polozenie kamery FPP i naiwne TPP
	protected:
		bool kontrolaKameryPrzezUzytkownika;
	private:		
		POINT poczatkowaPozycjaKursoraMyszy;
		float kameraR;
		#ifndef ARCBALL
		float kameraPhi,kameraTheta;
		//Prosty ArcBall
		float poprzedniaPozycjaKursoraMyszy3D[3];
		float korektaMacierzyModelWidok[16];
		#endif
		float kameraCelPhi,kameraCelTheta;
		float kameraX,kameraY,kameraZ;
		float polozenieKamery[3];
	public:		
		float* PolozenieKamery(float* bufor) const;

	//ArcBall
	#ifdef ARCBALL
	private:
		Matrix4fT Transform;
		Matrix3fT LastRot;
		Matrix3fT ThisRot;
		ArcBallT* ArcBall;
		Point2fT MousePt;
	public:
		void InicjujArcBall();
		~COknoGL(); //usuwanie obiektu ArcBall
	#endif

	//swobodne obroty
	bool swobodneObrotyAktywne;
	bool swobodneObroty_Wygaszanie;
	#ifdef ARCBALL
	Quat4fT swobodneObroty_kwaternionObrotu;
	void Obracaj(float kat,float x,float y,float z,bool wygaszanie);
	#else
	float swobodneObroty_szybkoscPhi;
	float swobodneObroty_szybkoscTheta;	
	void Obracaj(float szybkoscPhi,float szybkoscTheta,bool wygaszanie);
	#endif

	//oswietlenie
	public:
		float natezenie_swiatla_tla;
	private:
		void Oswietlenie();		
	protected:		
		virtual void ZrodlaSwiatla() = 0;		
	public:
		static void GladkiMaterial(bool czyGladki);
	protected:
		bool pauza;
};

#endif
