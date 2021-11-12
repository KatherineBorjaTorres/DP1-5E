
// rnaView.cpp: implementación de la clase CrnaView
//

#include "stdafx.h"
// Se pueden definir SHARED_HANDLERS en un proyecto ATL implementando controladores de vista previa, miniatura
// y filtro de búsqueda, y permiten compartir código de documentos con ese proyecto.
#ifndef SHARED_HANDLERS
#include "rna.h"
#endif

#include "rnaDoc.h"
#include "rnaView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


struct pila 
	{ 
		int x,y;
	}; 

FILE *r, *arna, *pal;

//unsigned int dato[1200][1200];
int e[100];
unsigned int sred[20]; 
struct neurona{
	unsigned int m;
	float peso[3000];
	float n;
	float a;
	float u;
	float k;
	unsigned int capa;
};

//struct neurona rna[10000];

struct nodo{
	unsigned int de;
	unsigned int iz;
	unsigned int ar;
	unsigned int ab;
	float par;
	float piz;
	float pde;
	float pab;
};

/*
EStado de los camiones:

1 - Disponible;
2 - Entregando
3 - Retornando
4 - Mantenimiento

*/

struct camion{
	CString codigo;
	float peso_bruto_tara;
	float peso_carga_GLP;
	float carga_GLP;
	float capacidad_carga_GLP;
	float peso_combinado;
	float capacidad_petroleo;
	float cantidad_petroleo;
	float consumo_petroleo;
	int cantidad_pedidos_atendidos;
	int recorrido_total;
	int posicion_x;
	int posicion_y;
	int longitud_ruta;
	int posicion_ruta;
	int estado;
};

struct posicion{
	int x;
	int y;
};

struct planta{
	int x;
	int y;
};

/*
1 - Sin atender
2 - En entrega
3 - Entregado
*/

struct pedido{
	int mes;
	int dia;
	int hora;
	int min;
	float tiempo_min;
	float cantidad;
	int x;
	int y;
	int plazo;
	int estado;
	int id;
};

struct nodo mapa[70][50]; //*
struct nodo graf[70][50];
struct posicion ruta[5000][100];  //*
struct camion flota[25];         //*
struct camion cola[100][10]; 
struct planta locales[10];      //*
struct pedido listaPedidos[1000]; //*
struct posicion rutaOptima[5000];
int longCola[10];

int rutaX[100];
int rutaY[100];


//struct dato vects[400];



// CrnaView

IMPLEMENT_DYNCREATE(CrnaView, CView)

BEGIN_MESSAGE_MAP(CrnaView, CView)

	ON_COMMAND(ID_LEXICO_APRENDIZAJE, OnLexicoAprendizaje)
	ON_COMMAND(ID_LEXICO_FUNCIONAMIENTO, OnLexicoFuncionamiento)

	// Comandos de impresión estándar
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	END_MESSAGE_MAP()

// Construcción o destrucción de CrnaView

CrnaView::CrnaView()
{
	// TODO: agregar aquí el código de construcción

}

CrnaView::~CrnaView()
{
}

BOOL CrnaView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: modificar aquí la clase Window o los estilos cambiando
	//  CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

BOOL Activar=FALSE;
BOOL ActivarF=FALSE;
// Dibujo de CrnaView


//double pmax=1000;
//double fx=50000000;
//double fx=8000000;


/***********  Datos maximos para cada variable del sistema ****************************/

double dmax=1000;
double wmax=500;
double imax=30;

double tmax=1;

