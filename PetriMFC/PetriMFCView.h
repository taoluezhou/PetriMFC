
// PetriMFCView.h : CPetriMFCView 类的接口
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
protected: // 仅从序列化创建
	CPetriMFCView();
	DECLARE_DYNCREATE(CPetriMFCView)

// 特性
public:
	CPetriMFCDoc* GetDocument() const;

// 操作
public:
	void  push_back(int x, int y, int d);
	void clear() { g_outDigt.clear(); };
// 重写
public:
	void DrawLine(CDC& dc ,CPoint p1, CPoint p2);
	void Drawword(CDC * pDC);
	virtual void OnDraw(CDC* pDC); // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CPetriMFCView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
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

#ifndef _DEBUG  // PetriMFCView.cpp 中的调试版本
inline CPetriMFCDoc* CPetriMFCView::GetDocument() const
   { return reinterpret_cast<CPetriMFCDoc*>(m_pDocument); }
#endif

