// DlgbarJog.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgbarJog.h"

// #include "MainFrm.h"

// DLL에 존재하는 각종 Dialog 사용
#include "UI\GausGUI\GxNumPad.h"
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"



//GxGUIFrame 사용
#include "GUIDefine.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "..\..\MainFrm.h"
#include "UI\Form\FormMotorUnitTest.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CDlgbarJog 대화 상자입니다.

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
IMPLEMENT_DYNAMIC(CDlgbarJog, CDialogBar)




enum
{
	TIMER_JOG = 0
};


CDlgbarJog::CDlgbarJog()
{
	m_dX = 0.0;
	m_dY = 0.0;

	m_curAxis = AXIS_ID_MAX;
	m_bMove = FALSE;

	m_AData[AXIS_JIG_SHUTTLE_Y1].bHorOrVer = FALSE;
	m_AData[AXIS_JIG_SHUTTLE_Y1].bForward = FALSE;

	m_AData[AXIS_JIG_SHUTTLE_Y2].bHorOrVer = FALSE;
	m_AData[AXIS_JIG_SHUTTLE_Y2].bForward = FALSE;

	m_AData[AXIS_INSPECTION_X1].bHorOrVer = TRUE;
	m_AData[AXIS_INSPECTION_X1].bForward = TRUE;

	m_AData[AXIS_INSPECTION_X2].bHorOrVer = TRUE;
	m_AData[AXIS_INSPECTION_X2].bForward = TRUE;

	m_AData[AXIS_ACTIVE_ALIGN_X1].bHorOrVer = TRUE;
	m_AData[AXIS_ACTIVE_ALIGN_X1].bForward = TRUE;

	m_AData[AXIS_ACTIVE_ALIGN_X2].bHorOrVer = TRUE;
	m_AData[AXIS_ACTIVE_ALIGN_X2].bForward = TRUE;
}

CDlgbarJog::~CDlgbarJog()
{
}

void CDlgbarJog::DoDataExchange(CDataExchange* pDX)
{
	CDialogBar::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgbarJog, CDialogBar)
	// ON_BN_CLICKED(IDC_BTN_BOTTOM_MENU1, OnBtnClickMenu1)
	// ON_UPDATE_COMMAND_UI(IDC_BTN_BOTTOM_MENU1, OnUpdateCmdUI)

	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_VIEW_END,	OnChangeViewEnd)			// 메인 화면이 변경 완료되었다.
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_WM_TIMER()
	ON_MESSAGE(CHANGE_MOTOR_AXIS,	OnChangeAxis)
	
END_MESSAGE_MAP()

BEGIN_EVENTSINK_MAP(CDlgbarJog, CDialogBar)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_SJUMP, DISPID_CLICK, CDlgbarJog::ClickGxbtnBarjogSjump, VTS_NONE)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_SAVE, DISPID_CLICK, CDlgbarJog::ClickBtnSave, VTS_NONE)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_MOVE, DISPID_CLICK, CDlgbarJog::ClickBtnMove, VTS_NONE)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_POS_TCH, DISPID_CLICK, CDlgbarJog::ClickBtnInputPos, VTS_NONE)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_POS, DISPID_CLICK, CDlgbarJog::ClickBtnApplyPos, VTS_NONE)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_UP, DISPID_MOUSEDOWN, CDlgbarJog::MouseDownJogUP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_UP, DISPID_MOUSEUP, CDlgbarJog::MouseUpJogUP, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_DOWN, DISPID_MOUSEDOWN, CDlgbarJog::MouseDownJogDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_DOWN, DISPID_MOUSEUP, CDlgbarJog::MouseUpJogDown, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_RIGHT, DISPID_MOUSEDOWN, CDlgbarJog::MouseDownJogRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_RIGHT, DISPID_MOUSEUP, CDlgbarJog::MouseUpJogRight, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_LEFT, DISPID_MOUSEDOWN, CDlgbarJog::MouseDownJogLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
	ON_EVENT(CDlgbarJog, IDC_GXBTN_BARJOG_LEFT, DISPID_MOUSEUP, CDlgbarJog::MouseUpJogLeft, VTS_I2 VTS_I2 VTS_I4 VTS_I4)
