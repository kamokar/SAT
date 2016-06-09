#include "OknoGL.h"
#include "resource.h"

#include <math.h>

#include "Varia.h"


#pragma region Funkcje WinMain i WndProc
extern COknoGL* p_okno;

int WINAPI WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	POINT polozenieOkna={100,100};
	POINT rozmiarOkna={800,600};	
	//if (!okno.Init(hInstance,polozenieOkna,rozmiarOkna))
	if (!p_okno->Init(hInstance,polozenieOkna,rozmiarOkna))
	{
		MessageBox(NULL,"Inicjacja okna nie powiod³a siê","Aplikacja OpenGL",MB_OK | MB_ICONERROR);
		return EXIT_FAILURE;
	}
	//else return okno.Run();
	else return p_okno->Run();
}

LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{		
	return p_okno->WndProc(hWnd,message,wParam,lParam);
}
#pragma endregion

/* ------------------------------------------- */

#pragma region Klasa COkno
bool COkno::Init(HINSTANCE uchwytAplikacji,POINT polozenieOkna,POINT rozmiarOkna)
{
	char nazwaOkna[]="Aplikacja OpenGL";

	WNDCLASSEX wc;
	wc.cbSize=sizeof(wc);
	wc.style= CS_HREDRAW | CS_VREDRAW | CS_OWNDC; //styl okna
	wc.lpfnWndProc= (WNDPROC)::WndProc; //procedura okna
	wc.cbClsExtra= 0; //dodatkowe bajty zarezerwowane za klas¹ okna
	wc.cbWndExtra= 0; //dodatkowe bajty zarezerwowane za instancj¹ okna
	wc.hInstance= uchwytAplikacji; //instancja aplikacji
	wc.hIcon= LoadIcon(uchwytAplikacji,MAKEINTRESOURCE(IDI_GLICON)); //uchwyt ikony
	wc.hIconSm= LoadIcon(uchwytAplikacji,MAKEINTRESOURCE(IDI_GLICON)); //uchwyt ikony
	wc.hCursor= LoadCursor(NULL, IDC_ARROW); //uchwyt kursora
	wc.hbrBackground= NULL; //uchwyt pêdzla t³a
	wc.lpszMenuName= NULL; //nazwa menu
	wc.lpszClassName= nazwaOkna; //nazwa klasy okna	

	//Rejestracja klasy okna
	if(RegisterClassEx(&wc)==0) return false;

	bool trybPelnoekranowy=false;

	//ustawienia dla okna
	DWORD stylOkna=WS_OVERLAPPEDWINDOW;

	if (trybPelnoekranowy)
	{
		//ustawienia dla trybu pelnoekranowego
		polozenieOkna.x=0;
		polozenieOkna.y=0;
		//rozmiarOkna.x=1024;
		//rozmiarOkna.y=768;
		RECT rozmiarEkranu;
		GetWindowRect(GetDesktopWindow(),&rozmiarEkranu);
		rozmiarOkna.x=rozmiarEkranu.right-rozmiarEkranu.left;
		rozmiarOkna.y=rozmiarEkranu.bottom-rozmiarEkranu.top;
		stylOkna=WS_POPUP;
		if (!ZmianaRozdzielczosci(rozmiarOkna.x,rozmiarOkna.y)) return false;
	}
			
	//Tworzenie okna
	uchwytOkna = CreateWindow(
		nazwaOkna, //nazwa klasy okna
		nazwaOkna, //nazwa okna
		stylOkna,
		polozenieOkna.x,polozenieOkna.y, //po³o¿enie okna (x,y)
		rozmiarOkna.x,rozmiarOkna.y, //rozmiar okna (szerokoœæ, wysokoœæ)
		NULL, //uchwyt okna nadrzêdnego (parent)
		NULL, //uchwyt menu
		uchwytAplikacji, //uchwyt instancji aplikacji
		NULL //parametr komunikatu informujacego o utworzeniu okna
	);

	if(uchwytOkna==NULL) return false;

	//Pokazanie i aktualizacja okna
	ShowWindow(uchwytOkna,SW_SHOW);
	UpdateWindow(uchwytOkna);	

	return true;
};

