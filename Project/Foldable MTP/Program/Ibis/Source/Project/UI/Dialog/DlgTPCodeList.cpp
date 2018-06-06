// DlgTPCodeList.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgTPCodeList.h"
#include "afxdialogex.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
//GxGUIFrame 사용
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CDlgTPCodeList 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTPCodeList, CDialog)

CDlgTPCodeList::CDlgTPCodeList(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTPCodeList::IDD, pParent)
{
	m_bCanNotCancel = FALSE;
}

CDlgTPCodeList::~CDlgTPCodeList()
{
}

void CDlgTPCodeList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgTPCodeList, CDialog)
END_MESSAGE_MAP()


// CDlgTPCodeList 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgTPCodeList, CDialog)
	ON_EVENT(CDlgTPCodeList, IDC_BTNX_LANG, DISPID_CLICK, CDlgTPCodeList::ClickBtnxLang, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_1, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode1, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_2, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode2, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_CONFIRM, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeConfirm, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_3, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode3, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_4, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode4, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_5, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode5, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_6, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode6, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_7, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode7, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_8, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode8, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_9, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode9, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_10, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode10, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_TPCODE_11, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeTpcode11, VTS_NONE)
	ON_EVENT(CDlgTPCodeList, IDC_GXBTN_LOSSTIME_CANCEL, DISPID_CLICK, CDlgTPCodeList::ClickGxbtnLosstimeCancel, VTS_NONE)
END_EVENTSINK_MAP()




void CDlgTPCodeList::SetLossTime( CSystemTime lossStart, CSystemTime lossEnd, CSystemTimeSpan lossSpan )
{
	m_LossStart = lossStart;
	m_LossEnd = lossEnd;
	m_LossSpan = lossSpan;
}
void CDlgTPCodeList::ChangeLanguage()
{
	CGxUICtrl::BroadcastButtonCpationIndex(this, m_Language, IDC_BTNX_LANG);
	CGxUICtrl::SetButtonIcon(this, IDC_BTNX_LANG, m_Language);	// 각 언어의 Index에 해당하는 아이콘을 표시한다.
	// 	SelectGroup_1st(m_nSelect_1st);
}

void CDlgTPCodeList::ClickBtnxLang()
{
	m_Language = (LANGUAGE)(( m_Language + 1 ) % 3);							// 표시되는 언어 인덱스, 0:한국어, 1:영어, 2:베트남어 ...
	ChangeLanguage();
}

BOOL CDlgTPCodeList::OnInitDialog()
{
	m_Language = (LANGUAGE)GetMainHandler()->m_nLangIdx;
	ChangeLanguage();
	if(m_strTPCode == TP_CODE_BM_BREAK_DOWN_MANUAL)
		ClickGxbtnLosstimeTpcode1();
	else if(m_strTPCode == TP_CODE_PM_REGULAR_PM)
		ClickGxbtnLosstimeTpcode2();
	else if(m_strTPCode == TP_CODE_PM_CHECK_EQUIPMENT)
		ClickGxbtnLosstimeTpcode3();
	else if(m_strTPCode == TP_CODE_PM_CHECK_QUALITY)
		ClickGxbtnLosstimeTpcode4();
	else if(m_strTPCode == TP_CODE_CM_IMPROVE_EQUIPMENT)
		ClickGxbtnLosstimeTpcode5();
	else if(m_strTPCode == TP_CODE_CM_IMPROVE_PROCESSING)
		ClickGxbtnLosstimeTpcode6();
	else if(m_strTPCode == TP_CODE_CM_SETUP_NEW_PRODUCT)
		ClickGxbtnLosstimeTpcode7();
	else if(m_strTPCode == TP_CODE_CHANGE_MODEL_SAME_MODEL)
		ClickGxbtnLosstimeTpcode8();
	else if(m_strTPCode == TP_CODE_CHANGE_MODEL_DIFFER_MODEL)
		ClickGxbtnLosstimeTpcode9();
	else if(m_strTPCode == TP_CODE_MATERIAL_CHANGE_MATERIAL)
		ClickGxbtnLosstimeTpcode10();
	else if(m_strTPCode == TP_CODE_MATERIAL_DOWN_MATERIAL)
		ClickGxbtnLosstimeTpcode11();
	//pDryRunOnOff->GetCaption()==_T("ON")?m_dry_run=TRUE:m_dry_run=FALSE;
	return TRUE;
}

