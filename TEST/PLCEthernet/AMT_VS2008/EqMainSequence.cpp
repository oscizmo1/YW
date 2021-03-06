// EqMainSequence.cpp: implementation of the CEqMainSequence class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "EqMainSequence.h"
#include "MainFrm.h"

enum
{
	indexSeqSend,
	indexSeqCont,
	indexSeqRcv,
	indexContUnit
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEqMainSequence::CEqMainSequence()
{
	m_pMotion = NULL;
	m_pMelsec = NULL;

	ZeroMemory(&m_SeqFlag , sizeof(tagSeqFlag) );
	ZeroMemory(&m_PGInfo , sizeof(PG_INFO) );
	ZeroMemory(&m_PanelInfoLoaderBuf , sizeof(PANEL_INFO) );
	ZeroMemory(&m_PanelInfo , sizeof(PANEL_INFO) );
	m_FlagInterlock.AreaSensor = TRUE;
	m_FlagInterlock.DoorSensor = TRUE;
	m_FlagInterlock.MainPower = TRUE;

	m_nSeqCellRecvNo = 0;
	m_nSeqCellSendNo = 0;
	m_nSeqCellContactNo = 0;
	m_nSeqInit = 0 ;
	m_nSeqContMoveNo = 0 ;
	m_bContactMoveEnable = FALSE ;
	m_iAutoSearchPtrSeqNo = 0 ;

	m_SeqFlag.bEqResume = FALSE;
	m_SeqFlag.bEqPause = TRUE;
	m_SeqFlag.bCellContactReq = FALSE ;
	m_SeqFlag.bCellContactComp = TRUE ;
	m_bUnScrapping = FALSE;
	m_nCntClean = 0;
	m_bNonStopReady = FALSE;

	m_nYBackCnt = 0;

	m_bNonStopDeviceRead = FALSE;
}

CEqMainSequence::~CEqMainSequence()
{

}

BOOL CEqMainSequence::Sequence()
{
	Sleep(20);

	if( GetApp()->IsRunStatus())
	{
		if(m_SeqFlag.bEqPause)
		{
			m_SeqFlag.bEqResume = TRUE;
			m_SeqFlag.bEqPause = FALSE;

			// SeqContactUnit Stop
			StopContactUnit();

			// L/C 보고
//			m_pComm->ProcessState2LC(eIdle);
		}
		SeqCellContProc();
		SeqCellRecv();
		SeqCellSend();
	}
	else
	{
		if(m_nSeqCellRecvNo > SEQ_LD_CHK_IN_REQ_BIT_ON/*SEQ_LD_CHK_IN_DO_BIT_ON*/)
		{
			SeqCellRecv();
		}
		else if( m_pMelsec->MelsecNetBitRecv(LD_OUT_READY) )
		{
			m_nSeqCellRecvNo = SEQ_LD_CHK_REQUEST;
			m_pMelsec->MelsecNetBitOff(LD_OUT_READY);
			m_pMelsec->MelsecNetBitOff(LD_OUT_REQ_SUPPLY);
			m_SeqFlag.bCellInReq = FALSE;
			m_SeqFlag.bCellInComp = TRUE;
		}

		if(m_nSeqCellSendNo > SEQ_UNLD_CHK_IN_REQ_BIT_ON/*SEQ_UNLD_CHK_IN_DO_BIT_ON*/)
		{
			SeqCellSend();
		}	// 2009.07.02 Hui
		else if( m_pMelsec->MelsecNetBitRecv(UNLD_OUT_READY) )
		{
			m_nSeqCellSendNo = SEQ_UNLD_CHK_REQUEST;
			m_pMelsec->MelsecNetBitOff(UNLD_OUT_READY);
			m_pMelsec->MelsecNetBitOff(UNLD_OUT_REQ_SUPPLY);
			m_SeqFlag.bCellOutReq = FALSE;
			m_SeqFlag.bCellOutComp = TRUE;
		}

		if(m_SeqFlag.bEqResume)
		{

			// Seq Init
			m_nSeqCellContactNo = MAIN_CHK_INTERLOCK;

			// MoveStageStep 초기화
			GetDeviceCtl()->m_nMoveStep = MS_CHK_EQUIPMENT;

			// SeqContactUnit Stop
			StopContactUnit();

			m_SeqFlag.bEqPause = TRUE;
			m_SeqFlag.bEqResume = FALSE;
		}
	}

	SeqContactUnit();
	if(IsSimulation() == FALSE)		CheckInterlock();

	return 1;
}

void CEqMainSequence::SeqCellSend()
{
	if(GetApp()->IsRunStatus() == FALSE)
		if( m_nSeqCellSendNo < SEQ_UNLD_CHK_IN_DO_BIT_ON )
			return;

	int nSeqNo = m_nSeqCellSendNo ;
	BOOL bFlag1, bFlag2 , bFlag3;

	static DWORD wSendStartTimer = 0 ;
	static DWORD wSendTimer = 0;
	int nErr;

	switch( nSeqNo )
	{
	case SEQ_UNLD_CHK_REQUEST:
		if(m_SeqFlag.bCellOutReq)
		{
			EqLog(indexSeqSend, nSeqNo, "Check\t[bCellOutReq:1]");
			//m_SeqFlag.bCellOutReq = FALSE ;
			m_SeqFlag.bCellOutComp = FALSE ;
			EqLog(indexSeqSend, nSeqNo, "Set\t[bCellOutComp:0]");
			nSeqNo = SEQ_UNLD_CHK_CONDITION ;
		}
		break;

	case SEQ_UNLD_CHK_CONDITION:
		nErr = GetDeviceCtl()->ChkContactUnitUp();
		if( nErr == ERR_NONE && GetDeviceCtl()->ChkClampUnit() /*&&  GetDeviceCtl()->ChkPanelExist()*/  )			
		{
			EqLog(indexSeqSend, nSeqNo, "Check\t[Contact Up, Clamp Off]");
			nSeqNo = SEQ_UNLD_DATA_WRITE;
		}
		else
		{
			if(nErr != ERR_NONE)
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, nErr);
			else if(GetDeviceCtl()->ChkClampUnit() == FALSE)
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TOP_CLAMP_OFF);
			else
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_WT_PANELCHK);
			nSeqNo = SEQ_UNLD_ERR;
		}
		break;

	case SEQ_UNLD_DATA_WRITE:
		// Panel Data Write..
		if( GetMainFrame()->m_bMelsecWrite )
			break;
		GetMainFrame()->m_bMelsecWrite = TRUE;
		m_pMelsec->GlassDataWrite(m_PanelInfo, DATA_OUTPUT_START);
		EqLog(indexSeqSend, nSeqNo, "Do\t[GlassDataWrite]");

		nSeqNo = SEQ_UNLD_OUT_READY_BIT_ON ;
		break;

	case SEQ_UNLD_OUT_READY_BIT_ON:
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcUnloadingReady, NULL);
		m_pMelsec->MelsecNetBitOn( UNLD_OUT_READY );
		EqLog(indexSeqSend, nSeqNo, "Bit On\t[UNLD_OUT_READY]");
		if( GetLineType() == SINGLE_ARM_TYPE )
			nSeqNo = SEQ_UNLD_CHK_LD_END ;
		else
			nSeqNo = SEQ_UNLD_OUT_REQ_BIT_ON ;
		break;

	case SEQ_UNLD_CHK_LD_END:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_END_SUPPLY );
		if ( !bFlag1 )
		{
			EqLog(indexSeqSend, nSeqNo, "Check Bit Off\t[LD_IN_END_SUPPLY]");
			nSeqNo = SEQ_UNLD_OUT_REQ_BIT_ON ;
		}
		break;

	case SEQ_UNLD_OUT_REQ_BIT_ON:
		m_pMelsec->MelsecNetBitOn( UNLD_OUT_REQ_SUPPLY );

		EqLog(indexSeqSend, nSeqNo, "Bit On\t[UNLD_OUT_REQ_SUPPLY]");
		nSeqNo = SEQ_UNLD_CHK_IN_REQ_BIT_ON;
		break;

	case SEQ_UNLD_CHK_IN_REQ_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_REQ_SUPPLY );

		if( bFlag1 )
		{
			EqLog(indexSeqSend, nSeqNo, "Check Bit On\t[UNLD_IN_REQ_SUPPLY]");
			//GetDeviceCtl()->EnableAreaSensor(TRUE);
			m_pMelsec->MelsecNetBitOn( UNLD_OUT_DO_SUPPLY );
			EqLog(indexSeqSend, nSeqNo, "Bit On\t[UNLD_OUT_DO_SUPPLY]");

			nSeqNo = SEQ_UNLD_CHK_IN_DO_BIT_ON;
		}
		break;

	case SEQ_UNLD_CHK_IN_DO_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_DO_SUPPLY );

		if( bFlag1 )
		{
			EqLog(indexSeqSend, nSeqNo, "Check Bit On\t[UNLD_IN_DO_SUPPLY]");

			m_pComm->InspectionResult2LC(m_PanelInfo.m_sCode , m_PanelInfo.m_sJudgement) ;
// 			m_pComm->PanelTransfer2LC(ePanelOut) ;
			nSeqNo = SEQ_UNLD_CHK_IN_END_BIT_ON;
		}
		break;

	case SEQ_UNLD_CHK_IN_END_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_END_SUPPLY );

		if( bFlag1 )
		{
			EqLog(indexSeqSend, nSeqNo, "Check Bit On\t[UNLD_IN_END_SUPPLY]");
			//GetDeviceCtl()->EnableAreaSensor(FALSE);
			m_pMelsec->MelsecNetBitOn( UNLD_OUT_END_SUPPLY );
			EqLog(indexSeqSend, nSeqNo, "Bit On\t[UNLD_OUT_END_SUPPLY]");
			nSeqNo = SEQ_UNLD_CHK_IN_ALL_BIT_OFF;
		}
		break; 

	case SEQ_UNLD_CHK_IN_ALL_BIT_OFF:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_REQ_SUPPLY );
		bFlag2 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_DO_SUPPLY );
		bFlag3 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_END_SUPPLY );

		if( !bFlag1 && !bFlag2 && !bFlag3 )
		{
			EqLog(indexSeqSend, nSeqNo, "Check Bit Off\t[UNLD_IN_REQ_SUPPLY, UNLD_IN_DO_SUPPLY, UNLD_IN_END_SUPPLY]");
			m_pComm->PanelTransfer2LC(ePanelOut) ;

			m_pMelsec->MelsecNetBitOff( UNLD_OUT_REQ_SUPPLY );
			m_pMelsec->MelsecNetBitOff( UNLD_OUT_DO_SUPPLY );
			m_pMelsec->MelsecNetBitOff( UNLD_OUT_END_SUPPLY );

			EqLog(indexSeqSend, nSeqNo, "Bit Off\t[UNLD_OUT_REQ_SUPPLY, UNLD_OUT_DO_SUPPLY, UNLD_OUT_END_SUPPLY]");

			if(GetMachinePCType() == A_AMT || GetMachinePCType() == B_AMT)
			{
				nSeqNo = SEQ_UNLD_CHK_INSP_END;
				wSendStartTimer = GetTickCount();
			}
			else
				nSeqNo = SEQ_UNLD_COMPLETE;
		}
		break;

	case SEQ_UNLD_CHK_INSP_END:
		if( GetTickCount() - wSendStartTimer < 3000 )		// 3초 동안..VI 결과를 기다린다...
		{
			if( GetApp()->m_bInspectionEnd )
			{
				EqLog(indexSeqSend, nSeqNo, "Check Set\t[m_bInspectionEnd]");

				m_pMelsec->MelsecNetBitOn( UNLD_OUT_VI_END_UNLOAD );
				EqLog(indexSeqSend, nSeqNo, "Bit On\t[UNLD_OUT_VI_END_UNLOAD]");

				nSeqNo = SEQ_UNLD_CHK_IN_INSP_END_ON;

				wSendStartTimer = GetTickCount();		
			}
		}
		else
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_INSPECTION_RESULT_FAIL);
			nSeqNo = SEQ_UNLD_ERR;
		}
		break;

	case SEQ_UNLD_CHK_IN_INSP_END_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_IN_VI_END_UNLOAD);

		if( GetTickCount() - wSendStartTimer < 600000 ) // 10분 동안 Unloader Response Check..
		{
			if( bFlag1 )
			{
				EqLog(indexSeqSend, nSeqNo, "Check Bit On\t[UNLD_IN_VI_END_UNLOAD]");
				wSendStartTimer = 0 ;

				m_pMelsec->MelsecNetBitOff( UNLD_OUT_VI_END_UNLOAD );
				EqLog(indexSeqSend, nSeqNo, "Bit Off\t[UNLD_OUT_VI_END_UNLOAD]");

				nSeqNo = SEQ_UNLD_COMPLETE;
			}
		}
		else
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_UNLOADER_DISCHARGE_TIMEOUT);
			nSeqNo = SEQ_UNLD_ERR;
		}
		break;

	case SEQ_UNLD_ERR:
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_NORMAL, ERR_CANCEL);
		GetApp()->ChangeStatus(STATUS_ERROR);

		nSeqNo = SEQ_LD_CHK_REQUEST;
		break;

	case SEQ_UNLD_COMPLETE:
		// EtherNet으로 바뀌면서 너무 느려져 여기서 한번 더 확인 [5/26/2009 OSC]
		bFlag1 = m_pMelsec->MelsecNetBitRecv( UNLD_OUT_REQ_SUPPLY );
		bFlag2 = m_pMelsec->MelsecNetBitRecv( UNLD_OUT_DO_SUPPLY );
		bFlag3 = m_pMelsec->MelsecNetBitRecv( UNLD_OUT_END_SUPPLY );

		if( !bFlag1 && !bFlag2 && !bFlag3 )
		{
			// GlassData와 GlassData Write영역 초기화
			if(GetMainFrame()->m_bMelsecWrite )
				break;
			GetMainFrame()->m_bMelsecWrite = TRUE;
			ClearGlassIDData(m_PanelInfo);
			ClearGlassData(m_PanelInfo);
			m_pMelsec->GlassDataWrite(m_PanelInfo, DATA_OUTPUT_START);

			::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcUnLoadingDone, NULL);
			m_SeqFlag.bCellOutReq = FALSE ;
			m_SeqFlag.bCellOutComp = TRUE ;
			EqLog(indexSeqSend, nSeqNo, "Set\t[bCellOutReq:0, bCellOutComp:1]");
			// START중에는 Loading & Unloading Bit 항상 On
			// 		m_pMelsec->MelsecNetBitOn(UNLD_OUT_READY);

			// Exchange Case  , Loading Sequence Start
			// Cell Out Report (L/C)
			nSeqNo = SEQ_UNLD_CHK_REQUEST ;
		}
		break;

	default:
		break;
	}

	m_nSeqCellSendNo = nSeqNo ;
}

void CEqMainSequence::SeqCellRecv()
{
	if(GetApp()->IsRunStatus() == FALSE)
		if( m_nSeqCellRecvNo < SEQ_LD_CHK_IN_DO_BIT_ON )
			return;

	int nSeqNo = m_nSeqCellRecvNo;

	BOOL bFlag1, bFlag2, bFlag3;
	static int nCnt1 = 0;
	int nErrCode = ERR_NONE;
	int nRet = 0;

	static DWORD wSendStartTimer = 0 ;
	static DWORD wRecvTimer = 0;

	switch( nSeqNo )
	{
	case SEQ_LD_CHK_REQUEST:
		if(m_SeqFlag.bCellInReq)
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[bCellInReq:1]");
			m_SeqFlag.bCellInComp = FALSE;
			EqLog(indexSeqRcv, nSeqNo, "Set\t[bCellInComp:0]");
			nSeqNo = SEQ_LD_CHK_CONDITION;

			// Non Stop 중에 여기를 지나가게 되면 처음 실행하던가 아니면 에러 후 재시작하는 경우밖에 없으므로
			// PLC측에 Fail Bit를 죽인다.
			if(m_bNonStopReady == FALSE)
			{
				if( GetMainFrame()->m_bMelsecWrite )
					return;
				GetMainFrame()->m_bMelsecWrite = TRUE;
				ResetNonStopFail();
			}

		}
		break;

	case SEQ_LD_CHK_CONDITION:
		nErrCode = GetDeviceCtl()->ChkContactUnitUp();
		if( nErrCode == ERR_NONE && diCLAMP_UNIT_STATUS   )			
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[ChkContactUnitUp():ERR_NONE]");
			nSeqNo = SEQ_LD_CHK_PANEL;
		}
		else
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, nErrCode);
			nSeqNo = SEQ_LD_ERR;
		}
		break;

	case SEQ_LD_CHK_PANEL:
		if(GetDeviceCtl()->ChkPanelExist())
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[ChkPanelExist():TRUE]");
			nSeqNo = SEQ_LD_COMPLETE;
		}
		else
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[ChkPanelExist():FALSE]");
			m_pMelsec->MelsecNetBitOff( LD_OUT_REQ_SUPPLY);
			m_pMelsec->MelsecNetBitOff( LD_OUT_DO_SUPPLY);
			m_pMelsec->MelsecNetBitOff( LD_OUT_END_SUPPLY);
			EqLog(indexSeqRcv, nSeqNo, "Set\t[LD_OUT_REQ_SUPPLY:0, LD_OUT_DO_SUPPLY:0, LD_OUT_END_SUPPLY:0]");
			nSeqNo = SEQ_LD_OUT_READY_BIT_ON;
		}
		break;

	case SEQ_LD_OUT_READY_BIT_ON:
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcLoadingReady, NULL);
		m_pMelsec->MelsecNetBitOn( LD_OUT_READY );
		EqLog(indexSeqRcv, nSeqNo, "Set\t[LD_OUT_READY:1]");
		if( GetLineType() == SINGLE_ARM_TYPE )
			nSeqNo = SEQ_LD_CHK_UNLD_END ;
		else
			nSeqNo = SEQ_LD_OUT_REQ_BIT_ON ;
		break;

	case SEQ_LD_CHK_UNLD_END:
		if( IsPanelUnloadComplecte() )
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[IsPanelUnloadComplecte():TRUE]");
			nSeqNo = SEQ_LD_OUT_REQ_BIT_ON ;
		}
		break;

	case SEQ_LD_OUT_REQ_BIT_ON:
		m_pMelsec->MelsecNetBitOn( LD_OUT_REQ_SUPPLY );
		EqLog(indexSeqRcv, nSeqNo, "Set\t[LD_OUT_REQ_SUPPLY:1]");
		m_watchRecv = GetTickCount();
		nSeqNo = SEQ_LD_CHK_IN_REQ_BIT_ON;
		break;

	case SEQ_LD_CHK_IN_REQ_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_REQ_SUPPLY );
		if( bFlag1 )
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[LD_IN_REQ_SUPPLY:1]");
			GetApp()->ChangeStatus(STATUS_RUN);	// 판넬이 들어오므로 READY에서 RUN으로 전환
			//GetDeviceCtl()->EnableAreaSensor(TRUE);
			m_pMelsec->MelsecNetBitOn( LD_OUT_DO_SUPPLY);
			EqLog(indexSeqRcv, nSeqNo, "Set\t[LD_OUT_DO_SUPPLY:1]");

			// Panel이 진입하면 Model Change가 끝난 거므로 혹시나 살아있는 Bit 모두 제거 [6/10/2009 OSC]
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_RESERVE);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_START);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_END);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_FAIL);
			EqLog(indexSeqRcv, nSeqNo, "Set\t[NON_STOP_PC_RESERVE:0, NON_STOP_PC_START:0, NON_STOP_PC_END:0, NON_STOP_PC_FAIL:0]");
			nSeqNo = SEQ_LD_CHK_IN_DO_BIT_ON;
		}
		else if(  GetDoc()->m_bUseAMC )
		{	// 2009.05.31
			// Model Change 사용 시 모델 체인지 예약 비트가 확인되면....
			bFlag2 = m_pMelsec->MelsecNetBitRecv( NON_STOP_PC_RESERVE );
			if(bFlag2)
			{
				GetDoc()->m_bNonStop_Run = TRUE;//  [8/17/2009 janghan]

				EqLog(indexSeqRcv, nSeqNo, "Check\t[NON_STOP_PC_RESERVE:1]");
				m_pMelsec->MelsecNetBitOff( LD_OUT_REQ_SUPPLY );
				m_pMelsec->MelsecNetBitOn( NON_STOP_PC_START );
				EqLog(indexSeqRcv, nSeqNo, "Set\t[LD_OUT_REQ_SUPPLY:0, NON_STOP_PC_START:1");
				m_bNonStopDeviceRead = FALSE;
				nSeqNo = SEQ_LD_READ_DEVICEID;
			}
			else if((GetTickCount() - m_watchRecv) > 60000 && GetApp()->m_RunStatus == STATUS_RUN)
				GetApp()->ChangeStatus(STATUS_READY);	// 판넬이 1분동안 안들어오면 RUN에서 READY으로 전환
		}
		else if((GetTickCount() - m_watchRecv) > 60000 && GetApp()->m_RunStatus == STATUS_RUN)
			GetApp()->ChangeStatus(STATUS_READY);	// 판넬이 1분동안 안들어오면 RUN에서 READY으로 전환
		break;