void coordenadas(CDC* pDC,int Xc,int Yc,int lx,int ly,float my, float mx,CString titulo);
void salto(FILE *fp, int num_bytes);
float CalculoRed(int nred);
/** Rutas **/
void aprendizaje(CDC* pDC)
{
	/*** Colores para graficar ***/
	CPen myPen1;
	CPen* pOldPen1;
	CPen myPen2;
	CPen* pOldPen2;
	
	
	myPen1.CreatePen(PS_SOLID,2,RGB(0,0,255)); // crear el pincel lineas punteadas
	myPen2.CreatePen(PS_SOLID,2,RGB(0,100,0)); // crear el pincel lineas punteadas

	CString mensaje;
	char c=92;
	int Xmax=1600;
	int Ymax=1200;

	int xc,yc;
	xc=20;
	yc=600;


	mensaje="Mapa.";
	pDC->TextOutW(600,30,mensaje);

	/**   Creacion de mapa  */

	int x = 70;
	int y = 50;
	for (int i=0; i<x; i++){
		for (int j=0; j<y; j++){
			if (i<x){
				mapa[i][j].de=i+1;
			}
			else{
				mapa[i][j].de=0;
			}
			if (i>0){
				mapa[i][j].iz=i-1;
			}
			else{
				mapa[i][j].iz=0;
			}
			if (j<y){
				mapa[i][j].ar=j+1;
			}
			else{
				mapa[i][j].ar=0;
			}
			if (j>0){
				mapa[i][j].ab=j-1;
			}
			else{
				mapa[i][j].ab=0;
			}
			mapa[i][j].pde=1;
			mapa[i][j].piz=1;
			mapa[i][j].par=1;
			mapa[i][j].pab=1;
		}
	}
	int flag=0;
	int iyt=13;
	int iyc=3;
	int iym=10;
	int ixt=13;
	int ixc=3;
	int ixm=10;
	y=45;
	for (int i=0; i<x; i++){
		for (int j=0; j<y; j++){
			for (int c=0; c<3; c++){
				for (int b=0; b<10; b++){
					pDC->SetPixel(xc+i*ixt+b,yc-j*iyt-c,RGB(200,200,200));
				}
			}
			for (int c=0; c<10; c++){
				for (int b=0; b<10; b++){
					pDC->SetPixel(xc+i*ixt+b,yc-j*iyt-iyc-c,RGB(0,0,0));
				}
			}
		}
	}
	int xi,yi,xf,yf, xg, yg, xga, yga;

	/*Inicio*/
	xi=10; yi=30;
	/*Fin*/
	xf=50; yf=10;
	xg=(xi-1)*ixt+ixm;
	yg=(yi-1)*iyt+iym;
	xga=xg;
	yga=yg;
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			pDC->SetPixel(xc+xg-2+i,yc-yg-2-j,RGB(255,0,0));
		}
	}
	xg=(xf-1)*ixt+ixm;
	yg=(yf-1)*iyt+iym;
	for (int i=0; i<5; i++){
		for (int j=0; j<5; j++){
			pDC->SetPixel(xc+xg-2+i,yc-yg-2-j,RGB(255,0,0));
		}
	}

	int ix, iy;
	int ir=0, flaginc;
	ix=xi;
	iy=yi;
	flaginc=0;
	rutaX[ir]=xi;
	rutaY[ir]=yi;
	ir++;
	pOldPen1=pDC->SelectObject(&myPen1);
	do{
		if (ix<xf){
			ix=mapa[ix][iy].de;
			flaginc=1;
		}
		if (ix>xf){
			ix=mapa[ix][iy].iz;
			flaginc=1;
		}
		rutaX[ir]=ix;
		rutaY[ir]=iy;
		xg=(ix-1)*ixt+ixm;
		yg=(iy-1)*iyt+iym;
		pDC->MoveTo(xc+xga+2,yc-yga-3);
		pDC->LineTo(xc+xg+2,yc-yg-3);
		xga=xg;
		yga=yg;
		ir++;
		if (iy<yf){
			iy=mapa[ix][iy].ar;
			flaginc=1;
		}
		if (iy>yf){
			iy=mapa[ix][iy].ab;
			flaginc=1;
		}
		rutaX[ir]=ix;
		rutaY[ir]=iy;
		xg=(ix-1)*ixt+ixm;
		yg=(iy-1)*iyt+iym;
		pDC->MoveTo(xc+xga+2,yc-yga-3);
		pDC->LineTo(xc+xg+2,yc-yg-3);
		xga=xg;
		yga=yg;
		ir++;
	}while ((ix!=xf) | (iy!=yf)); 
	mensaje.Format(_T("Inicio : (%d , %d)  Fin : (%d , %d)"),xi,yi,xf,yf);
	pDC->TextOutW(20,600,mensaje);
	mensaje="Ruta : ";
	pDC->TextOutW(20,620,mensaje);
	for (int i=0; i<ir; i++){
		mensaje.Format(_T("(%d , %d)"),rutaX[i],rutaY[i]);
		pDC->TextOutW(60+i*60,620,mensaje);
	}
}