WPARAM COkno::Run()
{
	//Petla glowna - obsluga komunikatow
	MSG msg;
	while(GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT COkno::WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	switch (message)
	{
		case WM_DESTROY: //Zamykanie okna => konczenie aplikacji
			//ChangeDisplaySettings(NULL,0); //przywrocenie domyslnej rozdzielczosci ekranu						
			PostQuitMessage(0);
			break;
		case WM_SIZE: //Zmiana rozmiaru okna
			RECT rect;
			GetClientRect(hWnd, &rect);
			szerokoscObszaruUzytkownika = rect.right - rect.left;
			wysokoscObszaruUzytkownika = rect.bottom - rect.top;			
			break;
		default: //Automatyczne przetwarzanie komunikatow
			return (DefWindowProc(hWnd, message, wParam, lParam));
	}

	return 0L;
}

bool COkno::ZmianaRozdzielczosci(long szerokosc,long wysokosc,long glebiaKolorow) const
{
	DEVMODE dmScreenSettings;	//struktura trybu wyswietlania
	memset(&dmScreenSettings,0,sizeof(dmScreenSettings));	//czyszczenie pamieci
	dmScreenSettings.dmSize=sizeof(dmScreenSettings);		//rozmiar struktury
	dmScreenSettings.dmPelsWidth	= szerokosc;			//nowa szerokosc ekranu
	dmScreenSettings.dmPelsHeight	= wysokosc;				//nowa wysokosc ekranu
	dmScreenSettings.dmBitsPerPel	= glebiaKolorow;		//ilosc bitow opisujacych kolor piksela
	dmScreenSettings.dmFields=DM_BITSPERPEL|DM_PELSWIDTH|DM_PELSHEIGHT;
	return ChangeDisplaySettings(&dmScreenSettings,CDS_FULLSCREEN)==DISP_CHANGE_SUCCESSFUL;
}
#pragma endregion

/* ------------------------------------------- */

#pragma region Klasa COknoGL
LRESULT COknoGL::WndProc(HWND hWnd, UINT message, WPARAM wParam,LPARAM lParam)
{
	const bool SWOBODNE_OBROTY_KAMERY=false;
	const int IDT_TIMER_OBROTOW_KAMERY=2;
	const int timerInterval=50;

	long wynik=COkno::WndProc(hWnd,message,wParam,lParam);	

	switch (message)
	{
		case WM_CREATE: //Utworzenie okna
			//zmienna uchwytOkna nie jest jeszcze zainicjowana
			InitWGL(hWnd);
			UstawienieSceny();
			//Wersja OpenGL na pasku tytulu				
			{
				char tytul[1024]="OpenGL ";
				strcat_s(tytul,(char*)glGetString(GL_VERSION));
				strcat_s(tytul,", GLU ");
				strcat_s(tytul,(char*)gluGetString(GLU_VERSION));
				SetWindowText(hWnd,tytul);
			}
			//swobodne obroty
			if(SWOBODNE_OBROTY_KAMERY)
				if(SetTimer(hWnd,IDT_TIMER_OBROTOW_KAMERY,50,NULL)==0)
					MessageBox(hWnd,"Nie uda³o siê ustawiæ timera","",MB_OK | MB_ICONERROR);						
			#ifdef ARCBALL
			//Obracaj(-5,0,1,0,false);
			#else
			//Obracaj(-5.0f,1.0f,false);
			#endif
			break;
		case WM_TIMER:
			switch(wParam)
			{
				case IDT_TIMER_OBROTOW_KAMERY:					
					if(swobodneObrotyAktywne)
					{						
						#ifdef ARCBALL									
						Matrix3fSetRotationFromQuat4f(&ThisRot, &swobodneObroty_kwaternionObrotu);
						Matrix3fMulMatrix3f(&ThisRot, &LastRot); //powtorzenie ostatniego obrotu
						Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
						LastRot=ThisRot;
						
						//wygaszanie swobodnych obrotow = zmniejszanie kata kolejnego obrotu
						if(swobodneObroty_Wygaszanie)
						{
							const float tempoWygaszania=0.97f;	
							swobodneObroty_kwaternionObrotu.s.W/=tempoWygaszania;
							swobodneObroty_kwaternionObrotu.s.X*=tempoWygaszania;
							swobodneObroty_kwaternionObrotu.s.Y*=tempoWygaszania;
							swobodneObroty_kwaternionObrotu.s.Z*=tempoWygaszania;
							if(fabs(swobodneObroty_kwaternionObrotu.s.X)<1E-3 && fabs(swobodneObroty_kwaternionObrotu.s.Y)<1E-3 && fabs(swobodneObroty_kwaternionObrotu.s.Z)<1E-3)
							{
								swobodneObrotyAktywne=false;								
							}
						}
						#else
						//kat=w*t
						kameraPhi+=swobodneObroty_szybkoscPhi;
						kameraTheta+=swobodneObroty_szybkoscTheta;
						//wygaszanie swobodnych obrotow
						if(swobodneObroty_Wygaszanie)
						{
							const float tempoWygaszania=0.97f;
							swobodneObroty_szybkoscPhi*=tempoWygaszania;
							swobodneObroty_szybkoscTheta*=tempoWygaszania;
							if(fabs(swobodneObroty_szybkoscPhi)<1E-3 && fabs(swobodneObroty_szybkoscTheta)<1E-3)
								swobodneObrotyAktywne=false;
						}
						#endif
						RysujScene();
					}
					break;
			}
			wynik=0;
			break;
		case WM_DESTROY: //Zamkniecie okna
			UsunWGL();
			KillTimer(uchwytOkna,IDT_TIMER_OBROTOW_KAMERY);
			break;
		case WM_SIZE: //Zmiana rozmiaru okna
			UstawienieSceny();
			break;
		case WM_PAINT: //Okno wymaga odrysowania
			RysujScene();
			ValidateRect(hWnd,NULL);
			break;
		
		case WM_KEYDOWN:			
			switch(wParam)
			{
				case VK_ESCAPE: 
					SendMessage(uchwytOkna,WM_DESTROY,0,0); 
					break;
				
				case VK_OEM_MINUS:
					natezenie_swiatla_tla-=0.01f;
					if(natezenie_swiatla_tla<0) natezenie_swiatla_tla=0;
					Oswietlenie();
					break;
				case VK_OEM_PLUS:
				case '=':
					natezenie_swiatla_tla+=0.01f;
					if(natezenie_swiatla_tla>1) natezenie_swiatla_tla=1;
					Oswietlenie();
					break;

				case 'C':
					static bool rzutowanieIzometryczne=false;
					rzutowanieIzometryczne=!rzutowanieIzometryczne;
					UstawienieSceny(rzutowanieIzometryczne);
					break;
			}
			
			if (wParam>='0' && wParam<='7')
			{
				GLenum swiatlo=GL_LIGHT0;
				switch (wParam)
				{
					case '1': swiatlo=GL_LIGHT1; break;
					case '2': swiatlo=GL_LIGHT2; break;
					case '3': swiatlo=GL_LIGHT3; break;
					case '4': swiatlo=GL_LIGHT4; break;
					case '5': swiatlo=GL_LIGHT5; break;
					case '6': swiatlo=GL_LIGHT6; break;
					case '7': swiatlo=GL_LIGHT7; break;
					default: swiatlo=GL_LIGHT0;
				}
				if (glIsEnabled(swiatlo)) glDisable(swiatlo);
				else glEnable(swiatlo);
			}
			
			RysujScene();
			break;
	}

	//kamera
	if(kontrolaKameryPrzezUzytkownika)
	{
		switch(message)
		{
			case WM_LBUTTONDOWN:			
				#ifdef ARCBALL				
				MousePt.s.X = (GLfloat)LOWORD(lParam);
				MousePt.s.Y = (GLfloat)HIWORD(lParam);			
				LastRot = ThisRot;
				ArcBall->click(&MousePt);
				#endif
				swobodneObrotyAktywne=false;				
			case WM_RBUTTONDOWN:
			case WM_MBUTTONDOWN:
				poczatkowaPozycjaKursoraMyszy.x=LOWORD(lParam);
				poczatkowaPozycjaKursoraMyszy.y=HIWORD(lParam);
				#ifndef ARCBALL
				//Prosty ArcBall
				TransformujPikselDoPrzestrzeniSceny(poczatkowaPozycjaKursoraMyszy,1,poprzedniaPozycjaKursoraMyszy3D);
				NormujWektor3fv(poprzedniaPozycjaKursoraMyszy3D);	
				#endif
				wynik=0;
				break;
			case WM_MOUSEMOVE: 
				if (wParam & (MK_LBUTTON | MK_RBUTTON | MK_MBUTTON))			
				{
					//POINT biezacaPozycjaKursoraMyszy=PozycjaKursoraMyszyWOknie(uchwytOkna);
					POINT biezacaPozycjaKursoraMyszy={LOWORD(lParam),HIWORD(lParam)};
					POINT przesuniecieKursoraMyszy=
						{biezacaPozycjaKursoraMyszy.x-poczatkowaPozycjaKursoraMyszy.x,
						biezacaPozycjaKursoraMyszy.y-poczatkowaPozycjaKursoraMyszy.y};
	
					if(przesuniecieKursoraMyszy.x==0 && przesuniecieKursoraMyszy.y==0) 
						break;

					if(wParam & MK_LBUTTON)
					{
						#ifdef ARCBALL
						MousePt.s.X = (GLfloat)biezacaPozycjaKursoraMyszy.x;
						MousePt.s.Y = (GLfloat)biezacaPozycjaKursoraMyszy.y;
						Quat4fT ThisQuat;					
						ArcBall->drag(&MousePt, &ThisQuat);					
						Matrix3fSetRotationFromQuat4f(&ThisRot, &ThisQuat);
						Matrix3fMulMatrix3f(&ThisRot, &LastRot);
						Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
						//swobodne obroty
						swobodneObrotyAktywne=true;
						swobodneObroty_kwaternionObrotu=ThisQuat;
						#else
						const float czuloscMyszy=5.0f;
						kameraPhi+=przesuniecieKursoraMyszy.x/czuloscMyszy;
						kameraTheta+=przesuniecieKursoraMyszy.y/czuloscMyszy;
	
						//Prosty ArcBall
						//obliczanie osi i kata obrotu
						float biezacaPozycjaKursoraMyszy3D[3];				
						TransformujPikselDoPrzestrzeniSceny(biezacaPozycjaKursoraMyszy,1,biezacaPozycjaKursoraMyszy3D);
						NormujWektor3fv(biezacaPozycjaKursoraMyszy3D);				
						float osObrotu[3];
						IloczynWektorowy3fv(poprzedniaPozycjaKursoraMyszy3D,biezacaPozycjaKursoraMyszy3D,osObrotu);
						float katObrotu=asin(DlugoscWektora3fv(osObrotu));
						const float czuloscObrotu=50.0f;
						katObrotu*=czuloscObrotu;
						NormujWektor3fv(osObrotu);				
	
						//kopiowanie wektora polozenia
						for(int i=0;i<3;i++) poprzedniaPozycjaKursoraMyszy3D[i]=biezacaPozycjaKursoraMyszy3D[i];
	
						//kumulacja obrotow w macierzy korektaMacierzyModelWidok
						glMatrixMode(GL_MODELVIEW);
						glLoadMatrixf(korektaMacierzyModelWidok);
						glRotatef(-katObrotu,osObrotu[0],osObrotu[1],osObrotu[2]);
						glGetFloatv(GL_MODELVIEW_MATRIX,korektaMacierzyModelWidok);
	
						//swobodne obroty
						Obracaj(1.0f*przesuniecieKursoraMyszy.x,1.0f*przesuniecieKursoraMyszy.y,swobodneObroty_Wygaszanie);
						#endif
					}
					if(wParam & MK_RBUTTON)
					{
						const float czuloscMyszy=75.0f;
						kameraX+=przesuniecieKursoraMyszy.x/czuloscMyszy;
						kameraY-=przesuniecieKursoraMyszy.y/czuloscMyszy;
					}
					if(wParam & MK_MBUTTON)
					{
						const float czuloscMyszy=5.0f;
						kameraCelPhi+=przesuniecieKursoraMyszy.x/czuloscMyszy;
						float zmianaCelTheta=przesuniecieKursoraMyszy.y/czuloscMyszy;
						if (fabs(kameraCelTheta+zmianaCelTheta)<90) kameraCelTheta+=zmianaCelTheta;
					}
	
					poczatkowaPozycjaKursoraMyszy.x=LOWORD(lParam);
					poczatkowaPozycjaKursoraMyszy.y=HIWORD(lParam);			
					//PozycjaKursoraMyszyWOknie(uchwytOkna,&pozycjaPoczatkowaKursoraMyszy);
					if(pauza) RysujScene();
				}
				wynik=0;
				break;		
			case WM_MOUSEWHEEL:
			{
				const float czuloscMyszy=10.0f;
				short ZmianaPozycjiRolki=(short)HIWORD(wParam);
				//zmiana odleglosci kamery od pocz. ukl. wsp.
				kameraR*=1+ZmianaPozycjiRolki/abs(ZmianaPozycjiRolki)/czuloscMyszy;
				if(pauza) RysujScene();
				wynik=0;
				break;
			}

			case WM_KEYDOWN:
				const float przesuniecie=0.1f;
				switch(wParam)
				{
					case 'W': 
					case VK_UP:					
						//kameraZ+=przesuniecie; 
						kameraZ+=cosDegf(kameraCelPhi)*przesuniecie;
						kameraX-=sinDegf(kameraCelPhi)*przesuniecie;
						break;
					case 'S':
					case VK_DOWN:
						//kameraZ-=przesuniecie;
						kameraZ-=cosDegf(kameraCelPhi)*przesuniecie;
						kameraX+=sinDegf(kameraCelPhi)*przesuniecie;
						break;
					case 'A':
					case VK_LEFT:
						//kameraX+=przesuniecie;
						kameraZ+=sinDegf(kameraCelPhi)*przesuniecie;
						kameraX+=cosDegf(kameraCelPhi)*przesuniecie;					
						break;
					case 'D':
					case VK_RIGHT:
						//kameraX-=przesuniecie;
						kameraZ-=sinDegf(kameraCelPhi)*przesuniecie;
						kameraX-=cosDegf(kameraCelPhi)*przesuniecie;					
						break;	
				}
				
				if(pauza) RysujScene();
				break;
		}
	}
	
	return wynik;
}

// ----------------- WGL -----------------

bool COknoGL::InitWGL(HWND uchwytOkna)
{
	uchwytDC = ::GetDC(uchwytOkna);
	if (!UstalFormatPikseli(uchwytDC)) return false; //Utworzenie kontekstu renderowania i uczynienie go aktywnym
	uchwytRC = wglCreateContext(uchwytDC);
	if (uchwytRC==NULL) return false;
	if (!wglMakeCurrent(uchwytDC, uchwytRC)) return false;			
	return true;
}

void COknoGL::UsunWGL()
{
	wglMakeCurrent(NULL,NULL); 
	wglDeleteContext(uchwytRC); 
	::ReleaseDC(uchwytOkna,uchwytDC);
}

bool COknoGL::UstalFormatPikseli(HDC uchwytDC) const
{ 
	PIXELFORMATDESCRIPTOR opisFormatuPikseli; 
	ZeroMemory(&opisFormatuPikseli,sizeof(opisFormatuPikseli)); 
	opisFormatuPikseli.nVersion=1; 
	opisFormatuPikseli.dwFlags=PFD_SUPPORT_OPENGL | PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER; //w oknie, podwojne buforowanie 
	opisFormatuPikseli.iPixelType=PFD_TYPE_RGBA; //typ koloru RGB 
	opisFormatuPikseli.cColorBits=32; //jakosc kolorów 4 bajty
	opisFormatuPikseli.cDepthBits=16; //glebokosc bufora Z (z-buffer) 
	opisFormatuPikseli.cStencilBits=1;
	opisFormatuPikseli.iLayerType=PFD_MAIN_PLANE; 
	int formatPikseli=ChoosePixelFormat(uchwytDC,&opisFormatuPikseli); 
	if (formatPikseli==0) return false; 
	if (!SetPixelFormat(uchwytDC,formatPikseli,&opisFormatuPikseli)) return false; 
	return true;
}

// ----------------- OpenGL -----------------

void COknoGL::UstawienieSceny(bool rzutowanieIzometryczne) 
{ 	
	glViewport(0,0,szerokoscObszaruUzytkownika,wysokoscObszaruUzytkownika); //okno OpenGL = wnetrze formy (domyslnie) 
	//glClearColor(0.0,0.0,0.0,1.0); //czarne tlo	
	//glClearColor(0.5,0.5,0.5,1.0); //szare tlo	
	glClearColor(1.0,1.0,1.0,1.0); //biale tlo	
	
	//ustawienie punktu projekcji 
	glMatrixMode(GL_PROJECTION); //prze³¹czenie na macierz projekcji
	glLoadIdentity();
	//left,right,bottom,top,znear,zfar (clipping) 
	float wsp=wysokoscObszaruUzytkownika/(float)szerokoscObszaruUzytkownika;
	if(!rzutowanieIzometryczne)
		glFrustum(-0.1, 0.1, wsp*-0.1, wsp*0.1, 0.3, 100.0); //mnozenie macierzy rzutowania przez macierz perspektywy - ustalanie frustum 	
		//gluPerspective(RadToDeg(2*atan(wsp*0.1/0.3)),1/wsp,0.3,100.0);
	else
		glOrtho(-3, 3, wsp*-3, wsp*3, 0.3, 100.0); //rzutowanie rownolegle
	//glScalef(1,-1,1); //do góry-nogami
	glMatrixMode(GL_MODELVIEW); //powrót do macierzy widoku modelu 
	glEnable(GL_DEPTH_TEST); //z-buffer aktywny = ukrywanie niewidocznych powierzchni 	
	glDepthFunc(GL_LEQUAL);

	#ifdef ARCBALL
	ArcBall->setBounds((float)szerokoscObszaruUzytkownika,(float)wysokoscObszaruUzytkownika);
	#endif

	Oswietlenie();
}

void COknoGL::UstawienieKamery()
{
	glLoadIdentity();
	glRotatef(kameraCelPhi,0,1,0);
	glRotatef(kameraCelTheta,cosDegf(kameraCelPhi),0,sinDegf(kameraCelPhi));		
	glTranslatef(kameraX,kameraY,kameraZ);

	glTranslatef(0,0,-kameraR);

	//gluLookAt(-kameraX,-kameraY,-kameraZ+kameraR, 
	//		  -kameraX+kameraR*sinDegf(kameraCelPhi),-kameraY-kameraR*sinDegf(kameraCelTheta),-kameraZ,
	//	      0,1,0);

	//wykonanie transformacji
	#ifndef ARCBALL
	//TPP na dwoch katach Eulera
	glRotatef(kameraPhi,0,1,0);
	glRotatef(kameraTheta,cosDegf(kameraPhi),0,sinDegf(kameraPhi));	
	//Prosty ArcBall
	//glMultMatrixf(korektaMacierzyModelWidok);
	#else
	glMultMatrixf(Transform.M);
	#endif
	
	//obliczenie polozenia kamery
	#ifndef ARCBALL
	polozenieKamery[0]=-kameraX-kameraR*sinDegf(-kameraPhi)*cosDegf(kameraTheta);
	polozenieKamery[1]=-kameraY-kameraR*sinDegf(kameraTheta);
	polozenieKamery[2]=-kameraZ-kameraR*cosDegf(kameraTheta)*cosDegf(-kameraPhi);
	#else
	polozenieKamery[0]=-kameraX-Transform.s.XZ*kameraR;
	polozenieKamery[1]=-kameraY-Transform.s.YZ*kameraR;
	polozenieKamery[2]=-kameraZ-Transform.s.ZZ*kameraR;
	#endif

	/*
	glLoadIdentity();
	gluLookAt(-polozenieKamery[0],-polozenieKamery[1],-polozenieKamery[2],
		0,0,0,
		0,1,0);
	*/
}

float* COknoGL::PolozenieKamery(float* bufor) const
{	
	for(int i=0;i<3;i++) bufor[i]=polozenieKamery[i];
	return bufor;
}

void COknoGL::RysujScene() 
{ 	
	//Przygotowanie bufora 
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); //czysci bufory 
	glLoadIdentity(); //macierz model-widok = macierz jednostkowa 

	WyswietlCzestoscRenderowania();

	UstawienieKamery();
	RysujAktorow();

	//Z bufora na ekran 
	SwapBuffers(uchwytDC); 
}

