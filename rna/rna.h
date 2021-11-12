
// rna.h: archivo de encabezado principal para la aplicación rna
//
#pragma once

#ifndef __AFXWIN_H__
	#error "incluir 'stdafx.h' antes de incluir este archivo para PCH"
#endif

#include "resource.h"       // Símbolos principales


// CrnaApp:
// Consulte la sección rna.cpp para obtener información sobre la implementación de esta clase
//

class CrnaApp : public CWinApp
{
public:
	CrnaApp();


// Reemplazos
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// Implementación
	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
};

extern CrnaApp theApp;