/** Dijkstra **/
void funcionamiento(CDC* pDC)
{
	/*** Colores para graficar ***/
	CPen myPen1;
	CPen* pOldPen1;
	CPen myPen2;
	CPen* pOldPen2;
	CPen myPen3;
	CPen* pOldPen3;
	CPen myPen4;
	CPen* pOldPen4;
	CPen myPen5;
	CPen* pOldPen5;
	
	
	myPen1.CreatePen(PS_SOLID,2,RGB(155,8,44)); // crear el pincel lineas punteadas
	myPen2.CreatePen(PS_SOLID,2,RGB(19,155,8)); // crear el pincel lineas punteadas
	myPen3.CreatePen(PS_SOLID,2,RGB(255,255,255)); // crear el pincel lineas punteadas
	myPen4.CreatePen(PS_SOLID,2,RGB(8,155,137)); // crear el pincel lineas punteadas
	myPen5.CreatePen(PS_SOLID,2,RGB(249,20,124)); // crear el pincel lineas punteadas

	CString mensaje;
	char c=92;
	int Xmax=1600;
	int Ymax=1200;

	int xc,yc;
	xc=20;
	yc=640;


	//mensaje="Mapa.";
	//pDC->TextOutW(600,30,mensaje);

	/**   Creacion de mapa  */

	int x = 70;
	int y = 50;
	for (int i=0; i<x; i++){
		for (int j=0; j<y; j++){
			if (i<x){
				mapa[i][j].de=i+1;
			}
			else{
				mapa[i][j].de=0;
			}
			if (i>0){
				mapa[i][j].iz=i-1;
			}
			else{
				mapa[i][j].iz=0;
			}
			if (j<y){
				mapa[i][j].ar=j+1;
			}
			else{
				mapa[i][j].ar=0;
			}
			if (j>0){
				mapa[i][j].ab=j-1;
			}
			else{
				mapa[i][j].ab=0;
			}
			mapa[i][j].pde=1;
			mapa[i][j].piz=1;
			mapa[i][j].par=1;
			mapa[i][j].pab=1;
		}
	}
	int flag=0;
	int iyt=13;
	int iyc=3;
	int iym=10;
	int ixt=13;
	int ixc=3;
	int ixm=10;
	//y=45;
	for (int i=0; i<x; i++){
		for (int j=0; j<y; j++){
			for (int c=0; c<10; c++){
				for (int b=0; b<10; b++){
					pDC->SetPixel(xc+i*ixt+b,yc-j*iyt-iyc-c,RGB(245,245,79));
				}
			}
		}
	}

	/*  Inicializacion de red de distribución   */

	/*  Local 0 - Principal*/

	int nl=3;            //- Cantidad de locales
	//int lc0, lc1, lc2; // - Longitud de la las colas 0, 1 y 2
	int nc=20;           // - Cantidad de camiones
	int np=0;		     // - Numero de pedidos
	float vel=50;        // - Velocidad de los camiones K/h;
	float tk = 60/vel;   // - Tiempo en recorrer un kilometro.
	float reloj = 0;
	float ts    = 1*24*60; // - Tiempo de simulacón.

	locales[0].x=12;
	locales[0].y=8;
	locales[1].x=42;
	locales[1].y=42;
	locales[2].x=63;
	locales[2].y=3;

	int xi,yi,xf,yf, xg, yg, xga, yga;

	for (int k=0; k<nl; k++){
		xg=(locales[k].x-1)*ixt+ixm;
		yg=(locales[k].y-1)*iyt+iym;
		xga=xg;
		yga=yg;
		for (int i=0; i<7; i++){
			for (int j=0; j<7; j++){
				if (k==0){
					//pDC->SetPixel(xc+xg-6+i,yc-yg-6-j,RGB(30,152,20));
					pDC->SetPixel(xc+xg-6+i,yc-yg-6-j,RGB(255,0,0));
				}
				else{
					pDC->SetPixel(xc+xg-6+i,yc-yg-6-j,RGB(11,104,104));
				}
			}
		}
	}

	/*   Flota    */
	int coc=0;
	for (int i=0; i<2; i++){
		flota[i].codigo.Format(_T("TA%02d"),coc+1);
		flota[i].peso_bruto_tara=2.5;
		flota[i].carga_GLP=25;
		flota[i].capacidad_carga_GLP=25;
		flota[i].peso_carga_GLP=12.5;
		flota[i].peso_combinado=15; 
		flota[i].estado=1;
		flota[i].posicion_x = 12;
		flota[i].posicion_y = 8;
		flota[i].cantidad_petroleo = 25;
		flota[i].capacidad_petroleo = 25;
		flota[i].longitud_ruta = 0;
		flota[i].posicion_ruta = 0;
		flota[i].cantidad_pedidos_atendidos = 0;
		flota[i].consumo_petroleo = 0;
		flota[i].recorrido_total = 0;
		pDC->TextOutW(940,20*i,flota[i].codigo);
		coc++;
	}
	coc=0;
	for (int i=2; i<6; i++){
		flota[i].codigo.Format(_T("TB%02d"),coc+1);
		flota[i].peso_bruto_tara=2.0;
		flota[i].carga_GLP=15;
		flota[i].capacidad_carga_GLP=15;
		flota[i].peso_carga_GLP=7.5;
		flota[i].peso_combinado=9.5;
		flota[i].estado=1;
		flota[i].posicion_x = 12;
		flota[i].posicion_y = 8;
		flota[i].cantidad_petroleo = 25;
		flota[i].capacidad_petroleo = 25;
		flota[i].longitud_ruta = 0;
		flota[i].posicion_ruta = 0;
		flota[i].cantidad_pedidos_atendidos = 0;
		flota[i].consumo_petroleo = 0;
		flota[i].recorrido_total = 0;
		pDC->TextOutW(940,20*i,flota[i].codigo);
		coc++;
	}
	coc=0;
	for (int i=6; i<10; i++){
		flota[i].codigo.Format(_T("TC%02d"),coc+1);
		flota[i].peso_bruto_tara=1.5;
		flota[i].carga_GLP=10;
		flota[i].capacidad_carga_GLP=10;
		flota[i].peso_carga_GLP=5.0;
		flota[i].peso_combinado=6.5;
		flota[i].estado=1;
		flota[i].posicion_x = 12;
		flota[i].posicion_y = 8;
		flota[i].cantidad_petroleo = 25;
		flota[i].capacidad_petroleo = 25;
		flota[i].longitud_ruta = 0;
		flota[i].posicion_ruta = 0;
		flota[i].cantidad_pedidos_atendidos = 0;
		flota[i].consumo_petroleo = 0;
		flota[i].recorrido_total = 0;
		pDC->TextOutW(940,20*i,flota[i].codigo);
		coc++;
	}
	coc=0;
	for (int i=10; i<20; i++){
		flota[i].codigo.Format(_T("TD%02d"),coc+1);
		flota[i].peso_bruto_tara=1;
		flota[i].carga_GLP=5;
		flota[i].capacidad_carga_GLP=5;
		flota[i].peso_carga_GLP=2.5;
		flota[i].peso_combinado=3.5;
		flota[i].estado=1;
		flota[i].posicion_x = 12;
		flota[i].posicion_y = 8;
		flota[i].cantidad_petroleo = 25;
		flota[i].capacidad_petroleo = 25;
		flota[i].longitud_ruta = 0;
		flota[i].posicion_ruta = 0;
		flota[i].cantidad_pedidos_atendidos = 0;
		flota[i].consumo_petroleo = 0;
		flota[i].recorrido_total = 0;
		pDC->TextOutW(940,20*i,flota[i].codigo);
		coc++;
	}

	/* Inicializa cola de local principal con todos los camiones */

	/*  Lista Pedidos  */

	np=3;


	float tinip=0;

	for (int i=0; i<1000; i++){
		listaPedidos[i].mes      = 10;
		listaPedidos[i].dia      = 5;
		listaPedidos[i].hora     = 8;
		listaPedidos[i].min      = 5;
		listaPedidos[i].cantidad = rand() % 25 + 1;
		listaPedidos[i].x        = rand() % 69 + 1;
		listaPedidos[i].y        = rand() % 49 + 1;
		listaPedidos[i].plazo    = 240;
		listaPedidos[i].estado   = 1;
		listaPedidos[i].id       = i;
		listaPedidos[i].tiempo_min = tinip + rand() % 240 + 1;
		tinip = listaPedidos[i].tiempo_min;
	}

	int ix, iy;
	int ir, nr, flaginc,longRutaD,longRutaC,longRutaV;
	float longRutaOptima=30000;
	float consumoRuta=0;
	int CantiPedidos=1000;
	int iro=0;
	//int indPedidos=0;
	longRutaV=0;
	nr=0;
	int flagAsignaCamion = 0;
	
	/*          Inicio   algoritmo                */

	do{
		//if (CantiPedidos>0 & reloj>listaPedidos[indPedidos].tiempo_min) {
		for (int ipp=0; ipp<CantiPedidos; ipp++){
			//CantiPedidos--;
			if (listaPedidos[ipp].estado==1){
				xf=listaPedidos[ipp].x; 
				yf=listaPedidos[ipp].y;
				for (int icl=0; icl<nc; icl++){
					if (flota[icl].estado==1 & flota[icl].capacidad_carga_GLP>=listaPedidos[ipp].cantidad){
						xi=flota[icl].posicion_x; 
						yi=flota[icl].posicion_y;
						xg=(xi-1)*ixt+ixm;
						yg=(yi-1)*iyt+iym;
						xga=xg;
						yga=yg;
						for (int i=0; i<5; i++){
							for (int j=0; j<5; j++){
								pDC->SetPixel(xc+xg-2+i,yc-yg-2-j,RGB(255,0,0));
							}
						}
						ix=xi;
						iy=yi;
						ir=0;
						longRutaD=0;
						nr=icl;
						flaginc=0;
						ruta[ir][nr].x = xi;
						ruta[ir][nr].y = yi;
						ir++;
						pOldPen1=pDC->SelectObject(&myPen1);
						/*  Algoritmo de busqueda de ruta mas corta   */
						do{
							if (ix<xf){
								ix=mapa[ix][iy].de;
								longRutaD++;
								flaginc=1;
							}
							if (ix>xf){
								ix=mapa[ix][iy].iz;
								longRutaD++;
								flaginc=1;
							}
							if (flaginc==1){
								ruta[ir][nr].x = ix;
								ruta[ir][nr].y = iy;
								ir++;
								flaginc=0;
							}
							if (iy<yf){
								iy=mapa[ix][iy].ar;
								longRutaD++;
								flaginc=1;
							}
							if (iy>yf){
								iy=mapa[ix][iy].ab;
								longRutaD++;
								flaginc=1;
							}
							if (flaginc==1){
								ruta[ir][nr].x = ix;
								ruta[ir][nr].y = iy;
								ir++;
								flaginc=0;
							}
						}while ((ix!=xf) | (iy!=yf)); 

						/*   Fin de algoritmo    */

						longRutaV=ir-1;
						mensaje.Format(_T("%d"),longRutaD);
						pDC->TextOutW(1000,nr*20,mensaje);
						if (longRutaOptima>longRutaD){
							longRutaOptima=longRutaD;
							longRutaC=ir;
							for (int i=0; i<ir; i++){
								rutaOptima[i].x=ruta[i][nr].x;
								rutaOptima[i].y=ruta[i][nr].y;
							}
						}
					
						mensaje.Format(_T("%d"),longRutaV);
						pDC->TextOutW(1045,nr*20,mensaje);

						consumoRuta = longRutaV*(flota[icl].peso_bruto_tara+listaPedidos[0].cantidad*0.5)/150;
						mensaje.Format(_T("%0.2f"),consumoRuta);
						pDC->TextOutW(1085,nr*20,mensaje);

						mensaje.Format(_T("%0.2f"),flota[icl].capacidad_carga_GLP);
						pDC->TextOutW(1130,nr*20,mensaje);
						
						//indPedidos++;
						flagAsignaCamion = 1;
					}
				}
				if (flagAsignaCamion==1){
					xg=(xf-1)*ixt+ixm;
					yg=(yf-1)*iyt+iym;
					for (int i=0; i<5; i++){
						for (int j=0; j<5; j++){
							pDC->SetPixel(xc+xg-2+i,yc-yg-2-j,RGB(255,0,0));
						}
					}
					listaPedidos[ipp].estado=2;
					flota[nr].estado=2;
					flota[nr].cantidad_pedidos_atendidos++;
					flota[nr].longitud_ruta = longRutaV;
					flota[nr].carga_GLP = listaPedidos[ipp].cantidad;
					flagAsignaCamion=0;
				}
			}
		}
		/*mensaje.Format(_T("longRutaOptima : %d"),longRutaV);
		pDC->TextOutW(1000,500,mensaje);
		mensaje.Format(_T("iro : %d"),iro);
		pDC->TextOutW(1000,530,mensaje);
		mensaje.Format(_T("Numero camion : %d"),nr);
		pDC->TextOutW(1000,560,mensaje);*/
		

		/*****   Graficar desplazamientos de camiones        ******/
		int igg=0;
		for (int i=0; i<nc; i++){
			if (flota[i].estado==2){
				xga=(flota[i].posicion_x-1)*ixt+ixm;
				yga=(flota[i].posicion_y-1)*iyt+iym;
				if (flota[i].posicion_ruta<flota[i].longitud_ruta){
					pOldPen1=pDC->SelectObject(&myPen3);
					if (flota[i].posicion_ruta>0){
						xg=(ruta[flota[i].posicion_ruta-1][i].x-1)*ixt+ixm;
						yg=(ruta[flota[i].posicion_ruta-1][i].y-1)*iyt+iym;
						pDC->MoveTo(xc+xga+2,yc-yga-3);
						pDC->LineTo(xc+xg+2,yc-yg-3);
					}
					flota[i].posicion_ruta++;
					if (i<2){
						pOldPen1=pDC->SelectObject(&myPen1);
					}
					if (i>1 & i<6){
						pOldPen2=pDC->SelectObject(&myPen2);
					}
					if (i>5 & i<10){
						pOldPen4=pDC->SelectObject(&myPen4);
					}
					if (i>9){
						pOldPen5=pDC->SelectObject(&myPen5);
					}
					xg=(ruta[flota[i].posicion_ruta][i].x-1)*ixt+ixm;
					yg=(ruta[flota[i].posicion_ruta][i].y-1)*iyt+iym;
					pDC->MoveTo(xc+xga+2,yc-yga-3);
					pDC->LineTo(xc+xg+2,yc-yg-3);
					flota[i].posicion_x = ruta[flota[i].posicion_ruta][i].x;
					flota[i].posicion_y = ruta[flota[i].posicion_ruta][i].y;
					flota[i].consumo_petroleo = flota[i].consumo_petroleo + (flota[i].peso_bruto_tara + flota[i].carga_GLP*0.5)/150;
					flota[i].recorrido_total = flota[i].recorrido_total++;
					flota[i].cantidad_petroleo = flota[i].cantidad_petroleo - (flota[i].peso_bruto_tara + flota[i].carga_GLP*0.5)/150;
				}
				else{
					flota[i].estado=3;
				}
			}
			if (flota[i].estado==3){
				xga=(flota[i].posicion_x-1)*ixt+ixm;
				yga=(flota[i].posicion_y-1)*iyt+iym;
				if (flota[i].posicion_ruta>0){
					pOldPen1=pDC->SelectObject(&myPen3);
					if (flota[i].posicion_ruta<flota[i].longitud_ruta){
						xg=(ruta[flota[i].posicion_ruta+1][i].x-1)*ixt+ixm;
						yg=(ruta[flota[i].posicion_ruta+1][i].y-1)*iyt+iym;
						pDC->MoveTo(xc+xga+2,yc-yga-3);
						pDC->LineTo(xc+xg+2,yc-yg-3);
					}
					flota[i].posicion_ruta--;
					if (i<2){
						pOldPen1=pDC->SelectObject(&myPen1);
					}
					if (i>1 & i<6){
						pOldPen2=pDC->SelectObject(&myPen2);
					}
					if (i>5 & i<10){
						pOldPen4=pDC->SelectObject(&myPen4);
					}
					if (i>9){
						pOldPen5=pDC->SelectObject(&myPen5);
					}
					xg=(ruta[flota[i].posicion_ruta][i].x-1)*ixt+ixm;
					yg=(ruta[flota[i].posicion_ruta][i].y-1)*iyt+iym;
					pDC->MoveTo(xc+xga+2,yc-yga-3);
					pDC->LineTo(xc+xg+2,yc-yg-3);
					flota[i].posicion_x = ruta[flota[i].posicion_ruta][i].x;
					flota[i].posicion_y = ruta[flota[i].posicion_ruta][i].y;
					flota[i].consumo_petroleo = flota[i].consumo_petroleo + (flota[i].peso_bruto_tara + flota[i].carga_GLP*0.5)/150;
					flota[i].recorrido_total = flota[i].recorrido_total++;
					flota[i].cantidad_petroleo = flota[i].cantidad_petroleo - (flota[i].peso_bruto_tara + flota[i].carga_GLP*0.5)/150;
				}
				else{
					flota[i].estado=1;
				}
			}
		
			igg++;
			mensaje.Format(_T("Cam. : %d Est. : %d C :%0.2f"),i,flota[i].estado,flota[i].carga_GLP);
			pDC->TextOutW(1170,20*(igg-1),mensaje);
		}
		reloj = reloj + tk;
		mensaje.Format(_T("Reloj : %0.2f min"),reloj);
		pDC->TextOutW(20,600,mensaje);
		int pedEntre = 0;
		int pedPendi = 0;
		int pedRetra = 0;
		float tpromep  = 0;
		float totalEntre = 0;
		float totalPetro = 0;
		float totalRecor = 0;
		for (int i=0; i<CantiPedidos; i++){
			if (listaPedidos[i].estado==1){
				pedPendi++;
			}
			if (listaPedidos[i].estado==2){
				pedEntre++;
				totalEntre = totalEntre + listaPedidos[i].cantidad;
			}
			if ((listaPedidos[i].tiempo_min+listaPedidos[i].plazo)<reloj){
				pedRetra++;
			}
			if (i<(CantiPedidos-1)){
				tpromep = tpromep + (listaPedidos[i+1].tiempo_min - listaPedidos[i].tiempo_min);
			}
		}
		
		for (int i=0; i<nc; i++){
			totalPetro = totalPetro + flota[i].consumo_petroleo;
			totalRecor = totalRecor + flota[i].recorrido_total;
		}
		mensaje.Format(_T("Total pedidos : %d"),CantiPedidos);
		pDC->TextOutW(1000,400,mensaje);
		mensaje.Format(_T("Pedidos entr. : %d"),pedEntre);
		pDC->TextOutW(1000,420,mensaje);
		mensaje.Format(_T("Pedidos pend. : %d"),pedPendi);
		pDC->TextOutW(1000,440,mensaje);
		mensaje.Format(_T("Pedidos retr. : %d"),pedRetra);
		pDC->TextOutW(1000,460,mensaje);
		mensaje.Format(_T("Tiempo prome. : %0.2f min."),tpromep/CantiPedidos);
		pDC->TextOutW(1000,480,mensaje);
		mensaje.Format(_T("Total entre. : %0.2f metros cúbicos."),totalEntre);
		pDC->TextOutW(1000,500,mensaje);
		mensaje.Format(_T("Total petro. : %0.2f Gl."),totalPetro);
		pDC->TextOutW(1000,520,mensaje);
		mensaje.Format(_T("Total recor. : %0.2f Km."),totalRecor);
		pDC->TextOutW(1000,540,mensaje);

		Sleep(100);
	}while (reloj<ts);
}


