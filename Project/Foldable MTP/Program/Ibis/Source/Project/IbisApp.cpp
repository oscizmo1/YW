
// Ibis.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "afxwinappex.h"

#include "IbisApp.h"
#include "MainFrm.h"
#include "IbisDoc.h"
#include "IbisView.h"

#include <locale.h> 

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


// CIbisApp

BEGIN_MESSAGE_MAP(CIbisApp, CWinApp)

END_MESSAGE_MAP()


// CIbisApp ����

CIbisApp::CIbisApp()
{
	// �ٽ� ���� ������ ����
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// ���� ���α׷��� ���� ��� ��Ÿ�� ������ ����Ͽ� ������ ���(/clr):
	//     1) �� �߰� ������ �ٽ� ���� ������ ������ ����� �۵��ϴ� �� �ʿ��մϴ�.
	//     2) ������Ʈ���� �����Ϸ��� System.Windows.Forms�� ���� ������ �߰��ؾ� �մϴ�.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: �Ʒ� ���� ���α׷� ID ���ڿ��� ���� ID ���ڿ��� �ٲٽʽÿ�(����).
	// ���ڿ��� ���� ����: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("YWDSP.BaseFrame.Ibis.Prototype"));

	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.

	//////////////////////////////////////////////////////////////////////////
	// Mempory Leak ����
	BOOL bCheck = 0;
	if(bCheck)
	{
		long num = 444927;
		// 		_CrtMemDumpAllObjectsSince(0); 
		_CrtSetBreakAlloc(num);
	}
	//////////////////////////////////////////////////////////////////////////
}

// ������ CIbisApp ��ü�Դϴ�.

CIbisApp			theApp;
CThreadInitializer	theThreadInitializer;
CLogUnit			theLog[LOG_MAX];
CLossTimeLog		theLossTimeLog;
CAxisParamBank		theAxisBank;
CCellBank			theCellBank;
CIOParamBank		theIOBank;
CConfigBank			theConfigBank;
CProcDataBank		theProcBank;
CRecipeBank			theRecipeBank;
CTeachBank			theTeachBank;
CFDCBank			theFDCBank;
CUnitCtrlFunc		theUnitFunc(MP2100_THREAD_MAIN_PROCESS);
CDeviceMotion		theDeviceMotion;
CDeviceIO			theDeviceIO;

//kjpark 20160912 socket test
CSocketDialog		theSocketInterFace;
CSerialManager		theSerialInterFace;

CDevicePDT_IF		theDevicePDT_IF;

CProductBank		theProductBank;
CTactTimeLog		theTactTimeLog;

CInspModuleBank		theInspModuleBank;
CThreadBank			theThreadBank;
CUnitStatusBank		theUnitStatusBank;

// CIbisApp �ʱ�ȭ

BOOL CIbisApp::InitInstance()
{

	// Start of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// ���α׷� �ߺ� ���� ���� (Mutex �̿�)

	// �ߺ� ��������� Mutex ����
    HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, _T("YW Ibis")); 
	
    // ���� �̹� ������� �ִٸ� Instance�� �̹� ������
    BOOL bDuplicateInstance = (::GetLastError() == ERROR_ALREADY_EXISTS); 
	
    if( hMutexOneInstance ) ::ReleaseMutex(hMutexOneInstance); 
	
    // �̹� �ϳ��� Instance�� �����ϸ� ���α׷� ����
    if( bDuplicateInstance ) {
        AfxMessageBox(_T("The program is already running."), MB_ICONERROR | MB_OK);
        return FALSE;
    }
	// end of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//////////////////////////////////////////////////////////////////////////
	// Parameter Load
	theConfigBank.ReadSystemFile();	// �갡 �ֿ켱���� Read�Ǿ�� �Ѵ�
	theConfigBank.ReadOptionFile();
	theConfigBank.ProgramRestartOptionInit();

	theConfigBank.ReadTimeOutFile();
	//kjpark 20161121 MES code list read
	theConfigBank.ReadNGCodeFile();
	theConfigBank.ReadNGAlarmLimitFile();
	theConfigBank.ReadDefectPatternFile();
	theConfigBank.ReadDefectRetestFile();
	theConfigBank.ReadBZoneDefectCodeFile();
	theConfigBank.ReadCIMFile();


	// Log Initialize
	InitializeLog();


	theProcBank.ReadAlarmFile();
	theProcBank.ReadPDTAlarmFile();

	theRecipeBank.ReadLastRecipeFile();

	

	theTeachBank.InitializeTeachParam();		// File Read���� ���� ������ ����
	theTeachBank.ReadTeachFile();	

	theAxisBank.InitializeAxisType();	// File Read���� ���� ������ ����
	theAxisBank.ReadAxisParam();

	theIOBank.InitializeIOParam();

	theFDCBank.InitItem();

	theProductBank.MainCount_Load();
	theProductBank.LoadReportFileToday();

	theUnitStatusBank.ReadStatus();
