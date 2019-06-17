#pragma once
#include "afxwin.h"
#include "afxcmn.h"

class CPetriMFCView;
// communicate 对话框

class communicate : public CDialogEx
{
	DECLARE_DYNAMIC(communicate)

public:
	communicate(CPetriMFCView* pDrawOwner , CWnd* pParent = NULL);   // 标准构造函数
	virtual ~communicate();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG1 };
#endif

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void Connect();
	bool RebuildBoard();
	afx_msg
		bool ConnectBoard();
	void DMA();
	int m_input;
	//afx_msg void OnBnClickedButton4();
	CEdit m_edit;
	afx_msg void OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
	CListCtrl m_list;
	afx_msg void OnBnClickedButton3();
	CEdit m_out;
	afx_msg void OnEnChangeEdit3();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnEnChangeEdit6();
	afx_msg void OnEnChangeEdit5();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();

private:
	CPetriMFCView* m_pDrawOwner;
public:
	afx_msg void OnBnClickedButton11();
	afx_msg void OnEnChangeEdit7();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	int m_addr;
	int m_value;
	afx_msg void OnBnClickedButton16();
	CListCtrl m_addr_input;
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
};
