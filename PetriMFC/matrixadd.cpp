// matrixadd.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PetriMFC.h"
#include "matrixadd.h"
#include "afxdialogex.h"


// matrixadd �Ի���

IMPLEMENT_DYNAMIC(matrixadd, CDialogEx)

matrixadd::matrixadd(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG2, pParent)
{

}

matrixadd::~matrixadd()
{
}

void matrixadd::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(matrixadd, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT1, &matrixadd::OnEnChangeEdit1)
	ON_BN_CLICKED(IDOK, &matrixadd::OnBnClickedOk)
END_MESSAGE_MAP()


// matrixadd ��Ϣ�������


void matrixadd::OnEnChangeEdit1()
{
	// TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
	// ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
	// ���������� CRichEditCtrl().SetEventMask()��
	// ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

	// TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void matrixadd::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//CDialogEx::OnOK();
	CFileDialog dlg(FALSE, _T("txt"), _T("test.txt"));//FALSE��ʾΪ�����Ϊ���Ի��򣬷���Ϊ���򿪡��Ի���

	if (dlg.DoModal() == IDOK)

	{

		CString strFile = dlg.GetPathName();//��ȡ����·��

		CString strEdit1, strEdit2;

		GetDlgItemText(IDC_EDIT1, strEdit1);//��ȡָ��ID�ı༭������

											//GetDlgItemText(IDC_EDIT1, strEdit2);//ͬ��

		CStdioFile file;

		if (file.Open(strFile, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite))//����/���ļ�

		{

			file.WriteString(strEdit1);//д������

			file.WriteString(_T("\r\n"));//����

			file.WriteString(strEdit2);//д������

			file.Close();//�ر�����ļ�

		}

	}
}