// 2009.05.31 여기서부터.
	case SEQ_LD_READ_DEVICEID:	// 디바이스 아이디를 읽고,
		if( !m_bNonStopDeviceRead )
		{
			if( GetMainFrame()->m_bMelsecRead )
				break;
			GetMainFrame()->m_bMelsecRead = TRUE;
			GetDoc()->m_NonStopDeviceID = m_pMelsec->NonStopDeviceRead();
			m_bNonStopDeviceRead = TRUE;
		}
		if( GetDoc()->m_NonStopDeviceID.GetLength() > 3 )
		{
			m_strLogText.Format("Do\t[NonStopDeviceRead():%s]", GetDoc()->m_NonStopDeviceID);
			EqLog(indexSeqRcv, nSeqNo, m_strLogText);
			GetComm()->NonStop2LC(NON_STOP_PC_START, GetDoc()->m_NonStopDeviceID);
			nSeqNo = SEQ_LD_PCB_CHANGE_POS;
		}
		else
		{
			if( GetMainFrame()->m_bMelsecWrite )
				break;
			GetMainFrame()->m_bMelsecWrite = TRUE;
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_NON_STOP_DEVICE_RECEIVE);
			SetNonStopFail();
			nSeqNo = SEQ_LD_ERR;
		}
		break;

	case SEQ_LD_PCB_CHANGE_POS:	// PCB 교체
		 nRet = GetDeviceCtl()->MoveStage(PCB_CHANGE_POS, TRUE);	// PCB 교체 위치 이동
		if( nRet == MOVE_OK_END)
		{
			EqLog(indexSeqRcv, nSeqNo, "Do\t[MoveStage():PCB_CHANGE_POS]");
			GetDoc()->m_bPCBChangeComp = FALSE;
			EqLog(indexSeqRcv, nSeqNo, "Set\t[m_bPCBChangeComp:0]");
			// 2009.05.31 여기서 교체 완료 확인을 위한 팝업 띄운다. ***********
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_PCB_CHANGE);	
			nSeqNo = SEQ_LD_FIND_DEVICEID;
		}
		else if( nRet < MOVE_ERR_NONE )
		{
			nSeqNo = SEQ_LD_ERR;
		}
		break;

	case SEQ_LD_FIND_DEVICEID:	// 디바이스 아이디와 매칭되는 모델을 찾는다.
		if( GetDoc()->m_bPCBChangeComp )	// 2009.05.31 팝업에서 리턴이 올때 까지 기다린다.
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[m_bPCBChangeComp:1]");
			if( SearchDeviceName(GetDoc()->m_NonStopDeviceID) == TRUE)	// Device ID와 Model Matching
			{
				// Matching 결과가 있으면 Model Change
				GetDoc()->SaveCurrentModelName();
				GetDoc()->OpenModelFile(m_pAmtData->m_CurModelInfo.strModelName);
				EqLog(indexSeqRcv, nSeqNo, "Do\t[OpenModelFile()]");
				nSeqNo = SEQ_LD_MODEL_CHANGE;
			}
			else						// Matching 결과가 없으면 Matching 선택 창 표시
			{
				GetDoc()->m_bModelMatchingComp = FALSE;
				EqLog(indexSeqRcv, nSeqNo, "Set\t[m_bModelMatchingComp:0]");
				::PostMessage(GetMainFrame()->m_hWnd, WM_MATCHING_DEVICE_MODEL, NULL, NULL);
				nSeqNo = SEQ_LD_MATCH_DEVICEID;
			}
		}
		break;

	case SEQ_LD_MATCH_DEVICEID:
		if( GetDoc()->m_bModelMatchingComp )	// Matching 결과가 끝날 때 까지 기다린다.
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[m_bModelMatchingComp:1]");
			if( SearchDeviceName(GetDoc()->m_NonStopDeviceID) == TRUE)
			{
				m_strLogText.Format("Do\t[SearchDeviceName():%s]", GetDoc()->m_NonStopDeviceID);
				// Model 변경
				EqLog(indexSeqRcv, nSeqNo, m_strLogText);
				GetDoc()->OpenModelFile(m_pAmtData->m_CurModelInfo.strModelName);
				m_strLogText.Format("Do\t[OpenModelFile():%s]", m_pAmtData->m_CurModelInfo.strModelName);
				EqLog(indexSeqRcv, nSeqNo, m_strLogText);
				GetDoc()->SaveCurrentModelName();
				nSeqNo = SEQ_LD_MODEL_CHANGE;
			}
		}
		break;

	case SEQ_LD_MODEL_CHANGE:	// 대기위치로 이동시킨다.
		nRet = GetDeviceCtl()->MoveStage( READY_POS, TRUE );
		if( nRet == MOVE_OK_END )
		{
			EqLog(indexSeqRcv, nSeqNo, "Do\t[MoveStage():READY_POS]");
			// 2009.05.31 여기서 모델 체인지 완료 보고 한다. *********
			m_pMelsec->MelsecNetBitOn( NON_STOP_PC_END );
			m_watchRecv = GetTickCount();
			EqLog(indexSeqRcv, nSeqNo, "Set\t[NON_STOP_PC_END:1]");
			GetComm()->NonStop2LC(NON_STOP_PC_END, GetDoc()->m_NonStopDeviceID);
			nSeqNo = SEQ_LD_MODEL_CHANGE_START_BIT_OFF;
		}
		else if( nRet < MOVE_ERR_NONE )
		{
			if( GetMainFrame()->m_bMelsecWrite )
				break;
			GetMainFrame()->m_bMelsecWrite = TRUE;
			SetNonStopFail();
			nSeqNo = SEQ_LD_ERR;
		}
		break;
	case SEQ_LD_MODEL_CHANGE_START_BIT_OFF:
		// 2초동안 END Bit 살려놓은 후 START를 죽인다.
		if((GetTickCount() - m_watchRecv) > 2000)
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[NON_STOP_PC_END:1]");
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_START);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_FAIL);
			EqLog(indexSeqRcv, nSeqNo, "Set\t[NON_STOP_PC_START:0, NON_STOP_PC_FAIL:0]");
			nSeqNo = SEQ_LD_MODEL_CHANGE_CHK_BIT_OFF;

			GetDoc()->m_bNonStop_Run = FALSE;
		}
		break;

	case SEQ_LD_MODEL_CHANGE_CHK_BIT_OFF:	// Bit 해제 확인(CheckInterlock에서 Bit 해제)
		if(m_pMelsec->MelsecNetBitRecv( NON_STOP_PC_RESERVE ) == 0 && m_pMelsec->MelsecNetBitRecv( NON_STOP_PC_END ) == 0)
		{
			EqLog(indexSeqRcv, nSeqNo, "Check\t[NON_STOP_PC_RESERVE:0, NON_STOP_PC_END:0]");
			nSeqNo = SEQ_LD_CHK_REQUEST;
		}
		break;
////////////// 여기까지.

	case SEQ_LD_CHK_IN_DO_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_DO_SUPPLY );

		if( bFlag1 )
		{
			EqLog(indexSeqRcv, nSeqNo, "Check Bit On\t[LD_IN_DO_SUPPLY]");
			nSeqNo = SEQ_LD_CHK_IN_UP_BIT_ON;
		}
		break;

	case SEQ_LD_CHK_IN_UP_BIT_ON:
		{
			bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_HAND_UP );
			bFlag2 = m_pMelsec->MelsecNetBitRecv( LD_IN_END_SUPPLY );

			if(bFlag1 || bFlag2)
			{
				EqLog(indexSeqRcv, nSeqNo, "Check Bit On\t[LD_IN_HAND_UP]");
				nSeqNo = SEQ_LD_CHK_IN_END_BIT_ON;
			}
		}
		break;
	case SEQ_LD_CHK_IN_END_BIT_ON:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_END_SUPPLY );
		if(bFlag1)
		{
			//GetDeviceCtl()->EnableAreaSensor(FALSE);
			EqLog(indexSeqRcv, nSeqNo, "Check Bit On\t[LD_IN_END_SUPPLY]");
			m_pMelsec->MelsecNetBitOn( LD_OUT_END_SUPPLY);
			EqLog(indexSeqRcv, nSeqNo, "Bit On\t[LD_OUT_END_SUPPLY]");

			nSeqNo = SEQ_LD_CHK_IN_ALL_BIT_OFF;
		}
		break;

	case SEQ_LD_CHK_IN_ALL_BIT_OFF:
		bFlag1 = m_pMelsec->MelsecNetBitRecv( LD_IN_REQ_SUPPLY );
		bFlag2 = m_pMelsec->MelsecNetBitRecv( LD_IN_DO_SUPPLY );
		bFlag3 = m_pMelsec->MelsecNetBitRecv( LD_IN_END_SUPPLY );

		if( !bFlag1 && !bFlag2 && !bFlag3 )
		{
			EqLog(indexSeqRcv, nSeqNo, "Check Bit Off\t[LD_IN_REQ_SUPPLY, LD_IN_DO_SUPPLY, LD_IN_END_SUPPLY]");

			m_pMelsec->MelsecNetBitOff( LD_OUT_REQ_SUPPLY );
			m_pMelsec->MelsecNetBitOff( LD_OUT_DO_SUPPLY );
			m_pMelsec->MelsecNetBitOff( LD_OUT_END_SUPPLY );

			EqLog(indexSeqRcv, nSeqNo, "Bit Off\t[LD_OUT_REQ_SUPPLY, LD_OUT_DO_SUPPLY, LD_OUT_END_SUPPLY]");
			
			nSeqNo = SEQ_LD_DATA_READ;
		}
		break;

	case SEQ_LD_DATA_READ:
		// Read Glass Data.
		if( GetMainFrame()->m_bMelsecRead )
			break;
		GetMainFrame()->m_bMelsecRead = TRUE;

		m_pMelsec->GlassDataRead(m_PanelInfoLoaderBuf, DATA_INPUT_START);

		if( CheckLoaderGlassData() == FALSE )
		{
			// 최대 3회 시도하나 보다...
			if(nCnt1 > 2)
			{
				// Glass Data Read Fail.
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PANELDATA_RECEIVE_FAIL);
				nCnt1 = 0;
				nSeqNo = SEQ_LD_ERR;
			}
			else
			{
				nCnt1 += 1;
			}
		}
		else
		{
			nCnt1 = 0;
			EqLog(indexSeqRcv, nSeqNo, "Check\t[CheckLoaderGlassData]");
			// Glass Data Copy
			GlassDataCopy();
			// LastGlass.ini에 기록
			SaveLastGlass();
			// Loader Clear
			ClearGlassData(m_PanelInfoLoaderBuf);
			nSeqNo = SEQ_LD_COMPLETE;
		}
		break;

	case SEQ_LD_ERR:
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_NORMAL, ERR_CANCEL);
		GetApp()->ChangeStatus(STATUS_ERROR);

		nSeqNo = SEQ_LD_CHK_REQUEST;
		break;
	case SEQ_LD_COMPLETE:
		if( GetMainFrame()->m_bMelsecWrite )
			break;
		GetMainFrame()->m_bMelsecWrite = TRUE;
		// PLC에서 볼 수 있도록 Write
		m_pMelsec->GlassDataWrite(m_PanelInfo, DATA_OUTPUT_START);

		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcLoadingDone, NULL);

		m_SeqFlag.bCellInReq = FALSE;
		m_SeqFlag.bCellInComp = TRUE;
		EqLog(indexSeqRcv, nSeqNo, "Set\t[bCellInReq:0, bCellInComp:1]");
		// START중에는 Loading & Unloading Bit 항상 On
// 		m_pMelsec->MelsecNetBitOn(LD_OUT_READY);
		nSeqNo = SEQ_LD_CHK_REQUEST;
		break;
	default:
		break;
	}

	m_nSeqCellRecvNo = nSeqNo ;
}
void CEqMainSequence::SeqCellContProc()
{
	if(GetApp()->IsRunStatus() == FALSE)
		return;

	int nSeqNo = m_nSeqCellContactNo ;

	//BOOL bSearch_Result = FALSE;//  [7/6/2009 janghan]

	CString Tilt_Err = "";//  [8/13/2009 janghan]

	static DWORD wCellConTimer = 0;
	static DWORD wTiltTimer = 0;
	int nRet = ERR_NONE;

	int nTiltCount = 0 ;
	int nTiltUpCount = 0;

	double dPos1, dPos2;
	CString strLog;
	BOOL bReq, bDo, bEnd;

	if(m_bPGStart == TRUE)
		nSeqNo = MAIN1_SND_PG_START;
	if(m_bPGStop == TRUE)
		nSeqNo = MAIN1_CHK_PG_END;

	switch(nSeqNo)
	{
	case MAIN_CHK_INTERLOCK:
		if( !GetDeviceCtl()->ChkArmStretch() )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				if( !GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) )
				{
					wCellConTimer = GetTickCount() ;
					GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_PRE_SIGN);
					EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign On.");
				}
			}
			nSeqNo = MAIN_BF_TILT_DOWN;
		}
		else
			nSeqNo = MAIN_CHK_INTERLOCK;
// 		else if( (GetTickCount() - wCellConTimer) > 300000 )
// 		{
// 			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_HANDLER_POS_TIMEOUT);	// 자동 런 중에 1분 이상 Arm Stretch 신호가 들어오지 않을 경우 알람 발생.
// 			nSeqNo = MAIN_ERR ;
// 		}
		break;

	case MAIN_BF_TILT_DOWN:
		if( (GetTickCount() - wCellConTimer ) > 1000 )
		{
			nRet = GetDeviceCtl()->StartMoveTilting(FALSE);
			if( nRet == MOVE_OK_START )
			{
				wCellConTimer = GetTickCount() ;
				EqLog(indexSeqCont, nSeqNo, "Move Start\t[Tilting Down]");
				nSeqNo = MAIN_BF_CHK_TILT_DOWN ;
			}
			else if( nRet < MOVE_ERR_NONE )
			{
				//GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
				//nSeqNo = MAIN_ERR ;
				nSeqNo = MAIN_CHK_INTERLOCK	;	// 여기서 핸들러 stretch가 빠질 때까지 기다리면 되지 굳이 에러 띄울 필요가...
			}
		}
		break;

	case MAIN_BF_CHK_TILT_DOWN:
		if( (GetTickCount() - wCellConTimer) > 2000 )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{	// 2009.06.15
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_MOVE_READY ) )
				{
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_READY );
					EqLog(indexSeqCont, nSeqNo, "MEL_SW2_TILT_MOVE_READY Bit Off");
				}
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_BUSY ) && GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
				{
					wTiltTimer = GetTickCount() ;
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
					EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[BUSY]");
				}
			}

			nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
			if( nRet == MOVE_OK_END )
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
				{
					if( GetMainFrame()->m_bMelsecRead )
						break;
					GetMainFrame()->m_bMelsecRead = TRUE;
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_CUR_POS, dPos1);
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_READY_POS, dPos2);
					GetMainFrame()->m_bMelsecRead = FALSE;

					if( dPos1 <= ( dPos2 + 1.0 ) )
					{
						if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
						{
							wTiltTimer = GetTickCount() ;
							GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
							EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[COMP]");
						}
						EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Down]");
						nSeqNo = MAIN_BF_CONTACT_OFF ;
						nTiltCount = 0;
					}
					else
					{
						EqLog(indexSeqCont, nSeqNo, "Move End Fail\t[Go MAIN_CHK_INTERLOCK]");
						strLog.Format(_T("Compare Position : CurPos = %.3f , ReadyPos = %.3f"), dPos1, dPos2);
						EqLog(indexSeqCont, nSeqNo, strLog);
						nSeqNo = MAIN_CHK_INTERLOCK ;
					}
				}
				else
				{
					EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Down]");
					nSeqNo = MAIN_BF_CONTACT_OFF ;
				}
			}
			else if( (nRet < MOVE_ERR_NONE))
			{
				if(nTiltCount == 3) // 계속적인 알람으로 인하여 3번더 Check하도록 수정  [9/30/2009 RomanticBoy]
				{
					if(!GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_AXIS_STOP ))
					{
						Tilt_Err = "MEL_SW2_TILT_ALARM";//  [8/13/2009 janghan]
						GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
						nSeqNo = MAIN_ERR ;
						nTiltCount = 0;
					}
				}
				else
				{
					nTiltCount++;
					nSeqNo = MAIN_CHK_INTERLOCK;
				}
			}
			else if ( ((GetTickCount() - wTiltTimer) > 30000) /*DioTimeout*/)
			{
				if(nTiltCount == 3)	// 계속적인 알람으로 인하여 3번더 Check하도록 수정  [9/30/2009 RomanticBoy]
				{
					if(!GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_AXIS_STOP ))
					{
						Tilt_Err = "Tilt DIO TimeOut";//  [8/13/2009 janghan]
						GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
						nSeqNo = MAIN_ERR ;
						nTiltCount = 0;
					}
				}
				else
				{
					nTiltCount++;
					nSeqNo = MAIN_CHK_INTERLOCK;
				}
			}
		}
		break;

	case MAIN_BF_CONTACT_OFF:
		wCellConTimer = GetTickCount() ;
		GetDeviceCtl()->ContactUnit(UP) ;
		EqLog(indexSeqCont, nSeqNo, "Do\t[Contact Up]");
		GetDoc()->BluOff();
		GetDoc()->BluOff();
