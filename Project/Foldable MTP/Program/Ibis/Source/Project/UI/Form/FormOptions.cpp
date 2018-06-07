// FormOptions.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormOptions.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxNumPad.h"
// CFormOptions

IMPLEMENT_DYNCREATE(CFormOptions, CFormView)

CFormOptions::CFormOptions()
	: CFormView(CFormOptions::IDD)
{
	m_first_load=FALSE;
}

CFormOptions::~CFormOptions()
{
}

void CFormOptions::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CFormOptions, CFormView)
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormOptions, CFormView)
	
	//20170306 kjpark D존 작업자 부저 시간 설정
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_SAVE, DISPID_CLICK, CFormOptions::ClickGxbtnOptionSave, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_DRYRUN_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionDryrunOnoff, VTS_NONE)
//	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_MCRREAD_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionMcrreadOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOAD, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLoad, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_FDC_ALARM_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionFdcAlarmOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_LOOFTEST_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionLooftestOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_TMDMATCH_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionTmdmatchOnoff, VTS_NONE)
//	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_JOBPROCESS_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionJobprocessOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_CELLINFO_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionCellinfoOnoff, VTS_NONE)
	//kjpark 20180113 Tmd Version 추가
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_HOST_VERSION, DISPID_CLICK, CFormOptions::ClickGxbtnOptionHostVersionOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_AARETRY_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionAaretryOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_ABRETRY_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionAbretryOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_BYPASS_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionBypassOnoff, VTS_NONE)
	ON_EVENT(CFormOptions, IDC_GXBTN_OPTION_NG_OPTION_ONOFF, DISPID_CLICK, CFormOptions::ClickGxbtnOptionNgOptionOnoff, VTS_NONE)
END_EVENTSINK_MAP()


// CFormOptions 진단입니다.

#ifdef _DEBUG
void CFormOptions::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormOptions::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormOptions 메시지 처리기입니다.
LRESULT CFormOptions::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;

	// 화면 갱신 타이머를 수행시키라는 지령인가 ?
	if ( bFlag )
	{
		LoadOption();
		m_first_load=TRUE;
		if(theProcBank.MachineIsRunState())
		{
			EnableWindow(FALSE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OPTION_EDIT_MODE, _T("MONITOR MODE"));
		}
		else
		{
			EnableWindow(TRUE);
			CGxUICtrl::SetStaticString(this, IDC_GXSTC_OPTION_EDIT_MODE, _T("EDIT MODE"));
		}
	}
	else
	{
// 		if(SaveOptionCompare() && m_first_load)
// 			ClickGxbtnOptionSave();
	}

	return 0;
}

void CFormOptions::UpdateOnoffValue_to_UI(BOOL bValue, int nBtnID)
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	if(pOnOff)
	{
		if(bValue)
		{
			pOnOff->SetCaption(_T("ON"));
			pOnOff->SetBackColor(GXCOLOR_ON);
		}
		else
		{
			pOnOff->SetCaption(_T("OFF"));
			pOnOff->SetBackColor(GXCOLOR_OFF);
		}
	}
}

void CFormOptions::UpdateIntValue_to_UI(int nValue, int nBtnID, CString strEndChar /*= _T("")*/)
{
	CGxButtonEx* pCtrl = (CGxButtonEx*)GetDlgItem(nBtnID);
	CString strCaption;
	strCaption.Format(_T("%d%s"), nValue, strEndChar);
	pCtrl->SetCaption(strCaption);
}

void CFormOptions::UpdateDoubleValue_to_UI(double dValue, int nBtnID, CString strEndChar /*= _T("")*/)
{
	CGxButtonEx* pCtrl = (CGxButtonEx*)GetDlgItem(nBtnID);
	CString strCaption;
	strCaption.Format(_T("%.1f%s"), dValue, strEndChar);
	pCtrl->SetCaption(strCaption);
}

void CFormOptions::GetUI_to_OnOffValue( BOOL &bValue, int nBtnID, CString strName /*= _T("")*/ )
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	BOOL bUIValue;
	if(pOnOff)
	{
		pOnOff->GetCaption()==(_T("ON")) ? (bUIValue = TRUE):(bUIValue = FALSE);
		if(bValue != bUIValue)
		{
			if(strName.GetLength() > 0)
			theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), _T("CONFIG"), strName, bValue, bUIValue);
			bValue = bUIValue;
		}
	}
}

void CFormOptions::GetUI_to_IntValue(int &nValue, int nBtnID, CString strName /*= _T("")*/ )
{
	CGxButtonEx* pCtl = (CGxButtonEx*)GetDlgItem(nBtnID);
	int nUIValue;
	if(pCtl)
	{
		CString strCaption = pCtl->GetCaption();        
		// 		strCaption.SpanIncluding(_T("0123456789"));
		nUIValue = _tstoi(strCaption.GetString());
		if(nValue != nUIValue)
		{
			if(strName.GetLength() > 0)
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), _T("CONFIG"), strName, nValue, nUIValue);
			nValue = nUIValue;
		}
	}
}

