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

	BOOL IsPanelUnloadComplecte();	// TRUE : Panel Unload �Ϸ�

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
	BOOL			m_bUnScrapping;	// CDlgCellInfoâ�� �� �ִ� ���� TRUE. Unscrap�� �������� Ȯ�� ���� ���

	BOOL			m_bPGStart;		// ���� Contact�� ��� [9/24/2009 RomanticBoy]	
	BOOL			m_bPGStop;		// ���� Contact�� ��� [9/24/2009 RomanticBoy]

	int				m_nCntClean;	// GetDoc()->m_nCleanSet�� ���ϱ� ���� �� �ǳ� ����ø��� Count�ϴ� ����

	DWORD m_watchRecv;
	CString m_strLogText;


	struct tagSeqFlag
	{
		//////////////////////////////////////////////////////////////////////////
		// Thread�� ���� & �����Ǵ� ������ ��Ȯ�� �˱� ���� ����
		BOOL bEqResume;	// STATUS_RUN or STATUS_READY ���¿��� Thread�� �ٽ� ���� �����ϸ� TRUE
		BOOL bEqPause;	// STATUS_RUN or STATUS_READY ���¿��� �ٸ� ���·� ��ȯ�Ǹ� TRUE
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
	BOOL CheckLoaderGlassData();	// TRUE�� Glass Data ������
	BOOL CheckGlassData();	// TRUE�� Glass Data ������
	void ClearGlassData(PANEL_INFO &panelinfo);
	void ClearGlassIDData( PANEL_INFO &panelinfo );

	void LastGlassToGlassData();
	void SaveLastGlass();

	void PGDataToGlassData();
	void GlassDataToPGData();
	void SetQCSampling();

	PG_INFO m_PGInfo;
	PANEL_INFO m_PanelInfoLoaderBuf;	// SeqCellRecv()���� �޴µ��� ���� ���� m_PanelInfo�� �����Ѵ�.
	PANEL_INFO m_PanelInfo;	

	//2009.05.31 Non Stop.
	BOOL SearchDeviceName(CString m_NonStopDeviceID);	// Device ID�� Model Matching�ϴ� �Լ�
	BOOL m_bNonStopReady;		// Non Stop ���� ��� bit ����ִ� ���� FLASE
	void SetNonStopFail();		// PLC���� Alarm Bit On �� WORD ������ "DOWN"���
	void ResetNonStopFail();	// PLC���� Alarm Bit Off �� WORD ������ "    "���
	
	//2009.06.11 Auto Tune
	BOOL SetAlignPos();        //  [6/11/2009 janghan]
	BOOL CheckLimit( int nAxis, double dPos ); //  [6/11/2009 janghan]

	int m_nYBackCnt;
	BOOL m_bNonStopDeviceRead;
};