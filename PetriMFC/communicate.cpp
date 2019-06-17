// communicate.cpp : 实现文件
//



#include "stdafx.h"
#include <vector>

#include "PetriMFC.h"
#include "communicate.h"
#include "afxdialogex.h"
#include "PCIE.h"
#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sstream>
#include <time.h> 
#include <sys/timeb.h>
#include <sys/types.h>
#include "PetriMFCView.h"

#define PCIE_VID                 0x1172
#define PCIE_DID                 0xE001
#define MAX_PCIE_CARD   256
#define DEMO_PCIE_USER_BAR      PCIE_BAR1
#define DEMO_PCIE_IO_ADDR       0x00
#define DEMO_PCIE_FIFO_ID       0x00
#define AUTONUM					13
#define TESTTIME				2499

// communicate 对话框

FILE *fp;
PCIE m_hPCIE;
PCIE_CONFIG m_szPcieInfo[256];
DWORD   m_dwCardNum;

int i = 0;
float ratio = 1.5;
float start_x = 400;
float start_y = 100;
float end_x = 960;
float end_y = 660;
float coordinate[100][2];		//点坐标
int decodeTimes = 0;			//手动模式下，已测试帧数
bool drawStatus = 0;			//手动模式下，画图标志
float snrMunual[TESTTIME];			//记录手动模式下信噪比
unsigned long frameManual[TESTTIME];	//记录手动模式下译码帧数
int sortInfo[TESTTIME];				//手动模式信噪比排序
int paintFlag = 0;				//曲线重绘标志
int paintNum = 0;				//重绘个数
bool resetStatus;
float testSnr[TESTTIME];
int notZeroTimes = 0;

unsigned long decodeStatus;				//译码完成标志位
unsigned long errNum[TESTTIME];				//记录误码帧数
unsigned long errStore;					//显示的误码帧数
unsigned long frame_pcie_clocks;		//pcie时钟数
unsigned long frame_one_clocks;			//译码时钟数
unsigned long frame_decoder_clocks;		//译码时钟数
unsigned long frame_all_clocks;			//译码时钟数
unsigned long frequency;
unsigned long latency;
float throughput;

float floatData[AUTONUM][2] = { { 1,0.847 },{ 1.2,0.694 },{ 1.4,0.571 },{ 1.6,0.400 },{ 1.8,0.262 },{ 2,0.158 },
{ 2.2,0.0833 },{ 2.4,0.0415 },{ 2.6,0.02043 },{ 2.8,0.00923 },{ 3,0.004396 },{ 3.2,0.001851 },{ 3.4,0.000823 } };	//软件仿真数据
float quaData[AUTONUM][2] = { { 1,0.850 },{ 1.2,0.695 },{ 1.4,0.573 },{ 1.6,0.401 },{ 1.8,0.263 },{ 2,0.160 },
{ 2.2,0.0844 },{ 2.4,0.04258 },{ 2.6,0.02062 },{ 2.8,0.00928 },{ 3,0.004504 },{ 3.2,0.001862 },{ 3.4,0.000826 } };	//软件量化数据
float snrAuto[AUTONUM] = { 1,1.2,1.4,1.6,1.8,2,2.2,2.4,2.6,2.8,3,3.2,3.4 };								//自动模式下固定信噪比
unsigned long frameAuto[AUTONUM] = { 10000,10000,10000,10000,10000,
100000,100000,100000,100000,200000,
500000,700000,1000000 };//自动模式下测试帧数


IMPLEMENT_DYNAMIC(communicate, CDialogEx)

communicate::communicate(CPetriMFCView* pDrawOwner ,CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DIALOG1, pParent)
	, m_input(0)
	, m_addr(0)
	, m_value(0)
{
	m_pDrawOwner = pDrawOwner;
}

communicate::~communicate()
{
}

