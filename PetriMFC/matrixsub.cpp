// matrixsub.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PetriMFC.h"
#include "matrixsub.h"
#include "afxdialogex.h"


// matrixsub �Ի���

IMPLEMENT_DYNAMIC(matrixsub, CDialogEx)

matrixsub::matrixsub(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG3, pParent)
{

}

matrixsub::~matrixsub()
{
}

void matrixsub::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(matrixsub, CDialogEx)
	ON_BN_CLICKED(IDOK, &matrixsub::OnBnClickedOk)
END_MESSAGE_MAP()


// matrixsub ��Ϣ�������


void matrixsub::OnBnClickedOk()
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