// 		GetDoc()->OmitOff();
		GetDoc()->OmitOn();
		nSeqNo = MAIN_BF_CHK_CONTACT_OFF ;
		break;

	case MAIN_BF_CHK_CONTACT_OFF:
		nRet = GetDeviceCtl()->ChkContactUnitUp();
		if ( nRet == ERR_NONE )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[Contact Up]");
			nSeqNo = MAIN_BF_CLAMP_OFF ;
		}
		else if ( GetTickCount() - wCellConTimer > 2000 )
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, nRet);
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN_BF_CLAMP_OFF:
		{
			wCellConTimer = GetTickCount() ;
			GetDeviceCtl()->ClampUnit(SENSOR_OFF) ;
			EqLog(indexSeqCont, nSeqNo, "Do\t[Clamp Off]");
			nSeqNo = MAIN_BF_CHK_CLAMP_OFF ;
		}
		break;

	case MAIN_BF_CHK_CLAMP_OFF:
		if( GetDeviceCtl()->ChkClampUnit() )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[Clamp Off]");
			nSeqNo = MAIN_BF_MOVE_READY_POS ;
		}
		else if( GetTickCount() - wCellConTimer > 2000)
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TOP_CLAMP_OFF);
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN_BF_MOVE_READY_POS:	//
		nRet = GetDeviceCtl()->MoveStage(READY_POS, TRUE);
		if (nRet == MOVE_OK_END )
		{
			EqLog(indexSeqCont, nSeqNo, "Move\t[Ready]");
			if(GetApp()->m_RunMode == MODE_VISION_SKIP )
				nSeqNo = MAIN_CHK_GLS_EXIST ;
			else if(GetApp()->m_RunMode == MODE_ALIGN_SKIP)
			{
				nSeqNo = MAIN_GLS_CLAMP_ON;
			}
			else
			{
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;

				GetDoc()->m_bAutoTune[0] = FALSE; //  [6/16/2009 janghan]
				GetDoc()->m_bAutoTune[1] = FALSE; //  [6/16/2009 janghan]
				GetDoc()->m_bAutoTune[2] = FALSE; //  [6/16/2009 janghan]
				GetDoc()->m_bAutoTune[3] = FALSE; //  [6/16/2009 janghan]

				GetDoc()->m_bUsed_Tune = FALSE; //  [7/6/2009 janghan]
				

				nSeqNo = MAIN_BF_SEARCH_CAM0_PCB ;
			}
		}
		else if(nRet < MOVE_ERR_NONE)
		{
			// SetMsgNo()는 MoveStage()안에서 이미 호출됨
			nSeqNo = MAIN_ERR ;
		}
		break;


	case MAIN_BF_SEARCH_CAM0_PCB:
		if(GetDoc()->m_pThreadLiveGrap->CheckLivePause())
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 ) )
			{	
				if(GetDoc()->m_bAutoTune_Using) GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_0];//  [6/16/2009 janghan]

				//if( GetAlignCtl()->SearchPattern2 (CAM_0_PCB_1 , FALSE) )//  [6/16/2009 janghan]
				if( GetAlignCtl()->SearchPattern(3 , FALSE) )//  [7/6/2009 janghan] CAM_0_PCB 
				{
					EqLog(indexSeqCont, nSeqNo, "Do\t[SearchPattern(CAM_0_PCB , FALSE)]");
					nSeqNo = MAIN_BF_SEARCH_CAM1_PCB ;
				}
				else
				{
					if(GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune[CAM_0] = TRUE; //  [6/16/2009 janghan]
						GetDoc()->m_strAutoTune_Reason = "CAM_0 PCB Mark Fail";//  [6/21/2009 janghan]
						nSeqNo = MAIN_BF_SEARCH_CAM1_PCB;
					}
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT1_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT1_MARK_FAIL);
						nSeqNo = MAIN_ERR ;
					}					
				}
			}
			else
				nSeqNo = MAIN_BF_SEARCH_CAM1_PCB;
		}
		break;

	case MAIN_BF_SEARCH_CAM1_PCB:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 ) )
			{
				if(GetDoc()->m_bAutoTune_Using) GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_1];//  [6/16/2009 janghan]

				if( GetAlignCtl()->SearchPattern(7 , FALSE) )//  [6/16/2009 janghan]
				{
					EqLog(indexSeqCont, nSeqNo, "Do\t[SearchPattern(CAM_1_PCB , FALSE)]");
					nSeqNo = MAIN_BF_SEARCH_CAM2_PCB ;
				}
				else
				{
					if(GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune[CAM_1] = TRUE; //  [6/16/2009 janghan]
						GetDoc()->m_strAutoTune_Reason = "CAM_1 PCB Mark Fail";//  [6/21/2009 janghan]
						nSeqNo = MAIN_BF_SEARCH_CAM2_PCB;
					}
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT2_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT2_MARK_FAIL);
						nSeqNo = MAIN_ERR ;
					}
					
				}
			}
			else
				nSeqNo = MAIN_BF_SEARCH_CAM2_PCB;
		}
		break;

	case MAIN_BF_SEARCH_CAM2_PCB:
		{
			if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2 ) )
			{
				if(GetDoc()->m_bAutoTune_Using) GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_2];//  [6/16/2009 janghan]

				if( GetAlignCtl()->SearchPattern(11 , FALSE) )//  [6/16/2009 janghan]
				{
					EqLog(indexSeqCont, nSeqNo, "Do\t[SearchPattern(CAM_2_PCB , FALSE)]");
					nSeqNo = MAIN_BF_SEARCH_CAM3_PCB ;
				}
				else
				{
					if(GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune[CAM_2] = TRUE; //  [6/16/2009 janghan]
						GetDoc()->m_strAutoTune_Reason = "CAM_2 PCB Mark Fail";//  [6/21/2009 janghan]
						nSeqNo = MAIN_BF_SEARCH_CAM3_PCB;
					}
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT3_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT3_MARK_FAIL);
						nSeqNo = MAIN_ERR ;
					}				
				}
			}
			else
				nSeqNo = MAIN_BF_SEARCH_CAM3_PCB ;
		}
		break;

	case MAIN_BF_SEARCH_CAM3_PCB:
		if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
			( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3 ) ) 
		{
			if(GetDoc()->m_bAutoTune_Using) GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_3];//  [6/16/2009 janghan]

			if( GetAlignCtl()->SearchPattern(15 , FALSE) )//  [6/16/2009 janghan]
			{
				EqLog(indexSeqCont, nSeqNo, "Do\t[SearchPattern(CAM_3_PCB , FALSE)]");
				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
				nSeqNo = MAIN_CHK_GLS_EXIST ;
			}
			else
			{
				if(GetDoc()->m_bAutoTune_Using)
				{
					GetDoc()->m_bAutoTune[CAM_3] = TRUE; //  [6/16/2009 janghan]
					GetDoc()->m_strAutoTune_Reason = "CAM_3 PCB Mark Fail";//  [6/21/2009 janghan]
					nSeqNo = MAIN_CHK_GLS_EXIST;
				}
				else
				{
					if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT4_MARK_SCORE_FAIL);
					else
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT4_MARK_FAIL);
					nSeqNo = MAIN_ERR ;
				}
				
			}
		}
		else
			nSeqNo = MAIN_CHK_GLS_EXIST ;
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
		// 로딩중이었다면 로딩 완료 Sequence로 이동
		if(nSeqNo == MAIN_CHK_GLS_EXIST && m_SeqFlag.bCellInComp == FALSE)
			nSeqNo = MAIN_CHK_GLS_RECV_COMP ;
		break;

	case MAIN_CHK_GLS_EXIST:

		if(IsPanelUnloadComplecte())
		{
			bReq = m_pMelsec->MelsecNetBitRecv(UNLD_IN_REQ_SUPPLY);
			bDo = m_pMelsec->MelsecNetBitRecv(UNLD_IN_DO_SUPPLY);
			bEnd = m_pMelsec->MelsecNetBitRecv(UNLD_IN_END_SUPPLY);
			if( bReq || bDo || bEnd )
			{

				EqLog(indexSeqCont, nSeqNo, "Unloading Communication is not Clear");
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_UNLD_COMMUNICATION_CLEAR);
				nSeqNo = MAIN_ERR;
				break;
			}

			GetDoc()->m_szPanelIDOld = GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID;		// Last H-Panel ID 저장 [10/5/2009 RomanticBoy]

			EqLog(indexSeqCont, nSeqNo, "Check\t[IsPanelUnloadComplecte]");
			if (GetDeviceCtl()->ChkPanelExist() )
			{	// Panel Exist.
				EqLog(indexSeqCont, nSeqNo, "Check\t[ChkPanelExist]");
				nSeqNo = MAIN_UNSCRAP ;
				if( GetApp()->m_RunMode == MODE_STAGE )	// 2009.06.15
					nSeqNo = MAIN_REPORT_GLS_IN ;
			}
			else
			{	// Panel Not Exist
				nSeqNo = MAIN_GLS_RECV ;
			}
		}
		else 
		{
			if(m_SeqFlag.bCellOutReq == FALSE)
			{
				m_SeqFlag.bCellOutReq = TRUE;
				EqLog(indexSeqCont, nSeqNo, "Set\t[bCellOutReq:1]");
			}
		}
		break;

	case MAIN_UNSCRAP:	
		if(CheckGlassData())
		{
			m_pComm->PanelTransfer2LC(ePanelScrap) ;

			m_pComm->PanelTransfer2LC(ePanelUnScrap) ;
			nSeqNo = MAIN_REPORT_GLS_IN ;
		}
		else
		{
			if(m_bUnScrapping == FALSE)
			{
				m_bUnScrapping = TRUE;
				EqLog(indexSeqCont, nSeqNo, "Set\t[m_bUnScrapping:1]");
				::PostMessage(GetMainFrame()->m_hWnd, WM_UNSCRAP, NULL, NULL);
			}
		}
		break;

	case MAIN_GLS_RECV:
		if( m_SeqFlag.bCellInReq == FALSE )
		{
			m_SeqFlag.bCellInReq = TRUE ;
			EqLog(indexSeqCont, nSeqNo, "Set\t[bCellInReq:1]");
		}
		// 로딩하는 동안 미리 FPC를 찾아놓는다.
		if(GetApp()->m_RunMode == MODE_ALIGN_SKIP || GetApp()->m_RunMode == MODE_VISION_SKIP || GetApp()->m_RunMode == MODE_STAGE/*드라이런 테스트 // 2009.06.15*/)
		{
			nSeqNo = MAIN_CHK_GLS_RECV_COMP ;
		}
		else
		{
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
			nSeqNo = MAIN_BF_SEARCH_CAM0_PCB ;
		}
		break;

	case MAIN_CHK_GLS_RECV_COMP:
		if ( (m_SeqFlag.bCellInReq == FALSE) && m_SeqFlag.bCellInComp )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[bCellInReq:0, bCellInComp:1]");
			wCellConTimer = GetTickCount() ;
			nSeqNo = MAIN_REPORT_GLS_IN ;
		}
		break; 

	case MAIN_REPORT_GLS_IN:	// Send EQ & Process Status to LC. & Send Panel Transfer Status to LC.
		// 다시 한번 Check
		if (GetDeviceCtl()->ChkPanelExist() )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[bCellOutReq:0, bCellOutComp:1]");
			GetApp()->ChangeStatus(STATUS_RUN);	// 판넬이 들어오므로 READY에서 RUN으로 전환
			m_pComm->PanelTransfer2LC(ePanelIn) ;

			/////////////////////////////////////////////////////
			// 2011.01.13 hccho
			//
			// 위의 ChangeStatus(STATUS_RUN)에서 
			// EQ State = eNormal
			// Proc State = eExecute 
			// 로 설정하고 보고 하고 있음.
			//
			// 정상적으로 처리 된다면 아래 부분은 실행되지 않을 코드로 보임.
			//
			// 아마 State 변경과 보고가 되지 않는 경우가 있어 
			// 아래처럼 강제로 한번 더 하는것 같은데
			// 바뀌지 않는 원인을 찾는것을 추천 함.
			//
			// 일단 그대로 둠.
			//
			if(m_pComm->LoadLastEQState() != eNormal)
				m_pComm->EQState2LC(eNormal) ;
			if(m_pComm->LoadLastEQProcess() != eExecute)
				m_pComm->ProcessState2LC(eExecute) ;
			////////////////////////////////////////////////////
			nSeqNo = MAIN_CHK_INSP_FLAG ;
		}
		else if ( GetTickCount() - wCellConTimer > 2000/*DioTimeout*/ )
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_WT_PANELCHK);
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN_CHK_INSP_FLAG:
//		if( GetApp()->m_RunMode == MODE_STAGE )
//		{
//			nSeqNo = MAIN1_CHK_INTERLOCK;
//			break;
//		}
		nSeqNo = MAIN_GLS_CLAMP_ON ;
		break;

	case MAIN_GLS_CLAMP_ON:
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcGlassClampOn, NULL);
		wCellConTimer = GetTickCount() ;
		GetDeviceCtl()->ClampUnit(SENSOR_ON) ;
		EqLog(indexSeqCont, nSeqNo, "Do\t[Clamp On]");
		Sleep(100);	// ON을 확인할 수가 없어서 강제로 Sleep 적용
		nSeqNo = MAIN_CHK_GLS_CLAMP_ON ;
		break;

	case MAIN_CHK_GLS_CLAMP_ON:	// Check Top Clamp On
		if ( GetDeviceCtl()->ChkClampUnit() == FALSE)
		{
			Sleep(100);	// ON을 확인할 수가 없어서 강제로 Sleep 적용
			EqLog(indexSeqCont, nSeqNo, "Check\t[Clamp On]");
			if( GetMachinePCType() == A_AMT || GetMachinePCType() == B_AMT )
			{
				nSeqNo = MAIN2_ALIGN_START ;
			}
			else
			{
				// VI SKIP일때도 검사결과는가상으로생성하여 보고 [9/16/2010 OSC]
				if(GetApp()->m_RunMode == MODE_VISION_SKIP)
				{
					GlassDataToPGData();
					GetApp()->m_bInspectionResult = TRUE;
					PGDataToGlassData();
					wCellConTimer = GetTickCount() ;
					nSeqNo = MAIN_REPORT_IR ;
				}
				else if(GetApp()->m_RunMode == MODE_ALIGN_SKIP)
				{
					nSeqNo = MAIN1_CHK_INTERLOCK ;
				}
				else
					nSeqNo = MAIN1_ALIGN_START ; 
			}
		}
		else if ( GetTickCount() - wCellConTimer > 2000/*DioTimeout*/ )
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TOP_CLAMP_ON);
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN1_CHK_INTERLOCK:
		if( !GetDeviceCtl()->ChkArmStretch() )
		{
			if(GetApp()->m_RunMode != MODE_ALIGN_SKIP)
				nSeqNo = MAIN1_ALIGN_START;
			else
				nSeqNo = MAIN1_TILT_UP;

		}
		else if( (GetTickCount() - wCellConTimer) > 60000 )
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_HANDLER_POS_TIMEOUT);	// 자동 런 중에 1분 이상 Arm Stretch 신호가 들어오지 않을 경우 알람 발생.
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN1_TILT_UP:
		if( (GetTickCount() - wCellConTimer) > 500 )
		{
			if( !GetDeviceCtl()->ChkArmStretch() )
			{
				nRet = GetDeviceCtl()->StartMoveTilting(TRUE);
				if( nRet == MOVE_OK_START )
				{
					wCellConTimer = GetTickCount() ;
					EqLog(indexSeqCont, nSeqNo, "Move Start\t[Tilting Up]");
					if(GetApp()->m_RunMode == MODE_STAGE || GetApp()->m_RunMode == MODE_ALIGN_SKIP)
						nSeqNo = MAIN1_CHK_TILT_UP;
					else
						nSeqNo = MAIN1_SND_PG_START ;	// 2009.06.17
				}
				else if( nRet < MOVE_ERR_NONE )
				{
					nSeqNo = MAIN_ERR ;
					//nSeqNo = MAIN1_CHK_INTERLOCK ;
				}
			}
		}
		break;

	case MAIN1_ALIGN_START:	// Align
		if( GetApp()->m_RunMode == MODE_ALIGN_SKIP || GetApp()->m_RunMode == MODE_STAGE )
		{
		}
		else
		{	
			m_SeqFlag. bCellContactReq = TRUE ;
			EqLog(indexSeqCont, nSeqNo, "Set\t[bCellContactReq]");
			// PG로 cell info 전송.
		}
		nSeqNo = MAIN1_CHK_ALIGN_COMP ;
		break;

	case MAIN1_CHK_ALIGN_COMP:	// Wait CellContactComplete
		if( GetApp()->m_RunMode == MODE_ALIGN_SKIP || GetApp()->m_RunMode == MODE_STAGE )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{
				if( !GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) )
				{
					wCellConTimer = GetTickCount();
					GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_PRE_SIGN);
					EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign On.");
				}
			}
			nSeqNo = MAIN1_TILT_UP;
			break;
		}
		if ( m_SeqFlag. bCellContactComp )
		{
			if(m_SeqFlag.bCellContactCancel == FALSE)
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
				{
					if( !GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) )
					{
						wCellConTimer = GetTickCount();
						GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_PRE_SIGN);
						EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign On.");
					}
				}
				EqLog(indexSeqCont, nSeqNo, "Check Set\t[bCellContactComp]");
				nSeqNo = MAIN1_TILT_UP ;
			}
			else
			{
				nSeqNo = MAIN_ERR ;
			}
		}
		break;

	case MAIN1_SND_PG_START:
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcInspectionStart, NULL);
		GetDoc()->BluOn();
		GetDoc()->BluOn();
		Sleep(GetDoc()->m_dwContactDelay);
		GetComm()->PGSend(CMD_CELL_INFO);
		GetApp()->m_bInspectionEnd = FALSE;
		GetApp()->m_bInspectionResult = FALSE;
		GetApp()->m_bInspectionStart = TRUE;
		GetComm()->PGSend(CMD_INSPECTION_YES);
		EqLog(indexSeqCont, nSeqNo, "Inspection Start");

		if(GetApp()->m_RunMode != MODE_ALIGN_SKIP)
			nSeqNo = MAIN1_CHK_TILT_UP ;
		if(GetApp()->m_RunMode == MODE_ALIGN_SKIP)
		{
			m_bPGStart = FALSE;
			nSeqNo = MAIN1_CHK_PG_END;
		}
		if(GetDoc()->m_bInspCount)
		{
			if(GetDoc()->m_nInsp_Count <= GetDoc()->m_nInspection)
			{
				if(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID != GetDoc()->m_LastGlass.strHPanelID)					// Panel ID Check후 Count진행 [9/28/2009 RomanticBoy]
				{
					GetDoc()->m_nInspectionCount = GetDoc()->m_nInspectionCount + 1;							//  [9/10/2009 janghan] 
					GetDoc()->m_nInsp_Count = GetDoc()->m_nInsp_Count+1;
				}
			}
			else
			{
				CString strTemp;
				strTemp.Format("%d Count Reset by System",GetDoc()->m_nInspectionCount);
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, CHK_INSP_COUNT);
				nSeqNo = MAIN_ERR;
				GetDoc()->SetLog(strTemp, LOG_INSP_COUNT);
				GetDoc()->SetLog(strTemp, LOG_SYSTEM);
				GetDoc()->m_nInsp_Count = 0;
			}
		}
		else
		{
			if(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID != GetDoc()->m_LastGlass.strHPanelID)					// Panel ID Check후 Count진행 [9/28/2009 RomanticBoy]
			{
				if (GetDoc()->m_nInspectionCount > 999)
				{
					GetDoc()->m_nInspectionCount = 0;
					GetDoc()->SetLog("999 Count Reset by System", LOG_SYSTEM);

				}
				else
					GetDoc()->m_nInspectionCount = GetDoc()->m_nInspectionCount + 1;							//  [9/10/2009 janghan] 
			}
		}
		::PostMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_INSPECT_COUNT, NULL, NULL);//  [9/10/2009 janghan]
		GetDoc()->SaveInspectionCount();//  [9/10/2009 janghan]

		break;

	case MAIN1_CHK_TILT_UP:
		if( (GetTickCount() - wCellConTimer) > 1000 )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{	// 2009.06.15
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_MOVE_INSP ) )
				{
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_INSP );
					EqLog(indexSeqCont, nSeqNo, "MEL_SW2_TILT_MOVE_INSP Bit Off");
				}
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_BUSY ) && GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
				{
					wTiltTimer = GetTickCount() ;
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN ) ;
					EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[BUSY]") ;
				}
			}

			nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
			if( nRet == MOVE_OK_END )
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
				{
					if( GetMainFrame()->m_bMelsecRead )
						break;
					GetMainFrame()->m_bMelsecRead = TRUE;
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_CUR_POS, dPos1);
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_INSPECT_POS, dPos2);
					GetMainFrame()->m_bMelsecRead = FALSE;

					if( dPos1 >= ( dPos2 - 1.0 ) )
					{
						if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
						{
							wTiltTimer = GetTickCount() ;
							GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
							EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[COMP]");
						}
						EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Up]");
						if(GetApp()->m_RunMode != MODE_ALIGN_SKIP)
							nSeqNo = MAIN1_CHK_PG_END ;
						else
							break;
					}
					else
					{
						EqLog(indexSeqCont, nSeqNo, "Move End Fail\t[Go MAIN1_TILT_UP]");
						strLog.Format(_T("Compare Position : CurPos = %.3f , InspectPos = %.3f"), dPos1, dPos2);
						EqLog(indexSeqCont, nSeqNo, strLog);
						nSeqNo = MAIN1_TILT_UP ;
						break;
					}
				}
				else
				{
					EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Up]");
					nSeqNo = MAIN1_CHK_PG_END ;
				}
				nTiltUpCount = 0;
			}
			else if( nRet < MOVE_ERR_NONE )
			{
				if(nTiltUpCount == 3)
				{
					Tilt_Err = "MEL_SW2_TILT_ALARM";//  [8/13/2009 janghan]
					GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
					nTiltUpCount = 0;
					nSeqNo = MAIN_ERR ;
				}
				else
				{
					nTiltUpCount++;
					nSeqNo = MAIN1_CHK_TILT_UP;
				}
			}
			else if ( GetTickCount() - wTiltTimer > 30000/*DioTimeout*/ )
			{
				if(nTiltUpCount == 3)
				{
					Tilt_Err = "Tilt DIO TimeOut";//  [8/13/2009 janghan]
					GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
					nTiltUpCount = 0;
					nSeqNo = MAIN_ERR ;
				}
				else
				{
					nTiltUpCount++;
					nSeqNo = MAIN1_CHK_TILT_UP;
				}
			}
		}
		break ;

	case MAIN1_CHK_PG_END:
		// STAGE Mode면 4초후에 검사 종료
		if(GetApp()->m_RunMode == MODE_STAGE)
		{
			if( (GetTickCount() - wCellConTimer) < 15000 ) break;
			if( (GetComm()->m_TactInspection.Stop(FALSE) > 14.0) && (GetApp()->m_bInspectionEnd == FALSE) )
			{
				GetApp()->m_bInspectionStart = FALSE;
				GetApp()->m_bInspectionEnd = TRUE;
				GetApp()->m_bInspectionResult = TRUE;
				GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
				GetComm()->m_TactInspection.Start();
			}
		}
		if(GetApp()->m_bInspectionEnd)
		{
			// Stage Mode면 m_bInspectionEnd Flag Set 후 1.5초 더 기다린다.
			if( (GetApp()->m_RunMode == MODE_STAGE) && (GetComm()->m_TactInspection.Stop(FALSE) < 1.5) )
			{
				nSeqNo = MAIN1_CHK_INTERLOCK_ ;

			}
			else
			{
				EqLog(indexSeqCont, nSeqNo, "Inspection End");
				GetDoc()->BluOff();
				GetDoc()->BluOff();
// 				GetDoc()->OmitOff();
				if(GetApp()->m_RunMode != MODE_ALIGN_SKIP)
					nSeqNo = MAIN1_CONTACT_OFF ;
				else
				{
					if(m_bPGStop == TRUE)
					{
						m_bPGStop = FALSE;
						nSeqNo = MAIN1_TILT_DOWN ;
					}
					else
						nSeqNo = MAIN1_CHK_PG_END;
				}
			}
		}
		break;

	case MAIN1_CONTACT_OFF:
		wCellConTimer = GetTickCount() ;
		GetDeviceCtl()->ContactUnit(SENSOR_OFF) ;
		EqLog(indexSeqCont, nSeqNo, "Do\t[Contact Up]");
		nSeqNo = MAIN1_CHK_CONTACT_OFF ;