// 	theUnitStatusBank.StatusInit();
	theProcBank.AZoneChannelNotUse_SyncUnitStatus();

	theCellBank.RestoreCellToFile();

	//////////////////////////////////////////////////////////////////////////
	// Device Open
	//if(theDeviceMotion.Open_Mp2100(MP2100_THREAD_MAIN_PROCESS) == FALSE)
	//{
	//	theLog[LOG_SYSTEM].AddBuf(_T("MP2100 Board Open Fail"));
	//}
 	if(theDeviceMotion.Open_NMC2() == FALSE)
 	{
 		theLog[LOG_SYSTEM].AddBuf(_T("NMC Open Fail"));
 	}
	if(theDeviceIO.Open() == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("DeviceNet Board Open Fail"));
	}
	theDevicePDT_IF.SetMachineIndex((MACHINE_INDEX)theConfigBank.m_System.m_MachineIndex);
	if(theDevicePDT_IF.Open() == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("MELSEC CC-Link IE Board Open Fail"));
	}
	
	if(theSerialInterFace.SerialOpen(SERIAL_LABELPRINT) == FALSE)
	{
		theLog[LOG_SYSTEM].AddBuf(_T("Label Printer 1ST Port Open Fail"));
	}
	//////////////////////////////////////////////////////////////////////////
	// Create Thread
	theThreadBank.CreateThread();

	//////////////////////////////////////////////////////////////////////////


	// �ܼ� ������Ʈ���� �޸𸮸� ������ ��� â�� ��µ� �� �ֵ��� �߰�
	// _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

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
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE ���̺귯���� �ʱ�ȭ�մϴ�.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit ��Ʈ���� ����Ϸ���  AfxInitRichEdit2()�� �־�� �մϴ�.	
	// AfxInitRichEdit2();

	// ǥ�� �ʱ�ȭ
	// �̵� ����� ������� �ʰ� ���� ���� ������ ũ�⸦ ���̷���
	// �Ʒ����� �ʿ� ���� Ư�� �ʱ�ȭ
	// ��ƾ�� �����ؾ� �մϴ�.
	// �ش� ������ ����� ������Ʈ�� Ű�� �����Ͻʽÿ�.
	// TODO: �� ���ڿ��� ȸ�� �Ǵ� ������ �̸��� ����
	// ������ �������� �����ؾ� �մϴ�.
	SetRegistryKey(_T("���� ���� ���α׷� �����翡�� ������ ���� ���α׷�"));
	LoadStdProfileSettings(4);  // MRU�� �����Ͽ� ǥ�� INI ���� �ɼ��� �ε��մϴ�.


	// ���� ���α׷��� ���� ���ø��� ����մϴ�. ���� ���ø���
	//  ����, ������ â �� �� ������ ���� ������ �մϴ�.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIbisDoc),
		RUNTIME_CLASS(CMainFrame),       // �� SDI ������ â�Դϴ�.
		RUNTIME_CLASS(CIbisView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// ǥ�� �� ���, DDE, ���� ���⿡ ���� ������� ���� �м��մϴ�.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// GDI+ 
	GdiplusStartup(&gdiplusToken, &gdiplusstartupinput, NULL);

	// ����ٿ� ������ ����� ����ġ�մϴ�.
	// ���� ���α׷��� /RegServer, /Register, /Unregserver �Ǵ� /Unregister�� ���۵� ��� FALSE�� ��ȯ�մϴ�.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	theConfigBank.ReadLastDataFile();

	//20161230 BKH �񰡵�list ���� �ҷ�����
	theLossTimeLog.LoadLosstimeItemList();
	theLossTimeLog.ReadLastLossTimeItem();

	// â �ϳ��� �ʱ�ȭ�Ǿ����Ƿ� �̸� ǥ���ϰ� ������Ʈ�մϴ�.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

// 	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INIT);
	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	theLog[LOG_SYSTEM].AddBuf(_T("Program Start %s"), PROGRAM_VERSION);
// 	theUnitFunc.SetOutPutIO(Y_MACHINE_LED_ON,OFF);

	// �ѹ��� ����� SaveImage�ϸ� �������� ���α׷� ���۽� Ʈ���� �ѹ� ����ش�

	//kjpark 20170902 Teach Shuttle UI �� ��ư ���� �߰�
	for(int jig = JIG_ID_A ; jig < JIG_ID_MAX ; jig++)
	{
		for(int ch = JIG_CH_1 ; ch < JIG_CH_MAX ; ch++)
		{
			theUnitFunc.MCR_Trigger((JIG_ID)jig, (JIG_CH)ch, TRUE);
		}
	}	
	Sleep(100);
	for(int jig = JIG_ID_A ; jig < JIG_ID_MAX ; jig++)
	{
		for(int ch = JIG_CH_1 ; ch < JIG_CH_MAX ; ch++)
		{
			theUnitFunc.MCR_Trigger((JIG_ID)jig, (JIG_CH)ch, FALSE);
		}
	}

	GetMainHandler()->LoadLastUserInspector();

	// ���̻簡 ���� ��쿡�� DragAcceptFiles�� ȣ���մϴ�.
	//  SDI ���� ���α׷������� ProcessShellCommand �Ŀ� �̷��� ȣ���� �߻��ؾ� �մϴ�.


	//
	// Visual Studio 2005, 2008 ~ ���� TRACE �� ��� �� �ѱ��� ����� ���,
	// _CrtDbgReport: String too long or IO Error ������ �߻���,
	//
	// #include <locale.h>     include �� ��
	//
	// _tsetlocale(LC_ALL, _T("korean"));  ȣ��
	//
	CString a;
	a.Format(_T("%s"), _tsetlocale(LC_ALL, _T("korean"))); 
	//kjpark 20160912 socket test
	theSocketInterFace.Create(CSocketDialog::IDD, m_pMainWnd);
	theSocketInterFace.ShowWindow(SW_HIDE);

	theSocketInterFace.SetSocketConnect();	

	theProcBank.m_bProgramInitFinish = TRUE;

	return TRUE;
}

