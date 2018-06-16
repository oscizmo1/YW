// FormManual.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmManual.h"
#include "dlgMelSecMonitor.h"
#include "MainFrm.h"
#include "DlgNumPad.h"

using namespace Gdiplus;	// Color class
enum
{
	NoActive = 0,
	Active
};


// CFormManual

IMPLEMENT_DYNCREATE(CFormManual, CFormView)

CFormManual::CFormManual()
	: CFormView(CFormManual::IDD)
{
	m_LableColor[Active] =	Color(Color::Lime).ToCOLORREF();
	m_LableColor[NoActive] =	Color(Color::Green).ToCOLORREF();
	m_iMoveAction = actNONE;
	m_bPosMoving = FALSE;

}

CFormManual::~CFormManual()
{
}

void CFormManual::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_PLC_READY, m_lblPlcReady);
	DDX_Control(pDX, IDC_LABEL_PLC_REQ, m_lblPlcReq);
	DDX_Control(pDX, IDC_LABEL_PLC_DO, m_lblPlcDo);
	DDX_Control(pDX, IDC_LABEL_PLC_END, m_lblPlcEnd);
	DDX_Control(pDX, IDC_LABEL_PLC_PM, m_lblPlcPm);
	DDX_Control(pDX, IDC_LABEL_PLC_ALARM, m_lblPlcAlarm);
	DDX_Control(pDX, IDC_LABEL_PLC_EMO, m_lblPlcEmo);
	DDX_Control(pDX, IDC_LABEL_PC_READY, m_lblPcReady);
	DDX_Control(pDX, IDC_LABEL_PC_REQ, m_lblPcReq);
	DDX_Control(pDX, IDC_LABEL_PC_DO, m_lblPcDo);
	DDX_Control(pDX, IDC_LABEL_PC_END, m_lblPcEnd);
	DDX_Control(pDX, IDC_LABEL_PC_PM, m_lblPcPm);
	DDX_Control(pDX, IDC_LABEL_PC_ALARM, m_lblPcAlarm);
	DDX_Control(pDX, IDC_LABEL_PC_EMO, m_lblPcEmo);
	DDX_Control(pDX, IDC_LABEL_PLC_READY2, m_lblPlcUnloadReady);
	DDX_Control(pDX, IDC_LABEL_PLC_REQ2, m_lblPlcUnloadReq);
	DDX_Control(pDX, IDC_LABEL_PLC_DO2, m_lblPlcUnloadDo);
	DDX_Control(pDX, IDC_LABEL_PLC_END2, m_lblPlcUnloadEnd);
	DDX_Control(pDX, IDC_LABEL_PLC_SORT, m_lblPlcUnloadSort);
	DDX_Control(pDX, IDC_LABEL_PC_READY2, m_lblPcUnloadReady);
	DDX_Control(pDX, IDC_LABEL_PC_REQ2, m_lblPcUnloadReq);
	DDX_Control(pDX, IDC_LABEL_PC_DO2, m_lblPcUnloadDo);
	DDX_Control(pDX, IDC_LABEL_PC_END2, m_lblPcUnloadEnd);
	DDX_Control(pDX, IDC_LABEL_PC_SORT, m_lblPcUnloadSort);
	DDX_Control(pDX, IDC_LABEL_PLC_LOCK, m_lblPlcLock);
	DDX_Control(pDX, IDC_LABEL_PC_LOCK, m_lblPcLock);
	DDX_Control(pDX, IDC_LABEL_PC_RUN, m_lblPcRun);
	DDX_Control(pDX, IDC_LABEL_PLC_PICK_LO, m_lblPlcPickLo);
	DDX_Control(pDX, IDC_ISWITCHLEDX_MANUAL_TOP_LIGHT, m_iswxManualTopLight);
	DDX_Control(pDX, IDC_ISWITCHLEDX_MANUAL_BTM_LIGHT, m_iswxManualBtmLight);
	DDX_Control(pDX, IDC_ISWITCHLEDX_MANUAL_BLU, m_iswxManualBLU);
	DDX_Control(pDX, IDC_LABEL_CLEAN_POPUP, m_lblCleanset);
	DDX_Control(pDX, IDC_ISWITCHLEDX_USE_MC, m_iswxUseAMC);
	DDX_Control(pDX, IDC_ISWITCHLEDX_USE_MC2, m_iswxUseCreate);
	DDX_Control(pDX, IDC_LABEL_CONTACT_DELAY, m_lblContactDelay);
	DDX_Control(pDX, IDC_EQSTATUS, m_lbEqstatus);
	DDX_Control(pDX, IDC_PROSTATUS, m_lbProstatus);
}