void COknoGL::WyswietlCzestoscRenderowania()
{		
	static unsigned long staraIloscTykniec = GetTickCount();;
	unsigned long nowaIloscTykniec = GetTickCount();
	if(nowaIloscTykniec==staraIloscTykniec) return;
	double f=1E3/(nowaIloscTykniec-staraIloscTykniec);
	f=floor(10.0*f)/10.0;
	staraIloscTykniec=nowaIloscTykniec;

	char bufor[256];
	SetWindowText(uchwytOkna,strcat(_gcvt(f,10,bufor),"Hz"));
}

//oswietlenie
void COknoGL::Oswietlenie()
{
	glEnable(GL_LIGHTING); //wlaczenie systemu oswietlania
	
	//swiatlo tla			
	const float kolor_tla[]={natezenie_swiatla_tla,
                             natezenie_swiatla_tla,
                             natezenie_swiatla_tla};
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT,kolor_tla);

	//material
	glEnable(GL_COLOR_MATERIAL);
	glColorMaterial(GL_FRONT_AND_BACK,GL_AMBIENT_AND_DIFFUSE);

	//zrodla swiatla
	glPushMatrix();
	glLoadIdentity();
	ZrodlaSwiatla();
	glPopMatrix();

	//mieszanie kolorow
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
}