int CIbisApp::ExitInstance()
{
	//TODO: �߰��� �߰� ���ҽ��� ó���մϴ�.
	//kjpark 20160912 socket test
	theSocketInterFace.DestroyWindow ();
	// GDI+ ��
	GdiplusShutdown(gdiplusToken);


	AfxOleTerm(FALSE);

	theLog[LOG_SYSTEM].AddBuf(_T("Program Exit\n"));


	// _CrtDumpMemoryLeaks();				// Meory Leak ���� ǥ��

	return CWinApp::ExitInstance();
}

void CIbisApp::InitializeLog()
{
	theLog[LOG_SYSTEM].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SYSTEM].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SYSTEM].SetFileName(_T("System.txt"));
	theLog[LOG_SYSTEM].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SYSTEM].SetOffsetMin(0);
	theLog[LOG_SYSTEM].SetAddTime(TRUE);
	theLog[LOG_SYSTEM].SetAddDay(FALSE);
	theLog[LOG_SYSTEM].SetPartition(FALSE);

	theLog[LOG_SEQUENCE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE].SetFileName(_T("Sequence.txt"));
	theLog[LOG_SEQUENCE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_RESTART].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_RESTART].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_RESTART].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_RESTART].SetFileName(_T("Sequence_Restart.txt"));
	theLog[LOG_SEQUENCE_RESTART].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_RESTART].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_RESTART].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_RESTART].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_RESTART].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_AZONE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_AZONE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_AZONE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_AZONE].SetFileName(_T("Sequence_AZone.txt"));
	theLog[LOG_SEQUENCE_AZONE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_AZONE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_AZONE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_AZONE].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_AZONE].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_SHUTTLE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_SHUTTLE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_SHUTTLE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_SHUTTLE].SetFileName(_T("Sequence_Shuttle.txt"));
	theLog[LOG_SEQUENCE_SHUTTLE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_SHUTTLE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_SHUTTLE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_SHUTTLE].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_SHUTTLE].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_BZONE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_BZONE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_BZONE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_BZONE].SetFileName(_T("Sequence_BZone.txt"));
	theLog[LOG_SEQUENCE_BZONE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_BZONE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_BZONE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_BZONE].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_BZONE].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_CZONE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_CZONE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_CZONE].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_CZONE].SetFileName(_T("Sequence_CZone.txt"));
	theLog[LOG_SEQUENCE_CZONE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_CZONE].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_CZONE].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_CZONE].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_CZONE].SetPartition(FALSE);

	theLog[LOG_SEQUENCE_PDT_IF].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SEQUENCE_PDT_IF].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SEQUENCE_PDT_IF].SetSubFolderName(_T("Sequence"));
	theLog[LOG_SEQUENCE_PDT_IF].SetFileName(_T("Sequence_PDT_IF.txt"));
	theLog[LOG_SEQUENCE_PDT_IF].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SEQUENCE_PDT_IF].SetOffsetMin(0);
	theLog[LOG_SEQUENCE_PDT_IF].SetAddTime(TRUE);
	theLog[LOG_SEQUENCE_PDT_IF].SetAddDay(FALSE);
	theLog[LOG_SEQUENCE_PDT_IF].SetPartition(FALSE);

	theLog[LOG_SOCKET].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SOCKET].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SOCKET].SetSubFolderName(_T("Socket"));
	theLog[LOG_SOCKET].SetFileName(_T("Socket.txt"));
	theLog[LOG_SOCKET].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SOCKET].SetOffsetMin(0);
	theLog[LOG_SOCKET].SetAddTime(TRUE);
	theLog[LOG_SOCKET].SetAddDay(FALSE);
	theLog[LOG_SOCKET].SetPartition(FALSE);

	//kjpark 20161121 PG ���� �α� ���� ����
	theLog[LOG_OP2PG_PACKET].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_OP2PG_PACKET].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_OP2PG_PACKET].SetSubFolderName(_T("Socket"));
	theLog[LOG_OP2PG_PACKET].SetFileName(_T("OP2PGPacket.txt"));
	theLog[LOG_OP2PG_PACKET].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_OP2PG_PACKET].SetOffsetMin(0);
	theLog[LOG_OP2PG_PACKET].SetAddTime(TRUE);
	theLog[LOG_OP2PG_PACKET].SetAddDay(FALSE);
	theLog[LOG_OP2PG_PACKET].SetPartition(TRUE);

	theLog[LOG_SOCKET_PG].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SOCKET_PG].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SOCKET_PG].SetSubFolderName(_T("Socket"));
	theLog[LOG_SOCKET_PG].SetFileName(_T("Socket_PG.txt"));
	theLog[LOG_SOCKET_PG].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SOCKET_PG].SetOffsetMin(0);
	theLog[LOG_SOCKET_PG].SetAddTime(TRUE);
	theLog[LOG_SOCKET_PG].SetAddDay(FALSE);
	theLog[LOG_SOCKET_PG].SetPartition(TRUE);

	theLog[LOG_SOCKET_ERR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SOCKET_ERR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SOCKET_ERR].SetSubFolderName(_T("Socket"));
	theLog[LOG_SOCKET_ERR].SetFileName(_T("Socket_Err.txt"));
	theLog[LOG_SOCKET_ERR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SOCKET_ERR].SetOffsetMin(0);
	theLog[LOG_SOCKET_ERR].SetAddTime(TRUE);
	theLog[LOG_SOCKET_ERR].SetAddDay(FALSE);
	theLog[LOG_SOCKET_ERR].SetPartition(FALSE);

	theLog[LOG_CIM].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_CIM].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_CIM].SetFileName(_T("Cim.txt"));
	theLog[LOG_CIM].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_CIM].SetOffsetMin(0);
	theLog[LOG_CIM].SetAddTime(TRUE);
	theLog[LOG_CIM].SetAddDay(FALSE);
	theLog[LOG_CIM].SetPartition(FALSE);

	theLog[LOG_SERIAL].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SERIAL].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SERIAL].SetFileName(_T("Serial.txt"));
	theLog[LOG_SERIAL].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SERIAL].SetOffsetMin(0);
	theLog[LOG_SERIAL].SetAddTime(TRUE);
	theLog[LOG_SERIAL].SetAddDay(FALSE);
	theLog[LOG_SERIAL].SetPartition(FALSE);

	theLog[LOG_SERIAL_GMS].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SERIAL_GMS].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SERIAL_GMS].SetFileName(_T("Serial_GMS.txt"));
	theLog[LOG_SERIAL_GMS].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SERIAL_GMS].SetOffsetMin(0);
	theLog[LOG_SERIAL_GMS].SetAddTime(TRUE);
	theLog[LOG_SERIAL_GMS].SetAddDay(FALSE);
	theLog[LOG_SERIAL_GMS].SetPartition(FALSE);

	theLog[LOG_OPERATION].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_OPERATION].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_OPERATION].SetFileName(_T("Operation.txt"));
	theLog[LOG_OPERATION].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_OPERATION].SetOffsetMin(0);
	theLog[LOG_OPERATION].SetAddTime(TRUE);
	theLog[LOG_OPERATION].SetAddDay(FALSE);
	theLog[LOG_OPERATION].SetPartition(FALSE);

	theLog[LOG_ERROR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_ERROR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_ERROR].SetFileName(_T("Error.txt"));
	theLog[LOG_ERROR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_ERROR].SetOffsetMin(0);
	theLog[LOG_ERROR].SetAddTime(TRUE);
	theLog[LOG_ERROR].SetAddDay(FALSE);
	theLog[LOG_ERROR].SetPartition(FALSE);

	theLog[LOG_ERROR_TEMP].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_ERROR_TEMP].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_ERROR_TEMP].SetFileName(_T("Error_Temp.txt"));
	theLog[LOG_ERROR_TEMP].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_ERROR_TEMP].SetOffsetMin(0);
	theLog[LOG_ERROR_TEMP].SetAddTime(TRUE);
	theLog[LOG_ERROR_TEMP].SetAddDay(FALSE);
	theLog[LOG_ERROR_TEMP].SetPartition(FALSE);

	theLog[LOG_MP2100].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MP2100].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MP2100].SetFileName(_T("MP2100.txt"));
	theLog[LOG_MP2100].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MP2100].SetOffsetMin(0);
	theLog[LOG_MP2100].SetAddTime(TRUE);
	theLog[LOG_MP2100].SetAddDay(FALSE);
	theLog[LOG_MP2100].SetPartition(FALSE);

	theLog[LOG_NMC].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_NMC].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_NMC].SetFileName(_T("NMC.txt"));
	theLog[LOG_NMC].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_NMC].SetAddTime(TRUE);
	theLog[LOG_NMC].SetAddDay(FALSE);
	theLog[LOG_NMC].SetPartition(FALSE);

	theLog[LOG_FDC].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_FDC].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_FDC].SetFileName(_T("FDC.txt"));
	theLog[LOG_FDC].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_FDC].SetAddTime(TRUE);
	theLog[LOG_FDC].SetAddDay(FALSE);
	theLog[LOG_FDC].SetPartition(FALSE);


	theLog[LOG_MOTOR_ERROR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MOTOR_ERROR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MOTOR_ERROR].SetFileName(_T("MotorError.txt"));
	theLog[LOG_MOTOR_ERROR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MOTOR_ERROR].SetAddTime(TRUE);
	theLog[LOG_MOTOR_ERROR].SetAddDay(FALSE);
	theLog[LOG_MOTOR_ERROR].SetPartition(FALSE);

	
	theLog[LOG_JUDGE].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_JUDGE].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_JUDGE].SetFileName(_T("Judge.txt"));
	theLog[LOG_JUDGE].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_JUDGE].SetAddTime(TRUE);
	theLog[LOG_JUDGE].SetAddDay(FALSE);
	theLog[LOG_JUDGE].SetPartition(FALSE);

	theLog[LOG_TRACKING].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TRACKING].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TRACKING].SetFileName(_T("Tracking.txt"));
	theLog[LOG_TRACKING].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TRACKING].SetAddTime(TRUE);
	theLog[LOG_TRACKING].SetAddDay(FALSE);
	theLog[LOG_TRACKING].SetPartition(FALSE);

	theLog[LOG_MCR].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_MCR].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_MCR].SetFileName(_T("MCR.txt"));
	theLog[LOG_MCR].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_MCR].SetAddTime(TRUE);
	theLog[LOG_MCR].SetAddDay(FALSE);
	theLog[LOG_MCR].SetPartition(FALSE);

	theLog[LOG_TACT_TIME].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_TACT_TIME].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_TACT_TIME].SetFileName(_T("Tact Time.csv"));
	theLog[LOG_TACT_TIME].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_TACT_TIME].SetAddTime(TRUE);
	theLog[LOG_TACT_TIME].SetAddDay(FALSE);
	theLog[LOG_TACT_TIME].SetPartition(FALSE);

	theLog[LOG_RETRY].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_RETRY].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_RETRY].SetFileName(_T("Retry.txt"));
	theLog[LOG_RETRY].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_RETRY].SetAddTime(TRUE);
	theLog[LOG_RETRY].SetAddDay(FALSE);
	theLog[LOG_RETRY].SetPartition(FALSE);

	theLog[LOG_SPECIAL_PROCESS].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_SPECIAL_PROCESS].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_SPECIAL_PROCESS].SetFileName(_T("SpecialProcess.txt"));
	theLog[LOG_SPECIAL_PROCESS].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_SPECIAL_PROCESS].SetAddTime(TRUE);
	theLog[LOG_SPECIAL_PROCESS].SetAddDay(FALSE);
	theLog[LOG_SPECIAL_PROCESS].SetPartition(FALSE);

	//kjpark 20180114 NG ���� ��� UI �߰�
	theLog[LOG_NG_RESULT_STATISTICS].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_NG_RESULT_STATISTICS].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_NG_RESULT_STATISTICS].SetFileName(_T("NG_Result_Statistics.txt"));
	theLog[LOG_NG_RESULT_STATISTICS].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_NG_RESULT_STATISTICS].SetAddTime(TRUE);
	theLog[LOG_NG_RESULT_STATISTICS].SetAddDay(FALSE);
	theLog[LOG_NG_RESULT_STATISTICS].SetPartition(FALSE);

	

}
