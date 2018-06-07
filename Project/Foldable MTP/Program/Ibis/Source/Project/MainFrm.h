
// MainFrm.h : CMainFrame 클래스의 인터페이스
//

#pragma once



// GxBaseForm
#include "UI\GausGUI\GxGUIFrame.h"		// for GxGuiFrame
#include "IbisApp.h"
//kjpark 20161022 설정->램프 설정 구현
#include <vector>
#include <map>
using namespace std;
#include "Etc/SystemTime.h"
#include "UI\GausGUI\GxMsgBox.h"

class CMainFrame : public CFrameWnd
{
	
protected: // serialization에서만 만들어집니다.
	CMainFrame();
	DECLARE_DYNCREATE(CMainFrame)

	// 프로그램 운용을 위한 변수
	BOOL		m_bOnlyOnce;													// 최초 수행여부
	BOOL		m_bTerminateFlag;												// 프로그램을 종료해도 되는지 여부
	BOOL		m_bModePM;


	
// 특성입니다.
public:

	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	CGxGUIFrame*		m_pGUIFrame;										// 화면 관리용 객체 선언

	CGxGUIFrame* GetGUIFrame() const { return m_pGUIFrame; }
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	LANGUAGE	m_nLangIdx;														// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...

	int		m_iCurPageRep;													// 현재 저장된 Report 페이지
	int		m_iCurPageSet;													// 현재 저장된 Setup  페이지
	int		m_iCurPageTea1;													// 현재 저장된 Teach1 페이지
	int		m_iCurPageCon;													// 현재 저장된 Config 페이지

	AXIS_ID				m_iSelectedAxis;												// 현재 클릭된 Axis
	TEACH_PARAM::ID		m_iTeachAxis;													// 현재 클릭된 Axis
	double				m_dTeachOffset;													// TeachMove할 때 적용될 Offset
	
	//void SetSelectedAxis(int val) { m_iSelectedAxis = val;  }
	void SetSelectedAxis(AXIS_ID val1, TEACH_PARAM::ID val2, double dOffset = 0.);
	AXIS_ID GetSelectedAxis() const { return m_iSelectedAxis; }
	TEACH_PARAM::ID GetTeachAxis() const { return m_iTeachAxis; }
	double GetTeachOffset()		{ return m_dTeachOffset; }
	



	void SetCurPageRep(int val) { m_iCurPageRep = val; }
	void SetCurPageCon(int val) { m_iCurPageCon = val; }
	void SetCurPageTea1(int val) { m_iCurPageTea1 = val; }
	void SetCurPageSet(int val) { m_iCurPageSet = val; }

	int GetCurPageRep() const { return 	m_iCurPageRep; 	}
	int GetCurPageCon() const { return m_iCurPageCon; }
	int GetCurPageTea1() const { return m_iCurPageTea1; }
	int GetCurPageSet() const { return m_iCurPageSet; }


	void setModePM(BOOL bMode)	{ m_bModePM = bMode; }
	BOOL getModePM()			{ return m_bModePM;	}

protected:
	CUserAccount		m_UserInspectorData;			//	UserData Class;
	CUserAccount		m_UserOperatorData;			//	UserData Class;
	CMapUserAccount		m_mapUserAccount;	

// 작업입니다.
public:
	//kjpark 20170920 LOGIN/OUT 구현
	CUserAccount GetCurOperatorUserInspectorData() const { return m_UserInspectorData; }				//	UserData
	void LoadLastUserInspector();
	void SetCurUserInspectorData(CUserAccount val);
	//kjpark 20170920 LOGIN/OUT 구현
	CUserAccount GetCurEngeneerUserOperatorData() const { return m_UserOperatorData; }				//	UserData
	void SetCurUserOperatorData(CUserAccount val);
	int getUserType(CString szID, CString szPW);		//	유저맵에서 ID와 PW가 있는지 체크후 존재한다면 해당 UserData의 Type을 return함.
	BOOL FindUser(CString szID);
	BOOL LoadUserAccountMap();								//	UserData.ini를 가져와서 map에 넣어줌.
	void ClearUserAccountMap();


// 재정의입니다.
public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

// 구현입니다.
public:
	virtual ~CMainFrame();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif


// 생성된 메시지 맵 함수
protected:

	afx_msg LRESULT OnViewChange(WPARAM wParam, LPARAM lParam);					// 화면 변경 실행
	afx_msg LRESULT OnViewChangeEnd(WPARAM wParam, LPARAM lParam);				// 화면 변경 실행 완료
	afx_msg LRESULT OnChangeRequestLanguage(WPARAM wParam, LPARAM lParam);	// 화면 표시 언어를 변경하도록 요청받았다. 
	//kjpark 20160919 Topdlg에 connect status 
	afx_msg LRESULT OnChangeCommState(WPARAM wParam, LPARAM lParam);			// 통신 상태 변경
	//kjpark 20170831 Interlock, Opcoall, Terminal UI 및 기능 구현
	afx_msg LRESULT OnChangeCIMMessageState(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnWarningShow(WPARAM wParam, LPARAM lParam);

	afx_msg LRESULT OnShowDialog(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnClose();
	
	//20160930 kjpark Serial Data 메세지 방식 구현
	afx_msg LRESULT OnMSGEvent(WPARAM wParam, LPARAM lParam);	
	
	//kjpark 20161019 양수버튼 체크
	int bZone_L_TimeCk;
	int bZone_R_TimeCk;
	int dZone_L_TimeCk;
	int dZone_R_TimeCk;

	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	BOOL m_bChkLampR_Flick;
	BOOL m_bChkLampY_Flick;
	BOOL m_bChkLampG_Flick;
	BOOL m_bLamp_FLick;

	BOOL m_bAZoneOperatorReady[JIG_ID_MAX];
	//kjpark 20161019 MainFrame timer 추가 - 인터락, 타워램프 컨트롤
	BOOL m_bTeachMode;
	

	int		m_nLanguageType;					// 설정된 언어 종류, 1:한국어, 2:영어, 3:베트남어 ...
	int GetLanguageType() { return m_nLangIdx;};
	BOOL m_bSTODetectFlag;					// STO가 발동되었는가 ?
	void SetSTODetectClear();
	BOOL GetSTODetected();
	
	
	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	CString			m_sLastSavedImage[JIG_ID_MAX][JIG_CH_MAX];					// 마지막에 저장된 이미지 파일이름;
	afx_msg LRESULT OnUpdateMCRConnect(WPARAM wParam, LPARAM lParam);			// MCR 연결 해제
	afx_msg LRESULT OnUpdateMCRData_CH1(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.
	afx_msg LRESULT OnUpdateMCRData_CH3(WPARAM wParam, LPARAM lParam);				// MCR Data가 수신되었다.

public:
	void LoadReportFile(CString strTime = _T(""));

	//////////////////////////////////////////////////////////////////////////
	// Manual 동작중 Light Curtain check하기 위한 구성
	// UI에서 이동 버튼 클릭시 m_bEnableLightCurtain를 TRUE하고 AXIS를 지정해주면
	// m_checkAxisLightCurtain가 Stop이 아닌 동안 체크하고 Stop이 되면
	// m_bEnableLightCurtain를 FALSE로 바꾸고 종료한다 [7/12/2017 OSC]
	BOOL m_bEnableLightCurtain;			// 체크 할지 말지...
	AXIS_ID m_checkAxisLightCurtain;	// 체크할 대상 축
	void ManualLightCurtainCheck();
	//////////////////////////////////////////////////////////////////////////

	void SetWindowMove();
	BOOL m_bBackDoorOpen;

	BOOL m_bWarningMsgShow;
	CGxMsgBox	m_WarningMsgBox;

	// 남아있는Cell 모두 TrackOut하고 정리
	void AllCellTrackOut();


};


