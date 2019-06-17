
// PetriMFC.h : PetriMFC Ӧ�ó������ͷ�ļ�
//
#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"       // ������

class CFileContentView;
// CPetriMFCApp:
// �йش����ʵ�֣������ PetriMFC.cpp
//

class CPetriMFCApp : public CWinAppEx
{
public:
	CPetriMFCApp();


// ��д
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

// ʵ��
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