END_EVENTSINK_MAP()


// CDlgbarJog 메시지 처리기입니다.
void CDlgbarJog::OnUpdateCmdUI(CCmdUI* pCmdUI)
{
	pCmdUI->Enable(TRUE);
}

LRESULT CDlgbarJog::OnChangeAxis(WPARAM wParam, LPARAM lParam)
{
	 TEACH_PARAM::ID iDTeach = GetMainHandler()->GetTeachAxis();

	 if ( iDTeach == TEACH_PARAM::PARAM_MAX)
	 	return 0;

	 CGxStaticEx* pStcCurTeaPos = (CGxStaticEx*)GetDlgItem( IDC_GXBTN_BARJOG_POS_TCH2 );
	 if ( pStcCurTeaPos->GetSafeHwnd() == NULL )
	 {
	 	pStcCurTeaPos->SetCaption(_T("ERROR") );
	 	return 0;
	 }

	 TEACH_PARAM tParam = theTeachBank.GetTeachParam(iDTeach);
	 m_curAxis = tParam.idAxis;

	 CGxStaticEx* pStcAxisName = (CGxStaticEx*)GetDlgItem( IDC_GXSTC_BARJOG_AXIS_NAME );
	 pStcAxisName->SetCaption(theAxisBank.m_Axis[m_curAxis].m_strAxisName);
	 if(pStcAxisName->GetBackColor() != GXCOLOR_MOTOR_ON)
		 pStcAxisName->SetBackColor(GXCOLOR_MOTOR_ON);


	 double dTeachPos = tParam.dPos + GetMainHandler()->GetTeachOffset();

	 pStcCurTeaPos->SetCaption(CStringSupport::N2C(dTeachPos, _T("%f")));
	 
	 // Offset이 적용되는 티칭값은 저장을 활성화 하지 않는다 [10/29/2016 OSC]
	 CGxButtonEx* pBtnSave = (CGxButtonEx*)GetDlgItem( IDC_GXBTN_BARJOG_SAVE );
	 if(GetMainHandler()->GetTeachOffset() != 0.)
	 {
		 if(pBtnSave->GetEnable())	 pBtnSave->EnableWindow(FALSE);
	 }
	 else
	 {
		 if(pBtnSave->GetEnable() == FALSE)	 pBtnSave->EnableWindow(TRUE);
	 }


	return 0;
}

// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDlgBar() 호출시
// 2. GxMSG_GUI_CHANGE_SUBMENU,	nSubID
// 3. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/

LRESULT CDlgbarJog::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	// Todo : 화면 데이터 표시 초기화를 해주고 갱신용 타이머를 동작/중지 시켜준다.
	BOOL bFlag = (BOOL)wParam;
	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		SetTimer(TIMER_JOG, 100, NULL );
	}
	else
	{
		KillTimer(TIMER_JOG);
	}

	return 0;
}

// 메인 메뉴 선택 화면 전환 완료
LRESULT CDlgbarJog::OnChangeViewEnd(WPARAM wParam, LPARAM lParam)
{
	// ToDo : 화면을 표시하기전에 하위 메뉴에 대한 동작을 기술
	UINT nIdx = (UINT)wParam;


	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CDlgbarJog::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	//CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_BTNX_BOTTOM_MENU1);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --


void CDlgbarJog::OnTimer(UINT_PTR nIDEvent)
{
	switch(nIDEvent)
	{
		case TIMER_JOG:
		{
			KillTimer(TIMER_JOG);
			UpdateUI();
			SetTimer(TIMER_JOG, 100, NULL);
			if(GetMainHandler()->GetCurEngeneerUserOperatorData().nLevel < 3)
			{
				CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_SAVE, FALSE);		
			}
			else
			{
				CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_SAVE, TRUE);
			}
			break;
		}
	}

	CDialogBar::OnTimer(nIDEvent);
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------