void communicate::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_input);
	DDX_Control(pDX, IDC_EDIT1, m_edit);
	//DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_EDIT3, m_out);
	DDX_Text(pDX, IDC_EDIT10, m_addr);
	DDX_Text(pDX, IDC_EDIT11, m_value);
	DDX_Control(pDX, IDC_LIST1, m_addr_input);
}


BEGIN_MESSAGE_MAP(communicate, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON1, &communicate::Connect)
	ON_BN_CLICKED(IDC_BUTTON2, &communicate::DMA)
	//ON_BN_CLICKED(IDC_BUTTON4, &communicate::OnBnClickedButton4)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &communicate::OnLvnItemchangedList1)
	ON_BN_CLICKED(IDC_BUTTON3, &communicate::OnBnClickedButton3)
	ON_EN_CHANGE(IDC_EDIT3, &communicate::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON5, &communicate::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON4, &communicate::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON6, &communicate::OnBnClickedButton6)
	ON_EN_CHANGE(IDC_EDIT6, &communicate::OnEnChangeEdit6)
	ON_EN_CHANGE(IDC_EDIT5, &communicate::OnEnChangeEdit5)
	ON_BN_CLICKED(IDC_BUTTON7, &communicate::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON8, &communicate::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON9, &communicate::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &communicate::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON11, &communicate::OnBnClickedButton11)
	ON_EN_CHANGE(IDC_EDIT7, &communicate::OnEnChangeEdit7)
	ON_BN_CLICKED(IDC_BUTTON12, &communicate::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &communicate::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &communicate::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &communicate::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &communicate::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &communicate::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &communicate::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &communicate::OnBnClickedButton19)
END_MESSAGE_MAP()


// communicate 消息处理程序


void communicate::Connect()
{
	// TODO: 在此添加控件通知处理程序代码
	decodeTimes = 0;
	drawStatus = 0;
	if (!m_hPCIE.IsDriverAvailable())
	{
		MessageBox(_T("Load Driver fail!"));
	}
	else {

		RebuildBoard();
		ConnectBoard();

	}
	//return bSuccess;
}

bool communicate::RebuildBoard()
{
	const WORD wVendorID = 0;
	const WORD wDeviceID = 0;
	bool bSuccess;
	DWORD dwDeviceNum;
	m_dwCardNum = 0;
	bSuccess = m_hPCIE.ScanCard(wVendorID, wDeviceID, &dwDeviceNum, m_szPcieInfo);
	if (!bSuccess) {
		MessageBox(_T("Failed to scan pcie card!"));
	}
	else if (dwDeviceNum == 0) {
		MessageBox(_T("Cannot find any desired pcie card!"));
	}
	else {
		m_dwCardNum = dwDeviceNum;
		int nDefaultIndex = 0;
		for (int i = 0; i < (int)dwDeviceNum; i++) {
			PCIE_CONFIG *p = (PCIE_CONFIG *)&m_szPcieInfo[i];
			if (p->VendorID == 0x1172 && p->DeviceID == 0xE001)
			{
			}
		}
	}
	return bSuccess;
}
	bool communicate::ConnectBoard() 
	{
		bool bSuccess;
		if (m_hPCIE.IsOpened())
			m_hPCIE.Close();

		bSuccess = m_hPCIE.Open(PCIE_VID, PCIE_DID, 0);
		if (bSuccess)
		{
			MessageBox(_T("Connect PCIE suc!"));
			/*this->button_reset->BackColor = System::Drawing::Color::FromArgb(static_cast<System::Int32>(static_cast<System::Byte>(70)), static_cast<System::Int32>(static_cast<System::Byte>(199)),
				static_cast<System::Int32>(static_cast<System::Byte>(128)));*/
		}
		else
		{
			MessageBox(_T("Connect PCIE error!"));
		}

		/*译码复位标志位*/
		if (m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 40, 2) == 0)
		{
			return 0;
		}
		resetStatus = 1;
		errStore = 0;
		frame_pcie_clocks = 0;
		frame_one_clocks = 0;
		frame_decoder_clocks = 0;
		frame_all_clocks = 0;
		frequency = 0;
		latency = 0;
		throughput = 0;
		//ClearLine();
		return bSuccess;
	}

	int parseInt(const char* str, std::vector<int>&  intResult)
	{
		intResult.clear();
		std::string s(str);
		std::istringstream iss(s);

		int dint = 0;
		while (iss && (iss >> dint))
		{
			intResult.push_back(dint);
		}

		return intResult.size();
	}