//		m_pComm->InspectionResult2LC(m_PanelInfo.m_sCode , m_PanelInfo.m_sJudgement) ;
		break;

	case MAIN1_CHK_CONTACT_OFF:
		::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcReleaseContact, NULL);
		if ( GetDeviceCtl()->ChkContactUnitUp() == ERR_NONE )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[Contact Up]");
			nSeqNo = MAIN1_CHK_INTERLOCK_ ;
		}
		else if ( GetTickCount() - wCellConTimer > 2000 )
		{// Error
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, GetDeviceCtl()->ChkContactUnitUp());
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN1_CHK_INTERLOCK_:
		if( !GetDeviceCtl()->ChkArmStretch() )
		{
			if( !GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) && (GetLineType() == REMOVE_TILTING_TYPE) )
			{
				wCellConTimer = GetTickCount();
				GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_PRE_SIGN);
				EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign On.");
			}
			nSeqNo = MAIN1_TILT_DOWN;
		}
		else if( (GetTickCount() - wCellConTimer) > 300000 )
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_HANDLER_POS_TIMEOUT);	// 자동 런 중에 1분 이상 Arm Stretch 신호가 들어오지 않을 경우 알람 발생.
			nSeqNo = MAIN_ERR ;
		}
		else
			nSeqNo = MAIN1_CHK_INTERLOCK;
		break;

	case MAIN1_TILT_DOWN:
		nRet = GetDeviceCtl()->StartMoveTilting(FALSE);
		if( nRet == MOVE_OK_START )
		{
			wCellConTimer = GetTickCount() ;
			EqLog(indexSeqCont, nSeqNo, "Move Start\t[Tilting Down]");
			//nSeqNo = MAIN1_CHK_TILT_DOWN;
			if(GetApp()->m_RunMode != MODE_ALIGN_SKIP)
				nSeqNo = MAIN1_MOVE_READY_POS ;
			else
				nSeqNo = MAIN1_CHK_TILT_DOWN;
		}
		else if( nRet < MOVE_ERR_NONE )
			nSeqNo = MAIN1_CHK_INTERLOCK_ ;
		break;

	case MAIN1_MOVE_READY_POS:
		nRet = GetDeviceCtl()->MoveStage(READY_POS, TRUE);
		if (nRet == MOVE_OK_END )
		{
			EqLog(indexSeqCont, nSeqNo, "Move\t[Ready]");
			nSeqNo = MAIN1_CHK_TILT_DOWN ;
		}
		else if(nRet < MOVE_ERR_NONE)
		{
			// SetMsgNo()는 MoveStage()안에서 이미 호출됨
			nSeqNo = MAIN_ERR ;
		}
		break;

	case MAIN1_CHK_TILT_DOWN:
		if( (GetTickCount() - wCellConTimer) > 1000 )
		{
			if( GetLineType() == REMOVE_TILTING_TYPE )
			{	// 2009.06.15
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_MOVE_READY ) )
				{
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_READY );
					EqLog(indexSeqCont, nSeqNo, "MEL_SW2_TILT_MOVE_READY Bit Off");
				}
				if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_BUSY ) && GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN ) )
				{
					wTiltTimer = GetTickCount() ;
					GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
					EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[BUSY]");
				}
			}

			nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
			if( nRet == MOVE_OK_END )
			{
				if( GetLineType() == REMOVE_TILTING_TYPE )
				{
					if( GetMainFrame()->m_bMelsecRead )
						break;
					GetMainFrame()->m_bMelsecRead = TRUE;
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_CUR_POS, dPos1);
					GetMelsec()->MelsecNetPosRead(TILT_INPUT_READY_POS, dPos2);
					GetMainFrame()->m_bMelsecRead = FALSE;

					if( dPos1 <= ( dPos2 + 1.0 ) )
					{
						if( GetMelsec()->MelsecNetBitRecv(MEL_SW2_TILT_PRE_SIGN) )
						{
							wTiltTimer = GetTickCount() ;
							GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_PRE_SIGN);
							EqLog(indexSeqCont, nSeqNo, "Tilt Pre-sign Off.[COMP]");
						}
						EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Down]");
						nSeqNo = MAIN_REPORT_IR ;
						nTiltCount = 0;
					}
					else
					{
						EqLog(indexSeqCont, nSeqNo, "Move End Fail\t[Go MAIN1_CHK_INTERLOCK_]");
						strLog.Format(_T("Compare Position : CurPos = %.3f , ReadyPos = %.3f"), dPos1, dPos2);
						EqLog(indexSeqCont, nSeqNo, strLog);
						nSeqNo = MAIN1_CHK_INTERLOCK_ ;
					}
				}
				else
				{
					EqLog(indexSeqCont, nSeqNo, "Move End\t[Tilting Down]");
					nSeqNo = MAIN_REPORT_IR ;
				}
			}
			else if(( nRet < MOVE_ERR_NONE))
			{
				if(nTiltCount == 3)
				{
					if(!GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_AXIS_STOP ))
					{
						Tilt_Err = "MEL_SW2_TILT_ALARM";//  [8/13/2009 janghan]
						GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

						EqLog(indexSeqCont, nSeqNo, "Move Fail\t[Tilting Down]\tnRet < MOVE_ERR_NONE");
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
						nSeqNo = MAIN_ERR ;
						nTiltCount = 0;
					}
				}
				else
				{
					nTiltCount++;
					nSeqNo = MAIN1_CHK_INTERLOCK_;
				}
			}
			else if ( (GetTickCount() - wTiltTimer > 30000)/*DioTimeout*/ )
			{
				if(nTiltCount == 3)
				{
					if(!GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_AXIS_STOP ))
					{
						Tilt_Err = "Tilt DIO TimeOut";//  [8/13/2009 janghan]
						GetDoc()->SetLog(Tilt_Err, LOG_TILT_ERR);//  [8/13/2009 janghan]

						EqLog(indexSeqCont, nSeqNo, "Move Fail\t[Tilting Down]\tTimeOut.");
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TILTING_MOTOR_MOVING);
						nSeqNo = MAIN_ERR ;
						nTiltCount++;
					}
				}
				else
				{
					nTiltCount++;
					nSeqNo = MAIN1_CHK_INTERLOCK_;
				}
			}
		}
		break;

	case MAIN2_ALIGN_START:	// Align
		{
			if( GetApp()->m_RunMode != MODE_ALIGN_SKIP )
			{
				m_SeqFlag. bCellContactReq = TRUE ;
				// PG로 cell info 전송.
				nSeqNo = MAIN2_CHK_ALIGN_COMP ;
			}
			else
				nSeqNo = MAIN2_SND_PRE_GRAB_START ;
		}
		break;

	case MAIN2_CHK_ALIGN_COMP:	// Wait CellContactComplete
		{
			if ( m_SeqFlag. bCellContactComp )
			{
				nSeqNo = MAIN1_SND_PG_START ;
			}
		}
		break;

	case MAIN2_SND_PRE_GRAB_START:	// Send Pre Grab Start to VI
		{
			wCellConTimer = GetTickCount() ;
			nSeqNo = MAIN2_CHK_PRE_GRAB_END ;
		}
		break; 

	case MAIN2_CHK_PRE_GRAB_END:	// Wait Receive Pre Grab End from VI. 
		{
			//		if ( PRE_GRAB_END ) 
			nSeqNo = MAIN2_SND_POST_GRAB_START ;
			//		else if ( GetTickCount() - wTimer > 10000/*VITimeOut1*/ )
			// 비점등 Time Out.
		}
		break;

	case MAIN2_SND_POST_GRAB_START:	// Send Post Grab Start to LC
		{
			wCellConTimer = GetTickCount() ;
			nSeqNo = MAIN2_CHK_POST_GRAB_END ;
		}
		break;

	case MAIN2_CHK_POST_GRAB_END:	// Wait Receive Post Grab & Inspection End from VI
		{
			// 		if ( USE_CMD_UNLOAD ) 
			// 			nSeqNo = 200 ;
			// 		else if ( USE_CMD_INSP_END )
			nSeqNo = MAIN2_CONTACT_OFF ;
			//		if ( GetTickCount() - wTimer > 90000/*VITimeOut2*/ )
			// 점등 Time Out.
		}
		break;

	case MAIN2_CONTACT_OFF:
		{
			wCellConTimer = GetTickCount() ;
			GetDeviceCtl()->ContactUnit(SENSOR_OFF) ;
			nSeqNo = MAIN2_CHK_CONTACT_OFF ;
		}
		break;

	case MAIN2_CHK_CONTACT_OFF:
		{
			if ( GetDeviceCtl()->ChkContactUnitUp() )
				nSeqNo = MAIN2_MOVE_READY_POS ;
			else if ( GetTickCount() - wCellConTimer > 2000 )
			{// Error
			}
		}
		break;

	case MAIN2_MOVE_READY_POS:
		{
			GetDeviceCtl()->MoveStage( READY_POS , FALSE ) ;
			nSeqNo = MAIN2_CHK_INSP_END ;
		}
		break;

	case MAIN2_CHK_INSP_END:
		{
			nSeqNo = MAIN2_CHK_QA ;
		}
		break;

	case MAIN2_CHK_QA:	// Hold 및 Deffect 관련 확인 작업 수행.
		{
			// 		if ( HoldCheck(sCode) )
			// 			;// Error
			// 		else if ( DeffectCnt(sCode) > VIErrCnt )
			// 		// Error
			// 		else
			nSeqNo = MAIN_REPORT_IR ;
		}
		break;

	case MAIN_REPORT_IR:
		if(GetApp()->m_bInspectionResult)
		{
			if(GetApp()->m_RunMode == MODE_STAGE)	// 검사할 때만 LC 전송
			{
				nSeqNo = MAIN_CLAMP_OFF ;
			}
			else
			{
				if(GetApp()->m_RunMode == MODE_VISION_SKIP)
				{
					if ( GetTickCount() - wCellConTimer > 10000/*DioTimeout*/ )
					{
						m_pComm->InspectionResult2LC(m_PanelInfo.m_sCode , m_PanelInfo.m_sJudgement) ;

						EqLog(indexSeqCont, nSeqNo, "Do\t[InspectionResult2LC]");
						nSeqNo = MAIN_CLAMP_OFF ;
					}
				}
				else
				{
					if(GetComm()->m_bQCFlag)
						SetQCSampling();
// 					m_pComm->InspectionResult2LC(m_PanelInfo.m_sCode , m_PanelInfo.m_sJudgement) ;

					EqLog(indexSeqCont, nSeqNo, "Do\t[InspectionResult2LC]");
					if(GetComm()->m_bQCFlag)	// QC
					{
						m_strLogText.Format("Check\t[QC:1]\tJudgenemt:%s, Code:%s", m_PanelInfo.m_sJudgement, m_PanelInfo.m_sCode);					
						EqLog(indexSeqCont, nSeqNo, m_strLogText);
					}
					nSeqNo = MAIN_CLAMP_OFF ;
				}
			}
		}
		break;
	case MAIN_CLAMP_OFF:
		wCellConTimer = GetTickCount() ;
		GetDeviceCtl()->ClampUnit(SENSOR_OFF) ;
		EqLog(indexSeqCont, nSeqNo, "Do\t[Clamp Off]");
		nSeqNo = MAIN_CHK_CLAMP_OFF ;
		break;
	case MAIN_CHK_CLAMP_OFF:
		if( GetDeviceCtl()->ChkClampUnit() )
		{
			EqLog(indexSeqCont, nSeqNo, "Check\t[Clamp Off]");
 		//	if(GetApp()->m_RunMode == MODE_STAGE)	// 이 3줄 주석 해제하면 Tilting만 반복 [5/27/2009 OSC]
 		//		nSeqNo = MAIN_END ;					//
 		//	else									//
				nSeqNo = MAIN_GLS_SEND ;
		}
		else if( GetTickCount() - wCellConTimer > 2000)
		{
			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_TOP_CLAMP_OFF);
			nSeqNo = MAIN_ERR ;
		}
		break;
	case MAIN_GLS_SEND:	// Glass Send
		m_SeqFlag.bCellOutReq = TRUE ;
		EqLog(indexSeqCont, nSeqNo, "Set\t[bCellOutReq:1]");
		nSeqNo = MAIN_END ;
		break;

	case MAIN_ERR:
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_NORMAL, ERR_CANCEL);
		GetApp()->ChangeStatus(STATUS_ERROR);

		nSeqNo = MAIN_CHK_INTERLOCK;
		break;
	case MAIN_END:
		m_nCntClean++; // 검사 완료시마다 Clean Count [5/9/2009 OSC]
		nSeqNo = MAIN_CHK_GLS_EXIST ;
		break; 
	}

	m_nSeqCellContactNo = nSeqNo ;
}

void CEqMainSequence::Log(CString strLogMsg)
{

	CString strMsg;

	strMsg.Format("[EQ]\t%s" , strLogMsg );
	GetDoc()->SetLog( strMsg , LOG_EQ);
}