/*static*/ void COknoGL::GladkiMaterial(bool czyGladki)
{
	const bool separacjaRozblysku=true;

	//stale uzywane przy separacji rozblysku	
	const int GL_LIGHT_MODEL_COLOR_CONTROL=0x81F8;
	const int GL_SINGLE_COLOR=0x81F9,GL_COLOR_SIMPLE=0x81F9;
	const int GL_SEPARATE_SPECULAR_COLOR=0x81FA;
	
	if(czyGladki)
	{
		//wlaczenie gladkiego materialu
		const float wsp_odbicia_szklo[4]={1.0,1.0,1.0,1.0};	
		glMaterialfv(GL_FRONT,GL_SPECULAR,wsp_odbicia_szklo);
		glMateriali(GL_FRONT,GL_SHININESS,30);
		//konieczne, aby tekstury nie psuly rozblysku
		if(separacjaRozblysku) glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SEPARATE_SPECULAR_COLOR); 
	}
	else
	{
		//wylaczenie gladkiego materialu
		const float wsp_odbicia_matowy[4]={0.0,0.0,0.0,1.0};
		//wylaczenie trybu separacji rozblysku dla tekstur
		if(separacjaRozblysku) glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL,GL_SINGLE_COLOR);
		glMaterialfv(GL_FRONT,GL_SPECULAR,wsp_odbicia_matowy);
		glMateriali(GL_FRONT,GL_SHININESS,0);
	}
}