void communicate::DMA()
{
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}
	
	//resFile1 = Write("test_wr.result");
	//resFile2 = gcnew System::IO::StreamWriter("test_rd.result");
	//const DWORD nTestSize = 256;  //modify by zxj 06111754
	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];
	int m[nTestSize];
	int p = 0;
	//this->dma_sts->Text = "";
	// Original Test pattern. If needed you can uncomment this section and to comment the proposal test pattern
	///*
	int j;
	
	//for (int i = 0; i<nTestSize; i++) {
	//	if (i == 0)
	//		*(pWrite + i) = m_input;
	//else
	//	*(pWrite + i) = *(pWrite + i - 1) * 13;
	
	//m_edit.GetWindowText(str);

	/*int d;
	d = GetDlgItemInt(IDC_EDIT1);*/
	CString str;
	m_edit.GetWindowText(str);//str=="1,2,3,5,9"
	str.Replace(L',', L' ');
	USES_CONVERSION;
	std::vector<int> intResults;
	parseInt(W2A(str), intResults);



	
	for (int i = 0; i<nTestSize && i< intResults.size(); i++) {
		//if (i == 0)
			pWrite[i]= intResults[i];
		//else
		//	*(pWrite + i) = *(pWrite + i - 1)+1;
			//d++;

		/*int sel = m_input;
		CString str1;
		str1.Format(_T("sel = %d"), sel);*/
			//j++;
		
		//	*(pWrite + i) = 1;
			//j++;
		  //*(pRead + i) = 5;
	
		//For debug porpose only. UnComment it as needed
		//resFile1->WriteLine("{1} Wr D {0:X4}", pWrite[i], i);
			//p++;
	}
	

	int l;
	l = GetDlgItemInt(IDC_EDIT6);
	
	// write

	bSuccess = m_hPCIE.DmaWrite(LocalAddr+l, pWrite, nTestSize);
	MessageBox(_T("DMA write done!"));
	//Sleep(2000);
	//for (int i = 0; i < nTestSize; i++) {
	
	//	bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR+4, p);
	//	p++;
	//if (bSuccess) {
	//	//		// read
	//	bSuccess = m_hPCIE.DmaRead(LocalAddr, pRead, nTestSize);
	//	//		//singel
	//	//		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
	//}
	////	int sel = pread;
	////	CString str1;
	////	str1.Format(_T("sel = %d"), sel);
	////	MessageBox(str1, _T("title"), MB_OK);
	////}

	////p++;
	////this->dma_sts->Text = "DMA write done";

	////this->dma_sts->Text = " ";

	//int tmp[nTestSize];
	//CString str, o_str;
	//for (int i = 0; i < nTestSize; i++) {
	//	tmp[i] = *(pRead + i);
	//	str.Format(_T("%i"), tmp[i]);  //把整型数组添加到字符串
	//								   //str.Format("%8.2f", pt[i]);
	//	o_str += str;
	//	o_str += ",";
	//	SetDlgItemText(IDC_EDIT3, o_str);
	//}

	// verify
	//	int sel[nTestSize];
	//int error = 0;
	//if (bSuccess) {
	//	for (int i = 0; i<nTestSize; i++) {
	//	if (*(pWrite + i) == *(pRead + i))
	//		{
	//		sel[i] = *(pRead+i);
	//	/* CString str1;
	//	 str1.Format(_T("sel = %d"), sel);
	//	  MessageBox(str1, _T("title"), MB_OK);*/
	//		SetDlgItemInt(IDC_EDIT3, sel[i]);

	//	  //MessageBox(_T("DMA Rd test "));
	//		}
	//		else
	//		{
	//			int sel = *(pRead + i);
	//			CString str1;
	//			str1.Format(_T("sel = %d"), sel);
	//			MessageBox(str1, _T("title"), MB_OK);
	//		 MessageBox(_T("DMA Rd test fail"));
	//		}

	//	}



	//}
	//}
	//else MessageBox(_T("dma failure!"));


	// free memory
	delete[]pWrite;
	delete[]pRead;
	//Close();
	//resFile2->Close();
}