void CEqMainSequence::SeqContactUnit()
{
	int nSeqNo = m_nSeqContactNo;
	DWORD dTimer = 0 ;
	int nRet;

	CString strLog_AutoTune;
	BOOL bSearch_Result;



	switch( nSeqNo )
	{
	case SEQ_CONTACT_CHK_REQUEST:
		{
			if( m_SeqFlag.bCellContactReq )
			{
				EqLog(indexContUnit, nSeqNo, "Align Start.");
				m_SeqFlag.bCellContactComp = FALSE ;
				nSeqNo = SEQ_CONTACT_CHK_INTERLOCK;
			}
		}
		break;

	case SEQ_CONTACT_CHK_INTERLOCK:
		// 통신 및 Interlock check
		if( !GetApp()->IsRunStatus() )
		{
			if( GetDeviceCtl()->ChkArmStretch() )
			{
  				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_HANDLER_CRASH_POS);
				nSeqNo = SEQ_CONTACT_ERROR;
				break;
			}
		}
		//if( GetDeviceCtl()->ChkArmStretch() == FALSE )
		//{
			// Blower Off
			GetDeviceCtl()->BlowUnit(SENSOR_OFF);
			dTimer = GetTickCount() ;
			GetDeviceCtl()->ContactUnit(UP);
			EqLog(indexContUnit, nSeqNo, "Do\tBlow Off, Contact Off.");
			nSeqNo = SEQ_CONTACT_UNIT_OFF_CHK;
		//}
		//else
		//{//Error.
		//	GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_HANDLER_CRASH_POS);
		//	nSeqNo = SEQ_CONTACT_ERROR;
		//}
		break;
	case SEQ_CONTACT_UNIT_OFF_CHK:
		{
			nRet = GetDeviceCtl()->ChkContactUnitUp();
			if( nRet == ERR_NONE )
			{
				EqLog(indexContUnit, nSeqNo, "Check\tBlow Off, Contact Off.");
				// 진행중이면 사전에 PCB를 Search하기 때문에 PCB Search 구간은 건너뛴다
				if(GetApp()->IsRunStatus())
					nSeqNo = SEQ_CONTACT_BLOW_ON ;
				else
					nSeqNo = SEQ_CONTACT_MOVE_READYPOS ;
			}
			else if(GetTickCount() - dTimer > 2000)
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, nRet);
				nSeqNo = SEQ_CONTACT_ERROR;
			}
		}
		break;

	case SEQ_CONTACT_MOVE_READYPOS:
		{
			if( !GetDeviceCtl()->ChkArmStretch() )
			{
				nRet = GetDeviceCtl()->MoveStage(READY_POS, TRUE);
				if (nRet == MOVE_OK_END )
				{
					// 미리 Live 일시정지
					EqLog(indexContUnit, nSeqNo, "Move Ready Position.");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
					nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB ;
				}
				else if(nRet < MOVE_ERR_NONE)
				{
					nSeqNo = SEQ_CONTACT_ERROR;
				}
			}
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM0_PCB:
		if(GetApp()->m_RunStatus == STATUS_RUN)
			::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcAlign, NULL);
		if(GetDoc()->m_pThreadLiveGrap->CheckLivePause())
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 ) )
			{
				if (GetDoc()->m_bUsed_Tune && GetDoc()->m_bAutoTune_Using)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_0];
					bSearch_Result =  GetAlignCtl()->SearchPattern2(CAM_0_PCB_1 , FALSE);
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(3 , FALSE);

				if( bSearch_Result)
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM0_PCB");
					nSeqNo = SEQ_CONTACT_SEARCH_CAM1_PCB ;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune_RUN = TRUE;
						GetDoc()->m_nCamera_Auto = CAM_0;
						GetDoc()->m_fAutoDistance_X =  0.;
						GetDoc()->m_fPCBmarkFind_X = 0.;
						GetDoc()->m_fMarkFind_X = 0.;
						GetDoc()->m_bAutoTune[CAM_0] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_0 PCB Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_0 PCB Mark Fail");

						nSeqNo = SEQ_CONTACT_BLOW_ON;
					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT1_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT1_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}					
				}
			}
			else
				nSeqNo = SEQ_CONTACT_SEARCH_CAM1_PCB;
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM1_PCB:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 ) )
			{
				if (GetDoc()->m_bUsed_Tune && GetDoc()->m_bAutoTune_Using)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_1];
					bSearch_Result =  GetAlignCtl()->SearchPattern2(CAM_1_PCB_1 , FALSE);
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(7 , FALSE);


				if( bSearch_Result )
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM1_PCB");
					nSeqNo = SEQ_CONTACT_SEARCH_CAM2_PCB ;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune_RUN = TRUE;
						GetDoc()->m_nCamera_Auto = CAM_1;
						GetDoc()->m_fAutoDistance_X =  0.;
						GetDoc()->m_fPCBmarkFind_X = 0.;
						GetDoc()->m_fMarkFind_X = 0.;
						GetDoc()->m_bAutoTune[CAM_1] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_1 PCB Mark Fail";//  [6/21/2009 janghan] 
						EqLog(indexContUnit, nSeqNo, "CAM_1 PCB Mark Fail");

						nSeqNo = SEQ_CONTACT_BLOW_ON;
					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT2_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT2_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}

				}
			}
			else
				nSeqNo = SEQ_CONTACT_SEARCH_CAM2_PCB;
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM2_PCB:
		{
			if(GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2 ) )
			{
				if (GetDoc()->m_bUsed_Tune && GetDoc()->m_bAutoTune_Using)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_2];
					bSearch_Result =  GetAlignCtl()->SearchPattern2(CAM_2_PCB_1 , FALSE);
				} 
				else bSearch_Result =  GetAlignCtl()->SearchPattern(11 , FALSE);


				if( bSearch_Result)
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM2_PCB");
					nSeqNo = SEQ_CONTACT_SEARCH_CAM3_PCB ;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune_RUN = TRUE;
						GetDoc()->m_nCamera_Auto = CAM_2;
						GetDoc()->m_fAutoDistance_X =  0.;
						GetDoc()->m_fPCBmarkFind_X = 0.;
						GetDoc()->m_fMarkFind_X = 0.;
						GetDoc()->m_bAutoTune[CAM_2] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_2 PCB Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_2 PCB Mark Fail");

						nSeqNo = SEQ_CONTACT_BLOW_ON;
					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT3_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT3_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}

				}
			}
			else
				nSeqNo = SEQ_CONTACT_SEARCH_CAM3_PCB ;
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM3_PCB:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3 ) ) 
			{
				if (GetDoc()->m_bUsed_Tune && GetDoc()->m_bAutoTune_Using)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizePCB_Mark[CAM_3];
					bSearch_Result =  GetAlignCtl()->SearchPattern2(CAM_3_PCB_1 , FALSE);
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(15 , FALSE);


				if( bSearch_Result )
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM3_PCB");
					nSeqNo = SEQ_CONTACT_BLOW_ON ;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						GetDoc()->m_bAutoTune_RUN = TRUE;
						GetDoc()->m_nCamera_Auto = CAM_3;
						GetDoc()->m_fAutoDistance_X =  0.;
						GetDoc()->m_fPCBmarkFind_X = 0.;
						GetDoc()->m_fMarkFind_X = 0.;
						GetDoc()->m_bAutoTune[CAM_3] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_3 PCB Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_3 PCB Mark Fail");

						nSeqNo = SEQ_CONTACT_BLOW_ON;
					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT4_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_PCB_CONTACT4_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}

				}
			}
			else
				nSeqNo = SEQ_CONTACT_BLOW_ON ;
		}
		break;


	case SEQ_CONTACT_BLOW_ON:
		{
			GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
			GetDeviceCtl()->BlowUnit(SENSOR_ON);
			EqLog(indexContUnit, nSeqNo, "Do\tBlow On.");
			if (GetDoc()->m_bAutoTune[0] || GetDoc()->m_bAutoTune[1] || GetDoc()->m_bAutoTune[2] || GetDoc()->m_bAutoTune[3] )
			{
				GetDoc()->m_bAutoTune_RUN = TRUE;
			}
			m_nYBackCnt = 0;
			nSeqNo = SEQ_CONTACT_MOVE_GRABPOS;
		}
		break;

	case SEQ_CONTACT_MOVE_GRABPOS:
		{  	
			if (GetDoc()->m_nYaxis_Back != 99)
			{
				nRet = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nYaxis_Back);
				EqLog(indexContUnit, nSeqNo, "MoveStage_Auto.");
			}
			else
			{
				if( !GetDeviceCtl()->ChkArmStretch() )
					nRet = GetDeviceCtl()->MoveStage(GRAB_POS, TRUE);
				else
					break;
			}

			//nRet = GetDeviceCtl()->MoveStage(GRAB_POS, TRUE);
			if (nRet == MOVE_OK_END )
			{
				EqLog(indexContUnit, nSeqNo, "Move Grab Position.");
				// 미리 Live 일시정지
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
				if (GetDoc()->m_bAutoTune_RUN && GetDoc()->m_bAutoTune_Using)//  AutoTune 시작 [6/11/2009 janghan]
				{
					GetDoc()->m_bUsed_Tune = TRUE;
					EqLog(indexContUnit, nSeqNo, "Auto Tune Start.");

					if (GetDoc()->m_bAutoTune[0]) GetDoc()->m_nCamera_Auto = CAM_0; 
					else if (GetDoc()->m_bAutoTune[1]) GetDoc()->m_nCamera_Auto = CAM_1;
					else if (GetDoc()->m_bAutoTune[2]) GetDoc()->m_nCamera_Auto = CAM_2;
					else if (GetDoc()->m_bAutoTune[3]) GetDoc()->m_nCamera_Auto = CAM_3;


					switch (GetDoc()->m_nCamera_Auto)
					{
					case CAM_0:
						EqLog(indexContUnit, nSeqNo, "Camera select : CAM_0.");
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();
						break;

					case CAM_1:
						EqLog(indexContUnit, nSeqNo, "Camera select : CAM_1.");
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();
						break;

					case CAM_2:
						EqLog(indexContUnit, nSeqNo, "Camera select : CAM_2.");
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();
						break;

					case CAM_3:
						EqLog(indexContUnit, nSeqNo, "Camera select : CAM_3.");
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();
						break;
					}
					//auto tune start 
					//GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
					GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;//  [6/28/2009 janghan]

					GetDoc()->m_fMaxScore = 0.f;

				}
				else nSeqNo = SEQ_CONTACT_SEARCH_CAM0_FPC ;
			}
			else if(nRet < MOVE_ERR_NONE)
			{
				nSeqNo = SEQ_CONTACT_ERROR ;
			}
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM0_FPC:
		{
			if(GetDoc()->m_pThreadLiveGrap->CheckLivePause())
			{
				if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
					( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 ) )
				{
					if (GetDoc()->m_bAutoTune_Using && GetDoc()->m_bUsed_Tune)
					{
						GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[CAM_0];
						bSearch_Result = GetAlignCtl()->SearchPattern2(CAM_0_FPC_0 , FALSE );
					} 
					else bSearch_Result = GetAlignCtl()->SearchPattern(CAM_0_FPC_0 , FALSE );					


					if( bSearch_Result )
					{
						EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM0_FPC");
						nSeqNo = SEQ_CONTACT_SEARCH_CAM1_FPC ;
					}
					else
					{
						if (GetDoc()->m_bAutoTune_Using)
						{
							/*if (GetDoc()->m_bUsed_Tune)
							{
							if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT1_MARK_SCORE_FAIL);
							else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT1_MARK_FAIL);
							nSeqNo = SEQ_CONTACT_ERROR ;
							} 
							else
							{*/
							GetDoc()->m_bAutoTune_RUN = TRUE;
							//GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
							GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;//  [6/28/2009 janghan]
							GetDoc()->m_fMaxScore = 0.f;
							GetDoc()->m_nCamera_Auto = CAM_0;
							GetDoc()->m_bAutoTune[CAM_0] = TRUE;
							GetDoc()->m_strAutoTune_Reason = "CAM_0 FFC Mark Fail";//  [6/21/2009 janghan]
							EqLog(indexContUnit, nSeqNo, "CAM_0 FFC Mark Fail");
							GetDoc()->m_nOldTime_Tune = GetTickCount();

							GetDoc()->m_bUsed_Tune = TRUE;

							nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_EDGE;
							//}

						} 
						else
						{
							if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
								GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT1_MARK_SCORE_FAIL);
							else
								GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT1_MARK_FAIL);
							nSeqNo = SEQ_CONTACT_ERROR ;
						}

					}
				}
				else
					nSeqNo = SEQ_CONTACT_SEARCH_CAM1_FPC;
			}
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM1_FPC:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 ) )
			{
				if (GetDoc()->m_bAutoTune_Using && GetDoc()->m_bUsed_Tune)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[CAM_1];
					bSearch_Result = GetAlignCtl()->SearchPattern2(CAM_1_FPC_1 , FALSE );
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(CAM_1_FPC_1-1 , FALSE );


				if( bSearch_Result )
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM1_FPC");
					nSeqNo = SEQ_CONTACT_SEARCH_CAM2_FPC ;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						/*if (GetDoc()->m_bUsed_Tune)
						{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT2_MARK_SCORE_FAIL);
						else
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT2_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
						} 
						else
						{*/
						GetDoc()->m_bAutoTune_RUN = TRUE;
						//GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;//  [6/28/2009 janghan]
						GetDoc()->m_fMaxScore = 0.f;
						GetDoc()->m_nCamera_Auto = CAM_1;
						GetDoc()->m_bAutoTune[CAM_1] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_1 FFC Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_1 FFC Mark Fail");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();

						GetDoc()->m_bUsed_Tune = TRUE;
						//}

					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT2_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT2_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}

				}
			}
			else
				nSeqNo = SEQ_CONTACT_SEARCH_CAM2_FPC;
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM2_FPC:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT ||
				(GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2) )
			{
				if (GetDoc()->m_bAutoTune_Using && GetDoc()->m_bUsed_Tune)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[CAM_2];
					bSearch_Result = GetAlignCtl()->SearchPattern2(CAM_2_FPC_1 , FALSE );
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(CAM_2_FPC_1-1 , FALSE );


				if( bSearch_Result )
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM2_FPC");
					nSeqNo = SEQ_CONTACT_SEARCH_CAM3_FPC;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						/*if (GetDoc()->m_bUsed_Tune)
						{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_SCORE_FAIL);
						else
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
						} 
						else
						{*/
						GetDoc()->m_bAutoTune_RUN = TRUE;
						//GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;//  [6/28/2009 janghan]
						GetDoc()->m_fMaxScore = 0.f;
						GetDoc()->m_nCamera_Auto = CAM_2;
						GetDoc()->m_bAutoTune[CAM_2];
						GetDoc()->m_strAutoTune_Reason = "CAM_2 FFC Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_2 FFC Mark Fail");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();

						GetDoc()->m_bUsed_Tune = TRUE;
						//	}

					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}

				}
			}
			else
				nSeqNo = SEQ_CONTACT_SEARCH_CAM3_FPC;
		}
		break;

	case SEQ_CONTACT_SEARCH_CAM3_FPC:
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				(GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_3) )
			{
				if (GetDoc()->m_bAutoTune_Using && GetDoc()->m_bUsed_Tune)
				{
					GetDoc()->m_nLevelCount = GetDoc()->m_nBinarizeFPC_Mark[CAM_3];
					bSearch_Result = GetAlignCtl()->SearchPattern2(CAM_3_FPC_1 , FALSE );
				} 
				else bSearch_Result = GetAlignCtl()->SearchPattern(CAM_3_FPC_1-1 , FALSE );


				if( bSearch_Result )
				{
					EqLog(indexContUnit, nSeqNo, "Comp\tSEQ_CONTACT_SEARCH_CAM3_FPC");
					nSeqNo = SEQ_CONTACT_CALIBRATION;
				}
				else
				{
					if (GetDoc()->m_bAutoTune_Using)
					{
						/*if (GetDoc()->m_bUsed_Tune)
						{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_SCORE_FAIL);
						else
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
						} 
						else
						{*/
						GetDoc()->m_bAutoTune_RUN = TRUE;
						//GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;//  [6/28/2009 janghan]
						GetDoc()->m_fMaxScore = 0.f;
						GetDoc()->m_nCamera_Auto = CAM_3;
						GetDoc()->m_bAutoTune[CAM_3] = TRUE;
						GetDoc()->m_strAutoTune_Reason = "CAM_3 FFC Mark Fail";//  [6/21/2009 janghan]
						EqLog(indexContUnit, nSeqNo, "CAM_3 FFC Mark Fail");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
						GetDoc()->m_nOldTime_Tune = GetTickCount();

						GetDoc()->m_bUsed_Tune = TRUE;
						//}

					} 
					else
					{
						if(GetAlignCtl()->m_dScore > 0 && GetAlignCtl()->m_dScore < LIMIT_SCORE)
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_SCORE_FAIL);
						else
							GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_FPC_CONTACT3_MARK_FAIL);
						nSeqNo = SEQ_CONTACT_ERROR ;
					}				

				}
			}
			else
				nSeqNo = SEQ_CONTACT_CALIBRATION;
		}
		break;

		//////////////////////////////////////////////////////////////////////////0카메라
	case SEQ_CONTACT_AUTOTUNE_CAM0_FFC_EDGE://  [6/11/2009 janghan]
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 ) )
			{
				//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
				if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCEdge_Max)
				{
					//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
					GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

					GetAlignCtl()->SearchPattern2(CAM_0_FPC_0 , FALSE);

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_EDGE;
				} 
				else
				{
					if (GetDoc()->m_fMaxScore > 0.f)
					{
						GetDoc()->m_nBinarizeFPC[CAM_0] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

						GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;

						GetDoc()->SaveBinarizeCondition();

						GetAlignCtl()->ContactPosCalibaration_FFC_Edge(GetDoc()->m_nCamera_Auto);//이동 위치 환산

						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE; //  [7/4/2009 janghan]

						EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_0 FFC Edge find complete.");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_MOVE_CENTER_CHK;

					} 
					else
					{	//PCB Edge 확인시작-> 못 찾을경우
						EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_0 FFC Edge find fail.");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_PCB_EDGE;

						//GetDoc()->m_nLevelCount = 100;//
						GetDoc()->m_nLevelCount = GetDoc()->m_nPCBEdge_Min;
						GetDoc()->m_fMaxScore = 0.f;
					}
				}
			}
			else
			{
				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM0_MOVE_CENTER_CHK:
		{
			GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

			if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //화면 중앙 이동
			{
				EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_0 Move Center.");
				GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

				GetDoc()->SaveBinarizeCondition();

				GetDoc()->SaveModelFile();// 최종 위치 저장

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;

				GetDoc()->m_nLevelCount = GetDoc()->m_nFFCMark_Min;

				GetDoc()->m_fMaxScore = 0.f;

				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]
			} 
			else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
				nSeqNo = SEQ_CONTACT_ERROR ;

				GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

				strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
					CAM_0, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0), GetDoc()->m_pMotion->GetCmdPos( AXIS_CCD_Y0),
					"CenterMove NG");
				GetDoc()->SetLog(strLog_AutoTune, LOG_AUTOTUNE);
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM0_PCB_EDGE:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_0_PCB_0 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_PCB_EDGE;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizePCB[CAM_0] =  GetDoc()->m_nTuneLevl;
					GetDoc()->SaveBinarizeCondition();
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
					nSeqNo = SEQ_CONTACT_ERROR ;
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

				}
				else//겹침 판정
				{
					GetDoc()->m_nYaxis_Back = 0;

					GetAlignCtl()->ContactPosCalibaration_FFC_Edge(CAM_0, TRUE); 

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_0 PCB Overlap.");

					nSeqNo = SEQ_CONTACT_MOVE_GRABPOS ;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
						"Pile NG");
				}
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_0_FPC_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC_Mark[CAM_0] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

					//GetDoc()->m_nLevelCount = 10;
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBMark_Min;

					GetDoc()->m_fMaxScore = 0.f;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_0 FFC Mark find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_PCB_MARK;
				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_0, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0),
						"FFC MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}

		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM0_PCB_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_0_PCB_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_PCB_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizePCB_Mark[CAM_0] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_CALIBRATION;

					GetDoc()->m_bAutoTune[CAM_0]=FALSE; 

					strLog_AutoTune.Format("%s, %d, %f, %f, %d, %d, %s, %d msec", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_0, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0),
						GetDoc()->m_nBinarizeFPC_Mark[CAM_0], GetDoc()->m_nBinarizePCB_Mark[CAM_0],
						GetDoc()->m_strAutoTune_Reason, GetTickCount() - GetDoc()->m_nOldTime_Tune);
					GetDoc()->SetLog(strLog_AutoTune, LOG_AUTOTUNE);

					if (GetDoc()->m_bAutoTune[CAM_1])
					{
						GetDoc()->m_nCamera_Auto = CAM_1;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;

					} 
					else if (GetDoc()->m_bAutoTune[CAM_2])
					{
						GetDoc()->m_nCamera_Auto = CAM_2;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;					}
					else if (GetDoc()->m_bAutoTune[CAM_3])
					{
						GetDoc()->m_nCamera_Auto = CAM_3;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;
					}
					else 
					{
						GetDoc()->m_bAutoTune_RUN = FALSE;
						GetDoc()->m_bAutoTune[CAM_0] = FALSE;
						GetDoc()->m_nYaxis_Back = 99;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
						//GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
						nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
					}
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_0, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0),
						"PCB MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}
		}
		break;

	//case SEQ_CONTACT_AUTOTUNE_CAM0_CALIBRATION:
	//	{
	//		GetDoc()->m_fAutoDistance_X =  (GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X) * GetApp()->m_dFovX;

	//		GetAlignCtl()->ContactPosCalibaration_SortX(CAM_0);//이동 위치 환산

	//		nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_SORT_X;

	//	}
	//	break;

	//case SEQ_CONTACT_AUTOTUNE_CAM0_SORT_X:
	//	{
	//		GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

	//		if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //동
	//		{
	//			GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

	//			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;
	//			GetDoc()->SaveBinarizeCondition();

	//			GetDoc()->SaveModelFile();// 최종 위치 저장

	//			GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
	//			GetDoc()->m_fMaxScore = 0.f;

	//			GetDoc()->m_bAutoTune[CAM_0]=FALSE; 

	//			if (GetDoc()->m_bAutoTune[CAM_1])
	//			{
	//				GetDoc()->m_nCamera_Auto = CAM_1;
	//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
	//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//			} 
	//			else if (GetDoc()->m_bAutoTune[CAM_2])
	//			{
	//				GetDoc()->m_nCamera_Auto = CAM_2;
	//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
	//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//			}
	//			else if (GetDoc()->m_bAutoTune[CAM_3])
	//			{
	//				GetDoc()->m_nCamera_Auto = CAM_3;
	//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
	//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//			}
	//			else 
	//			{
	//				GetDoc()->m_bAutoTune_RUN = FALSE;
	//				GetDoc()->m_bAutoTune[CAM_0] = FALSE;
	//				GetDoc()->m_nYaxis_Back = 99;
	//				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//				nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
	//			}
	//		} 
	//		else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
	//		{
	//			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
	//			nSeqNo = SEQ_CONTACT_ERROR ;
	//		}
	//	}
	//	break;
