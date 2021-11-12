
// rnaView.h: interfaz de la clase CrnaView
//

#pragma once


class CrnaView : public CView
{
protected: // Crear s�lo a partir de serializaci�n
	CrnaView();
	DECLARE_DYNCREATE(CrnaView)

// Atributos
public:
	CrnaDoc* GetDocument() const;

// Operaciones
public:

// Reemplazos
public:
	virtual void OnDraw(CDC* pDC);  // Reemplazado para dibujar esta vista
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementaci�n
public:
	virtual ~CrnaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignaci�n de mensajes generadas
protected:

	afx_msg void OnLexicoAprendizaje();
	afx_msg void OnLexicoFuncionamiento();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Versi�n de depuraci�n en rnaView.cpp
inline CrnaDoc* CrnaView::GetDocument() const
   { return reinterpret_cast<CrnaDoc*>(m_pDocument); }
#endif

