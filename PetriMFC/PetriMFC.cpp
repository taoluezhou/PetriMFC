
// PetriMFC.cpp : ����Ӧ�ó��������Ϊ��
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
	// �����ļ��ı�׼�ĵ�����
	ON_COMMAND(ID_FILE_NEW, &CWinAppEx::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinAppEx::OnFileOpen)
	// ��׼��ӡ��������
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinAppEx::OnFilePrintSetup)
	ON_COMMAND(ID_VIEW_FIRSTVIEW, &CPetriMFCApp::OnViewFirstview)
	ON_COMMAND(ID_VIEW_SECONDVIEW, &CPetriMFCApp::OnViewSecondview)
END_MESSAGE_MAP()


// CPetriMFCApp ����

CPetriMFCApp::CPetriMFCApp()
{
	m_bHiColorIcons = TRUE;

	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���Ӧ�ó��������ù�����������ʱ֧��(/clr)�����ģ���: 
	//     1) �����д˸������ã�������������������֧�ֲ�������������
	//     2) ��������Ŀ�У������밴������˳���� System.Windows.Forms ������á�
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: ������Ӧ�ó��� ID �ַ����滻ΪΨһ�� ID �ַ�����������ַ�����ʽ
	//Ϊ CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("PetriMFC.AppID.NoVersion"));

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}

// Ψһ��һ�� CPetriMFCApp ����

CPetriMFCApp theApp;


// CPetriMFCApp ��ʼ��

BOOL CPetriMFCApp::InitInstance()
{
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinAppEx::InitInstance();


	// ��ʼ�� OLE ��
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// ʹ�� RichEdit �ؼ���Ҫ AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));
	LoadStdProfileSettings(4);  // ���ر�׼ INI �ļ�ѡ��(���� MRU)


	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// ע��Ӧ�ó�����ĵ�ģ�塣  �ĵ�ģ��
	// �������ĵ�����ܴ��ں���ͼ֮�������
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CPetriMFCDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ��ܴ���
		RUNTIME_CLASS(CPetriMFCView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ������׼ shell ���DDE�����ļ�������������
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);



	// ��������������ָ�������  ���
	// �� /RegServer��/Register��/Unregserver �� /Unregister ����Ӧ�ó����򷵻� FALSE��
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// Ψһ��һ�������ѳ�ʼ���������ʾ����������и���
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	return TRUE;
}

int CPetriMFCApp::ExitInstance()
{
	//TODO: �����������ӵĸ�����Դ
	AfxOleTerm(FALSE);

	return CWinAppEx::ExitInstance();
}

// CPetriMFCApp ��Ϣ�������


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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

// �������жԻ����Ӧ�ó�������
void CPetriMFCApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CPetriMFCApp �Զ������/���淽��

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

// CPetriMFCApp ��Ϣ�������





void CAboutDlg::OnConfigrationCommunicationport()
{
	// TODO: �ڴ���������������
}


void CPetriMFCApp::OnViewFirstview()
{
	// TODO: �ڴ���������������
	if (m_pAnotherView == NULL)
		return;//�ڶ�����ͼ�����ڣ������л�

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
	// TODO: �ڴ���������������
	createThe2ndView();//���ȴ����ڶ�����ͼ
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
		AfxMessageBox(L"�ڶ�����ͼδ�򿪣���Ӳ˵������second view.");
		return;
	}
	CString s = _T("����ļ��ǣ�../");	s += fileName; s += _T("\n");

	CFile f;
	bool bOpened = f.Open(_T("../") + fileName, CFile::modeRead);
	if (bOpened)
	{
		enum { eMaxLentht = 1024 * 1024 * 5 };
		char* pppc = new char[eMaxLentht];//���5MB
		memset(pppc, 0, eMaxLentht);
		std::auto_ptr<char> apc(pppc);//��������ڴ�

		unsigned int fileLength = f.GetLength();
		f.Read(pppc, fileLength>eMaxLentht ? eMaxLentht : fileLength);
		USES_CONVERSION;
		s += A2W(pppc);

	}
	else
		s += _T("�ļ���ʧ��!");


	m_pAnotherView->SetWindowText(s);
}
void CPetriMFCApp::openFolder(const CString& fileName)
{
	if (m_pAnotherView)
		m_pAnotherView->SetWindowText(_T("��򿪵��ļ����ǣ�") + fileName);
	else
		AfxMessageBox(L"�ڶ�����ͼΪ�򿪣���Ӳ˵������second view.");
}

void CPetriMFCApp::createThe2ndView()
{
	if (m_pAnotherView)
		return;
	//����һ���µ���ͼ

	m_pFirstView = ((CFrameWnd*)m_pMainWnd)->GetActiveView();;
	m_pAnotherView = new CFileContentView();


	//�ĵ�����ͼ����
	CDocument* m_pDoc = ((CFrameWnd*)m_pMainWnd)->GetActiveDocument();
	CCreateContext context;
	context.m_pCurrentDoc = m_pDoc;

	//������ͼ
	UINT m_IDFORANOTHERVIEW = AFX_IDW_PANE_FIRST + 1;
	CRect rect;
	m_pAnotherView->Create(NULL, NULL, WS_CHILD, rect, m_pMainWnd, m_IDFORANOTHERVIEW, &context);
	m_pAnotherView->SetWindowText(L"This is 2nd view.");
}