//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////1번 카메라
	case SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE://  [6/11/2009 janghan]
		{

			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_1_FPC_0 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_EDGE;
				break;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC[CAM_1] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신
					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;

					GetAlignCtl()->ContactPosCalibaration_FFC_Edge(GetDoc()->m_nCamera_Auto);//이동 위치 환산

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
					m_nYBackCnt = 0;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_1 FFC Edge find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_MOVE_CENTER_CHK;
					break;
				} 
				else
				{	//PCB Edge 확인시작-> 못 찾을경우
					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_1 FFC Edge find fail.");
					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_PCB_EDGE;

					//GetDoc()->m_nLevelCount = 100;//
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBEdge_Min;
					GetDoc()->m_fMaxScore = 0.f;
					break;
				}
			}
			//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM1_MOVE_CENTER_CHK:
		{
			GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

			if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //화면 중앙 이동
			{
				EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_1 Move Center.");
				GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

				GetDoc()->SaveBinarizeCondition();

				GetDoc()->SaveModelFile();// 최종 위치 저장

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_MARK;
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				//GetDoc()->m_nLevelCount = 10;
				GetDoc()->m_nLevelCount = GetDoc()->m_nFFCMark_Min;

				GetDoc()->m_fMaxScore = 0.f;
			} 
			else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
				nSeqNo = SEQ_CONTACT_ERROR ;

				strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
					CAM_1, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1),
					"centerMove NG");
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
				GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM1_PCB_EDGE:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_1_PCB_0 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_PCB_EDGE;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
					nSeqNo = SEQ_CONTACT_ERROR ;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

				}
				else//겹침 판정
				{
					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_1 PCB Overlap.");
					//Y축 후진 코드
					if( m_nYBackCnt < 10 )
					{
						m_nYBackCnt++;
						GetDoc()->m_nYaxis_Back = 1;

						GetAlignCtl()->ContactPosCalibaration_FFC_Edge(CAM_1, TRUE); 

						nSeqNo = SEQ_CONTACT_MOVE_GRABPOS ;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

						strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
							CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
							"Pile NG");
					}
					else	// Y축 10회 후진 시도에도 못찾았을 경우 Alarm 
					{		// [7/18/2009 Hui]
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
						nSeqNo = SEQ_CONTACT_ERROR ;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE ;
						GetDoc()->m_bUsed_Tune = FALSE ;
					}
				}
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM1_FFC_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_1_FPC_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_FFC_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC_Mark[CAM_1] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신
					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

					//GetDoc()->m_nLevelCount = 10;
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBMark_Min;

					GetDoc()->m_fMaxScore = 0.f;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_1 FFC Mark find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_PCB_MARK;
				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_1, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1),
						"FFC MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}

		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM1_PCB_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_1_PCB_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_PCB_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizePCB_Mark[CAM_1] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					strLog_AutoTune.Format("%s, %d, %f, %f, %d, %d, %s, %d msec ", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_1, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1),
						GetDoc()->m_nBinarizeFPC_Mark[CAM_1], GetDoc()->m_nBinarizePCB_Mark[CAM_1],
						GetDoc()->m_strAutoTune_Reason, GetTickCount() - GetDoc()->m_nOldTime_Tune);
					GetDoc()->SetLog(strLog_AutoTune, LOG_AUTOTUNE);

					//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_CALIBRATION;

					if (GetDoc()->m_bAutoTune[CAM_2])
					{
						GetDoc()->m_nCamera_Auto = CAM_2;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
					}
					else if (GetDoc()->m_bAutoTune[CAM_3])
					{
						GetDoc()->m_nCamera_Auto = CAM_3;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
					}
					else 
					{
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
						GetDoc()->m_bAutoTune_RUN = FALSE;
						GetDoc()->m_bAutoTune[CAM_1] = FALSE;
						GetDoc()->m_nYaxis_Back = 99;
						//GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
						nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
					}
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_1, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1),
						"PCB MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}
		}
		break;

	//case SEQ_CONTACT_AUTOTUNE_CAM1_CALIBRATION:
	//	{
	//		GetDoc()->m_fAutoDistance_X =  (GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X) * GetApp()->m_dFovX;

	//		GetAlignCtl()->ContactPosCalibaration_SortX(CAM_1);//이동 위치 환산

	//		nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM1_SORT_X;

	//	}
	//	break;

	//case SEQ_CONTACT_AUTOTUNE_CAM1_SORT_X:
	//	{
	//		GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

	//		if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //동
	//		{
	//			GetDoc()->SetAlignPos(CAM_1);//현재 좌표 저장//Align 위치 저장 

	//			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;
	//			GetDoc()->SaveBinarizeCondition();

	//			GetDoc()->SaveModelFile();// 최종 위치 저장

	//			GetDoc()->m_bAutoTune[CAM_1]=FALSE; 

	//			GetDoc()->m_nLevelCount = 100;//edge는 100이하에서 이상한 곳에서 Maxscore가 나올 가능성이 있어서...
	//			GetDoc()->m_fMaxScore = 0.f;

	//			if (GetDoc()->m_bAutoTune[CAM_2])
	//			{
	//				GetDoc()->m_nCamera_Auto = CAM_2;
	//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;
	//			}
	//			else if (GetDoc()->m_bAutoTune[CAM_3])
	//			{
	//				GetDoc()->m_nCamera_Auto = CAM_3;
	//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
	//			}
	//			else 
	//			{
	//				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	//				GetDoc()->m_bAutoTune_RUN = FALSE;
	//				GetDoc()->m_bAutoTune[CAM_1] = FALSE;
	//				GetDoc()->m_nYaxis_Back = 99;
	//				nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
	//			}
	//		} 
	//		else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
	//		{
	//			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
	//			nSeqNo = SEQ_CONTACT_ERROR ;
	//		}
	//	}
	//	break;
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////2번 카메라

	case SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE://  [6/11/2009 janghan]
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_2_FPC_0 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_EDGE;

				break;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC[CAM_2] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;

					GetAlignCtl()->ContactPosCalibaration_FFC_Edge(GetDoc()->m_nCamera_Auto);//이동 위치 환산

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					m_nYBackCnt = 0;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_2 FFC Edge find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_MOVE_CENTER_CHK;

					break;
				} 
				else
				{	//PCB Edge 확인시작-> 못 찾을경우
					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_2 FFC Edge find fail.");
					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_PCB_EDGE;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

					//GetDoc()->m_nLevelCount = 100;//
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBEdge_Min;
					GetDoc()->m_fMaxScore = 0.f;
					break;
				}
			}
			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM2_MOVE_CENTER_CHK:
		{
			GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

			if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //화면 중앙 이동
			{
				EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_2 Move Center.");
				GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

				GetDoc()->SaveBinarizeCondition();

				GetDoc()->SaveModelFile();// 최종 위치 저장

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_MARK;
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				//GetDoc()->m_nLevelCount = 10;
				GetDoc()->m_nLevelCount = GetDoc()->m_nFFCMark_Min;

				GetDoc()->m_fMaxScore = 0.f;
			} 
			else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
				nSeqNo = SEQ_CONTACT_ERROR ;

				strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
					CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
					"centerMove NG");
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM2_PCB_EDGE:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_2_PCB_0 , FALSE);				

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_PCB_EDGE;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
					nSeqNo = SEQ_CONTACT_ERROR ;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

				}
				else//겹침 판정
				{
					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_2 PCB Overlap.");
					if( m_nYBackCnt < 10)
					{
						m_nYBackCnt++;
						//Y축 후진 코드 //현재 위치에서 조금씩 뒤로 물러나 FFC Edge find를 재 실시 
						GetDoc()->m_nYaxis_Back = 2;//축 넘버 

						GetAlignCtl()->ContactPosCalibaration_FFC_Edge(CAM_2, TRUE); 

						nSeqNo = SEQ_CONTACT_MOVE_GRABPOS ;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

						strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
							CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
							"Pile NG");
					}
					else	// Y축 10회 후진 시도에도 못찾았을 경우 Alarm 
					{		// [7/18/2009 Hui]
						GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
						nSeqNo = SEQ_CONTACT_ERROR ;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE ;
						GetDoc()->m_bUsed_Tune = FALSE ;
					}
				}
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM2_FFC_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_2_FPC_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_FFC_MARK;

			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC_Mark[CAM_2] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

					//GetDoc()->m_nLevelCount = 10;
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBMark_Min;

					GetDoc()->m_fMaxScore = 0.f;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_2 FFC Mark find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_PCB_MARK;
				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
						"FFC MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}

		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM2_PCB_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_2_PCB_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_PCB_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizePCB_Mark[CAM_2] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_nLevelCount = GetDoc()->m_nTuneLevl;

					//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_CALIBRATION;

					GetDoc()->m_bAutoTune[CAM_2]=FALSE; 
					GetDoc()->m_nNewTime_Tune = GetTickCount();

					strLog_AutoTune.Format("%s, %d, %f, %f, %d, %d, %s, %d msec ", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
						GetDoc()->m_nBinarizeFPC_Mark[CAM_2], GetDoc()->m_nBinarizePCB_Mark[CAM_2],
						GetDoc()->m_strAutoTune_Reason, GetDoc()->m_nNewTime_Tune - GetDoc()->m_nOldTime_Tune);
					GetDoc()->SetLog(strLog_AutoTune, LOG_AUTOTUNE);

					if (GetDoc()->m_bAutoTune[CAM_3])
					{
						GetDoc()->m_nCamera_Auto = CAM_3;
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
						GetDoc()->m_nLevelCount = GetDoc()->m_nFFCEdge_Min;
					}
					else 
					{
						GetDoc()->m_bAutoTune_RUN = FALSE;
						GetDoc()->m_bAutoTune[CAM_2] = FALSE;
						GetDoc()->m_nYaxis_Back = 99;
						//GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
						nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;

					}
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;
					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_2, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2),
						"PCB MARK NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}
		}
		break;

		//case SEQ_CONTACT_AUTOTUNE_CAM2_CALIBRATION:
		//	{
		//		GetDoc()->m_fAutoDistance_X =  (GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X) * GetApp()->m_dFovX;

		//		GetAlignCtl()->ContactPosCalibaration_SortX(CAM_2);//이동 위치 환산

		//		nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM2_SORT_X;
		//		//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;

		//	}
		//	break;

		//case SEQ_CONTACT_AUTOTUNE_CAM2_SORT_X:
		//	{
		//		GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

		//		if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //동
		//		{
		//			GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

		//			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;
		//			GetDoc()->SaveBinarizeCondition();

		//			GetDoc()->SaveModelFile();// 최종 위치 저장

		//			GetDoc()->m_bAutoTune[CAM_2]=FALSE; 

		//			if (GetDoc()->m_bAutoTune[CAM_3])
		//			{
		//				GetDoc()->m_nCamera_Auto = CAM_3;
		//				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
		//				//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
		//			}
		//			else 
		//			{
		//				GetDoc()->m_bAutoTune_RUN = FALSE;
		//				GetDoc()->m_bAutoTune[CAM_2] = FALSE;
		//				GetDoc()->m_nYaxis_Back = 99;
		//				nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
		//				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
		//			}
		//		} 
		//		else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
		//		{
		//			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
		//			nSeqNo = SEQ_CONTACT_ERROR ;
		//		}
		//	}
		//	break;
		//////////////////////////////////////////////////////////////////////////

		//////////////////////////////////////////////////////////////////////////3번 카메라
	case SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE://  [6/11/2009 janghan]
		{
			if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT ||
				( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT && GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 ) )
			{
				//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
				if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCEdge_Max)
				{
					//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
					GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

					GetAlignCtl()->SearchPattern2(CAM_3_FPC_0 , FALSE);

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_EDGE;
				} 
				else
				{
					if (GetDoc()->m_fMaxScore > 0.f)
					{
						GetDoc()->m_nBinarizeFPC[CAM_3] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

						GetDoc()->SaveBinarizeCondition();

						GetAlignCtl()->ContactPosCalibaration_FFC_Edge(GetDoc()->m_nCamera_Auto);//이동 위치 환산
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

						EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_3 FFC Edge find complete.");

						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_MOVE_CENTER_CHK;

					} 
					else
					{	//PCB Edge 확인시작-> 못 찾을경우
						EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_3 FFC Edge find fail.");
						nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_PCB_EDGE;
						GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

						//GetDoc()->m_nLevelCount = 100;//
						GetDoc()->m_nLevelCount = GetDoc()->m_nPCBEdge_Min;
						GetDoc()->m_fMaxScore = 0.f;
					}
				}
			}
			else// AutoTune 종료
			{
				nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				GetDoc()->m_bAutoTune_RUN = FALSE;
				GetDoc()->m_bAutoTune[3] =  FALSE;
				GetDoc()->m_nYaxis_Back = 99;
				GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM3_MOVE_CENTER_CHK:
		{
			GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

			if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //화면 중앙 이동
			{
				EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_3 Move Center.");
				GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

				GetDoc()->SaveBinarizeCondition();

				GetDoc()->SaveModelFile();// 최종 위치 저장

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_MARK;
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

				GetDoc()->m_nLevelCount = 10;

				GetDoc()->m_fMaxScore = 0.f;
			} 
			else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
			{
				GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
				nSeqNo = SEQ_CONTACT_ERROR ;

				strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
					CAM_3, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3),
					"centerMove NG");
				GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
				GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM3_PCB_EDGE:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBEdge_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_3_PCB_0 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_PCB_EDGE;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_FOV_OUT);
					nSeqNo = SEQ_CONTACT_ERROR ;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

				}
				else//겹침 판정
				{
					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_3 PCB Overlap.");
					//Y축 후진 코드 //현재 위치에서 조금씩 뒤로 물러나 FFC Edge find를 재 실시
					GetDoc()->m_nYaxis_Back = 3;

					GetAlignCtl()->ContactPosCalibaration_FFC_Edge(CAM_3, TRUE); 

					nSeqNo = SEQ_CONTACT_MOVE_GRABPOS ;
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]

					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_3, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3),
						"Pile NG");
				}
			}
		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM3_FFC_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nFFCMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_3_FPC_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_FFC_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizeFPC_Mark[CAM_3] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

					//GetDoc()->m_nLevelCount = 10;
					GetDoc()->m_nLevelCount = GetDoc()->m_nPCBMark_Min;

					GetDoc()->m_fMaxScore = 0.f;

					EqLog(indexContUnit, nSeqNo, "AutoTune\tCAM_3 FFC Mark find complete.");

					nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_PCB_MARK;
				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;

					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_3, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3),
						"FFC Mark NG");

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}

		}
		break;

	case SEQ_CONTACT_AUTOTUNE_CAM3_PCB_MARK:
		{
			//if (GetDoc()->m_nLevelCount < BINARIZE_MAX_LEVEL)
			if (GetDoc()->m_nLevelCount < GetDoc()->m_nPCBMark_Max)
			{
				//GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + BINARIZE_LEVEL_STEP;
				GetDoc()->m_nLevelCount = GetDoc()->m_nLevelCount + GetDoc()->m_nLevelStep;

				GetAlignCtl()->SearchPattern2(CAM_3_PCB_1 , FALSE);

				nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_PCB_MARK;
			} 
			else
			{
				if (GetDoc()->m_fMaxScore > 0.f)
				{
					GetDoc()->m_nBinarizePCB_Mark[CAM_3] = GetDoc()->m_nTuneLevl;// 2진화 Level 갱신

					GetDoc()->SaveBinarizeCondition();

					strLog_AutoTune.Format("%s, %d, %f, %f, %d, %d, %s, %d msec ", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_3, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3),
						GetDoc()->m_nBinarizeFPC_Mark[CAM_3], GetDoc()->m_nBinarizePCB_Mark[CAM_3],
						GetDoc()->m_strAutoTune_Reason, GetTickCount() - GetDoc()->m_nOldTime_Tune);
					GetDoc()->SetLog(strLog_AutoTune, LOG_AUTOTUNE);

					//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_CALIBRATION;  

					GetDoc()->m_nYaxis_Back = 99;
					GetDoc()->m_bAutoTune_RUN = FALSE;
					GetDoc()->m_bAutoTune[3]=FALSE;

					GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
					//GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]

					nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;//다시 처음으로 일반 서치 시작					

				} 
				else// FFC Mark 회손 판정 - Auto Tune Fault!!!
				{
					GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MARK_LINE_BAD);
					nSeqNo = SEQ_CONTACT_ERROR ;

					strLog_AutoTune.Format("%s, %d, %f, %f, %s", GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, 
						CAM_3, GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3), GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3),
						"PCB MARk NG");
					GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
					GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
				}
			}
		}
		break;

		//case SEQ_CONTACT_AUTOTUNE_CAM3_CALIBRATION:
		//	{
		//		GetDoc()->m_fAutoDistance_X =  (GetDoc()->m_fPCBmarkFind_X - GetDoc()->m_fMarkFind_X) * GetApp()->m_dFovX;

		//		GetAlignCtl()->ContactPosCalibaration_SortX(CAM_3);//이동 위치 환산

		//		nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM3_SORT_X;

		//	}
		//	break;

		//case SEQ_CONTACT_AUTOTUNE_CAM3_SORT_X:
		//	{
		//		GetDoc()->m_nMotor_Move_CHK = GetDeviceCtl()->MoveStage_Auto(GRAB_POS, GetDoc()->m_nCamera_Auto);

		//		if (GetDoc()->m_nMotor_Move_CHK  == MOVE_OK_END) //동
		//		{
		//			GetDoc()->SetAlignPos(GetDoc()->m_nCamera_Auto);//현재 좌표 저장//Align 위치 저장 

		//			//nSeqNo = SEQ_CONTACT_AUTOTUNE_CAM0_FFC_MARK;

		//			GetDoc()->SaveBinarizeCondition();

		//			GetDoc()->SaveModelFile();// 최종 위치 저장

		//			//GetDoc()->m_bAutoTune[CAM_3]=FALSE;

		//			GetDoc()->m_nYaxis_Back = 99;
		//			GetDoc()->m_bAutoTune_RUN = FALSE;
		//			GetDoc()->m_bAutoTune[3]=FALSE;

		//			GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;

		//			nSeqNo = SEQ_CONTACT_SEARCH_CAM0_PCB;//다시 처음으로 일반 서치 시작
		//			
		//		} 
		//		else if( GetDoc()->m_nMotor_Move_CHK < MOVE_ERR_NONE )
		//		{
		//			GetMainFrame()->SetMsgNo(MSG_AUTO_SELECT, ERR_AUTOTUNE_MOVE_CENTER);  //추후 에러코드 추가
		//			nSeqNo = SEQ_CONTACT_ERROR ;
		//		}
		//	}
		//	break;
		///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	case SEQ_CONTACT_CALIBRATION:
		//if(GetDoc()->m_bAutoTune_RUN) GetAlignCtl()->ContactPosCalibaration_FFC_Edge(0);
		GetAlignCtl()->ContactPosCalibaration(0);
		GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;//  [7/4/2009 janghan]
		GetDoc()->m_bUsed_Tune = FALSE;//  [7/6/2009 janghan]
		EqLog(indexContUnit, nSeqNo, "ContactPosCalibration.");

		nSeqNo = SEQ_CONTACT_MOVE_CONTACTPOS;
		break;
	case SEQ_CONTACT_MOVE_CONTACTPOS:
		nRet = GetDeviceCtl()->MoveStage(CONTACT_POS, TRUE);
		if (nRet == MOVE_OK_END )
		{
			EqLog(indexContUnit, nSeqNo, "Comp\tMove Contact Position.");
			nSeqNo = SEQ_CONTACT_UNIT_ON ;

		}
		else if(nRet < MOVE_ERR_NONE)
			nSeqNo = SEQ_CONTACT_ERROR ;
		GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		break;

	case SEQ_CONTACT_UNIT_ON:
		if(GetApp()->m_RunStatus == STATUS_RUN)
			::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcContactOn, NULL);
		GetDeviceCtl()->ContactUnit(DOWN);
		dTimer = GetTickCount() ;
		EqLog(indexContUnit, nSeqNo, "Do\tContact Unit Down.");
		nSeqNo =  SEQ_CONTACT_BLOW_OFF;
		break;

	case SEQ_CONTACT_BLOW_OFF:
		Sleep(GetDoc()->m_dwContactDelay);
		GetDeviceCtl()->BlowUnit(SENSOR_OFF);
		EqLog(indexContUnit, nSeqNo, "Do\tBlow Off.");

		nSeqNo = SEQ_CONTACT_COMPLETE ;