void CFormOptions::GetUI_to_DoubleValue(double &dValue, int nBtnID, CString strName)
{
	CGxButtonEx* pCtl = (CGxButtonEx*)GetDlgItem(nBtnID);
	double dUIValue;
	if(pCtl)
	{
		CString strCaption = pCtl->GetCaption();
// 		strCaption.SpanIncluding(_T("0123456789."));
		dUIValue = _tstof(strCaption.GetString());
		if(dValue != dUIValue)
		{
			if(strName.GetLength() > 0)
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %f -> %f"), _T("CONFIG"), strName, dValue, dUIValue);
			dValue = dUIValue;
		}
	}
}

void CFormOptions::ChangeOnOffValue( int nBtnID )
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	if(pOnOff)
	{
		if(pOnOff->GetCaption() ==_T("OFF"))
		{
			pOnOff->SetCaption(_T("ON"));
			pOnOff->SetBackColor(GXCOLOR_ON);
		}
		else
		{
			pOnOff->SetCaption(_T("OFF"));
			pOnOff->SetBackColor(GXCOLOR_OFF);
		}
	}
}

void CFormOptions::ChangeIntValue(int nBtnID, int nMin, int nMax, CString strEndChar /*= _T("")*/)
{
	CGxNumPad	dlgNumPad;
	dlgNumPad.SetTitle(_T("Input Value"));
	dlgNumPad.SetIntegerMode(TRUE);

	if ( dlgNumPad.DoModal() == IDOK )
	{
		int nValue;
		dlgNumPad.GetValue( &nValue );
		if(nValue < nMin) return;
		if(nValue > nMax) return;

		CGxButtonEx* pStcTeachVal = (CGxButtonEx*) GetDlgItem(nBtnID);

		CString strCaption;
		strCaption.Format(_T("%d%s"), nValue, strEndChar);
		pStcTeachVal->SetCaption(strCaption);
	}
}

void CFormOptions::ChangeDoubleValue(int nBtnID, double dMin, int dMax, CString strEndChar /*= _T("")*/)
{
	CGxNumPad	dlgNumPad;
	dlgNumPad.SetTitle(_T("Input Value"));
	dlgNumPad.SetIntegerMode(FALSE);

	if ( dlgNumPad.DoModal() == IDOK )
	{
		double dValue;
		dlgNumPad.GetValue( &dValue );
		if(dValue < dMin) return;
		if(dValue > dMax) return;

		CGxButtonEx* pStcTeachVal = (CGxButtonEx*) GetDlgItem(nBtnID);

		CString strCaption;
		strCaption.Format(_T("%.1f%s"), dValue, strEndChar);
		pStcTeachVal->SetCaption(strCaption);
	}
}

void CFormOptions::CopyReloadParam()
{
	m_configSystem = theConfigBank.m_System;
	m_configOption = theConfigBank.m_Option;
}

void CFormOptions::WriteLogReloadParam()
{
	WriteLogReloadParam_System();
	WriteLogReloadParam_Option();
}