float CalculoRed(int nred){

	return 0.00; 
	
}

void salto(FILE *fp, int num_bytes)
{
	
}

void coordenadas(CDC* pDC,int Xc,int Yc,int lx,int ly,float my, float mx, CString titulo)
{
	CPen myPen;
	CPen* pOldPen;
	CFont myFont;
	CFont* pOldFont;
	int i,dx,dy;
   float ix,iy;
   CString mensaje,tex;
   ix=mx/10;
   iy=my/5;
   dx=lx/10;
   dy=ly/5;
   //settextstyle(2,0,1);
   //setusercharsize(2,1,2,1);
   myFont.CreateFont(12,0,0,0,0,FALSE,FALSE,FALSE,0,0,0,0,0,0);
   pOldFont=pDC->SelectObject(&myFont);
   //pDC->TextOut(Xc+lx/4-30,Yc-ly,titulo);
   //setusercharsize(1,1,1,1);
   mensaje="seg.";
   pDC->TextOut(Xc+lx+5,Yc-15,mensaje);
   //setusercharsize(2,3,2,3);
   myPen.CreatePen(PS_DOT,1,RGB(0,0,255)); // crear el pincel lineas punteadas
   pOldPen=pDC->SelectObject(&myPen);
   for (i = 1; i <= 5; i++)
   {
	//line(Xc,Yc+dy*i,Xc+2,Yc+dy*i);
	pDC->MoveTo(Xc,Yc+dy*i);
    pDC->LineTo(Xc+5,Yc+dy*i);
	tex.Format(_T("%6.2f"),iy*i);
	pDC->TextOut(Xc+3,Yc+dy*i-5,tex);
   }
   for (i = 1; i <= 5; i++)
   {
	//line(Xc,Yc-dy*i,Xc+2,Yc-dy*i);
	pDC->MoveTo(Xc,Yc-dy*i);
    pDC->LineTo(Xc+5,Yc-dy*i);
	tex.Format(_T("%6.2f"),iy*i);
	//sprintf_s(tex,"%6.2f",iy*i);
	pDC->TextOut(Xc+3,Yc-dy*i-5,tex);
   }
   for (i = 1; i <= 10; i++)
   {
	//line(Xc+dx*i,Yc,Xc+dx*i,Yc+2);
	pDC->MoveTo(Xc+dx*i,Yc);
    pDC->LineTo(Xc+dx*i,Yc+5);
	tex.Format(_T("%6.2f"),ix*i);
	//sprintf_s(tex,"%4.2f",ix*i);
	pDC->TextOut(Xc+dx*i-3,Yc+5,tex);
   }
  // setbkcolor(15);
  pDC->SelectObject(pOldPen);
  myPen.DeleteObject();
  pDC->SelectObject(pOldFont);
  myFont.DeleteObject();
	//esto grafica la data
	myPen.CreatePen(PS_SOLID,1,RGB(0,0,255));
	pOldPen=pDC->SelectObject(&myPen);
	pDC->MoveTo(Xc,Yc);
  // setcolor(1);
    //line(Xc,Yc-ly,Xc,Yc+ly);
   pDC->MoveTo(Xc,Yc-ly);
   pDC->LineTo(Xc,Yc+ly);
   //line(Xc,Yc,Xc+lx,Yc);
   pDC->MoveTo(Xc,Yc);
   pDC->LineTo(Xc+lx,Yc);
   //line(Xc,Yc-ly,Xc-2,Yc-ly+6);
   pDC->MoveTo(Xc,Yc-ly);
   pDC->LineTo(Xc-2,Yc-ly+6);
   //line(Xc,Yc-ly,Xc+2,Yc-ly+6);
   pDC->MoveTo(Xc,Yc-ly);
   pDC->LineTo(Xc+2,Yc-ly+6);
   //line(Xc,Yc+ly,Xc+2,Yc+ly-6);
   pDC->MoveTo(Xc,Yc+ly);
   pDC->LineTo(Xc+2,Yc+ly-6);
   //line(Xc,Yc+ly,Xc-2,Yc+ly-6);
   pDC->MoveTo(Xc,Yc+ly);
   pDC->LineTo(Xc-2,Yc+ly-6);
   //line(Xc+lx,Yc,Xc+lx-6,Yc+2);
   pDC->MoveTo(Xc+lx,Yc);
   pDC->LineTo(Xc+lx-6,Yc+2);
   //line(Xc+lx,Yc,Xc+lx-6,Yc-2);
   pDC->MoveTo(Xc+lx,Yc);
   pDC->LineTo(Xc+lx-6,Yc-2);
}