void CDlgbarJog::ClickBtnInputPos()
{
	CGxNumPad	dlgNumPad;
	dlgNumPad.SetTitle(_T("Input Teaching Position Value"));
	
	if ( dlgNumPad.DoModal() == IDOK )
	{
		double dbValue;
		dlgNumPad.GetValue( &dbValue );

		CGxStaticEx* pStcTeachVal = (CGxStaticEx*) GetDlgItem(IDC_GXBTN_BARJOG_POS_TCH2);
		
		pStcTeachVal->SetCaption(CStringSupport::N2C(dbValue, _T("%f")));
	}
} 

void CDlgbarJog::ClickBtnApplyPos()
{
	CGxStaticEx* pStcTeachVal = (CGxStaticEx*) GetDlgItem(IDC_GXBTN_BARJOG_POS_TCH2);
	CGxStaticEx* pStcPos = (CGxStaticEx*) GetDlgItem(IDC_GXBTN_BARJOG_POS2);
	
	if ( pStcTeachVal->GetSafeHwnd() == NULL || pStcPos->GetSafeHwnd() == NULL )
	{
		pStcTeachVal->SetCaption(_T("ERROR") );		
		pStcPos->SetCaption(_T("ERROR"));
		return;
	}

	pStcTeachVal->SetCaption(pStcPos->GetCaption());

}

void CDlgbarJog::MouseDownJogUP(short Button, short Shift, long x, long y)			//		UP 클릭
{
	MovePositionByJog(UP);
}


void CDlgbarJog::MouseUpJogUP(short Button, short Shift, long x, long y)			//		UP 클릭
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	m_bMove = FALSE;
}


void CDlgbarJog::MouseDownJogDown(short Button, short Shift, long x, long y)		//	DOWN 클릭
{
	MovePositionByJog(DOWN);
}


void CDlgbarJog::MouseUpJogDown(short Button, short Shift, long x, long y)			//	DOWN 클릭
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	m_bMove = FALSE;
}


void CDlgbarJog::MouseDownJogRight(short Button, short Shift, long x, long y)		//	RIGHT 클릭
{
	MovePositionByJog(RIGHT);
}


void CDlgbarJog::MouseUpJogRight(short Button, short Shift, long x, long y)			//	RIGHT 클릭
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	m_bMove = FALSE;
}


void CDlgbarJog::MouseDownJogLeft(short Button, short Shift, long x, long y)		//	LEFT 클릭
{
	MovePositionByJog(LEFT);
}


void CDlgbarJog::MouseUpJogLeft(short Button, short Shift, long x, long y)			//	LEFT 클릭
{
	theDeviceMotion.AxisStop(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	m_bMove = FALSE;
}


void CDlgbarJog::ClickBtnSave()
{
	CGxStaticEx* pStcPos = (CGxStaticEx*) GetDlgItem(IDC_GXBTN_BARJOG_POS_TCH2);
	
	double dGetPos = _tstof(pStcPos->GetCaption().GetBuffer());
	
	if ( GetMainHandler()->GetTeachAxis() == TEACH_PARAM::PARAM_MAX )	return;

	TEACH_PARAM tParam = theTeachBank.GetTeachParam((TEACH_PARAM::ID)GetMainHandler()->GetTeachAxis());
	tParam.dPos = dGetPos;
		
	/////////////////////////////////////////////////////////
	CArray <TEACH_PARAM, TEACH_PARAM> arrTeach;

	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++ )
	{
		arrTeach.Add(theTeachBank.GetTeachParam((TEACH_PARAM::ID) i));
	}
	
	/////////////////////////////////////////////////////////////
	CGxMsgBox	dlgMsgBox;

	// Origin이 안되면 티칭값 저장 불가 [11/1/2016 OSC]
	AxisStatus status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	if(status.bOriginComplete == FALSE)
	{
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("ORIGIN을 먼저 실행해주세요."), 
			_T("Please run ORIGIN first"),		
			_T("Xin vui lòng chạy ORIGIN đầu tiên"), GetMainHandler()->m_nLangIdx );

		int nRet = dlgMsgBox.DoModal();
		return;
	}

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("TEACH값을 저장하시겠습니까?"), 
		_T("Do you want to save the TEACH value?"),		
		_T("Bạn có muốn lưu các giá trị TEACH?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;

	if ( arrTeach.GetSize() != 0 )
	{
		theTeachBank.SetTeachParam(tParam);
		theTeachBank.SaveTeachFile();
		// RecipeBank에서 보고함 [9/26/2017 OSC]
// 		// 티칭값을 RMS에 보고하므로 티칭값이 변경될 때마다 Recipe 변경 보고를 해야 한다 [12/1/2016 OSC]
// 		theSocketInterFace.m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, theRecipeBank.m_strRecipeName);

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Notify"), _T("Notify"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("저장 완료."), 
			_T("Save completed."),		
			_T("Lưu đã hoàn thành.") , GetMainHandler()->m_nLangIdx);
		dlgMsgBox.DoModal();
	}
}