void communicate::OnLvnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码

	*pResult = 0;
}



void communicate::OnBnClickedButton3()
{
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}

	
	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];
	
	int d;
	d = GetDlgItemInt(IDC_EDIT2);
	int p;
	p = GetDlgItemInt(IDC_EDIT5);

	
		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR+p, d);
	
	
}


void communicate::OnEnChangeEdit3()
{
	
}


void communicate::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bSuccess;
	/*int d;
	d = GetDlgItemInt(IDC_EDIT2);*/
	unsigned long pread;
	//bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, d);
	int p;
	p = GetDlgItemInt(IDC_EDIT5);

		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + p, &pread);
	int sel = pread;
	SetDlgItemInt(IDC_EDIT4, sel);
}


void communicate::OnBnClickedButton4()
{
	bool bSuccess;
	//const DWORD nTestSize = 256; //by zxj 06111534
	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];
	// TODO: 在此添加控件通知处理程序代码
	int p;
	p = GetDlgItemInt(IDC_EDIT7);

	// read
		bSuccess = m_hPCIE.DmaRead(LocalAddr+p, pRead, nTestSize);
	
		
		
	CString str, o_str,o_str2,o_str3;
	CString str1, str2,str3;
	
	//if (bSuccess)
	//{
	//	CFile f;
	//	f.Open(_T("G:\\gxy\\PetriMFC_new\\PetriMFC\\Debug\\read.txt"), CFile::modeWrite | CFile::modeRead | CFile::modeCreate);
	//	
	//	for (int i = 0; i < 16; i++)
	//	{
	//		str.Format(_T("%d"), (int)pRead[i]);  //把整型数组添加到字符串
	//								   //str.Format("%8.2f", pt[i]);

	//		o_str += str;
	//		o_str += ",";
	//		f.Write(str, str.GetLength());
	//		//f.Write(_T(","), 1);
	//	}
	//	f.Write(_T("\r\n"),2);
	//	SetDlgItemText(IDC_EDIT3, o_str);
	//		int a, s[20], rem, j;
	//		a = pRead[0];
	//		j = 0;
	//		while (j < 8)
	//		{
	//			rem = a % 2;
	//			a = a / 2;
	//			s[i++] = rem;
	//			j++;
	//		}
	//		for (int i = 0; i < 8; i++)
	//		{
	//			str1.Format(_T("%d"), (int)s[i]);  //把整型数组添加到字符串
	//											  //str.Format("%8.2f", pt[i]);

	//			/*o_str2 += str1;
	//			o_str2 += ",";*/
	//			f.Write(str1, str1.GetLength());
	//		}
	//		//SetDlgItemText(IDC_EDIT3, o_str);
	//		f.Write(_T("/r/n"), 2);
	//		for (int i = 1; i < 11; i++)
	//		{

	//			str2.Format(_T("%d"), (int)pRead[i]);  //把整型数组添加到字符串
	//												  //str.Format("%8.2f", pt[i]);
	//			/*o_str3 += str2;
	//			o_str3 += ",";*/
	//			f.Write(str2, str2.GetLength());
	//		}
	//		f.Write(_T("/r/n"), 2);
	//		

	//		f.Close();
	//}
	delete[]pWrite;
	delete[]pRead;

}


void communicate::OnBnClickedButton6()
{

	// TODO: 在此添加控件通知处理程序代码
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}


	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];

	


	bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 4);
	if (bSuccess)
	{
		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
	}

	

}


