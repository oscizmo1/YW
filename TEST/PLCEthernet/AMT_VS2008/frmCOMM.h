#pragma once
#include "xnetctrl.h"
#include "MNetH.h"
#include "afxwin.h"
#include "msflexgrid_pos.h"

// CFormCOMM form view

class CFormCOMM : 
	public CFormView
{
	DECLARE_DYNCREATE(CFormCOMM)

	virtual ~CFormCOMM();

public:
	CFormCOMM();           // protected constructor used by dynamic creation
	enum { IDD = IDD_FORM_COMM };
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
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);
	virtual void OnInitialUpdate();

	CXnetctrl1 XNetCtrl;
	DECLARE_EVENTSINK_MAP()
	void InitXNet();
	void ReceiveXnetctrl(long mesgType, long mesgID, long mesgLen);
	void CommCloseXNetCtrl();
	void CommEstablishXnetctrl();
public:
	long PanelTransfer2LC(EPanelEvents iID);
	CString m_strPanelIn, m_strPanelOut;
	long ProcessState2LC(int ProcessCode , CString strCode="", CString strErrMsg="");
	long EQState2LC(int EQCode, CString strCode="", CString strErrMsg="");
	// Alarm 보고
	int m_nAlarmState;
	int m_nAlarmID;
	CString m_strAlarmText;
	void Alarm2LC(int iOnOff, int iID = ERR_NONE, CString strErrorText = _T(""));
	//
	void NonStop2LC(int nBitAddress, CString strDeviceID);
	BOOL m_bQCFlag;
	long InspectionResult2LC(CString strCode, CString strJudgement);
	void LoginInfo2LC(ELogIn iID, CString strName, CString strID);
	long SendString(LPCTSTR strInfo, long nLen);
	void CommLog(LPCTSTR szFrom, LPCTSTR szTo, LPCTSTR szText);
	void InitCommInfoGrid();
	void UpdateCommInfoGrid(PANEL_INFO info);
	//////////////////////////////////////////////////////////////////////////
	// LC 중복보고 막기 위해 File 저장 [11/9/2010 OSC]
public:
	int LoadLastEQState();
	void SaveLastEQState(int nEQStateNo);
	CString LoadLastEQProcessPacket();
	int LoadLastEQProcess();
	void SaveLastEQProcess(int process, CString strProcess);
	//////////////////////////////////////////////////////////////////////////
	BOOL ContactOff();
	int m_nContactOffTime;

	int m_iUNI_CODE;
	int m_iPMCode;						// PM Code.//
	EPMMode m_bPMSel;					// PM Select.//
	BOOL m_bPMProcess;
	BOOL m_bPMMode;
	BOOL m_bPMEnable;					// PM 가능 여부.//
	int m_iCurProcessState;				// 현재 Process 상태.//
	int m_iCurEQState;					// 현재 장비 상태.//
	int m_iCurProcessState_Bak;		
	int m_iCurEQState_Bak;
	BOOL m_bProcessCancelMode;
	BOOL m_bProcessCancelMode_Orgin;

	int m_iReciveSize;
	int m_iPanelSelectCnt;
	short	m_PanelSelect;
	int m_nSec; //20081110 Hui : Mecha Modelchange관련.
	BOOL m_bVIWait;	// 20081110 Hui

	int m_nBakMsg;
	double m_dAlarmSetTimer;

	// Socket
public:
	CDataSocket		m_SocketLC, m_SocketVI, m_SocketPG;	// LC는 Data와 연결상태만 관리
	void SaveSocketParam(void);
	void LoadSocketParam(void);
	void InitSocketVI();
	void InitSocketPG();
	void ParseVIMessage(CDataSocket* pDataSocket);
	void ParsePGMessage(CDataSocket* pDataSocket);
	void GlassStageDataDisplay();
	void PGSend(unsigned char sCommand, CString strError = _T(""));
	CStopWatch m_TactInspection;

	// Event & Message & control
public:
	CHistoryEdit m_LCHistory;
	CHistoryEdit m_VIHistory;
	afx_msg LRESULT OnPacketReceive( WPARAM wParam, LPARAM lParam );
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CMFCButton m_radioCommLoaderAddr;
	CMFCButton m_radioCommLoaderInfo;
	CMFCButton m_radioCommPanelInfo;
	CMFCButton m_radioCommUnloaderAddr;
	CMsflexgrid_pos m_CommInfoGrid1;
	CMsflexgrid_pos m_CommInfoGrid2;
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedRadioComm1();
	afx_msg void OnBnClickedRadioComm2();
	afx_msg void OnBnClickedRadioComm3();
	afx_msg void OnBnClickedRadioComm4();
};


