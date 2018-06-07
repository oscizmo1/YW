// DlgReportSub.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "DlgReportSub.h"
#include "afxdialogex.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
//GxGUIFrame 사용
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CDlgReportSub 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgReportSub, CDialogEx)

CDlgReportSub::CDlgReportSub(CWnd* pParent /*=NULL*/)
	: CDialogEx(CDlgReportSub::IDD, pParent)
{

}

CDlgReportSub::~CDlgReportSub()
{
}

void CDlgReportSub::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BOOL CDlgReportSub::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// Display 상태 Update - LSH171218
	// Window가 생성된 직후에 호출되는 Message임으로 여기서 Window초기화 함수를 호출해야 한다.
	// 그렇게 하지 않으면 실체가 없는 윈도우를 포인터로 지정하기 때문에 허상 포인터에 의하여 프로그램이 죽는다.
	UpdateDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CDlgReportSub, CDialogEx)
END_MESSAGE_MAP()


void CDlgReportSub::InitData()
{
	// List 초기화 - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX+1 ; nJig++)
	{
		for(int nCh=0 ; nCh<JIG_CH_MAX+1 ; nCh++)
			m_strValueList[nJig][nCh] = _T("");
	}

	// 사용하는 UI ID 저장 - LSH171217
	/* A Jig */
	m_nUiIdList[0][0] = IDC_GXSTA_CONTACTINFO_AJIG_CH1;
	m_nUiIdList[0][1] = IDC_GXSTA_CONTACTINFO_AJIG_AVERAGE;
	/* B Jig */
	m_nUiIdList[1][0] = IDC_GXSTA_CONTACTINFO_BJIG_CH1;
	m_nUiIdList[1][1] = IDC_GXSTA_CONTACTINFO_BJIG_AVERAGE;
	/* Total */
	m_nUiIdList[2][0] = IDC_GXSTA_CONTACTINFO_TOTAL_CH1_AVERAGE;
	m_nUiIdList[2][1] = IDC_GXSTA_CONTACTINFO_TOTAL_AVERAGE;
}


// cContactInfo 메시지 처리기입니다.
void CDlgReportSub::SetProductItem(ProductInfo* pData, UINT nColumn)
{
	// 데이터를 저장할 임시 변수 선언 - LSH171218
	CString strTemp = _T("");
	UINT nTmepCh = 0, nTmepJig = 0, nTempProduct = 0;

	// 데이터 초기화 - LSH171218
	InitData();

	// 입력받은 시간값 저장 - LSH171218
	m_strHour.Format(_T("%d"), pData->m_nHour);

	// Column name 저장 - LSH171218
	m_strColumnName = theProductBank.GetProductColumnName((PRODUCT_COLUMN)nColumn);

	// Main data list의 값을 저장 한다 - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
	{
		for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
		{
			// 값을 가져온다 - LSH171218
			strTemp.Format(_T("%d/%d"), pData->m_vetProductData[nColumn][nJig][nCh], pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh]);
			// 퍼센트로 계산된 값 입력 - LSH171218
			strTemp += _T("(") + theProductBank.CalcPercent(pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh], pData->m_vetProductData[nColumn][nJig][nCh]) + _T(")");
			m_strValueList[nJig][nCh] += strTemp;
		}
	}


	// JIG별 Total값을 저장 한다 - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
	{
		// 임시 변수 초기화
		nTmepCh = nTmepJig = nTempProduct = 0;

		// 각 Ch값을 모두 더한다- LSH171218
		for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
		{
			// 각 ch값 - LSH171218
			nTmepCh += pData->m_vetProductData[nColumn][nJig][nCh];
			// 총 생산량- LSH171218
			nTempProduct += pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh];
		}

		// CH별 생산량을 입력 - LSH171218
		strTemp.Format(_T("%d/%d"), nTmepCh, nTempProduct);
		// 퍼센트로 계산된 값 입력 - LSH171218
		strTemp += _T("(") + theProductBank.CalcPercent(nTempProduct, nTmepCh) + _T(")");
		m_strValueList[nJig][JIG_CH_MAX] += strTemp;
	}


	// Ch별 Total값을 저장 한다 - LSH171218
	for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
	{
		// 임시 변수 초기화
		nTmepCh = nTmepJig = nTempProduct = 0;

		// 각 Ch값을 모두 더한다- LSH171218
		for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
		{
			// 각 ch값 - LSH171218
			nTmepJig += pData->m_vetProductData[nColumn][nJig][nCh];
			// 총 생산량- LSH171218
			nTempProduct += pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh];
		}

		// JIG별 값을 입력 - LSH171218
		strTemp.Format(_T("%d/%d"), nTmepJig, nTempProduct);
		// 퍼센트로 계산된 값 입력 - LSH171218
		strTemp +=  _T("(") + theProductBank.CalcPercent(nTempProduct, nTmepJig) + _T(")");
		m_strValueList[JIG_ID_MAX][nCh] += strTemp;
	}
	
	// 총 수량 값을 가져온다 - LSH
	strTemp.Format(_T("%d/%d"), pData->m_nTotalData[nColumn], pData->m_nTotalData[PRODUCT_COLUMN_PRODUCT]);
	// 총 수량의 퍼센트 값을 가져온다 - LSH
	strTemp +=  _T("(") + theProductBank.CalcPercent(pData->m_nTotalData[PRODUCT_COLUMN_PRODUCT], pData->m_nTotalData[nColumn]) + _T(")");
	m_strValueList[JIG_ID_MAX][JIG_CH_MAX] = strTemp;
}

void CDlgReportSub::UpdateDisplay()
{
	// 임시 저장 변수 선언 - LSH171218
	CString strTemp = _T("");

	// Title 입력 - LSH171218
	strTemp = m_strColumnName + _T(" rate of each Channel at ") + m_strHour + _T(" o'Clock");
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_CONTACTINFO_TITLE, strTemp);

	// List 상태 Update - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX+1 ; nJig++)
		for(int nCh=0 ; nCh<JIG_CH_MAX+1 ; nCh++)
			CGxUICtrl::SetStaticString(this, m_nUiIdList[nJig][nCh], m_strValueList[nJig][nCh]);
}


// CDlgReportSub 메시지 처리기입니다.
BEGIN_EVENTSINK_MAP(CDlgReportSub, CDialogEx)
	ON_EVENT(CDlgReportSub, IDOK, DISPID_CLICK, CDlgReportSub::ClickOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgReportSub::ClickOk()
{
	OnCancel();
}