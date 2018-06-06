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
	//	eTMR_HostConnect			= 0,						// HOST ������ �õ��Ѵ�.

	//};
	BOOL m_bXComInit;
	CXComProCtrl m_XComPro;										// XCom OCX
	CAutomation	m_Automation;								// HOST<->EQP �޽��� ��� ���μ���

	CListBox m_lstbxMsg;									// CIM �α� ǥ�� ListBox
	CListBox m_lstbxCommMsg;								// COMM �α� ǥ�� ListBox

	BOOL m_bLogByMesgPass;									// XCom ��ü ��� �α� �ۼ� ����

	BOOL bWriteDataEvent[MAX_JIG_CNT][MAX_JIG_CELL_CNT];
	

	// ���� ���� ���
	BOOL	m_bEQConnectFlag;									// �PC���� TCP/IP ���� ���� ǥ��


public:
	void SecsEventXcomproctrl1(short nEventId, long lParam);
	void SecsMsgXcomproctrl1();
	
	void SetConfig_XComLog();								// XCom Log ���� ����
	void SetConnect_XComComm();								// XCom ���� üũ
	


	void ViewCIMEqStateDip();
	void ViewCIMWriteDip(LPCTSTR szLogText);
	void ViewCOMWriteDip(LPCTSTR szLogText);

	BOOL SetCellInformation(CString strCellID, CString strProductID, CString strCellInfoResult);
	BOOL SetCellLotInformation(CString strCellID, CString sName, CString sValue);
	BOOL SetCellLotDefectInformation(CString strCellID, int nValCnt);//20170502 BKH, �Ｚ ��û�������� �ɼ��ڵ� S6F205, �ҷ� Defect ������ ���� 
	//kjpark 20180123 CELL INFO, JOB PROCESS Cellid �߰�
	BOOL SetCellJobPrecessState(BOOL bJobState , CString strCellID, CString strProductID, CString strStepID, CString strJobState );//JobProcessState

	BOOL SetPPIDCreate(CString strRecipeName);	// �����κ��� �ű� ������ ���� [9/27/2017 OSC]

	BOOL SetOPCall(CString strModuleID);
	BOOL SetInterlock(CString strInterlockMode);
	BOOL SetInterlockRequest(int RCMD, CString strUnitID);
	void EventInterlockRequestAck(CString RCMD, BOOL bAck);
	void SetPPIDChange(void);
	BOOL SetTerminalMessage();	
	BOOL SetControlState(void);										// ����� CIM ���¸� �����ϱ� ���� ������ ó���� �����Ѵ�.
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
	void EventInspectionResultReport();	// 2016-11-17, jhLee, S6F11, CEID 609, �ֿ� �˻� �Ϸ� ������ �̺�Ʈ�� �������� �Ѵ�.

	// void EventRepairInfoRequest();
	void EventCodeInfoRequest();
	void AlarmReportSend();
	void EventERMS();

	void SetEquipFunctionChange(CString strEFID, CString strEFST, CString strMSG);
	void EventEquipFunctionChangeAck(int nAck);


	// 2016-06-03, jhLee
	// QMS �� �񰡵� Log ��� ��� ����
	//
	// void WriteQMSLog(UINT nJig, UINT nCellPos);
	// 
	// �񰡵��� ���ռ�Log �߰� 2014.03.25[JYC]
	// void WriteDownLog();
	// void WriteStateChangeLog();
	// CString GetQMSReturnString(E_QMS_RETURN eQMSRtn);

	BOOL SetUseLogByMesg(BOOL bOption);
	BOOL SetLogByMesgFlagPath();
	BOOL MakeFileUseLogingByMessage();
	BOOL SetInitUseLogByMesg();

public:

	// 2016-06-03, jhLee, QMS �� �񰡵� ���� Log ��� Thread ����
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

	afx_msg LRESULT OnEQConnect(WPARAM wParam, LPARAM lParam);					// �PC�� ���� 
	afx_msg LRESULT OnEQDisconnect(WPARAM wParam, LPARAM lParam);				// �PC�� ���� ����
	afx_msg LRESULT OnHostCommUpdate(WPARAM wParam, LPARAM lParam);			// HOST ��� ������ ���� ǥ��


	void ClickGxbtnCommMode1();
	void ClickGxbtnCommMode2();
	void ClickGxbtnCommMode3();
	void ClickGxbtnCommConnect();
};