/*
		if ( (GetTickCount() - dTimer) > GetDoc()->m_dwContactDelay )
		{
			GetDeviceCtl()->BlowUnit(SENSOR_OFF);
			EqLog(indexContUnit, nSeqNo, "Do\tBlow Off.");

			nSeqNo = SEQ_CONTACT_COMPLETE ;
		}
		else
		{
			Sleep(GetDoc()->m_dwContactDelay);
			nSeqNo = SEQ_CONTACT_BLOW_OFF;
		}
*/
		break;

	case SEQ_CONTACT_ERROR:
		{
			nSeqNo = SEQ_CONTACT_CHK_REQUEST ;
			// Run중일 경우는 SeqCellCont에서 Show한다.
			if(GetApp()->IsRunStatus() == FALSE)
				::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_NORMAL, ERR_CANCEL);			m_SeqFlag.bCellContactCancel = TRUE;
			m_SeqFlag.bCellContactComp = TRUE ;
			m_SeqFlag.bCellContactReq = FALSE;

			GetDoc()->m_bAutoTune[0] = FALSE;
			GetDoc()->m_bAutoTune[1] = FALSE;
			GetDoc()->m_bAutoTune[2] = FALSE;
			GetDoc()->m_bAutoTune[3] = FALSE;

			GetDoc()->m_nMotor_Move_CHK =  FALSE;  
			GetDoc()->m_nYaxis_Back = 99;     
			GetDoc()->m_nCamera_Auto =  0;
			GetDoc()->m_bAutoTune_RUN = FALSE;
			GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

		}
		break;
	case SEQ_CONTACT_COMPLETE:
		{
			EqLog(indexContUnit, nSeqNo, "Align Complete.");
			nSeqNo = SEQ_CONTACT_CHK_REQUEST ;
			m_SeqFlag.bCellContactCancel = FALSE;
			m_SeqFlag.bCellContactComp = TRUE ;
			m_SeqFlag.bCellContactReq = FALSE;

			GetDoc()->m_bAutoTune[0] = FALSE;
			GetDoc()->m_bAutoTune[1] = FALSE;
			GetDoc()->m_bAutoTune[2] = FALSE;
			GetDoc()->m_bAutoTune[3] = FALSE;

			GetDoc()->m_nMotor_Move_CHK =  FALSE;  
			GetDoc()->m_nYaxis_Back = 99;     
			GetDoc()->m_nCamera_Auto =  0;
			GetDoc()->m_bAutoTune_RUN = FALSE;
			GetDoc()->m_strAutoTune_Reason = "";//  [6/21/2009 janghan]

		}
		break;
	}

	m_nSeqContactNo = nSeqNo ;
}

BOOL CEqMainSequence::OnReqMoveToPos()
{
	if( m_bContactMoveEnable == TRUE || m_SeqFlag.bContactMoving == TRUE )	
		return FALSE;

	m_bContactMoveEnable = TRUE;
	m_SeqFlag.bContactMoveReq = TRUE;

	return TRUE;
}

void CEqMainSequence::GlassDataCopy()
{
	memcpy(m_PanelInfo.m_sHPanelID			, 	m_PanelInfoLoaderBuf.m_sHPanelID		,	sizeHPANELID		);
	memcpy(m_PanelInfo.m_sEPanelID			, 	m_PanelInfoLoaderBuf.m_sEPanelID		,	sizeEPANELID		);
	memcpy(m_PanelInfo.m_sBatchID			, 	m_PanelInfoLoaderBuf.m_sBatchID			,	sizeBATCHID			);
	memcpy(m_PanelInfo.m_sProdType			, 	m_PanelInfoLoaderBuf.m_sProdType		,	sizePRODTYP			);
	memcpy(m_PanelInfo.m_sProdKind			, 	m_PanelInfoLoaderBuf.m_sProdKind		,	sizePRODKND			);
	memcpy(m_PanelInfo.m_sDeviceID			, 	m_PanelInfoLoaderBuf.m_sDeviceID		,	sizeDEVICEID		);
	memcpy(m_PanelInfo.m_sStepID			, 	m_PanelInfoLoaderBuf.m_sStepID			,	sizeSTEPID			);
	memcpy(m_PanelInfo.m_sPPID				, 	m_PanelInfoLoaderBuf.m_sPPID			,	sizePPID			);
	memcpy(m_PanelInfo.m_sThickness			, 	m_PanelInfoLoaderBuf.m_sThickness		,	sizeTHICKNESS		);
	memcpy(m_PanelInfo.m_sInspectionFlag	, 	m_PanelInfoLoaderBuf.m_sInspectionFlag	,	sizeINSPFLG			);
	memcpy(m_PanelInfo.m_sPanelSize			, 	m_PanelInfoLoaderBuf.m_sPanelSize		,	sizePANELSIZE		);
	memcpy(m_PanelInfo.m_sPanelPosition		, 	m_PanelInfoLoaderBuf.m_sPanelPosition	,	sizePANELPOS		);
	memcpy(m_PanelInfo.m_sCount1			, 	m_PanelInfoLoaderBuf.m_sCount1			,	sizeCOUNT1			);
	memcpy(m_PanelInfo.m_sCount2			, 	m_PanelInfoLoaderBuf.m_sCount2			,	sizeCOUNT2			);
	memcpy(m_PanelInfo.m_sGrade				, 	m_PanelInfoLoaderBuf.m_sGrade			,	sizeGRADE			);
	memcpy(m_PanelInfo.m_sComment			, 	m_PanelInfoLoaderBuf.m_sComment			,	sizeCOMMENT			);
	memcpy(m_PanelInfo.m_sCompSize			, 	m_PanelInfoLoaderBuf.m_sCompSize		,	sizeCOMPSIZE		);
	memcpy(m_PanelInfo.m_sReadingFlag		, 	m_PanelInfoLoaderBuf.m_sReadingFlag		,	sizeREADFLG			);
	memcpy(m_PanelInfo.m_sPanelState		, 	m_PanelInfoLoaderBuf.m_sPanelState		,	sizePANELSTATE		);
	memcpy(m_PanelInfo.m_sJudgement			, 	m_PanelInfoLoaderBuf.m_sJudgement		,	sizeJUDGEMENT		);
	memcpy(m_PanelInfo.m_sCode				, 	m_PanelInfoLoaderBuf.m_sCode			,	sizeCODE			);
	memcpy(m_PanelInfo.m_sRunLine			, 	m_PanelInfoLoaderBuf.m_sRunLine			,	sizeRUNLINE			);
	memcpy(m_PanelInfo.m_sUniqueID			, 	m_PanelInfoLoaderBuf.m_sUniqueID		,	sizeUNIQUEID		);
	memcpy(m_PanelInfo.m_sPairHPanelID		, 	m_PanelInfoLoaderBuf.m_sPairHPanelID	,	sizePAIR_HID		);
	memcpy(m_PanelInfo.m_sPairEPanelID		, 	m_PanelInfoLoaderBuf.m_sPairEPanelID	,	sizePAIR_EID		);
	memcpy(m_PanelInfo.m_sPairGrade			, 	m_PanelInfoLoaderBuf.m_sPairGrade		,	sizePAIR_GRADE		);
	memcpy(m_PanelInfo.m_sPairUniqueID		, 	m_PanelInfoLoaderBuf.m_sPairUniqueID	,	sizePAIR_UNIQUEID	);
	memcpy(m_PanelInfo.m_sFlowRecipe		, 	m_PanelInfoLoaderBuf.m_sFlowRecipe		,	sizeFLOWRECIPE		);
	memcpy(m_PanelInfo.m_sReserved			, 	m_PanelInfoLoaderBuf.m_sReserved		,	sizeRESERVED		);
	memcpy(m_PanelInfo.m_sBitsSignals		, 	m_PanelInfoLoaderBuf.m_sBitsSignals		,	sizeBITSIGNAL		);
	memcpy(m_PanelInfo.m_sReferData			, 	m_PanelInfoLoaderBuf.m_sReferData		,	sizeREFERDATA		);

	// garbage value 방지하기 위해 강제로 써줌.
	memcpy(m_PanelInfo.m_sStepID, "7M0205", 5);
	memcpy(m_PanelInfo.m_sUniqueID, "0", 1);
	memcpy(m_PanelInfo.m_sRunLine, "0", 1);

	memcpy(m_PanelInfo.m_sReserved, "NULL", 4); // Default Reserved Data : "NULL"
}

void CEqMainSequence::ProcessInit()
{
	if( m_pMelsec->MelsecNetBitRecv(UNLD_OUT_READY) &&
		m_pMelsec->MelsecNetBitRecv(UNLD_OUT_REQ_SUPPLY) && 
		m_pMelsec->MelsecNetBitRecv(UNLD_OUT_DO_SUPPLY) &&
		m_pMelsec->MelsecNetBitRecv(UNLD_IN_PLC_PANEL_CHECK) )
	{
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_CHECK, CHK_UNLOAD_COMP) ;
		return;
	}
	m_nSeqCellRecvNo = SEQ_LD_CHK_REQUEST;
	m_nSeqCellContactNo = MAIN_CHK_INTERLOCK;
	m_nSeqCellSendNo = SEQ_UNLD_CHK_REQUEST;
	m_nSeqContactNo = SEQ_CONTACT_CHK_REQUEST;

	m_SeqFlag.bCellInReq = FALSE ;
	m_SeqFlag.bCellInComp = TRUE ;

	m_SeqFlag.bCellOutReq = FALSE ;
	m_SeqFlag.bCellOutComp = TRUE ;

	for(int i = MEL_READY; i <= MEL_HAND_UP; i++)
	{
		m_pMelsec->MelsecNetBitOff(LOAD_OUTPUT_START+i);
		m_pMelsec->MelsecNetBitOff(UNLOAD_OUTPUT_START+i);
	}
	// GlassData Scrap
	m_pComm->PanelTransfer2LC(ePanelScrap) ;
	ClearGlassData(m_PanelInfoLoaderBuf);
	ClearGlassData(m_PanelInfo);
	m_pMelsec->GlassDataWrite(m_PanelInfo, DATA_OUTPUT_START);

	// Inspection End
	GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
	GetDoc()->BluOff();
	GetApp()->m_bInspectionStart = FALSE;
	GetApp()->m_bInspectionEnd = TRUE;
	GetApp()->m_bInspectionResult = TRUE;

	if( GetLineType() == REMOVE_TILTING_TYPE )
	{
		GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_INSP );
		GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_MOVE_READY );
	}

	// FormAuto의 Process Monitoring 초기화
	::SendMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_PROCESS, ProcMax, NULL);
}

void CEqMainSequence::StopContactUnit()
{
	m_nSeqContactNo = SEQ_CONTACT_CHK_REQUEST ;
	m_SeqFlag.bCellContactComp = TRUE ;
	m_SeqFlag.bCellContactReq = FALSE;
}

void CEqMainSequence::CheckInterlock()
{
	CPortMap* pPortMap = GetDoc()->GetPortMap();

	//////////////////////////////////////////////////////////////////////////
	// START Switch
	if(DioInput(pPortMap->iStartSwitch))
	{
		GetDoc()->SetLog("Start 스위치 누름",LOG_COMM);
		GetApp()->Start();
	}

	//////////////////////////////////////////////////////////////////////////
	// STOP Switch
	if(DioInput(pPortMap->iStopSwitch))
	{
		GetDoc()->SetLog("Stop 스위치 누름",LOG_COMM);
		GetApp()->Stop();
	}

	//////////////////////////////////////////////////////////////////////////
	// RESET Switch
	if(DioInput(pPortMap->iResetSwitch))
	{
		// I/O나 기타 다른 것들로 해제 된 것을 확인 가능한 것들은 전부 확인 후에 실행
		// 해제 안된것이 있으면 Reset을 할 수 없다
		GetDoc()->SetLog("Reset 스위치 누름",LOG_COMM);
		if(m_FlagInterlock.MainPower && m_FlagInterlock.DoorSensor
			&& m_FlagInterlock.AreaSensor)
			GetApp()->Reset();
	}

	//////////////////////////////////////////////////////////////////////////
	// Main Power
	if(DioInput(pPortMap->iPowerOnCheck) == FALSE)
	{
		if(m_FlagInterlock.MainPower)
		{
			m_FlagInterlock.MainPower = FALSE;
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_MAIN_POWER);
			GetApp()->ChangeStatus(STATUS_ERROR);
			m_pMotion->HomeFlagResetAll();
		}
	}
	else
	{
		if(m_FlagInterlock.MainPower == FALSE)
			m_FlagInterlock.MainPower = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Door sensor
	if(DioInput(pPortMap->iDoorSensorRCover1))
	{
		if(m_FlagInterlock.DoorSensor)
		{
			m_FlagInterlock.DoorSensor = FALSE;
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_DOOR_OPEN);
			GetApp()->ChangeStatus(STATUS_ERROR);
			m_pMotion->HomeFlagResetAll();
		}
	}
	else
	{
		if(m_FlagInterlock.DoorSensor == FALSE)
			m_FlagInterlock.DoorSensor = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// Area sensor	
	// Tilting 구동중일 때만 Check
	// Handler 진입시는 물류쪽에서 Check한다.
	if(GetDeviceCtl()->m_bEnableAreaSensor)
	{
		if(DioInput(pPortMap->iAreaSensor) == FALSE)
		{
			if(m_FlagInterlock.AreaSensor)
			{
				m_FlagInterlock.AreaSensor = FALSE;
				m_pMotion->E_StopAll();
				::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_AREA_SENSOR);
				GetApp()->ChangeStatus(STATUS_ERROR);
			}
		}
		else
		{
			if(m_FlagInterlock.AreaSensor == FALSE)
				m_FlagInterlock.AreaSensor = TRUE;
		}
	}
	else
	{
		if(m_FlagInterlock.AreaSensor == FALSE)
			m_FlagInterlock.AreaSensor = TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	// EMO
	if(DioInput(pPortMap->iEmoSwitch) == FALSE)
	{
		::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_AUTO_SELECT, ERR_EMO_SWITCH_ON);
		GetApp()->ChangeStatus(STATUS_EMO);
		m_pMotion->HomeFlagResetAll();
	}
	else
	{
		if(GetApp()->m_RunStatus == STATUS_EMO)			GetApp()->ChangeStatus(STATUS_STOP);
	}

	//////////////////////////////////////////////////////////////////////////
	// Tilting interlock 신호
	BOOL bSafeTilting = FALSE;
	if(GetLineType() == REMOVE_TILTING_TYPE)
	{
		// REMOVE_TILTING_TYPE이면 Contact Y만 Check한다
		// Contact Unit Y-axis poosition check
		if(
			CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_0].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y0))
			&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_1].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y1))
			&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_2].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y2))
			&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_3].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y3))
			)
			bSafeTilting = TRUE;
	}
	else
	{
		// Home sensor 및 이동명령중 flag check
		if(m_pMotion->CheckHomeSensor(AXIS_TILT_MOVE) && (GetDeviceCtl()->m_bMoveTilting == FALSE))
		{
			// Tilting position check
			if(	(GetDoc()->m_dUnTiltingPos == m_pMotion->GetCmdPos(AXIS_TILT_MOVE))
				&& (CEtc::CheckPosition(GetDoc()->m_dUnTiltingPos, m_pMotion->GetActPos(AXIS_TILT_MOVE)))	)
			{
				// Contact Unit Y-axis poosition check
				if(
					CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_0].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y0))
					&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_1].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y1))
					&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_2].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y2))
					&& CEtc::CheckPosition(m_pAmtData->m_CurModelInfo.ReadyPos[CAM_3].dy, m_pMotion->GetCmdPos(AXIS_CCD_Y3))
					)
					bSafeTilting = TRUE;
			}
		}
	}
	GetDeviceCtl()->SetSafeTilting(bSafeTilting);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// 기타 항시 체크해줘야 하는것들
	// Clamp 신호
	if(GetDeviceCtl()->ChkClampUnit())		m_pMelsec->MelsecNetBitOn(UNLD_OUT_CLAMP_OFF); 
	else									m_pMelsec->MelsecNetBitOff(UNLD_OUT_CLAMP_OFF);

	// Area 신호
	if(DioInput(pPortMap->iAreaSensor))		m_pMelsec->MelsecNetBitOn(MEL_SW_AREA);
	else									m_pMelsec->MelsecNetBitOff(MEL_SW_AREA);

	// Panel check 신호
	if(GetDeviceCtl()->ChkPanelExist())
	{
		if( !m_pMelsec->MelsecNetBitRecv(UNLD_IN_PLC_PANEL_CHECK) )
			m_pMelsec->MelsecNetBitOn(MEL_SW_PANEL_EXIST);
		else
		{
			if( m_SeqFlag.bCellInReq )
				m_pMelsec->MelsecNetBitOn(MEL_SW_PANEL_EXIST);
		}
	}
	else		m_pMelsec->MelsecNetBitOff(MEL_SW_PANEL_EXIST);

	// Non Stop Model change 예약 BIT 확인
	if( GetDoc()->m_bUseAMC  && (GetApp()->m_RunStatus == STATUS_RUN || GetApp()->m_RunStatus == STATUS_READY))
	{
		if(m_pMelsec->MelsecNetBitRecv(NON_STOP_PLC_RESERVE))	
		{
			// PLC측의 예약 명령 Bit가 살면 여기서 PC쪽 예약 Bit를 살린다. [6/10/2009 OSC]
			if(m_bNonStopReady)		m_bNonStopReady = FALSE;
			m_pMelsec->MelsecNetBitOn(NON_STOP_PC_RESERVE);
		}
		else
		{
			// Non Stop 예약 명령 비트가 꺼지면 PC의 예약 Bit와 완료 Bit를 끈다. [6/10/2009 OSC]
			if(m_bNonStopReady == FALSE)
			{
				if( GetMainFrame()->m_bMelsecWrite )
					return;
				GetMainFrame()->m_bMelsecWrite = TRUE;
				m_bNonStopReady = TRUE;
				ResetNonStopFail();
			}
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_FAIL);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_RESERVE);
			m_pMelsec->MelsecNetBitOff(NON_STOP_PC_END);
		}
	}
	//////////////////////////////////////////////////////////////////////////
}

