#pragma once
#include "Automation.h"
#include "afxcmn.h"
#include "afxwin.h"
#include "CimClass/EquipFunction.h"
#include "MachineClass/UnitStatusBank.h"
#include "XComProCtrl.h"

// CViewCIM form view

class CEquipFunction;
class CViewCIM : public CFormView
{
	DECLARE_DYNCREATE(CViewCIM)

	//protected:
public:
	CViewCIM();           // protected constructor used by dynamic creation
	virtual ~CViewCIM();

public:
	enum { IDD = IDD_VIEW_CIM };
#ifdef _DEBUG
	virtual void AssertValid() const;
#ifndef _WIN32_WCE
	virtual void Dump(CDumpContext& dc) const;
#endif
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()

	virtual void OnInitialUpdate();

	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnDestroy();

public:


	//enum {
	//	eTMR_HostConnect			= 0,						// HOST 접속을 시도한다.

	//};
	BOOL m_bXComInit;
	CXComProCtrl m_XComPro;										// XCom OCX
	CAutomation	m_Automation;								// HOST<->EQP 메시지 통신 프로세서

	CListBox m_lstbxMsg;									// CIM 로그 표시 ListBox
	CListBox m_lstbxCommMsg;								// COMM 로그 표시 ListBox

	BOOL m_bLogByMesgPass;									// XCom 자체 통신 로그 작성 여부

	BOOL bWriteDataEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	

	// 이전 상태 기억
	BOOL	m_bEQConnectFlag;									// 운영PC와의 TCP/IP 연결 상태 표시


public:
	void SecsEventXcomproctrl1(short nEventId, long lParam);
	void SecsMsgXcomproctrl1();
	
	void SetConfig_XComLog();								// XCom Log 설정 지정
	void SetConnect_XComComm();								// XCom 연결 체크
	


	void ViewCIMEqStateDip();
	void ViewCIMWriteDip(LPCTSTR szLogText);
	void ViewCOMWriteDip(LPCTSTR szLogText);

	BOOL SetCellInformation(CString strCellID, CString strProductID, CString strCellInfoResult);
	BOOL SetCellLotInformation(CString strCellID, CString sName, CString sValue);
	BOOL SetCellLotDefectInformation(CString strCellID, int nValCnt);//20170502 BKH, 삼성 요청사항으로 옵션코드 S6F205, 불량 Defect 정보를 보냄 
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid 추가
	BOOL SetCellJobPrecessState(BOOL bJobState , CString strCellID, CString strProductID, CString strStepID, CString strJobState );//JobProcessState

	BOOL SetPPIDCreate(CString strRecipeName);	// 상위로부터 신규 레시피 생성 [9/27/2017 OSC]

	BOOL SetOPCall(CString strModuleID);
	BOOL SetInterlock(CString strInterlockMode);
	BOOL SetInterlockRequest(int RCMD, CString strUnitID);
	void EventInterlockRequestAck(CString RCMD, BOOL bAck);
	void SetPPIDChange(void);
	BOOL SetTerminalMessage();	
	BOOL SetControlState(void);										// 운영측에 CIM 상태를 전송하기 위해 데이터 처리를 진행한다.
	BOOL SetDataPCVersion(void);
	BOOL GetLineID(void);
	void EventChangeControlState(void);
	void LoadConfig(void);
	void SaveConfig(void);
	void EventCellProcessStart();
	void EventCellProcessComplete(UINT nUnit, UINT nCellPos);
	void EventCellEndInspection(UINT nJig, UINT nCellPos);
	void EventEquipmentStatusChange();
	void EventLossCode();		//20141223 sms 
	void EventLogIn_Out();		//GT.PARK - Log In/Out
	void EquipmentStatusChange(E_EQST eEqst, CString cOn_Off);
	BOOL EquipmentStatusCheck(E_EQST eEqst, CString cOn_Off);
	BOOL SetEqStatus(short sEqst);
	void ControlStateChange(E_CRST eCRST);
	void EventPPIDCreate();
	void EventPPIDDelete();
	void EventPPIDModify();
	void EventPPIDChange();
	void EventPPIDOperation();
	void EventMaterialChange();
	void EventOPCall();
	void EventInterlock();
	void EventCellInfoRequest();
	void EventCellLotInfoRequest(CString cEventCode);
	void EventInspectionResultReport();	// 2016-11-17, jhLee, S6F11, CEID 609, 주요 검사 완료 시점에 이벤트를 보내도록 한다.

	// void EventRepairInfoRequest();
	void EventCodeInfoRequest();
	void AlarmReportSend();
	void EventERMS();

	void SetEquipFunctionChange(CString strEFID, CString strEFST, CString strMSG);
	void EventEquipFunctionChangeAck(int nAck);


	// 2016-06-03, jhLee
	// QMS 및 비가동 Log 기록 기능 삭제
	//
	// void WriteQMSLog(UINT nJig, UINT nCellPos);
	// 
	// 비가동율 정합성Log 추가 2014.03.25[JYC]
	// void WriteDownLog();
	// void WriteStateChangeLog();
	// CString GetQMSReturnString(E_QMS_RETURN eQMSRtn);

	BOOL SetUseLogByMesg(BOOL bOption);
	BOOL SetLogByMesgFlagPath();
	BOOL MakeFileUseLogingByMessage();
	BOOL SetInitUseLogByMesg();

public:

	// 2016-06-03, jhLee, QMS 및 비가동 관련 Log 기록 Thread 삭제
	//// Thread 
	//void CreateThread();
	//void DeleteThread();
	//CWinThread* m_pWriteDataThread;
	//HANDLE		m_hWriteDataThread;
	//static UINT thrWriteDataThread(LPVOID pParam);
	//BOOL m_bWriteDataThreadRun;

	CEquipFunction m_EquipFunction;
	void EventEquipFunctionUpdate(tag_Message *pMsg);
	void EventEquipFunctionChange(EQUIP_FUNCTION* function, UINT nIndexSize,CString strBYWHO);

	CUnitStatusBank m_UnitStatusBank;
	void SetUnitStatusChange(int nJig, int nCh);
	void SetUnitInterlockConfirm(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID);
	void SetUnitOPCallConfirm(int nJig, int nCh, CString strCellID, CString strProductID, CString strStepID);

public:
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedBtnViewCimMinimize();

	afx_msg LRESULT OnEQConnect(WPARAM wParam, LPARAM lParam);					// 운영PC와 연결 
	afx_msg LRESULT OnEQDisconnect(WPARAM wParam, LPARAM lParam);				// 운영PC와 연결 끊김
	afx_msg LRESULT OnHostCommUpdate(WPARAM wParam, LPARAM lParam);			// HOST 통신 데이터 수신 표시


	void ClickGxbtnCommMode1();
	void ClickGxbtnCommMode2();
	void ClickGxbtnCommMode3();
	void ClickGxbtnCommConnect();
};