void communicate::OnEnChangeEdit6()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void communicate::OnEnChangeEdit5()
{
	// TODO:  如果该控件是 RICHEDIT 控件，它将不
	// 发送此通知，除非重写 CDialogEx::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}


void communicate::OnBnClickedButton7()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}


	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];




	bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 128);
	if (bSuccess)
	{
		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
	}
}


void communicate::OnBnClickedButton8()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}


	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];




	bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 8);
	if (bSuccess)
	{
		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
	}
}


void communicate::OnBnClickedButton9()
{
	// TODO: 在此添加控件通知处理程序代码
	bool bSuccess;
	// TODO: 在此添加控件通知处理程序代码
	if (!(m_hPCIE.IsOpened())) {
		MessageBox(_T("Connect to the PCIE Port first"));
		return;
	}


	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;
	unsigned long pread;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];
	int p;
	p = GetDlgItemInt(IDC_EDIT12);



	bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 8, p);
	if (bSuccess)
	{
		bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 8, &pread);
	}
}



void communicate::OnBnClickedButton10()
{
	//m_pDrawOwner->clear();//清空先前保存的数据


	bool bSuccess;
	const DWORD nTestSize = 256;
	const PCIE_LOCAL_ADDRESS LocalAddr = 0;

	BYTE *pWrite = new BYTE[nTestSize];
	BYTE *pRead = new BYTE[nTestSize];
	// TODO: 在此添加控件通知处理程序代码
	int p;
	p = GetDlgItemInt(IDC_EDIT8);

	//		// read
	m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	FILE *fp;
	fp = fopen("ee.plain", "r");
	float xy[200];
	int max = 10000;
	int  len, m, n = 0, a = 0, g = 0;
	char str[500];
	char coord[50][50] = { 0 };//10个坐标，依次是x1,y1,x2,y2..................这里数组你要确定下大小
	char c;
	char x[200], y, z, l, s;
	int i;

	while (fgets(str, max, fp) != NULL)///如果一行不确定多少位最好赋值大一点
	{
		m = a = 0;
		int k = 0;
		const DWORD nTestSize = 256;
		BYTE *pRead = new BYTE[nTestSize];
		bool bSuccess;
		len = strlen(str);
		str[len - 1] = '\0';  /*去掉换行符*/
		if (str[0] == 'n')		///如果读到的是node
		{
			if (str[7] == ' ')
			{
				m = 8;				///往后前进8位
				while (str[m] != '\0')///直到这一行的最后
				{
					if (str[m] > 45 && str[m] < 58) // 读到的是数字或者小数点(参照ASCII表)
					{
						coord[a][k] = str[m];///读到数组
						k++;					///数组往后一位
						m++;					///往后读一位
					}
					else if (str[m] == ' ')		///如果是空格，不读，但是文件往后一位，数组保存到下一层(a++)，从这一层的第一个开始保存()
					{
					a++;
						m++;
						k = 0;
				}
					else                       ///如果读到的不是数字也不是空格，也就是读到文档的后边那些信息
					{
						m++;
						break;
					}
				}
				for (int i = 0; i < a; i++)		///输出测试
				{
					
				float b = atof(coord[i]);
				xy[i] = b;
					
				}
			if (str[5] == 'P'&&str[6] == '1')
			{
				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
				m_pDrawOwner->push_back(xy[1] * 100,  (xy[0] + xy[3] / 2) * 100 , pRead[1]);
			}
				if (str[5] == 'P'&&str[6] == '2')
			{
					//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[2]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[2]);

	 			}
	 			if (str[5] == 'P'&&str[6] == '3')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[3]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[3]);
					
	 			}
	 			if (str[5] == 'P'&&str[6] == '4')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[4]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[4]);
	 			}
	 			if (str[5] == 'P'&&str[6] == '5')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[5]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[5]);
	 			}
	 			if (str[5] == 'P'&&str[6] == '6')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[6]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[6]);
	 			}

	 			if (str[5] == 'P'&&str[6] == '7')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[7]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[7]);
	 			}
	 			if (str[5] == 'P'&&str[6] == '8')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[8]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[8]);
	 			}
	 			if (str[5] == 'P'&&str[6] == '9')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[9]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[9]);
	 			}
	 			
	 			if (str[5] == 'T'&&str[6] == '1')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				int a, s[20], i, rem, j, b;
	 				a = pRead[0];
					i = 0;
					j = 0;
	 				while (j < 8)
	 				{
	 					rem = a % 2;
	 					a = a / 2;
	 					s[i++] = rem;
	 					j++;
	 				}
	 				//str1.Format(_T("%d"), s[7]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[0]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '2')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[6]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[1]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '3')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[5]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[2]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '4')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[4]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[3]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '5')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[3]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[4]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '6')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[2]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[5]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '7')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[1]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[6]);
	 			}
	 			if (str[5] == 'T'&&str[6] == '8')
	 			{
					//CString str1;
					m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
					int a, s[20], i, rem, j, b;
					a = pRead[0];
					i = 0;
					j = 0;
					while (j < 8)
					{
						rem = a % 2;
						a = a / 2;
						s[i++] = rem;
						j++;
					}
					//str1.Format(_T("%d"), s[0]);
					//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, s[7]);
	 			}

	 		}
	 		else if (str[8] == ' ')
	 		{
	 			m = 9;				 //往后前进8位
	 			while (str[m] != '\0')//直到这一行的最后
	 			{
	 				if (str[m] > 45 && str[m] < 58)   //读到的是数字或者小数点(参照ASCII表)
	 				{
	 					coord[a][k] = str[m]; //读到数组
	 					k++;					 //数组往后一位
	 					m++;					 //往后读一位
	 				}
	 				else if (str[m] == ' ')		 //如果是空格，不读，但是文件往后一位，数组保存到下一层(a++)，从这一层的第一个开始保存()
	 				{
	 					a++;
	 					m++;
	 					k = 0;
	 				}
	 				else                        //如果读到的不是数字也不是空格，也就是读到文档的后边那些信息
	 				{
	 					m++;
	 					break;
	 				}
	 			}
	 			for (int i = 0; i < a; i++)		//输出测试
	 			{
	 				/*for (int j = 0; j < 7; j++)
	 				{
	 				printf("%c", coord[i][j]);
	 				}
	 				printf("\n");*/
	 				float b = atof(coord[i]);
	 				xy[i] = b;
	 			}
	 			/*if (str[5] == 'P')
	 			{
	 				 p++;
	 				  pcie.DmaRead(16, pRead, nTestSize);
	 				 pDC->Ellipse((xy[1] + xy[2] / 2) * 100, (xy[0] - xy[2] / 2) * 100, (xy[1] - xy[2] / 2) * 100, (xy[0] + xy[2] / 2) * 100);
	 				 CString str;
	 				 str.Format(_T("%d"), p);
	 				 pDC->TextOutW(xy[1] * 100, xy[0] * 100, _T("p") + str);
	 			

	 				
	 			}*/
	 			if (str[5] == 'P'&&str[6] == '1'&&str[7] == '0')
	 			{
	 				//CString str1;
	 				m_hPCIE.DmaRead(LocalAddr + p, pRead, nTestSize);
	 				//str1.Format(_T("%d"), pRead[10]);
	 				//pDC->TextOutW(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, str1);
					m_pDrawOwner->push_back(xy[1] * 100, (xy[0] + xy[3] / 2) * 100, pRead[10]);
	 			}
	 			else if (str[5] == 'T')
	 			{
	 				/*t++;
	 				pDC->Rectangle((xy[1] + xy[3] / 2) * 100, (xy[0] - xy[2] / 2) * 100, (xy[1] - xy[3] / 2) * 100, (xy[0] + xy[2] / 2) * 100);
	 				CString str;
	 				str.Format(_T("%d"), t);
	 				pDC->TextOutW(xy[1] * 100, xy[0] * 100, _T("t") + str);*/
	 			}

			}


		}
	}
	m_pDrawOwner->Invalidate();

	}



	void communicate::OnBnClickedButton11()
	{
		// TODO: 在此添加控件通知处理程序代码
		//m_pDrawOwner->clear();//清空先前保存的数据


		bool bSuccess;
		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];
		// TODO: 在此添加控件通知处理程序代码
		int p;
		p = GetDlgItemInt(IDC_EDIT7);

		//		// read
		m_hPCIE.DmaRead(LocalAddr + p+16, pRead, nTestSize);
		int sel = pRead[0];
		SetDlgItemInt(IDC_EDIT9, sel);
		m_pDrawOwner->push_back(0, 0, pRead[0]);
		m_pDrawOwner->Invalidate();
	}


	void communicate::OnEnChangeEdit7()
	{
		// TODO:  如果该控件是 RICHEDIT 控件，它将不
		// 发送此通知，除非重写 CDialogEx::OnInitDialog()
		// 函数并调用 CRichEditCtrl().SetEventMask()，
		// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

		// TODO:  在此添加控件通知处理程序代码
	}


	void communicate::OnBnClickedButton12()
	{
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 1);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}


	void communicate::OnBnClickedButton13()
	{
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 2);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}


	void communicate::OnBnClickedButton14()
	{
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 64);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}


	void communicate::OnBnClickedButton15()
	{
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 32);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}


	void communicate::OnBnClickedButton16()
	{
		
		// TODO: 在此添加控件通知处理程序代码
		int i = 0, j = 0;
		i = m_addr;
		j = m_value;
		//MoveTo(100 * i, 100 * j);
		//LineTo(200 * i, 200 * j);
	}


	void communicate::OnBnClickedButton17()
	{
		// TODO: 在此添加控件通知处理程序代码
		CString strvalue[] = { TEXT("addr"),TEXT("0"), TEXT("1"), TEXT("2"), TEXT("3"), TEXT("4"), TEXT("5"),TEXT("6"),TEXT("7"),TEXT("8") };
		//设置表头
		for (int i = 0; i < 9; i++) {
			m_addr_input.InsertColumn(i, strvalue[i], LVCFMT_LEFT, 100);
		}
		m_addr_input.InsertItem(0, TEXT("0+"));
		m_addr_input.InsertItem(1, TEXT("8+"));
		m_addr_input.InsertItem(2, TEXT("16+"));
		m_addr_input.InsertItem(3, TEXT("24+"));
		m_addr_input.InsertItem(4, TEXT("32+"));
		m_addr_input.InsertItem(5, TEXT("40+"));
		m_addr_input.InsertItem(6, TEXT("48+"));
		m_addr_input.InsertItem(7, TEXT("56+"));
		for(int i=0;i<9;i++){
			//m_addr_input.InsertItem(i,TEXT("8+"));
			for (int j = 1; j < 20; j++) {
				m_addr_input.SetItemText(i, j, TEXT("123"));

				//m_addr_input.InsertItem(0, TEXT("123"));
			}
		}
		
	}


	void communicate::OnBnClickedButton18()
	{
		// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 256);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}


	void communicate::OnBnClickedButton19()
	{
		// TODO: 在此添加控件通知处理程序代码
		// TODO: 在此添加控件通知处理程序代码
		bool bSuccess;
		// TODO: 在此添加控件通知处理程序代码
		if (!(m_hPCIE.IsOpened())) {
			MessageBox(_T("Connect to the PCIE Port first"));
			return;
		}


		const DWORD nTestSize = 256;
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		unsigned long pread;

		BYTE *pWrite = new BYTE[nTestSize];
		BYTE *pRead = new BYTE[nTestSize];




		bSuccess = m_hPCIE.Write32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, 512);
		if (bSuccess)
		{
			bSuccess = m_hPCIE.Read32(DEMO_PCIE_USER_BAR, DEMO_PCIE_IO_ADDR + 4, &pread);
		}
	}
