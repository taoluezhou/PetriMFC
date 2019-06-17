#pragma once

class CsdiTextDoc;
// CFileContentView view

class CFileContentView : public CEditView
{
	DECLARE_DYNCREATE(CFileContentView)

protected:

public:
	CFileContentView();           // protected constructor used by dynamic creation
	virtual ~CFileContentView();
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

CsdiTextDoc* GetDocument()
{
	return (CsdiTextDoc*)m_pDocument;
}

protected:
	DECLARE_MESSAGE_MAP()
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
};