void CDlgbarJog::ClickBtnMove()
{
 	AxisMoveParam param;
 	param.dAcc = theAxisBank.m_Axis[m_curAxis].m_dDefaultAccel;
	param.dDec = theAxisBank.m_Axis[m_curAxis].m_dDefaultDecel;

	if ( m_curAxis == AXIS_ID_MAX )
			return;

	if(theUnitFunc.DoorClose_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.LightCurtain_Check() == FALSE)
	{
		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.AutoTeachKey_AutoCheck() == FALSE)
	{
		if(theUnitFunc.DoorLockOn_Check() == FALSE)
		{
			CGxMsgBox	dlgMsgBox;
			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
			dlgMsgBox.SetMessage(FALSE, 
				theProcBank.m_strLastKorMsg, 
				theProcBank.m_strLastEngMsg,		
				theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

			dlgMsgBox.DoModal();
			return;
		}
	}

	if(theUnitFunc.GetInPutIOCheck(X_MC_ON_CHECK) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("MC Power가 Off되어 있습니다."), 
			_T("Please Turn On MC Power."),		
			_T("Vui lòng bật MC Power ON.") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	STO_STATE sto = theUnitFunc.STO_Check();
	if(sto != STO_READY)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();

		// STO_WARNING 상태면 진행은 가능하니 메세지만 표시하고 넘어간다 [9/19/2017 OSC]
		if(sto == STO_ALARM)
			return;
	}


	// Origin이 안되면 TeachMove 불가 [11/1/2016 OSC]
	AxisStatus status = theDeviceMotion.GetAxisStatus(MP2100_THREAD_MAIN_PROCESS, m_curAxis);
	if(status.bOriginComplete == FALSE)
	{

		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("ORIGIN을 먼저 실행해주세요."), 
			_T("Please run ORIGIN first"),		
			_T("Xin vui lòng chạy ORIGIN đầu tiên") , GetMainHandler()->m_nLangIdx);

		dlgMsgBox.DoModal();
		return;
	}

	if(theUnitFunc.CheckTeachMoveInterlock(GetMainHandler()->GetTeachAxis(), GetMainHandler()->GetTeachOffset()) == FALSE)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
		dlgMsgBox.SetMessage(FALSE, 
			theProcBank.m_strLastKorMsg, 
			theProcBank.m_strLastEngMsg,		
			theProcBank.m_strLastVnmMsg, GetMainHandler()->m_nLangIdx );

		dlgMsgBox.DoModal();
		return;
	}

	param.dSpeed = theAxisBank.m_Axis[m_curAxis].m_dJogSpeedMiddle;
	theDeviceMotion.TeachMove(MP2100_THREAD_MAIN_PROCESS, GetMainHandler()->GetTeachAxis(), GetMainHandler()->GetTeachOffset());

	// 라이트커튼 감지되면 정지하도록 설정 [9/15/2017 OSC]
	theUnitFunc.LightCurtainMuteLamp_OnOff(JIG_ID_MAX, OFF);
	GetMainHandler()->m_checkAxisLightCurtain = m_curAxis;
	GetMainHandler()->m_bEnableLightCurtain = TRUE;
}
/// -------------------------------------------------------------------
void CDlgbarJog::ClickGxbtnBarjogSjump()
{	
	CMainFrame* pMain = (CMainFrame*)AfxGetMainWnd();
	int iSel = pMain->GetSelectedAxis();
	int iSel2 = pMain->GetTeachAxis();

	if (iSel == AXIS_ID_MAX )		return;		//	 선택된 버튼이 Axis가 아닌경우 리턴

	::PostMessage( ((CFrameWnd *)AfxGetMainWnd())->m_hWnd, GxMSG_GUI_CHANGE_VIEW, (WPARAM)eFORM_Setup4, FALSE);

	CGxGUIFrame* pFrame = pMain->m_pGUIFrame;	
	CFormMotorUnitTest* pMView = (CFormMotorUnitTest*)pFrame->GetView(eFORM_Setup4);		//	MotorUnitCtrl View 가져옴

	if ( pMView->GetSafeHwnd() == NULL )		return;		//	View를 가져오지 못했으면 return

	pMView->SelectedAxis(iSel,iSel2);

}
//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