BEGIN_MESSAGE_MAP(CFormManual, CFormView)
	ON_BN_CLICKED(IDC_BTN_MANU_CMP_ON, &CFormManual::OnBnClickedBtnManuCmpOn)
	ON_BN_CLICKED(IDC_BTN_MENU_CMP_OFF, &CFormManual::OnBnClickedBtnMenuCmpOff)
	ON_BN_CLICKED(IDC_BTN_MOVE_GRAB_POS, &CFormManual::OnBnClickedBtnMoveAlignPos)
	ON_BN_CLICKED(IDC_BTN_MENU_FULL_CONT, &CFormManual::OnBnClickedBtnMenuFullCont)
	ON_BN_CLICKED(IDC_BTN_MENU_BLOW_ON, &CFormManual::OnBnClickedBtnMenuBlowOn)
	ON_BN_CLICKED(IDC_BTN_MENU_BLOW_OFF, &CFormManual::OnBnClickedBtnMenuBlowOff)
	ON_BN_CLICKED(IDC_BTN_MENU_CONT_ON, &CFormManual::OnBnClickedBtnMenuContOn)
	ON_BN_CLICKED(IDC_BTN_MENU_CONT_OFF, &CFormManual::OnBnClickedBtnMenuContOff)
	ON_BN_CLICKED(IDC_BTN_MENU_MOVE_READY_POS, &CFormManual::OnBnClickedBtnMenuMoveReadyPos)
	ON_BN_CLICKED(IDC_BTN_MENU_RELEASE_ALL, &CFormManual::OnBnClickedBtnMenuReleaseAll)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_LOADER_COMM_CLR, &CFormManual::OnBnClickedBtnLoaderCommClr)
	ON_BN_CLICKED(IDC_BTN_TILTING_POS, &CFormManual::OnBnClickedBtnTiltingPos)
	ON_BN_CLICKED(IDC_BTN_UNTILTING_POS, &CFormManual::OnBnClickedBtnUntiltingPos)
	ON_BN_CLICKED(IDC_BTN_UNLOADER_COMM_CLR, &CFormManual::OnBnClickedBtnUnloaderCommClr)
	ON_BN_CLICKED(IDC_BTN_INTERLOCK_PASS, &CFormManual::OnBnClickedBtnInterlockPass)
	ON_BN_CLICKED(IDC_BTN_MELSEC_IOCHECK, &CFormManual::OnBnClickedBtnMelsecIocheck)
	ON_BN_CLICKED(IDC_RADIO_GRAB_END, &CFormManual::OnBnClickedRadioGrabEnd)
	ON_BN_CLICKED(IDC_RADIO_INSP_END, &CFormManual::OnBnClickedRadioInspEnd)
	ON_BN_CLICKED(IDC_BTN_CLEAN_ALPPY, &CFormManual::OnBnClickedBtnCleanAlppy)
	ON_BN_CLICKED(IDC_BTN_INIT_VI_COMM, &CFormManual::OnBnClickedBtnInitViComm)
END_MESSAGE_MAP()


// CFormManual diagnostics

#ifdef _DEBUG
void CFormManual::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormManual::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormManual message handlers

BOOL CFormManual::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormManual::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	m_lblCleanset.put_Caption("0");

	m_pMelsec = GetMelsec();

	CString strText;
	strText.Format("%d", GetDoc()->m_dwContactDelay);
	m_lblContactDelay.put_Caption(strText);
}
BEGIN_EVENTSINK_MAP(CFormManual, CFormView)
	ON_EVENT(CFormManual, IDC_LABEL_INSP_CNT, DISPID_CLICK, CFormManual::OnSetRetryCnt, VTS_NONE)
	ON_EVENT(CFormManual, IDC_LABEL_INSP_TIMEOUT, DISPID_CLICK, CFormManual::OnSetInspTimeOut, VTS_NONE)
	ON_EVENT(CFormManual, IDC_ISWITCHLEDX_MANUAL_TOP_LIGHT, 2, CFormManual::OnChangeUserIswitchledxManualTopLight, VTS_NONE)
	ON_EVENT(CFormManual, IDC_ISWITCHLEDX_MANUAL_BTM_LIGHT, 2, CFormManual::OnChangeUserIswitchledxManualBtmLight, VTS_NONE)
	ON_EVENT(CFormManual, IDC_ISWITCHLEDX_MANUAL_BLU, 2, CFormManual::OnChangeUserIswitchledxManualBlu, VTS_NONE)
	ON_EVENT(CFormManual, IDC_LABEL_CLEAN_POPUP, DISPID_CLICK, CFormManual::ClickLabelCleanPopup, VTS_NONE)
	ON_EVENT(CFormManual, IDC_ISWITCHLEDX_USE_MC, 2, CFormManual::OnChangeUserIswitchledxUseMc, VTS_NONE)
	ON_EVENT(CFormManual, IDC_ISWITCHLEDX_USE_MC2, 2, CFormManual::OnChangeUserIswitchledxUseMc2, VTS_NONE)
	ON_EVENT(CFormManual, IDC_LABEL_CONTACT_DELAY, DISPID_CLICK, CFormManual::ClickLabelContactDelay, VTS_NONE)
