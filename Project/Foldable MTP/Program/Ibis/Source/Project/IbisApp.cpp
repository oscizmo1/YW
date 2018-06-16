
// Ibis.cpp : 응용 프로그램에 대한 클래스 동작을 정의합니다.
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


// CIbisApp 생성

CIbisApp::CIbisApp()
{
	// 다시 시작 관리자 지원
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_ALL_ASPECTS;
#ifdef _MANAGED
	// 응용 프로그램을 공용 언어 런타임 지원을 사용하여 빌드한 경우(/clr):
	//     1) 이 추가 설정은 다시 시작 관리자 지원이 제대로 작동하는 데 필요합니다.
	//     2) 프로젝트에서 빌드하려면 System.Windows.Forms에 대한 참조를 추가해야 합니다.
	System::Windows::Forms::Application::SetUnhandledExceptionMode(System::Windows::Forms::UnhandledExceptionMode::ThrowException);
#endif

	// TODO: 아래 응용 프로그램 ID 문자열을 고유 ID 문자열로 바꾸십시오(권장).
	// 문자열에 대한 서식: CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("YWDSP.BaseFrame.Ibis.Prototype"));

	// TODO: 여기에 생성 코드를 추가합니다.
	// InitInstance에 모든 중요한 초기화 작업을 배치합니다.

	//////////////////////////////////////////////////////////////////////////
	// Mempory Leak 추적
	BOOL bCheck = 0;
	if(bCheck)
	{
		long num = 444927;
		// 		_CrtMemDumpAllObjectsSince(0); 
		_CrtSetBreakAlloc(num);
	}
	//////////////////////////////////////////////////////////////////////////
}

// 유일한 CIbisApp 개체입니다.

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

// CIbisApp 초기화

BOOL CIbisApp::InitInstance()
{

	// Start of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	// 프로그램 중복 실행 방지 (Mutex 이용)

	// 중복 실행방지用 Mutex 생성
    HANDLE hMutexOneInstance = ::CreateMutex(NULL, TRUE, _T("YW Ibis")); 
	
    // 만약 이미 만들어져 있다면 Instance가 이미 존재함
    BOOL bDuplicateInstance = (::GetLastError() == ERROR_ALREADY_EXISTS); 
	
    if( hMutexOneInstance ) ::ReleaseMutex(hMutexOneInstance); 
	
    // 이미 하나의 Instance가 존재하면 프로그램 종료
    if( bDuplicateInstance ) {
        AfxMessageBox(_T("The program is already running."), MB_ICONERROR | MB_OK);
        return FALSE;
    }
	// end of GxGUI -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	//////////////////////////////////////////////////////////////////////////
	// Parameter Load
	theConfigBank.ReadSystemFile();	// 얘가 최우선으로 Read되어야 한다
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

	

	theTeachBank.InitializeTeachParam();		// File Read하지 않을 고정값 설정
	theTeachBank.ReadTeachFile();	

	theAxisBank.InitializeAxisType();	// File Read하지 않을 고정값 설정
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


	// 콘솔 프로젝트에서 메모리릭 정보가 출력 창에 출력될 수 있도록 추가
	// _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

	// 응용 프로그램 매니페스트가 ComCtl32.dll 버전 6 이상을 사용하여 비주얼 스타일을
	// 사용하도록 지정하는 경우, Windows XP 상에서 반드시 InitCommonControlsEx()가 필요합니다. 
	// InitCommonControlsEx()를 사용하지 않으면 창을 만들 수 없습니다.
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 응용 프로그램에서 사용할 모든 공용 컨트롤 클래스를 포함하도록
	// 이 항목을 설정하십시오.
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	if (!AfxSocketInit())
	{
		AfxMessageBox(IDP_SOCKETS_INIT_FAILED);
		return FALSE;
	}

	// OLE 라이브러리를 초기화합니다.
	if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}

	AfxEnableControlContainer();

	EnableTaskbarInteraction(FALSE);

	// RichEdit 컨트롤을 사용하려면  AfxInitRichEdit2()가 있어야 합니다.	
	// AfxInitRichEdit2();

	// 표준 초기화
	// 이들 기능을 사용하지 않고 최종 실행 파일의 크기를 줄이려면
	// 아래에서 필요 없는 특정 초기화
	// 루틴을 제거해야 합니다.
	// 해당 설정이 저장된 레지스트리 키를 변경하십시오.
	// TODO: 이 문자열을 회사 또는 조직의 이름과 같은
	// 적절한 내용으로 수정해야 합니다.
	SetRegistryKey(_T("로컬 응용 프로그램 마법사에서 생성된 응용 프로그램"));
	LoadStdProfileSettings(4);  // MRU를 포함하여 표준 INI 파일 옵션을 로드합니다.


	// 응용 프로그램의 문서 템플릿을 등록합니다. 문서 템플릿은
	//  문서, 프레임 창 및 뷰 사이의 연결 역할을 합니다.
	CSingleDocTemplate* pDocTemplate;
	pDocTemplate = new CSingleDocTemplate(
		IDR_MAINFRAME,
		RUNTIME_CLASS(CIbisDoc),
		RUNTIME_CLASS(CMainFrame),       // 주 SDI 프레임 창입니다.
		RUNTIME_CLASS(CIbisView));
	if (!pDocTemplate)
		return FALSE;
	AddDocTemplate(pDocTemplate);


	// 표준 셸 명령, DDE, 파일 열기에 대한 명령줄을 구문 분석합니다.
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);


	// GDI+ 
	GdiplusStartup(&gdiplusToken, &gdiplusstartupinput, NULL);

	// 명령줄에 지정된 명령을 디스패치합니다.
	// 응용 프로그램이 /RegServer, /Register, /Unregserver 또는 /Unregister로 시작된 경우 FALSE를 반환합니다.
	if (!ProcessShellCommand(cmdInfo))
		return FALSE;

	theConfigBank.ReadLastDataFile();

	//20161230 BKH 비가동list 파일 불러오기
	theLossTimeLog.LoadLosstimeItemList();
	theLossTimeLog.ReadLastLossTimeItem();

	// 창 하나만 초기화되었으므로 이를 표시하고 업데이트합니다.
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