void CFormOptions::WriteLogReloadParam_System()
{
	if(m_configSystem.m_strLineNo != theConfigBank.m_System.m_strLineNo)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strLineNo"), m_configSystem.m_strLineNo, theConfigBank.m_System.m_strLineNo);
	}

	if(m_configSystem.m_strDebugLogPath != theConfigBank.m_System.m_strDebugLogPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strDebugLogPath"), m_configSystem.m_strDebugLogPath, theConfigBank.m_System.m_strDebugLogPath);
	}

	if(m_configSystem.m_strRecipePath != theConfigBank.m_System.m_strRecipePath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strRecipePath"), m_configSystem.m_strRecipePath, theConfigBank.m_System.m_strRecipePath);
	}

	if(m_configSystem.m_strRecipeBackupPath != theConfigBank.m_System.m_strRecipeBackupPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strRecipeBackupPath"), m_configSystem.m_strRecipeBackupPath, theConfigBank.m_System.m_strRecipeBackupPath);
	}

	if(m_configSystem.m_strConfigPath != theConfigBank.m_System.m_strConfigPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strConfigPath"), m_configSystem.m_strConfigPath, theConfigBank.m_System.m_strConfigPath);
	}

	if(m_configSystem.m_strConfigBackupPath != theConfigBank.m_System.m_strConfigBackupPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strConfigBackupPath"), m_configSystem.m_strConfigBackupPath, theConfigBank.m_System.m_strConfigBackupPath);
	}

	if(m_configSystem.m_strCellLogPath != theConfigBank.m_System.m_strCellLogPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strCellLogPath"), m_configSystem.m_strCellLogPath, theConfigBank.m_System.m_strCellLogPath);
	}

	if(m_configSystem.m_strLosstimeLogPath != theConfigBank.m_System.m_strLosstimeLogPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strLosstimeLogPath"), m_configSystem.m_strLosstimeLogPath, theConfigBank.m_System.m_strLosstimeLogPath);
	}

	if(m_configSystem.m_strMCRImageSavePath != theConfigBank.m_System.m_strMCRImageSavePath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strMCRImageSavePath"), m_configSystem.m_strMCRImageSavePath, theConfigBank.m_System.m_strMCRImageSavePath);
	}

	if(m_configSystem.m_strFDCPath != theConfigBank.m_System.m_strFDCPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strFDCPath"), m_configSystem.m_strFDCPath, theConfigBank.m_System.m_strFDCPath);
	}

	if(m_configSystem.m_strProgramDataPath != theConfigBank.m_System.m_strProgramDataPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strProgramDataPath"), m_configSystem.m_strProgramDataPath, theConfigBank.m_System.m_strProgramDataPath);
	}

	if(m_configSystem.m_strMachineDataPath != theConfigBank.m_System.m_strMachineDataPath)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %s -> %s"), 
			_T("CONFIG"), _T("m_strMachineDataPath"), m_configSystem.m_strMachineDataPath, theConfigBank.m_System.m_strMachineDataPath);
	}

}

void CFormOptions::WriteLogReloadParam_Option()
{

	if(m_configOption.m_OffsetHour != theConfigBank.m_Option.m_OffsetHour)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_OffsetHour"), m_configOption.m_OffsetHour, theConfigBank.m_Option.m_OffsetHour);
	}
	if(m_configOption.m_bFDCAlarm != theConfigBank.m_Option.m_bFDCAlarm)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bFDCAlarm"), m_configOption.m_bFDCAlarm, theConfigBank.m_Option.m_bFDCAlarm);
	}
	if(m_configOption. m_dFDCCheckTime != theConfigBank.m_Option. m_dFDCCheckTime)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %f -> %f"), 
			_T("CONFIG"), _T(" m_dFDCCheckTime"), m_configOption. m_dFDCCheckTime, theConfigBank.m_Option. m_dFDCCheckTime);
	}
	if(m_configOption.m_bUseCellInforRequest != theConfigBank.m_Option.m_bUseCellInforRequest)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseCellInforRequest"), m_configOption.m_bUseCellInforRequest, theConfigBank.m_Option.m_bUseCellInforRequest);
	}
	//kjpark 20180113 Tmd Version 추가
	if(m_configOption.m_bUseHostVersionSend != theConfigBank.m_Option.m_bUseHostVersionSend)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseHostVersionSend"), m_configOption.m_bUseHostVersionSend, theConfigBank.m_Option.m_bUseHostVersionSend);		
	}
	if(m_configOption.m_bUseTMDNameMatch != theConfigBank.m_Option.m_bUseTMDNameMatch)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseTMDNameMatch"), m_configOption.m_bUseTMDNameMatch, theConfigBank.m_Option.m_bUseTMDNameMatch);
	}
	if(m_configOption.m_bUseRetryAA != theConfigBank.m_Option.m_bUseRetryAA)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseRetryAA"), m_configOption.m_bUseRetryAA, theConfigBank.m_Option.m_bUseRetryAA);
	}
	if(m_configOption.m_bUseRetryAB != theConfigBank.m_Option.m_bUseRetryAB)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseRetryAB"), m_configOption.m_bUseRetryAB, theConfigBank.m_Option.m_bUseRetryAB);
	}
	if(m_configOption.m_bUseNgoption != theConfigBank.m_Option.m_bUseNgoption)
	{
		theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), 
			_T("CONFIG"), _T("m_bUseNgoption"), m_configOption.m_bUseNgoption, theConfigBank.m_Option.m_bUseNgoption);
	}
}

void CFormOptions::LoadOption()
{
	UpdateOnoffValue_to_UI(theProcBank.m_bDryRunMode, IDC_GXBTN_OPTION_DRYRUN_ONOFF);
 	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bFDCAlarm, IDC_GXBTN_OPTION_FDC_ALARM_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseLoofTest, IDC_GXBTN_OPTION_LOOFTEST_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseByPass, IDC_GXBTN_OPTION_BYPASS_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseTMDNameMatch	, IDC_GXBTN_OPTION_TMDMATCH_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseCellInforRequest	, IDC_GXBTN_OPTION_CELLINFO_ONOFF);
	//kjpark 20180113 Tmd Version 추가
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseHostVersionSend , IDC_GXBTN_OPTION_HOST_VERSION);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseRetryAA	, IDC_GXBTN_OPTION_AARETRY_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseRetryAB	, IDC_GXBTN_OPTION_ABRETRY_ONOFF);
	UpdateOnoffValue_to_UI(theConfigBank.m_Option.m_bUseNgoption	, IDC_GXBTN_OPTION_NG_OPTION_ONOFF);
}