#ifdef ARCBALL
void COknoGL::InicjujArcBall()
{
	Matrix3fSetIdentity(&LastRot);
	Matrix3fSetIdentity(&ThisRot);
    //Matrix4fSetRotationFromMatrix3f(&Transform, &ThisRot);
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			Transform.M[i+4*j]=(i==j)?1.0f:0.0f;
	ArcBall=new ArcBallT(640.0f, 480.0f);	
}

COknoGL::~COknoGL()
{
	delete ArcBall;
}
#endif

//swobodne obroty
#ifdef ARCBALL
void COknoGL::Obracaj(float kat,float x,float y,float z,bool wygaszanie) //por. argumenty glRoatatef
{
	float s=sinDegf(kat/2.0f);
	float c=cosDegf(kat/2.0f);
	swobodneObroty_kwaternionObrotu.s.W=c;
	swobodneObroty_kwaternionObrotu.s.X=s*x;
	swobodneObroty_kwaternionObrotu.s.Y=s*y;
	swobodneObroty_kwaternionObrotu.s.Z=s*z;
	swobodneObroty_Wygaszanie=wygaszanie;
	swobodneObrotyAktywne=true;
}
#else
void COknoGL::Obracaj(float szybkoscPhi,float szybkoscTheta,bool wygaszanie)
{	
	swobodneObroty_szybkoscPhi=szybkoscPhi;
	swobodneObroty_szybkoscTheta=szybkoscTheta;
	swobodneObroty_Wygaszanie=wygaszanie;
	swobodneObrotyAktywne=true;
}
#endif

#pragma endregion