void CDlgTPCodeList::ClickGxbtnLosstimeConfirm()
{
	//20170322	BKH 일단 임시로 사용안하도록 주석처리 하였습니다.
	//LoadLosstimeItemList();
	if(m_strTPCode.GetLength() < 1)
	{
		CGxMsgBox	dlgMsgBox;

		dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
		dlgMsgBox.SetTitle(_T("알림"), _T("Confirm"), _T("Confirm"));
		dlgMsgBox.SetMessage(FALSE, 
			_T("비가동 사유를 선택해주세요."), 
			_T("Please select losstime items."),		
			_T("Please select losstime items.") , m_Language);

		dlgMsgBox.DoModal();
		return;
	}
	OnOK();
}
BOOL CDlgTPCodeList::LoadLosstimeItemList()
{

	CString sFilename;
	sFilename.Format(_T("%s\\%s"), theConfigBank.m_System.m_strProgramDataPath, NAME_FILE_LOSSTIME_LIST);
	CIni ini(sFilename);
	// 파일이 존재하는지 점검
	if(CFileSupport::FileCheck(sFilename)==FALSE)
	{
		CString sMsg;
		sMsg.Format(_T("* File not found : %s"), sFilename);

		CGxMsgBox	dlgMsgBox;
		dlgMsgBox.SetTitle(_T("알림"), _T("Notifications"), _T("thông báo"));
		dlgMsgBox.SetMessage(FALSE, sMsg, sMsg, sMsg, m_Language);
		dlgMsgBox.DoModal();

		// 오류 내용을 보여준다.
		return FALSE;
	}
// 	m_rLosstimeList.sTPCode =ini.GetString(_T("ShortCut"),m_strTPCode);

	return TRUE;
}

void CDlgTPCodeList::ClickGxbtnLosstimeTpcode1()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn1->GetCaption();
	pTP_Code_Btn1->SetFontBold(TRUE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_AQUA);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode2()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn2->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(TRUE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_ORANGE);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}





void CDlgTPCodeList::ClickGxbtnLosstimeTpcode3()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn3->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(TRUE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_ORANGE);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode4()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn4->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(TRUE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_ORANGE);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode5()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn5->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(TRUE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_TOMATO);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode6()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn6->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(TRUE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_TOMATO);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode7()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn7->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(TRUE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_TOMATO);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode8()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn8->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(TRUE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_VIIOLET);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode9()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn9->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(TRUE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_VIIOLET);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode10()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn10->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(TRUE);
	pTP_Code_Btn11->SetFontBold(FALSE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_SPRING_GREEN);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_DARKGRAY);
}


void CDlgTPCodeList::ClickGxbtnLosstimeTpcode11()
{
	CGxButtonEx* pTP_Code_Btn1= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_1);
	CGxButtonEx* pTP_Code_Btn2= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_2);
	CGxButtonEx* pTP_Code_Btn3= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_3);
	CGxButtonEx* pTP_Code_Btn4= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_4);
	CGxButtonEx* pTP_Code_Btn5= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_5);
	CGxButtonEx* pTP_Code_Btn6= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_6);
	CGxButtonEx* pTP_Code_Btn7= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_7);
	CGxButtonEx* pTP_Code_Btn8= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_8);
	CGxButtonEx* pTP_Code_Btn9= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_9);
	CGxButtonEx* pTP_Code_Btn10= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_10);
	CGxButtonEx* pTP_Code_Btn11= (CGxButtonEx*)GetDlgItem(IDC_GXBTN_LOSSTIME_TPCODE_11);
	m_strTPCode=pTP_Code_Btn11->GetCaption();
	pTP_Code_Btn1->SetFontBold(FALSE);
	pTP_Code_Btn2->SetFontBold(FALSE);
	pTP_Code_Btn3->SetFontBold(FALSE);
	pTP_Code_Btn4->SetFontBold(FALSE);
	pTP_Code_Btn5->SetFontBold(FALSE);
	pTP_Code_Btn6->SetFontBold(FALSE);
	pTP_Code_Btn7->SetFontBold(FALSE);
	pTP_Code_Btn8->SetFontBold(FALSE);
	pTP_Code_Btn9->SetFontBold(FALSE);
	pTP_Code_Btn10->SetFontBold(FALSE);
	pTP_Code_Btn11->SetFontBold(TRUE);
	pTP_Code_Btn1->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn2->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn3->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn4->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn5->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn6->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn7->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn8->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn9->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn10->SetBackColor(GXCOLOR_DARKGRAY);
	pTP_Code_Btn11->SetBackColor(GXCOLOR_SPRING_GREEN);
}


void CDlgTPCodeList::ClickGxbtnLosstimeCancel()
{
	// 아무것도 입력 안하고 창 닫는거 방지
	if(m_bCanNotCancel == FALSE)
		OnCancel();
}