void CEqMainSequence::PGDataToGlassData()
{
	CString strJudgement, strJudgementCopy;
	if(GetApp()->m_RunMode != MODE_VISION_SKIP)
	{
		strJudgement.Format("%s", m_PGInfo.m_sJudgement);
	}
	else
		strJudgement.Format("OK");

	strJudgementCopy = strJudgement.SpanIncluding(_T("0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ-")); 


	memcpy(m_PanelInfo.m_sJudgement,	(unsigned char*)(LPCTSTR)strJudgementCopy,	sizeJUDGEMENT);	m_PanelInfo.m_sJudgement[sizeJUDGEMENT] = NULL;
	memcpy(m_PanelInfo.m_sCode,			m_PGInfo.m_sCode,							sizeCODE);		m_PanelInfo.m_sCode[sizeCODE] = NULL;
	memcpy(m_PanelInfo.m_sSumPath,		m_PGInfo.m_sSumPath,						sizeSUMPATH);	m_PanelInfo.m_sSumPath[sizeSUMPATH] = NULL;
	memcpy(m_PanelInfo.m_sRawPath,		m_PGInfo.m_sRawPath,						sizeRAWPATH);	m_PanelInfo.m_sRawPath[sizeRAWPATH] = NULL;
	memcpy(m_PanelInfo.m_sImagePath,	m_PGInfo.m_sImagePath,						sizeIMGPATH);	m_PanelInfo.m_sImagePath[sizeIMGPATH] = NULL;
	memcpy(m_PanelInfo.m_sDisk,			m_PGInfo.m_sDisk,							sizeDISK);		m_PanelInfo.m_sDisk[sizeDISK] = NULL;
	memcpy(m_PanelInfo.m_sOPID,			m_PGInfo.m_sOPID,							sizeOPID);		m_PanelInfo.m_sDisk[sizeOPID] = NULL;
}

void CEqMainSequence::GlassDataToPGData()
{
	strcpy_s(m_PGInfo.m_sHPanelID, m_PanelInfo.m_sHPanelID);
	strcpy_s(m_PGInfo.m_sDeviceID, m_PanelInfo.m_sDeviceID);
	strcpy_s(m_PGInfo.m_sBatchID,  m_PanelInfo.m_sBatchID);
	strcpy_s(m_PGInfo.m_sStepID,  m_PanelInfo.m_sStepID);
}

void CEqMainSequence::SetQCSampling()
{
	// 10번째 bit(0번부터 count 시작)를 On한다.	16진수 : 0x0400		2진수 : 00000100 00000000
	m_PanelInfo.m_sBitsSignals[1] = m_PanelInfo.m_sBitsSignals[1] | 0x04;
}

void CEqMainSequence::EqLog( int nFunc, int nSeqNo, LPCTSTR szText )
{
	CString strSeq, strLog;
	if(nFunc == indexContUnit)
		strSeq = "SeqContactUnit";
	else if(nFunc == indexSeqRcv)
		strSeq = "SeqCellRecv";
	else if(nFunc == indexSeqCont)
		strSeq = "SeqCellContProc";
	else if(nFunc == indexSeqSend)
		strSeq = "SeqCellSend";
	strLog.Format("[%s:%d]\t%s", strSeq, nSeqNo, szText);
	GetDoc()->SetLog(strLog, LOG_EQ);
}

BOOL CEqMainSequence::CheckLoaderGlassData()
{
	// SESL요청 사항으로 NonStop 진행이 아닐때 Device ID가 없을 경우 Device ID 없이 진행 [8/17/2009 janghan]////////////////////////////////
	BOOL bCheck_DeviceID = TRUE;

	if (GetDoc()->m_bNonStop_Run)
	{
		if(m_PanelInfoLoaderBuf.m_sDeviceID[0] != ' ') bCheck_DeviceID = TRUE;
		else bCheck_DeviceID = FALSE;
	}
	else bCheck_DeviceID = TRUE;
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if(GetApp()->m_RunMode == MODE_STAGE)
		return TRUE;
	// HPanelID나 EPanelID 둘 중 하나가 있고 DeviceID가 있을 때 (HPanelID || EPanelID) && DeviceID
	//if( (m_PanelInfoLoaderBuf.m_sHPanelID[0] != ' ' || m_PanelInfoLoaderBuf.m_sEPanelID[0] != ' ') && m_PanelInfoLoaderBuf.m_sDeviceID[0] != ' ')
	if( (m_PanelInfoLoaderBuf.m_sHPanelID[0] != ' ' || m_PanelInfoLoaderBuf.m_sEPanelID[0] != ' ') && bCheck_DeviceID )//  [8/17/2009 janghan]

		return TRUE;
	return FALSE;
}
BOOL CEqMainSequence::CheckGlassData()
{
	if(GetApp()->m_RunMode == MODE_STAGE)
		return TRUE;

	// HPanelID나 EPanelID 둘 중 하나가 있고 DeviceID가 있을 때 (HPanelID || EPanelID) && DeviceID
	if( (m_PanelInfo.m_sHPanelID[0] != ' ' || m_PanelInfo.m_sEPanelID[0] != ' ') && m_PanelInfo.m_sDeviceID[0] != ' ')
		return TRUE;
	return FALSE;
}

void CEqMainSequence::LastGlassToGlassData()
{
	// 뒤에 공백문자를 유지하기 위해 memcpy 사용
	memcpy(m_PanelInfo.m_sHPanelID ,		GetDoc()->m_LastGlass.strHPanelID.GetBuffer(0),		GetDoc()->m_LastGlass.strHPanelID.GetLength());
	memcpy(m_PanelInfo.m_sEPanelID ,		GetDoc()->m_LastGlass.strEPanelID.GetBuffer(0),		GetDoc()->m_LastGlass.strEPanelID.GetLength());
	memcpy(m_PanelInfo.m_sBatchID ,			GetDoc()->m_LastGlass.strBatchID.GetBuffer(0),		GetDoc()->m_LastGlass.strBatchID.GetLength());
	memcpy(m_PanelInfo.m_sDeviceID ,		GetDoc()->m_LastGlass.strDeviceID.GetBuffer(0),		GetDoc()->m_LastGlass.strDeviceID.GetLength());
	memcpy(m_PanelInfo.m_sInspectionFlag ,	GetDoc()->m_LastGlass.strInspection.GetBuffer(0),	GetDoc()->m_LastGlass.strInspection.GetLength());
	memcpy(m_PanelInfo.m_sPairHPanelID ,	GetDoc()->m_LastGlass.strPairH.GetBuffer(0),		GetDoc()->m_LastGlass.strPairH.GetLength());
	memcpy(m_PanelInfo.m_sPairEPanelID ,	GetDoc()->m_LastGlass.strPairE.GetBuffer(0),		GetDoc()->m_LastGlass.strPairE.GetLength());
	strcpy_s(m_PGInfo.m_sOPName, GetDoc()->m_LastGlass.strOPID.GetBuffer(0));
	strcpy_s(m_PGInfo.m_sOPName, GetDoc()->m_LastGlass.strOPName.GetBuffer(0));
}

void CEqMainSequence::SaveLastGlass()
{
	if(CheckGlassData())
	{
		GetDoc()->m_LastGlass.strHPanelID	= m_PanelInfo.m_sHPanelID;
		GetDoc()->m_LastGlass.strEPanelID	= m_PanelInfo.m_sEPanelID;
		GetDoc()->m_LastGlass.strBatchID	= m_PanelInfo.m_sBatchID;
		GetDoc()->m_LastGlass.strDeviceID	= m_PanelInfo.m_sDeviceID;
		GetDoc()->m_LastGlass.strInspection	= m_PanelInfo.m_sInspectionFlag;
		GetDoc()->m_LastGlass.strPairH		= m_PanelInfo.m_sPairHPanelID;
		GetDoc()->m_LastGlass.strPairE		= m_PanelInfo.m_sPairEPanelID;
	}
	GetDoc()->m_LastGlass.strOPID		= m_PGInfo.m_sOPID;
	GetDoc()->m_LastGlass.strOPName		= m_PGInfo.m_sOPName;

	if(CheckGlassData())
		GetDoc()->SaveLastGlassFile();
}

void CEqMainSequence::ClearGlassData( PANEL_INFO &panelinfo )
{
	// PLC 정보이기 때문에 공백문자로 항상 채운다.
	ZeroMemory(&panelinfo , sizeof(PANEL_INFO) );
	memset(panelinfo.m_sHPanelID		,	' ',	sizeHPANELID				);
	memset(panelinfo.m_sEPanelID		,	' ',	sizeEPANELID				);
	memset(panelinfo.m_sBatchID			,	' ',	sizeBATCHID					);
	memset(panelinfo.m_sProdType		,	' ',	sizePRODTYP					);
	memset(panelinfo.m_sProdKind		,	' ',	sizePRODKND					);
	memset(panelinfo.m_sDeviceID		,	' ',	sizeDEVICEID				);
	memset(panelinfo.m_sStepID			,	' ',	sizeSTEPID					);
	memset(panelinfo.m_sPPID			,	' ',	sizePPID					);
	memset(panelinfo.m_sThickness		,	' ',	sizeTHICKNESS				);
	memset(panelinfo.m_sInspectionFlag	,	' ',	sizeINSPFLG					);
	memset(panelinfo.m_sPanelSize		,	' ',	sizePANELSIZE				);
	memset(panelinfo.m_sPanelPosition	,	' ',	sizePANELPOS				);
	memset(panelinfo.m_sCount1			,	' ',	sizeCOUNT1					);
	memset(panelinfo.m_sCount2			,	' ',	sizeCOUNT2					);
	memset(panelinfo.m_sGrade			,	' ',	sizeGRADE					);
	memset(panelinfo.m_sComment			,	' ',	sizeCOMMENT					);
	memset(panelinfo.m_sCompSize		,	' ',	sizeCOMPSIZE				);
	memset(panelinfo.m_sReadingFlag		,	' ',	sizeREADFLG					);
	memset(panelinfo.m_sPanelState		,	' ',	sizePANELSTATE				);
	memset(panelinfo.m_sJudgement		,	' ',	sizeJUDGEMENT				);
	memset(panelinfo.m_sCode			,	' ',	sizeCODE					);
	memset(panelinfo.m_sRunLine			,	' ',	sizeRUNLINE					);
	memset(panelinfo.m_sUniqueID		,	' ',	sizeUNIQUEID				);
	memset(panelinfo.m_sPairHPanelID	,	' ',	sizePAIR_HID				);
	memset(panelinfo.m_sPairEPanelID	,	' ',	sizePAIR_EID				);
	memset(panelinfo.m_sPairGrade		,	' ',	sizePAIR_GRADE				);
	memset(panelinfo.m_sPairUniqueID	,	' ',	sizePAIR_UNIQUEID			);
	memset(panelinfo.m_sFlowRecipe		,	' ',	sizeFLOWRECIPE				);
// 	memset(panelinfo.m_sReserved		,	' ',	sizeRESERVED				);
	memset(panelinfo.m_sBitsSignals		,	' ',	sizeBITSIGNAL				);
	memset(panelinfo.m_sReferData		,	' ',	sizeREFERDATA				);
	memset(panelinfo.m_sRawPath			,	' ',	sizeRAWPATH					);
	memset(panelinfo.m_sSumPath			,	' ',	sizeSUMPATH					);
	memset(panelinfo.m_sImagePath		,	' ',	sizeIMGPATH					);
	memset(panelinfo.m_sDisk			,	' ',	sizeDISK					);
	memset(panelinfo.m_sOPID			,	' ',	sizeOPID					);
}

BOOL CEqMainSequence::IsPanelUnloadComplecte()
{
	short bitReq = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_REQ_SUPPLY);
	short bitDo = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_DO_SUPPLY);
	short bitEnd = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_END_SUPPLY);

	if(m_SeqFlag.bCellOutReq || (m_SeqFlag.bCellOutComp == FALSE)
		|| bitReq || bitDo || bitEnd
		)
		return FALSE;
	else
		return TRUE;
}
BOOL CEqMainSequence::SearchDeviceName(CString m_NonStopDeviceID)
{
	//요청 사항..
	// Ex)	LTA460HA-L02		[구형 등록 Device Name]
	//		LTA460HA01			[신형 등록 Device Name]
	//	9,10 번째 자리에서 '-'로 구별 한다...

	if( m_NonStopDeviceID.GetLength() < 8 )	return FALSE;
	CString strPath;

	int nSize = GetDoc()->m_AryDevice.GetSize();
	int i;

	for( i = 0 ; i < nSize ; i++ )
	{
		CMTDoc::STRUCT_DEVICE tmpDevice;
		tmpDevice = GetDoc()->m_AryDevice.GetAt( nSize - ( i+ 1) );

		if( tmpDevice.strDeviceName == m_NonStopDeviceID )	
		{
			strPath.Format("%s\\%s\\System.INI" , GetApp()->m_strRecipePath ,tmpDevice.strModelName );
			if(CFileSupport::FileCheck(strPath))
			{
				m_pAmtData->m_CurModelInfo.strModelName = tmpDevice.strModelName ;
				GetDoc()->m_CurDeviceID = tmpDevice.strDeviceName;
				return TRUE;
			}
		}
	}

	return FALSE ;
}

void CEqMainSequence::SetNonStopFail()
{
	m_pMelsec->MelsecNetBitOn(NON_STOP_PC_FAIL);
	m_pMelsec->NonStopFailTypeWrite(NON_STOP_FT_DOWN);
	GetComm()->NonStop2LC(NON_STOP_PC_FAIL, GetDoc()->m_NonStopDeviceID);
}

void CEqMainSequence::ResetNonStopFail()
{
	m_pMelsec->MelsecNetBitOff(NON_STOP_PC_FAIL);
	m_pMelsec->NonStopFailTypeWrite(NON_STOP_FT_NONE);
}

BOOL CEqMainSequence::SetAlignPos()
{
	POSITION_DATA ReadyPos[MAX_CAM_NUM];
	POSITION_DATA GrabPos[MAX_CAM_NUM];

	STRUCT_MODEL_DATA* pModelData = &(GetDoc()->m_pAmtData->m_CurModelInfo);

	// 일단 현재 position을 Grab position에 저장한다.
	GrabPos[CAM_0].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0);
	GrabPos[CAM_0].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y0);
	GrabPos[CAM_1].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1);
	GrabPos[CAM_1].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y1);
	GrabPos[CAM_2].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2);
	GrabPos[CAM_2].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y2);
	GrabPos[CAM_3].dx = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3);
	GrabPos[CAM_3].dy = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_Y3);

	// 사용하는 Cam, 안하는 Cam 선정
	BOOL bUse[MAX_CAM_NUM] = {TRUE, TRUE, TRUE, TRUE};
	if(pModelData->iContactNum == CAmtData::TWO_CONT)
	{
		bUse[CAM_0] = FALSE;	bUse[CAM_3] = FALSE;
	}
	else if(pModelData->iContactNum == CAmtData::ONE_CONT)
	{
		for(int i = CAmtData::CONTACT_0; i <= CAmtData::CONTACT_3; i++)
		{
			if(i != pModelData->iContactor)	bUse[i-1] = FALSE;
		}
	}

	// Grab position을 Ready position에 저장
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		if(bUse[i])
		{
			ReadyPos[i].dx = GrabPos[i].dx;
			// Ready Offset 차감하여 저장
			ReadyPos[i].dy = GrabPos[i].dy - pModelData->TV_StageReady;
		}
		else
		{
			// One Contact인데 Side 선택이면 현재 위치를 그대로 넣는다. [5/8/2009 OSC]
			if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_0
				&& i == CAM_1)
			{
				ReadyPos[i].dx = GrabPos[i].dx;
				ReadyPos[i].dy = GrabPos[i].dy;
			}
			else if(pModelData->iContactNum == CAmtData::ONE_CONT && pModelData->iContactor ==  CAmtData::CONTACT_3
				&& i == CAM_2)
			{
				ReadyPos[i].dx = GrabPos[i].dx;
				ReadyPos[i].dy = GrabPos[i].dy;
			}
			else
			{
				// 안쓰는 위치는 10mm로 고정
				ReadyPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
				ReadyPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
				GrabPos[i].dx 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
				GrabPos[i].dy 	= GetDoc()->m_pAmtData->QUAD_NOTUSEDAXIS_POS;
			}
		}
	}
	// S/W Limit check
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		if(CheckLimit(i, ReadyPos[i].dx) == FALSE)	return 0;
		if(CheckLimit(i+4, ReadyPos[i].dy) == FALSE) return 0;
		if(CheckLimit(i, GrabPos[i].dx) == FALSE)	return 0;
		if(CheckLimit(i+4, GrabPos[i].dy) == FALSE)	return 0;
	}
	// 최종 적용
	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		/*m_dReadyPos[i].dx = ReadyPos[i].dx;
		m_dReadyPos[i].dy = ReadyPos[i].dy;
		m_dGrabPos[i].dx = GrabPos[i].dx;
		m_dGrabPos[i].dy = GrabPos[i].dy;*/

		pModelData->ReadyPos[i].dx = ReadyPos[i].dx;
		pModelData->ReadyPos[i].dy = ReadyPos[i].dy;
		pModelData->GrapPos[i].dx = GrabPos[i].dx;
		pModelData->GrapPos[i].dy = GrabPos[i].dy;
	}

	return 1;
}

BOOL CEqMainSequence::CheckLimit( int nAxis, double dPos )
{
	if(GetDoc()->m_pMotorParam[nAxis].dPosSoftLimit < dPos)
	{
		AfxMessageBox("This value is over the S/W Positive limit");
		return FALSE;
	}
	if(GetDoc()->m_pMotorParam[nAxis].dNegSoftLimit > dPos)
	{
		AfxMessageBox("This value is lower the S/W Negative limit");
		return FALSE;
	}
	return TRUE;
}

void CEqMainSequence::ClearGlassIDData( PANEL_INFO &panelinfo )
{
	memset(panelinfo.m_sHPanelID		,	' '							,	sizeHPANELID				);
	memset(panelinfo.m_sEPanelID		,	' '							,	sizeEPANELID				);
	memset(panelinfo.m_sBatchID			,	' '							,	sizeBATCHID					);
	memset(panelinfo.m_sPairHPanelID	,	' '							,	sizePAIR_HID				);
	memset(panelinfo.m_sPairEPanelID	,	' '							,	sizePAIR_EID				);
	memset(panelinfo.m_sPairGrade		,	' '							,	sizePAIR_GRADE				);
	memset(panelinfo.m_sPairUniqueID	,	' '							,	sizePAIR_UNIQUEID			);
	memset(panelinfo.m_sFlowRecipe		,	' '							,	sizeFLOWRECIPE				);
// 	memset(panelinfo.m_sReserved		,	' '							,	sizeRESERVED				);
	memset(panelinfo.m_sBitsSignals		,	' '							,	sizeBITSIGNAL				);
	memset(panelinfo.m_sReferData		,	' '							,	sizeREFERDATA				);
	memset(panelinfo.m_sRawPath			,	' '							,	sizeRAWPATH					);
	memset(panelinfo.m_sSumPath			,	' '							,	sizeSUMPATH					);
	memset(panelinfo.m_sImagePath		,	' '							,	sizeIMGPATH					);
	memset(panelinfo.m_sDisk			,	' '							,	sizeDISK					);
	memset(panelinfo.m_sOPID			,	' '							,	sizeOPID					);
}