END_EVENTSINK_MAP()

void CFormManual::OnSetRetryCnt()
{
	// TODO: Add your message handler code here
}

void CFormManual::OnSetInspTimeOut()
{
	// TODO: Add your message handler code here
}

void CFormManual::OnBnClickedBtnManuCmpOn()
{
	CLAMP_UNIT(SENSOR_ON);
}

void CFormManual::OnBnClickedBtnMenuCmpOff()
{
	CLAMP_UNIT(SENSOR_OFF);
}

void CFormManual::OnBnClickedBtnMoveAlignPos()
{
	if(m_iMoveAction == actNONE)
		m_iMoveAction = actGRAB_MOVE;
}

void CFormManual::OnBnClickedBtnMenuFullCont()
{
	if( GetDoc()->m_pEqMainSeq->m_SeqFlag.bCellContactComp)
	{
		GetDoc()->m_nYaxis_Back = 99;//  [8/10/2009 janghan]
		GetDeviceCtl()->ClampUnit(TRUE);
		Sleep(100);
		GetDoc()->m_pEqMainSeq->m_SeqFlag.bCellContactReq = TRUE;
	}
}

void CFormManual::OnBnClickedBtnMenuBlowOn()
{
	BLOW_UNIT(SENSOR_ON);
}

void CFormManual::OnBnClickedBtnMenuBlowOff()
{
	BLOW_UNIT(SENSOR_OFF);
}

void CFormManual::OnBnClickedBtnMenuContOn()
{
	CONTACT_UNIT(DOWN);
}

void CFormManual::OnBnClickedBtnMenuContOff()
{
	// Inspection End
	GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
	GetDoc()->BluOff();

	Sleep(500);
	CONTACT_UNIT(UP);
}

void CFormManual::OnBnClickedBtnMenuMoveReadyPos()
{
	if(m_iMoveAction == actNONE)
		m_iMoveAction = actREADY_MOVE;
}

void CFormManual::OnBnClickedBtnMenuReleaseAll()
{
	if(m_iMoveAction == actNONE)
	{
		// Inspection End
		GetComm()->PGSend(CMD_ERR_MSG, "PG OFF");
		GetDoc()->BluOff();

		m_iMoveAction = actREADY_MOVE;
	}
}

