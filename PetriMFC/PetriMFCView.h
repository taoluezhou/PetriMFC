
// PetriMFCView.h : CPetriMFCView ��Ľӿ�
//

#pragma once
#include <vector>
class Digt_xy
{
public:
	int x;
	int y;
	int digt;
};
class CPetriMFCDoc;

#if 0
class CPetriMFCView : public CView
#else
class CPetriMFCView : public CScrollView
#endif
{
protected: // �������л�����
	CPetriMFCView();
	DECLARE_DYNCREATE(CPetriMFCView)

// ����
public:
	CPetriMFCDoc* GetDocument() const;

// ����
public:
	void  push_back(int x, int y, int d);
	void clear() { g_outDigt.clear(); };
// ��д
public:
	void DrawLine(CDC& dc ,CPoint p1, CPoint p2);
	void Drawword(CDC * pDC);
	virtual void OnDraw(CDC* pDC); // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CPetriMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnConfigrationCommunicationport();

protected:
	std::vector<Digt_xy> g_outDigt;
};

#ifndef _DEBUG  // PetriMFCView.cpp �еĵ��԰汾
inline CPetriMFCDoc* CPetriMFCView::GetDocument() const
   { return reinterpret_cast<CPetriMFCDoc*>(m_pDocument); }
#endif

