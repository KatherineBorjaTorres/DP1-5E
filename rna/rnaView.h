
// rnaView.h: interfaz de la clase CrnaView
//

#pragma once


class CrnaView : public CView
{
protected: // Crear sólo a partir de serialización
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

// Implementación
public:
	virtual ~CrnaView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Funciones de asignación de mensajes generadas
protected:

	afx_msg void OnLexicoAprendizaje();
	afx_msg void OnLexicoFuncionamiento();
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // Versión de depuración en rnaView.cpp
inline CrnaDoc* CrnaView::GetDocument() const
   { return reinterpret_cast<CrnaDoc*>(m_pDocument); }
#endif

