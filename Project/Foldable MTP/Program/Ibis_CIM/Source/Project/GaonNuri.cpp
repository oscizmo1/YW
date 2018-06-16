
// GaonNuri.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "GaonNuri.h"
#include "MainFrm.h"

#include "GaonNuriDoc.h"
#include "GaonNuriView.h"

#include "Define.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


char CGaonNuriApp::m_strAppPath[_MAX_PATH];
char CGaonNuriApp::m_strExePath[_MAX_PATH];
CString CGaonNuriApp::m_strMainPath;

// CGaonNuriApp

BEGIN_MESSAGE_MAP(CGaonNuriApp, CWinApp)
	ON_COMMAND(ID_APP_ABOUT, &CGaonNuriApp::OnAppAbout)
	// ǥ�� ������ ���ʷ� �ϴ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_NEW, &CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, &CWinApp::OnFileOpen)
	// ǥ�� �μ� ���� ����Դϴ�.
	ON_COMMAND(ID_FILE_PRINT_SETUP, &CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()



// CGaonNuriApp ����

CGaonNuriApp::CGaonNuriApp()
{

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}

// ������ CGaonNuriApp ��ü�Դϴ�.

CGaonNuriApp theApp;

//CMyMessage*			m_clsMyMsg;

// CGaonNuriApp �ʱ�ȭ

BOOL CGaonNuriApp::InitInstance()
{
	HANDLE hMutex = NULL;
	hMutex = CreateMutex(NULL, FALSE, _T("GaonNuri"));
	if( ERROR_ALREADY_EXISTS == ::GetLastError() )  {
		AfxMessageBox("GaonNuri is already Running !!!!.");
		CloseHandle(hMutex);
		return FALSE;
	} 

	// ���� ���α׷� �Ŵ��佺Ʈ�� ComCtl32.dll ���� 6 �̻��� ����Ͽ� ���־� ��Ÿ����
	// ����ϵ��� �����ϴ� ���, Windows XP �󿡼� �ݵ�� InitCommonControlsEx()�� �ʿ��մϴ�. 
	// InitCommonControlsEx()�� ������� ������ â�� ���� �� �����ϴ�.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ���� ���α׷����� ����� ��� ���� ��Ʈ�� Ŭ������ �����ϵ���
	// �� �׸��� �����Ͻʽÿ�.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox( _T("AfxSocketInit() Initial Fail !") );
		return FALSE;
	}

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}
	AfxEnableControlContainer();
	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("GaonNuri SDV D-AMT"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.

	ReadSystemFile();

	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CGaonNuriDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CGaonNuriView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);



	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();
	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.

	Initialize();

// 	for(int i = 0; i < MAX_INTERLOCK_MES; i++)
// 		g_pData->m_tInterlockMES[i].Clear();

	return TRUE;
}

BOOL CGaonNuriApp::Initialize(void)
{
	strcpy_s(g_pData->m_cVersionUseCellLog, _PROGRAM_VERSION);			// Program version ����


	((CMainFrame*)m_pMainWnd)->InitializeSocket();

	// �ʱ� ȭ������ ǥ���Ѵ�.
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_CIM, (LPARAM)0);

	//d ((CMainFrame *)AfxGetMainWnd())->OnMenuSelect(IDC_BUTTON_MENU_CIM);

	// �⺻ ���� ǥ��
	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_INFO_UPDATE, 0, 0);		// �⺻���� ���� ������ ǥ���Ѵ�.


	return TRUE;
}


#define PATH_SYSTEM	_T("D:\\YWDSP\\MachineData")
void CGaonNuriApp::ReadSystemFile()
{
	CString strPath;
	strPath.Format(_T("%s\\Ibis_CIM.ini"), PATH_SYSTEM);

	CIni ini(strPath);

	// PATH
	//kjpark 20170901 LOG ��� ����
	m_strMainPath = ini.GetString(_T("PATH"), _T("m_strMainPath"), _T("D:\\YWDSP"));
}

// ���� ���α׷� ������ ���Ǵ� CAboutDlg ��ȭ �����Դϴ�.

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_ABOUTBOX };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

// �����Դϴ�.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()

// ��ȭ ���ڸ� �����ϱ� ���� ���� ���α׷� ����Դϴ�.
void CGaonNuriApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CGaonNuriApp �޽��� ó����



const char *const CGaonNuriApp::GetApplicationPath()
{
	// ��� ����Ÿ path�� ���� ���丮�� ..�� sub���丮 �̴�
	static BOOL fInit = FALSE;

	if (fInit == FALSE) {
		fInit = TRUE;

		::GetCurrentDirectory(_MAX_PATH, m_strExePath);
		sprintf_s(m_strAppPath, "%s\\..\\", m_strExePath);
	}

	return m_strAppPath;
}