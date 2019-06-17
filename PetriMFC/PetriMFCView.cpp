
// PetriMFCView.cpp : CPetriMFCView 类的实现
//

#include "stdafx.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "PetriMFC.h"
#endif

#include "PetriMFCDoc.h"
#include "PetriMFCView.h"
#include "PCIE.h"
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
#ifdef _DEBUG
#define new DEBUG_NEW
#define max 10000
#endif
PCIE pcie;

// CPetriMFCView

IMPLEMENT_DYNCREATE(CPetriMFCView, CScrollView)

BEGIN_MESSAGE_MAP(CPetriMFCView, CScrollView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CScrollView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPetriMFCView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_COMMAND(ID_CONFIGRATION_COMMUNICATIONPORT, &CPetriMFCView::OnConfigrationCommunicationport)
END_MESSAGE_MAP()

// CPetriMFCView 构造/析构

CPetriMFCView::CPetriMFCView()
{
	// TODO: 在此处添加构造代码
	//bool o;
//system("petri.exe");
//system("matrix.exe");
//system("dot E:\\VSproject\\PetriMFC4\\PetriMFC\\petri.gv -Tplain -o ee.plain");
}

CPetriMFCView::~CPetriMFCView()
{
}

BOOL CPetriMFCView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CPetriMFCView 绘制

void CPetriMFCView::push_back(int x, int y, int dit)
{

	Digt_xy d;
	d.x = x; d.y = y; d.digt =dit;
	g_outDigt.push_back(d);
	
}

void CPetriMFCView::DrawLine(CDC& dc , CPoint p1, CPoint p2)
{
	//CClientDC dc(this);//获取客户窗口DC
	CPen pen, pen1, *oldpen;
	int PenLineWidth = 2;//为了根据线条宽度设置箭头的大小
	pen.CreatePen(PS_SOLID, PenLineWidth, RGB(0, 0, 0));
	pen1.CreatePen(PS_SOLID, PenLineWidth, RGB(0, 0, 0));
	oldpen = dc.SelectObject(&pen);

	double theta = 3.1415926 / 15 * PenLineWidth;//转换为弧度
	double Px, Py, P1x, P1y, P2x, P2y;
	//以P2为原点得到向量P2P1（P）
	Px = p1.x - p2.x;
	Py = p1.y - p2.y;
	//向量P旋转theta角得到向量P1
	P1x = Px*cos(theta) - Py*sin(theta);
	P1y = Px*sin(theta) + Py*cos(theta);
	//向量P旋转-theta角得到向量P2
	P2x = Px*cos(-theta) - Py*sin(-theta);
	P2y = Px*sin(-theta) + Py*cos(-theta);
	//伸缩向量至制定长度
	double x1, x2;
	int length = 15;
	x1 = sqrt(P1x*P1x + P1y*P1y);
	P1x = P1x*length / x1;
	P1y = P1y*length / x1;
	x2 = sqrt(P2x*P2x + P2y*P2y);
	P2x = P2x*length / x2;
	P2y = P2y*length / x2;
	//平移变量到直线的末端
	P1x = P1x + p2.x;
	P1y = P1y + p2.y;
	P2x = P2x + p2.x;
	P2y = P2y + p2.y;



	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.SelectObject(&pen1);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(P1x, P1y);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(P2x, P2y);

	dc.MoveTo(P1x, P1y);
	dc.LineTo(P2x, P2y);

	CPoint ptVertex[3];

	ptVertex[0].x = p2.x;
	ptVertex[0].y = p2.y;
	ptVertex[1].x = P1x;
	ptVertex[1].y = P1y;
	ptVertex[2].x = P2x;
	ptVertex[2].y = P2y;
	//填充三角形区域
	CBrush br(RGB(0, 0, 0));
	CRgn rgn;
	rgn.CreatePolygonRgn(ptVertex, 3, ALTERNATE);
	dc.FillRgn(&rgn, &br);

	dc.SelectObject(oldpen);

	br.DeleteObject();
	rgn.DeleteObject();
}


void CPetriMFCView::Drawword(CDC*pDC)
{
	CString str;
	int nnn = g_outDigt.size();
	for (int i = 0; i < g_outDigt.size() ; i++)
	{
		const Digt_xy& d = g_outDigt[i];
		str.Format(_T("%d"), d.digt);
		pDC->TextOutW(d.x, d.y, str);
	}
	
}
void CPetriMFCView::OnDraw(CDC* pDC)
{
	CPetriMFCDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	int  len, m, n = 0, a = 0;
	char str[500];
	char c;
	char x[200], y, z, l, s;
	int i;
	
	int p = 0, t = 0;
	FILE *fp;
	
	fp = fopen("ee.plain", "r");
	
	if (fp == NULL)
	{
		exit(3);
	}

	while (fgets(str, 499, fp) != NULL)///如果一行不确定多少位最好赋值大一点
	{
		float xy[200];
		char coord[50][50] = { 0 };//10个坐标，依次是x1,y1,x2,y2..................这里数组你要确定下大小
		m = a = 0;
		int k = 0;
		const DWORD nTestSize = 256;
		BYTE *pRead = new BYTE[nTestSize];
		const PCIE_LOCAL_ADDRESS LocalAddr = 0;
		bool bSuccess;
		len = strlen(str);
		str[len - 1] = '\0';  /*去掉换行符*/
		if (str[0] == 'n')		///如果读到的是node
		{
			m = 5;
			while (str[m] != '\0')///直到这一行的最后
			{
				if (str[m] !=' ') 
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

			for (int i = 0; i < 4; i++)		///输出测试
			{
				float b = atof(coord[i+1]);
				xy[i] = b;
				printf("\n");
			}

			if (str[5] == 'P')
			{


				p++;
				pDC->Ellipse((xy[1] + xy[2] / 2) * 100, (xy[0] - xy[2] / 2) * 100, (xy[1] - xy[2] / 2) * 100, (xy[0] + xy[2] / 2) * 100);
				CString str;
				str.Format(_T("%d"), p);
				pDC->TextOutW(xy[1] * 100, xy[0] * 100, _T("p") + str);

			}
			else if (str[5] == 'T')
			{
				t++;
				pDC->Rectangle((xy[1] + xy[3] / 2) * 100, (xy[0] - xy[2] / 2) * 100, (xy[1] - xy[3] / 2) * 100, (xy[0] + xy[2] / 2) * 100);
				CString str;
				str.Format(_T("%d"), t);
				pDC->TextOutW(xy[1] * 100, xy[0] * 100, _T("t") + str);
			}

		}
		else if (str[0] == 'e')			///如果读到边
		{
			m = 5;
			while (str[m] != '\0')///直到这一行的最后
			{
				if (str[m] != ' ')
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
			int num_count = atoi(coord[2]) * 2;
			for (int j = 0; j < num_count; j++)
			{
				double b = atof(coord[j+3]);
				xy[j] = b;
			}
			for (int j = 0; j < num_count -2;)
			{
				pDC->MoveTo(xy[j + 1] * 100, xy[ j ] * 100);
				pDC->LineTo(xy[j + 3] * 100, xy[j + 2] * 100);
				j = j + 2;
			}

			float x1 = xy[num_count - 3], y1 = xy[num_count - 4], x2 = xy[num_count - 1], y2 = xy[num_count - 2];
			float bl = 1;
			float hypo = sqrt((x1+x2)*(x1+x2)+ (y1 + y2)*(y1 + y2));
			x2 = x2 + (x2 - x1) * bl / hypo;
			y2 = y2 + (y2 - y1) * bl / hypo;
			CPoint p1(xy[num_count - 3] * 100, xy[num_count - 4] * 100), p2(x2 * 100, y2 * 100);
			DrawLine(*pDC, p1, p2);
			
		}

	}
	fclose(fp);
	Drawword(pDC);

}




// CPetriMFCView 打印


void CPetriMFCView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPetriMFCView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CPetriMFCView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CPetriMFCView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}

void CPetriMFCView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPetriMFCView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPetriMFCView 诊断

#ifdef _DEBUG
void CPetriMFCView::AssertValid() const
{
	__super::AssertValid();
}

void CPetriMFCView::Dump(CDumpContext& dc) const
{
	__super::Dump(dc);
}

CPetriMFCDoc* CPetriMFCView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPetriMFCDoc)));
	return (CPetriMFCDoc*)m_pDocument;
}
#endif //_DEBUG


// CPetriMFCView 消息处理程序


void CPetriMFCView::OnInitialUpdate()
{
	//SetScrollSizes(MM_TEXT, GetDocument()->GetDocSize());

	__super::OnInitialUpdate();
	SetScrollSizes(MM_TEXT, CSize(2000, 800));
	// TODO: 在此添加专用代码和/或调用基类
}


void CPetriMFCView::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnHScroll(nSBCode, nPos, pScrollBar);
	Invalidate(TRUE);
}


void CPetriMFCView::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CScrollView::OnVScroll(nSBCode, nPos, pScrollBar);
	Invalidate(TRUE);
}


void CPetriMFCView::OnConfigrationCommunicationport()
{
	communicate dia(this);
	dia.DoModal();
}
