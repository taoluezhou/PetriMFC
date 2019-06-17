
// PetriMFC.h : PetriMFC 应用程序的主头文件
//
#pragma once

#ifndef __AFXWIN_H__
	#error "在包含此文件之前包含“stdafx.h”以生成 PCH 文件"
#endif

#include "resource.h"       // 主符号

class CFileContentView;
// CPetriMFCApp:
// 有关此类的实现，请参阅 PetriMFC.cpp
//

class CPetriMFCApp : public CWinAppEx
{
public:
	CPetriMFCApp();


// 重写
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// 实现
	UINT  m_nAppLook;
	BOOL  m_bHiColorIcons;

	virtual void PreLoadState();
	virtual void LoadCustomState();
	virtual void SaveCustomState();

	afx_msg void OnAppAbout();
	DECLARE_MESSAGE_MAP()
	afx_msg void OnViewFirstview();
	afx_msg void OnViewSecondview();
	void OnUpdateSwitchviewFirstview(CCmdUI * pCmdUI);
	void OnUpdateSwitchviewSecondview(CCmdUI * pCmdUI);
public:
	void openFile(const CString & fileName);
	void openFolder(const CString & fileName);
private:
	CView* m_pFirstView;
	CFileContentView* m_pAnotherView;
	void createThe2ndView();
};

extern CPetriMFCApp theApp;
