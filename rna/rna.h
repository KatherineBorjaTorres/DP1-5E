
// rna.h: archivo de encabezado principal para la aplicaci�n rna
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // S�mbolos principales


// CrnaApp:
// Consulte la secci�n rna.cpp para obtener informaci�n sobre la implementaci�n de esta clase
//

class CrnaApp : public CWinApp
{
public:
	CrnaApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementaci�n
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CrnaApp theApp;
