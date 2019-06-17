// matrixsub.cpp : 实现文件
//

#include "stdafx.h"
#include "PetriMFC.h"
#include "matrixsub.h"
#include "afxdialogex.h"


// matrixsub 对话框

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


// matrixsub 消息处理程序


void matrixsub::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	//CDialogEx::OnOK();
	CFileDialog dlg(FALSE, _T("txt"), _T("test.txt"));//FALSE表示为“另存为”对话框，否则为“打开”对话框

	if (dlg.DoModal() == IDOK)

	{

		CString strFile = dlg.GetPathName();//获取完整路径

		CString strEdit1, strEdit2;

		GetDlgItemText(IDC_EDIT1, strEdit1);//获取指定ID的编辑框内容

											//GetDlgItemText(IDC_EDIT1, strEdit2);//同上

		CStdioFile file;

		if (file.Open(strFile, CStdioFile::modeCreate | CStdioFile::modeNoTruncate | CStdioFile::modeWrite))//创建/打开文件

		{

			file.WriteString(strEdit1);//写入内容

			file.WriteString(_T("\r\n"));//换行

			file.WriteString(strEdit2);//写入内容

			file.Close();//关闭输出文件

		}

	}
}