void CFormOptions::ClickGxbtnOptionSave()
{
//	CMainFrame *pMainFrm = (CMainFrame *)AfxGetMainWnd();
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("수정하신 작업을 적용하겠습니까?"), 
		_T("Would you like to apply your modifications?"),		
		_T("Bạn có muốn áp dụng các thay đổi?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("CONFIG"), _T("OPTION_SAVE"));

	/* MACHINE */
	GetUI_to_OnOffValue(theProcBank.m_bDryRunMode, IDC_GXBTN_OPTION_DRYRUN_ONOFF, _T("DRY RUN"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bFDCAlarm, IDC_GXBTN_OPTION_FDC_ALARM_ONOFF, _T("FDC ALARM"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseLoofTest, IDC_GXBTN_OPTION_LOOFTEST_ONOFF, _T("LOOF TEST"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseByPass, IDC_GXBTN_OPTION_BYPASS_ONOFF, _T("BY PASS"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseTMDNameMatch, IDC_GXBTN_OPTION_TMDMATCH_ONOFF, _T("MCR READ"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseCellInforRequest, IDC_GXBTN_OPTION_CELLINFO_ONOFF, _T("FDC ALARM"));
	//kjpark 20180113 Tmd Version 추가
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseHostVersionSend , IDC_GXBTN_OPTION_HOST_VERSION, _T("HOST VERSION"));
	/* RETRY */
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseRetryAA, IDC_GXBTN_OPTION_AARETRY_ONOFF, _T("AA RETRY"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseRetryAB, IDC_GXBTN_OPTION_ABRETRY_ONOFF, _T("AB RETRY"));
	GetUI_to_OnOffValue(theConfigBank.m_Option.m_bUseNgoption, IDC_GXBTN_OPTION_NG_OPTION_ONOFF, _T("NG OPTION"));

	theConfigBank.SaveOptionFile();
	theConfigBank.SaveNGAlarmLimitFile();
	theConfigBank.SaveTimeOutFile();
//	pMainFrm->m_SetValue_Flag=FALSE;
	LoadOption();
}



//////////////////// MACHINE ////////////////////
void CFormOptions::ClickGxbtnOptionDryrunOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_DRYRUN_ONOFF);
}

void CFormOptions::ClickGxbtnOptionFdcAlarmOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_FDC_ALARM_ONOFF);
}

void CFormOptions::ClickGxbtnOptionLooftestOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_LOOFTEST_ONOFF);
}

void CFormOptions::ClickGxbtnOptionBypassOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_BYPASS_ONOFF);
}

void CFormOptions::ClickGxbtnOptionTmdmatchOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_TMDMATCH_ONOFF);
}

void CFormOptions::ClickGxbtnOptionCellinfoOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_CELLINFO_ONOFF);
}

//kjpark 20180113 Tmd Version 추가
void CFormOptions::ClickGxbtnOptionHostVersionOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_HOST_VERSION);
}

//////////////////// RETRY ////////////////////
void CFormOptions::ClickGxbtnOptionAaretryOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_AARETRY_ONOFF);
}

void CFormOptions::ClickGxbtnOptionAbretryOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_ABRETRY_ONOFF);
}

void CFormOptions::ClickGxbtnOptionNgOptionOnoff()
{
	ChangeOnOffValue(IDC_GXBTN_OPTION_NG_OPTION_ONOFF);
}

void CFormOptions::ClickGxbtnOptionLoad()
{
	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("Option File들을 다시 Reload하겠습니까?"), 
		_T("Would you wont reload option files?"),		
		_T("Bạn muốn reload lại Option files?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("CONFIG"), _T("OPTION_RELOAD"));

	CopyReloadParam();

	theConfigBank.ReadSystemFile();	// 얘가 최우선으로 Read되어야 한다
	theConfigBank.ReadOptionFile();			//O
	theConfigBank.ReadNGAlarmLimitFile();
	theProcBank.ReadAlarmFile();
	theProcBank.ReadPDTAlarmFile();
	theAxisBank.ReadAxisParam();
	theConfigBank.ReadNGCodeFile();
	theConfigBank.ReadDefectPatternFile();	//O
	theConfigBank.ReadDefectRetestFile();	//O
	theConfigBank.ReadBZoneDefectCodeFile();
	theConfigBank.ReadTimeOutFile();
	WriteLogReloadParam();

	LoadOption();
}