// 	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_INIT);
	theProcBank.ChangeMachineState(CONST_MACHINE_STATE::MACHINE_PAUSE);
	theLog[LOG_SYSTEM].AddBuf(_T("Program Start %s"), PROGRAM_VERSION);
// 	theUnitFunc.SetOutPutIO(Y_MACHINE_LED_ON,OFF);

	// 한번도 안찍고 SaveImage하면 오류나서 프로그램 시작시 트리거 한번 살려준다

	//kjpark 20170902 Teach Shuttle UI 및 버튼 동작 추가
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

	// 접미사가 있을 경우에만 DragAcceptFiles를 호출합니다.
	//  SDI 응용 프로그램에서는 ProcessShellCommand 후에 이러한 호출이 발생해야 합니다.


	//
	// Visual Studio 2005, 2008 ~ 에서 TRACE 문 사용 시 한글을 사용할 경우,
	// _CrtDbgReport: String too long or IO Error 오류가 발생시,
	//
	// #include <locale.h>     include 한 후
	//
	// _tsetlocale(LC_ALL, _T("korean"));  호출
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
	//TODO: 추가한 추가 리소스를 처리합니다.
	//kjpark 20160912 socket test
	theSocketInterFace.DestroyWindow ();
	// GDI+ 用
	GdiplusShutdown(gdiplusToken);


	AfxOleTerm(FALSE);

	theLog[LOG_SYSTEM].AddBuf(_T("Program Exit\n"));


	// _CrtDumpMemoryLeaks();				// Meory Leak 정보 표시

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

	//kjpark 20161121 PG 소켓 로그 따로 저장
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

	//kjpark 20180114 NG 판정 통계 UI 추가
	theLog[LOG_NG_RESULT_STATISTICS].SetType(CLogUnit::WriteType_NoWait);
	theLog[LOG_NG_RESULT_STATISTICS].SetFolderPath(theConfigBank.m_System.m_strDebugLogPath);
	theLog[LOG_NG_RESULT_STATISTICS].SetFileName(_T("NG_Result_Statistics.txt"));
	theLog[LOG_NG_RESULT_STATISTICS].SetOffsetHour(theConfigBank.m_Option.m_OffsetHour);
	theLog[LOG_NG_RESULT_STATISTICS].SetAddTime(TRUE);
	theLog[LOG_NG_RESULT_STATISTICS].SetAddDay(FALSE);
	theLog[LOG_NG_RESULT_STATISTICS].SetPartition(FALSE);

	

}
