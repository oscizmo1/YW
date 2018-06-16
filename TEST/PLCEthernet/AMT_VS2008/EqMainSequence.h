#pragma once

#include "Ex_Class\xsequence.h"
#include "CIF32.h"
#include "MMC39.h"
#include "AmtData.h"
#include "frmCOMM.h"
#include "PLCComm.h"


class CMTDoc;

class CEqMainSequence  : public XSequence  
{
public:
	void SetMelsec(CPLCComm *  pMelsec) {m_pMelsec = pMelsec ;}
	void SetMMC39(CMMC39 *  pMotion) {m_pMotion = pMotion ;}
	void SetAmtData(CAmtData *  pAmtData) {m_pAmtData = pAmtData ;}




	BOOL Sequence();
	CEqMainSequence();
	virtual ~CEqMainSequence();

public:
	void ProcessInit();
	BOOL OnReqMoveToPos();
	void Log(CString strLogMsg);
	void SeqCellContProc();
	void SeqCellRecv();
	void SeqCellSend();
	void CheckInterlock();

	void SeqContactUnit();
	void StopContactUnit();

	BOOL IsPanelUnloadComplecte();	// TRUE : Panel Unload 완료

	void EqLog(int nFunc, int nSeqNo,  LPCTSTR szText);

	CPLCComm*		m_pMelsec;
	CMMC39*			m_pMotion;
	CAmtData*		m_pAmtData;
	CFormCOMM*		m_pComm;

	int				m_nSeqCellRecvNo;
	int				m_nSeqCellSendNo;
	int				m_nSeqCellContactNo;
	int				m_nSeqInit;
	int				m_nSeqContactNo;
	int				m_nSeqContMoveNo;
	int				m_iAutoSearchPtrSeqNo;

	BOOL			m_bContactMoveEnable;
	BOOL			m_bUnScrapping;	// CDlgCellInfo창이 떠 있는 동안 TRUE. Unscrap이 끝났는지 확인 위해 사용

	BOOL			m_bPGStart;		// 수동 Contact시 사용 [9/24/2009 RomanticBoy]	
	BOOL			m_bPGStop;		// 수동 Contact시 사용 [9/24/2009 RomanticBoy]

	int				m_nCntClean;	// GetDoc()->m_nCleanSet과 비교하기 위해 매 판넬 진행시마다 Count하는 변수

	DWORD m_watchRecv;
	CString m_strLogText;


	struct tagSeqFlag
	{
		//////////////////////////////////////////////////////////////////////////
		// Thread가 시작 & 정지되는 시점을 정확히 알기 위해 삽입
		BOOL bEqResume;	// STATUS_RUN or STATUS_READY 상태여서 Thread가 다시 돌기 시작하면 TRUE
		BOOL bEqPause;	// STATUS_RUN or STATUS_READY 상태에서 다른 상태로 전환되면 TRUE
		//////////////////////////////////////////////////////////////////////////

		BOOL bCellInReq;
		BOOL bCellInComp;
		BOOL bCellInCancel;
		
		BOOL bCellOutReq;
		BOOL bCellOutComp;
		BOOL bCellOutCancel;
		
		BOOL bCellContactReq;
		BOOL bCellContactComp;
		BOOL bCellContactCancel;

		BOOL bContactMoveComp;
		BOOL bContactMoveReq;
		BOOL bContactMoving;
		
	} m_SeqFlag;

	struct FlagInterlock
	{
		BOOL MainPower;
		BOOL DoorSensor;
		BOOL AreaSensor;
	}m_FlagInterlock;

	void GlassDataCopy();
	BOOL CheckLoaderGlassData();	// TRUE면 Glass Data 존재함
	BOOL CheckGlassData();	// TRUE면 Glass Data 존재함
	void ClearGlassData(PANEL_INFO &panelinfo);
	void ClearGlassIDData( PANEL_INFO &panelinfo );

	void LastGlassToGlassData();
	void SaveLastGlass();

	void PGDataToGlassData();
	void GlassDataToPGData();
	void SetQCSampling();

	PG_INFO m_PGInfo;
	PANEL_INFO m_PanelInfoLoaderBuf;	// SeqCellRecv()에서 받는데만 쓰고 이후 m_PanelInfo로 복사한다.
	PANEL_INFO m_PanelInfo;	

	//2009.05.31 Non Stop.
	BOOL SearchDeviceName(CString m_NonStopDeviceID);	// Device ID와 Model Matching하는 함수
	BOOL m_bNonStopReady;		// Non Stop 예약 명령 bit 살아있는 동안 FLASE
	void SetNonStopFail();		// PLC측에 Alarm Bit On 및 WORD 영역에 "DOWN"기록
	void ResetNonStopFail();	// PLC측에 Alarm Bit Off 및 WORD 영역에 "    "기록
	
	//2009.06.11 Auto Tune
	BOOL SetAlignPos();        //  [6/11/2009 janghan]
	BOOL CheckLimit( int nAxis, double dPos ); //  [6/11/2009 janghan]

	int m_nYBackCnt;
	BOOL m_bNonStopDeviceRead;
};