void CrnaView::OnDraw(CDC* pDC)
{
	CrnaDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	
	if (Activar==TRUE)
	{	aprendizaje(pDC);
	//	savedata();
		Activar=FALSE;

	}
	if (ActivarF==TRUE)
	{	funcionamiento(pDC);
		ActivarF=FALSE;
	}

	// TODO: agregar aquí el código de dibujo para datos nativos
}


// Impresión de CrnaView

BOOL CrnaView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Preparación predeterminada
	return DoPreparePrinting(pInfo);
}

void CrnaView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar inicialización adicional antes de imprimir
}

void CrnaView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: agregar limpieza después de imprimir
}


// Diagnósticos de CrnaView

#ifdef _DEBUG
void CrnaView::AssertValid() const
{
	CView::AssertValid();
}

void CrnaView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CrnaDoc* CrnaView::GetDocument() const // La versión de no depuración es en línea
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CrnaDoc)));
	return (CrnaDoc*)m_pDocument;
}
#endif //_DEBUG


// Controladores de mensaje de CrnaView


void CrnaView::OnLexicoAprendizaje() 
{
	// TODO: Add your command handler code here

	Activar=TRUE;
	InvalidateRect(NULL,TRUE);

}

void CrnaView::OnLexicoFuncionamiento() 
{
	// TODO: Add your command handler code here

	ActivarF=TRUE;
	InvalidateRect(NULL,TRUE);

}