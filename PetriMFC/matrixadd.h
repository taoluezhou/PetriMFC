#pragma once


// matrixadd �Ի���

class matrixadd : public CDialogEx
{
	DECLARE_DYNAMIC(matrixadd)

public:
	matrixadd(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~matrixadd();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DIALOG2 };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEdit1();
	afx_msg void OnBnClickedOk();
};