void CFormManual::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == 0)
	{
		short nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_READY	);
		if(m_lblPlcReady.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcReady.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_REQ_SUPPLY	);
		if(m_lblPlcReq.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcReq.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_DO_SUPPLY	);
		if(m_lblPlcDo.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPlcDo.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_END_SUPPLY	);
		if(m_lblPlcEnd.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcEnd.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_PM_MODE		);	
		if(m_lblPlcPm.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPlcPm.put_BackColor(		m_LableColor[nColorIndex]);
// 		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_ALARM		);
// 		if(m_lblPlcAlarm.get_BackColor() != m_LableColor[nColorIndex])
// 			m_lblPlcAlarm.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_EMO_MODE	);
		if(m_lblPlcEmo.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcEmo.put_BackColor(		m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_READY		);
		if(m_lblPcReady.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPcReady.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_REQ_SUPPLY	);
		if(m_lblPcReq.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPcReq.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_DO_SUPPLY	);	
		if(m_lblPcDo.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPcDo.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_END_SUPPLY	);
		if(m_lblPcEnd.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPcEnd.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_PM_MODE	);
		if(m_lblPcPm.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPcPm.put_BackColor(	m_LableColor[nColorIndex]);
// 		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_ALARM		);
// 		if(m_lblPcAlarm.get_BackColor() != m_LableColor[nColorIndex])	
// 			m_lblPcAlarm.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_OUT_EMO_MODE	);
		if(m_lblPcEmo.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPcEmo.put_BackColor(	m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_IN_READY			);
		if(m_lblPlcUnloadReady.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcUnloadReady.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_IN_REQ_SUPPLY	);
		if(m_lblPlcUnloadReq.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcUnloadReq.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_IN_DO_SUPPLY		);
		if(m_lblPlcUnloadDo.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcUnloadDo.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_IN_END_SUPPLY	);
		if(m_lblPlcUnloadEnd.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPlcUnloadEnd.put_BackColor(	m_LableColor[nColorIndex]);
		//nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_IN_SORT		);
		//if(m_lblPlcUnloadSort.get_BackColor() != m_LableColor[nColorIndex])
			//m_lblPlcUnloadSort.put_BackColor(	m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_READY		);
		if(m_lblPcUnloadReady.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPcUnloadReady.put_BackColor(	m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_REQ_SUPPLY	);
		if(m_lblPcUnloadReq.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPcUnloadReq.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_DO_SUPPLY	);	
		if(m_lblPcUnloadDo.get_BackColor() != m_LableColor[nColorIndex])	
			m_lblPcUnloadDo.put_BackColor(		m_LableColor[nColorIndex]);
		nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_END_SUPPLY	);	
		if(m_lblPcUnloadEnd.get_BackColor() != m_LableColor[nColorIndex])		
			m_lblPcUnloadEnd.put_BackColor(		m_LableColor[nColorIndex]);
		//nColorIndex = m_pMelsec->MelsecNetBitRecv(UNLD_OUT_SORT		);	
		//if(m_lblPcUnloadSort.get_BackColor() != m_LableColor[nColorIndex])
			//m_lblPcUnloadSort.put_BackColor(	m_LableColor[nColorIndex]);

// 		nColorIndex = m_pMelsec->MelsecNetBitRecv(MEL_SW_LOADER_TILT	);
// 		if(((CLabel*)GetDlgItem(IDC_LABEL_PLC_LOCK))->get_BackColor() != m_LableColor[nColorIndex])
// 			((CLabel*)GetDlgItem(IDC_LABEL_PLC_LOCK))->put_BackColor(m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(MEL_SW_LOADER_TILT	);
		if(m_lblPlcLock.get_BackColor() != m_LableColor[nColorIndex])
			m_lblPlcLock.put_BackColor(m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(MEL_SW_TILT			);
		if(m_lblPcLock.get_BackColor() != m_LableColor[nColorIndex])
			m_lblPcLock.put_BackColor(m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(MEL_SW_RUN_STATE		);
		if(m_lblPcRun.get_BackColor() != m_LableColor[nColorIndex])
			m_lblPcRun.put_BackColor(m_LableColor[nColorIndex]);

		nColorIndex = m_pMelsec->MelsecNetBitRecv(LD_IN_HAND_UP			);
		if(m_lblPlcPickLo.get_BackColor() != m_LableColor[nColorIndex])
			m_lblPlcPickLo.put_BackColor(m_LableColor[nColorIndex]);
		if (GetComm()->m_iCurEQState == eEqNothing)
		{
			m_lbEqstatus.put_Caption("NOTHING");
		}
		else if (GetComm()->m_iCurEQState == eNormal)
		{
			m_lbEqstatus.put_Caption("NORMAL");
		}
		else if (GetComm()->m_iCurEQState == eFault)
		{
			m_lbEqstatus.put_Caption("FAULT");
		}
		else if (GetComm()->m_iCurEQState == ePM)
		{			
			m_lbEqstatus.put_Caption("FAULT");
		}
		if (GetComm()->m_iCurProcessState == eInit)
		{
			m_lbProstatus.put_Caption("INIT");
		}
		else if (GetComm()->m_iCurProcessState == eExecute)
		{
			m_lbProstatus.put_Caption("EXECUTE");
		}
		else if (GetComm()->m_iCurProcessState == ePause)
		{
			m_lbProstatus.put_Caption("PAUSE");
		}
		else if (GetComm()->m_iCurProcessState == eIdle)
		{
			m_lbProstatus.put_Caption("IDLE");
		}

	}
	else if(nIDEvent == TEACH_MOVE)
	{
		if( m_iMoveAction != actNONE )	
			GetDeviceCtl()->ManualSeqContactUnitMove(m_iMoveAction, m_bPosMoving);
	}
	else if(nIDEvent == TIMER_TILT_WAIT)
	{
		int nRet = GetDeviceCtl()->CheckMoveTilting(GetDeviceCtl()->m_bTilt);
		if(nRet == MOVE_OK_END)	KillTimer(TIMER_TILT_WAIT);
		else if(nRet < MOVE_ERR_NONE)
		{
			::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
			GetApp()->ChangeStatus(STATUS_ERROR);
			KillTimer(TIMER_TILT_WAIT);
		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CFormManual::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	if(bShow)
	{
		SetTimer(0, 100, NULL);
		SetTimer(TEACH_MOVE , 100, NULL);

		m_iswxManualTopLight.put_Active(GetDoc()->m_bLightOn[id_OMIT]);
		m_iswxManualBtmLight.put_Active(GetDoc()->m_bLightOn[id_BtmLight]);
		m_iswxManualBLU.put_Active(GetDoc()->m_bLightOn[id_BLU]);
		m_iswxUseAMC.put_Active(GetDoc()->m_bUseAMC);
		m_iswxUseCreate.put_Active(GetDoc()->m_bUseAutoCreate);

		if(GetApp()->IsRunStatus())	this->EnableWindow(FALSE);
		else						this->EnableWindow(TRUE);
	}
	else
	{
		KillTimer(0);
		KillTimer(TEACH_MOVE);
	}
}

void CFormManual::OnBnClickedBtnLoaderCommClr()
{
	CWaitCursor wc;
	GetMelsec()->MelsecNetBitOn(MEL_SW_LOADER_CLEAR);
	for(int i = MEL_READY; i <= MEL_HAND_UP; i++)
		GetMelsec()->MelsecNetBitOff(LOAD_OUTPUT_START+i);
	GetDeviceCtl()->m_dToggleTimer = GetTickCount();
	GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
	wc.Restore();
}

void CFormManual::OnBnClickedBtnTiltingPos()
{
	if(GetDeviceCtl()->m_bMoveTilting == FALSE)
	{
		if( (GetLineType() == REMOVE_TILTING_TYPE) )
			if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
				GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_PRE_SIGN );
		if(!GetDeviceCtl()->ChkArmStretch())
		{
			if(GetDeviceCtl()->StartMoveTilting(TRUE) == MOVE_OK_START)
			{
				GetDeviceCtl()->m_dTimer = GetTickCount() ;
				GetMainFrame()->SetTimer(TIMER_TILT_WAIT, 500, NULL);
			}
			else
			{
				if( (GetLineType() == REMOVE_TILTING_TYPE) )
					if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
						GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
				//	::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
				//	GetApp()->ChangeStatus(STATUS_ERROR);
			}
		}
		else
			GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );

	}
}

void CFormManual::OnBnClickedBtnUntiltingPos()
{
	if(GetDeviceCtl()->m_bMoveTilting == FALSE)
	{
		if( (GetLineType() == REMOVE_TILTING_TYPE) )
			if( !GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
			GetMelsec()->MelsecNetBitOn( MEL_SW2_TILT_PRE_SIGN );
		if(!GetDeviceCtl()->ChkArmStretch())
		{
			if(GetDeviceCtl()->StartMoveTilting(FALSE) == MOVE_OK_START)
			{
				GetDeviceCtl()->m_dTimer = GetTickCount() ;
				GetMainFrame()->SetTimer(TIMER_TILT_WAIT, 500, NULL);
			}
			else
			{
				if( (GetLineType() == REMOVE_TILTING_TYPE) )
					if( GetMelsec()->MelsecNetBitRecv( MEL_SW2_TILT_PRE_SIGN )  )
						GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
				//	::PostMessage(GetMainFrame()->m_hWnd, WM_SHOW_MSG_BOX, MSG_ERR_MANUAL, ERR_TILTING_MOTOR_MOVING);
				//	GetApp()->ChangeStatus(STATUS_ERROR);
			}
		}
		else
			GetMelsec()->MelsecNetBitOff( MEL_SW2_TILT_PRE_SIGN );
	}
}

void CFormManual::OnBnClickedBtnUnloaderCommClr()
{
	CWaitCursor wc;
	GetMelsec()->MelsecNetBitOn(MEL_SW_UNLOADER_CLEAR);
	for(int i = MEL_READY; i <= MEL_HAND_UP; i++)
		GetMelsec()->MelsecNetBitOff(UNLOAD_OUTPUT_START+i);
	GetDeviceCtl()->m_dToggleTimer = GetTickCount();
	GetMainFrame()->SetTimer(TIMER_BIT_TOGGLE, 1000, NULL);
	wc.Restore();
}

void CFormManual::OnBnClickedBtnInterlockPass()
{
	GetDeviceCtl()->m_bIgnoreInterlock = GetDeviceCtl()->m_bIgnoreInterlock ? FALSE:TRUE;

	if( GetLineType() == REMOVE_TILTING_TYPE )
	{
		if( GetDeviceCtl()->m_bIgnoreInterlock )
		{
			GetMelsec()->MelsecNetBitOn(MEL_SW2_TILT_INTERLOCK);
		}
		else
		{
			GetMelsec()->MelsecNetBitOff(MEL_SW2_TILT_INTERLOCK);
		}
	}
}

void CFormManual::OnBnClickedBtnMelsecIocheck()
{
	CdlgMelSecMonitor dlg;
	dlg.DoModal();
}

void CFormManual::OnBnClickedRadioGrabEnd()
{
	// TODO: Add your control notification handler code here
	GetApp()->m_bInspectionEnd = TRUE;
}

void CFormManual::OnBnClickedRadioInspEnd()
{
	GetApp()->m_bInspectionResult = TRUE;
}

void CFormManual::OnChangeUserIswitchledxManualTopLight()
{
	// TODO: Add your message handler code here
	if(m_iswxManualTopLight.get_Active())
	{
		GetDoc()->OmitOn();
	}
	else
	{
		GetDoc()->OmitOff();
	}
}

void CFormManual::OnChangeUserIswitchledxManualBtmLight()
{
	// TODO: Add your message handler code here
	if(m_iswxManualBtmLight.get_Active())
	{
		GetDoc()->BtmOn();
	}
	else
	{
		GetDoc()->BtmOff();
	}
}

void CFormManual::OnChangeUserIswitchledxManualBlu()
{
	// TODO: Add your message handler code here
	if(m_iswxManualBLU.get_Active())
	{
		GetDoc()->BluOn();
		GetDoc()->BluOn();
	}
	else
	{
		GetDoc()->BluOff();
		GetDoc()->BluOff();
	}
}

void CFormManual::ClickLabelCleanPopup()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s %s", dlg.m_strEditText, "Cell");
		if( dlg.m_strEditText !="" )	m_lblCleanset.put_Caption(strText);
		else if(dlg.m_strEditText == "")	m_lblCleanset.put_Caption("0 Cell");

		m_strCleanning.Format("%s", dlg.m_strEditText);
	}
}

void CFormManual::OnBnClickedBtnCleanAlppy()
{
	// TODO: Add your control notification handler code here
}

void CFormManual::OnChangeUserIswitchledxUseMc()
{
	if(m_iswxUseAMC.get_Active())
	{
		GetDoc()->m_bUseAMC = TRUE;
	}
	else
	{
		GetDoc()->m_bUseAMC = FALSE;
	}
	GetDoc()->SaveAutoModelChangeOption();
}

void CFormManual::OnChangeUserIswitchledxUseMc2()
{
	// TODO: Add your message handler code here
	if(m_iswxUseCreate.get_Active())
	{
		GetDoc()->m_bUseAutoCreate = TRUE;
	}
	else
	{
		GetDoc()->m_bUseAutoCreate = FALSE;
	}
	GetDoc()->SaveAutoModelChangeOption();
}

void CFormManual::OnBnClickedBtnInitViComm()
{
	// TODO: Add your control notification handler code here
//	m_pMelsec->GlassDataWrite(GetDoc()->m_pEqMainSeq->m_PanelInfo,	DATA_OUTPUT_START);
}

void CFormManual::ClickLabelContactDelay()
{
	CDlgNumPad dlg;

	if( dlg.DoModal() == IDOK )
	{
		CString strText;

		strText.Format("%s%s", dlg.m_strEditText, "%");
		if( strText == "" )
			return;

		DWORD dwDelay = atoi(strText);
		if( dwDelay < 0 || dwDelay > 10000 )
			return;

		GetDoc()->m_dwContactDelay = dwDelay;
		GetDoc()->SaveContiactDelay();

		strText.Format("%d", GetDoc()->m_dwContactDelay);
		m_lblContactDelay.put_Caption(strText);
	}
}