void CDlgbarJog::MovePositionByJog(DIREC Direc)	//	iWH가 True : X축이동, False : Z축 이동
{
	AXIS_ID IDAxis = (AXIS_ID)GetMainHandler()->GetSelectedAxis();
	if(IDAxis == AXIS_ID_MAX )
		return;
	if(m_bMove)
		return;

	BOOL bHorOrVer	= m_AData[IDAxis].bHorOrVer;			//	bHorOrVer가 TRUE면 X축, FALSE면 Z축
	BOOL bForward	= m_AData[IDAxis].bForward;
	if( (Direc == UP) || (Direc == LEFT) )
		bForward = bForward ? FALSE:TRUE;

	if ( (Direc == UP || Direc == DOWN) && bHorOrVer == TRUE)
	{
		return;
	}
	else if ((Direc == LEFT || Direc == RIGHT) && bHorOrVer == FALSE)
	{
		return;
	}

	AxisMoveParam param;
	param.dAcc = theAxisBank.m_Axis[IDAxis].m_dDefaultAccel;
	param.dDec = theAxisBank.m_Axis[IDAxis].m_dDefaultDecel;

	param.dSpeed = theAxisBank.m_Axis[IDAxis].m_dJogSpeedMiddle;

	param.idAxis = IDAxis;	
	theDeviceMotion.AxisMoveJog(MP2100_THREAD_MAIN_PROCESS, IDAxis, bForward, param);

	m_bMove = TRUE;
}


//--------------------------------------------------------------------------------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------------------------------------------------

void CDlgbarJog::UpdateUI()
{	
	AXIS_ID IDAxis = (AXIS_ID)GetMainHandler()->GetSelectedAxis();
	
	if ( IDAxis == AXIS_ID_MAX )
		return;
	
	CGxStaticEx* pSTtcCurPos = (CGxStaticEx*)GetDlgItem( IDC_GXBTN_BARJOG_POS2 );	
	if ( pSTtcCurPos->GetSafeHwnd() == NULL  )
	{
		pSTtcCurPos->SetCaption(_T("ERROR") );		
		return;
	}
	
	double dValue = 0;
	theDeviceMotion.GetPosition(MP2100_THREAD_MAIN_PROCESS, IDAxis, dValue);		///	현재위치 표시
	pSTtcCurPos->SetCaption(CStringSupport::N2C(dValue, _T("%f")));

//--------------------------------------------------------------------------------------------------------



	// 현재 모터가 횡 이동 이면 위,아래 버튼 Disable   |   축 이동이면 좌,우 버튼 Disable

	BOOL bHorOrVer	= m_AData[IDAxis].bHorOrVer;			//	bHorOrVer가 TRUE면 X축, FALSE면 Z축

	if ( bHorOrVer == TRUE )
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_UP, FALSE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_DOWN, FALSE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_LEFT, TRUE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_RIGHT, TRUE);
	}
	else 
	{
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_UP, TRUE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_DOWN, TRUE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_LEFT, FALSE);
		CGxUICtrl::SetButtonEnabled(this, IDC_GXBTN_BARJOG_RIGHT, FALSE);
	}
}

