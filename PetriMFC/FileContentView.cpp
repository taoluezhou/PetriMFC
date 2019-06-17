// FileContentView.cpp : implementation file
//

#include "stdafx.h"
#include "PetriMFC.h"
#include "FileContentView.h"


// CFileContentView

IMPLEMENT_DYNCREATE(CFileContentView, CEditView)

CFileContentView::CFileContentView()
{

}

CFileContentView::~CFileContentView()
{
}

BEGIN_MESSAGE_MAP(CFileContentView, CEditView)
END_MESSAGE_MAP()


// CFileContentView diagnostics

#ifdef _DEBUG
void CFileContentView::AssertValid() const
{
	CEditView::AssertValid();
}

#ifndef _WIN32_WCE
void CFileContentView::Dump(CDumpContext& dc) const
{
	CEditView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFileContentView message handlers

BOOL CFileContentView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Add your specialized code here and/or call the base class
cs.style = AFX_WS_DEFAULT_VIEW | WS_VSCROLL | ES_AUTOVSCROLL | ES_MULTILINE | ES_NOHIDESEL;
	return CEditView::PreCreateWindow(cs);
}
