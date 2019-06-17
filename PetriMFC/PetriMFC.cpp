
// PetriMFC.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include <memory>
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "PetriMFC.h"
#include "MainFrm.h"

#include "PetriMFCDoc.h"
#include "PetriMFCView.h"
#include "FileContentView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CPetriMFCApp

BEGIN_MESSAGE_MAP(CPetriMFCApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CPetriMFCApp::OnAppAbout)
	// 基于文件的标准文档命令
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// 标准打印设置命令
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_VIEW_FIRSTVIEW, &CPetriMFCApp::OnViewFirstview)
	ON_COMMAND(ID_VIEW_SECONDVIEW, &CPetriMFCApp::OnViewSecondview)
END_MESSAGE_MAP()


// CPetriMFCApp 构造

CPetriMFCApp::CPetriMFCApp()
{
	m_bHiColorIcons = TRUE;

	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 如果应用程序是利用公共语言运行时支持(/clr)构建的，则: 
	//     1) 必须有此附加设置，“重新启动管理器”支持才能正常工作。
	//     2) 在您的项目中，您必须按照生成顺序向 System.Windows.Forms 添加引用。
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PetriMFC.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CPetriMFCApp 对象

CPetriMFCApp theApp;


// CPetriMFCApp 初始化

BOOL CPetriMFCApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// 初始化 OLE 库
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要 AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));
	LoadStdProfileSettings(4);  // 加载标准 INI 文件选项(包括 MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 注册应用程序的文档模板。  文档模板
	// 将用作文档、框架窗口和视图之间的连接
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPetriMFCDoc),
		RUNTIME_CLASS(CMainFrame),       // 主 SDI 框架窗口
		RUNTIME_CLASS(CPetriMFCView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 分析标准 shell 命令、DDE、打开文件操作的命令行
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// 调度在命令行中指定的命令。  如果
	// 用 /RegServer、/Register、/Unregserver 或 /Unregister 启动应用程序，则返回 FALSE。
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// 唯一的一个窗口已初始化，因此显示它并对其进行更新
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CPetriMFCApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CPetriMFCApp 消息处理程序


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnConfigrationCommunicationport();
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
	ON_COMMAND(ID_CONFIGRATION_COMMUNICATIONPORT, &CAboutDlg::OnConfigrationCommunicationport)
END_MESSAGE_MAP()

// 用于运行对话框的应用程序命令
void CPetriMFCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CPetriMFCApp 自定义加载/保存方法

void CPetriMFCApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
	bNameValid = strName.LoadString(IDS_EXPLORER);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EXPLORER);
}

void CPetriMFCApp::LoadCustomState()
{
}

void CPetriMFCApp::SaveCustomState()
{
}

// CPetriMFCApp 消息处理程序





void CAboutDlg::OnConfigrationCommunicationport()
{
	// TODO: 在此添加命令处理程序代码
}


void CPetriMFCApp::OnViewFirstview()
{
	// TODO: 在此添加命令处理程序代码
	if (m_pAnotherView == NULL)
		return;//第二个视图不存在，无需切换

	UINT temp = ::GetWindowLong(m_pAnotherView->m_hWnd, GWL_ID);

	::SetWindowLong(m_pAnotherView->m_hWnd, GWL_ID, ::GetWindowLong(m_pFirstView->m_hWnd, GWL_ID));

	::SetWindowLong(m_pFirstView->m_hWnd, GWL_ID, temp);



	m_pAnotherView->ShowWindow(SW_HIDE);

	m_pFirstView->ShowWindow(SW_SHOW);



	((CFrameWnd*)m_pMainWnd)->SetActiveView(m_pFirstView);

	((CFrameWnd*)m_pMainWnd)->RecalcLayout();

	m_pFirstView->Invalidate();
}


void CPetriMFCApp::OnViewSecondview()
{
	// TODO: 在此添加命令处理程序代码
	createThe2ndView();//首先创建第二个视图
	UINT temp = ::GetWindowLong(m_pAnotherView->m_hWnd, GWL_ID);

	::SetWindowLong(m_pAnotherView->m_hWnd, GWL_ID, ::GetWindowLong(m_pFirstView->m_hWnd, GWL_ID));

	::SetWindowLong(m_pFirstView->m_hWnd, GWL_ID, temp);



	m_pFirstView->ShowWindow(SW_HIDE);

	m_pAnotherView->ShowWindow(SW_SHOW);



	((CFrameWnd*)m_pMainWnd)->SetActiveView(m_pAnotherView);

	((CFrameWnd*)m_pMainWnd)->RecalcLayout();

	m_pAnotherView->Invalidate();
}
void CPetriMFCApp::OnUpdateSwitchviewFirstview(CCmdUI *pCmdUI)
{
	CView* pv = ((CFrameWnd*)m_pMainWnd)->GetActiveView();;

	pCmdUI->Enable(m_pAnotherView != NULL && pv != m_pFirstView);
}

void CPetriMFCApp::OnUpdateSwitchviewSecondview(CCmdUI *pCmdUI)
{
	CView* pv = ((CFrameWnd*)m_pMainWnd)->GetActiveView();;

	pCmdUI->Enable(pv != m_pAnotherView);
}

void CPetriMFCApp::openFile(const CString& fileName)
{
	if (m_pAnotherView == NULL)
	{
		AfxMessageBox(L"第二个视图未打开，请从菜单栏里打开second view.");
		return;
	}
	CString s = _T("你打开文件是：../");	s += fileName; s += _T("\n");

	CFile f;
	bool bOpened = f.Open(_T("../") + fileName, CFile::modeRead);
	if (bOpened)
	{
		enum { eMaxLentht = 1024 * 1024 * 5 };
		char* pppc = new char[eMaxLentht];//最大5MB
		memset(pppc, 0, eMaxLentht);
		std::auto_ptr<char> apc(pppc);//负责回收内存

		unsigned int fileLength = f.GetLength();
		f.Read(pppc, fileLength>eMaxLentht ? eMaxLentht : fileLength);
		USES_CONVERSION;
		s += A2W(pppc);

	}
	else
		s += _T("文件打开失败!");


	m_pAnotherView->SetWindowText(s);
}
void CPetriMFCApp::openFolder(const CString& fileName)
{
	if (m_pAnotherView)
		m_pAnotherView->SetWindowText(_T("你打开的文件夹是：") + fileName);
	else
		AfxMessageBox(L"第二个视图为打开，请从菜单栏里打开second view.");
}

void CPetriMFCApp::createThe2ndView()
{
	if (m_pAnotherView)
		return;
	//创建一个新的视图

	m_pFirstView = ((CFrameWnd*)m_pMainWnd)->GetActiveView();;
	m_pAnotherView = new CFileContentView();


	//文档和视图关联
	CDocument* m_pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	CCreateContext context;
	context.m_pCurrentDoc = m_pDoc;

	//创建视图
	UINT m_IDFORANOTHERVIEW = AFX_IDW_PANE_FIRST + 1;
	CRect rect;
	m_pAnotherView->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_IDFORANOTHERVIEW, &context);
	m_pAnotherView->SetWindowText(L"This is 2nd view.");
}

