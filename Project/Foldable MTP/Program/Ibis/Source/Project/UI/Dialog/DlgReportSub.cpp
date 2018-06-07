// DlgReportSub.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "DlgReportSub.h"
#include "afxdialogex.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
//GxGUIFrame ���
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"

// CDlgReportSub ��ȭ �����Դϴ�.

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

	// Display ���� Update - LSH171218
	// Window�� ������ ���Ŀ� ȣ��Ǵ� Message������ ���⼭ Window�ʱ�ȭ �Լ��� ȣ���ؾ� �Ѵ�.
	// �׷��� ���� ������ ��ü�� ���� �����츦 �����ͷ� �����ϱ� ������ ��� �����Ϳ� ���Ͽ� ���α׷��� �״´�.
	UpdateDisplay();

	return TRUE;  // return TRUE unless you set the focus to a control
}


BEGIN_MESSAGE_MAP(CDlgReportSub, CDialogEx)
END_MESSAGE_MAP()


void CDlgReportSub::InitData()
{
	// List �ʱ�ȭ - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX+1 ; nJig++)
	{
		for(int nCh=0 ; nCh<JIG_CH_MAX+1 ; nCh++)
			m_strValueList[nJig][nCh] = _T("");
	}

	// ����ϴ� UI ID ���� - LSH171217
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


// cContactInfo �޽��� ó�����Դϴ�.
void CDlgReportSub::SetProductItem(ProductInfo* pData, UINT nColumn)
{
	// �����͸� ������ �ӽ� ���� ���� - LSH171218
	CString strTemp = _T("");
	UINT nTmepCh = 0, nTmepJig = 0, nTempProduct = 0;

	// ������ �ʱ�ȭ - LSH171218
	InitData();

	// �Է¹��� �ð��� ���� - LSH171218
	m_strHour.Format(_T("%d"), pData->m_nHour);

	// Column name ���� - LSH171218
	m_strColumnName = theProductBank.GetProductColumnName((PRODUCT_COLUMN)nColumn);

	// Main data list�� ���� ���� �Ѵ� - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
	{
		for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
		{
			// ���� �����´� - LSH171218
			strTemp.Format(_T("%d/%d"), pData->m_vetProductData[nColumn][nJig][nCh], pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh]);
			// �ۼ�Ʈ�� ���� �� �Է� - LSH171218
			strTemp += _T("(") + theProductBank.CalcPercent(pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh], pData->m_vetProductData[nColumn][nJig][nCh]) + _T(")");
			m_strValueList[nJig][nCh] += strTemp;
		}
	}


	// JIG�� Total���� ���� �Ѵ� - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
	{
		// �ӽ� ���� �ʱ�ȭ
		nTmepCh = nTmepJig = nTempProduct = 0;

		// �� Ch���� ��� ���Ѵ�- LSH171218
		for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
		{
			// �� ch�� - LSH171218
			nTmepCh += pData->m_vetProductData[nColumn][nJig][nCh];
			// �� ���귮- LSH171218
			nTempProduct += pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh];
		}

		// CH�� ���귮�� �Է� - LSH171218
		strTemp.Format(_T("%d/%d"), nTmepCh, nTempProduct);
		// �ۼ�Ʈ�� ���� �� �Է� - LSH171218
		strTemp += _T("(") + theProductBank.CalcPercent(nTempProduct, nTmepCh) + _T(")");
		m_strValueList[nJig][JIG_CH_MAX] += strTemp;
	}


	// Ch�� Total���� ���� �Ѵ� - LSH171218
	for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
	{
		// �ӽ� ���� �ʱ�ȭ
		nTmepCh = nTmepJig = nTempProduct = 0;

		// �� Ch���� ��� ���Ѵ�- LSH171218
		for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
		{
			// �� ch�� - LSH171218
			nTmepJig += pData->m_vetProductData[nColumn][nJig][nCh];
			// �� ���귮- LSH171218
			nTempProduct += pData->m_vetProductData[PRODUCT_COLUMN_PRODUCT][nJig][nCh];
		}

		// JIG�� ���� �Է� - LSH171218
		strTemp.Format(_T("%d/%d"), nTmepJig, nTempProduct);
		// �ۼ�Ʈ�� ���� �� �Է� - LSH171218
		strTemp +=  _T("(") + theProductBank.CalcPercent(nTempProduct, nTmepJig) + _T(")");
		m_strValueList[JIG_ID_MAX][nCh] += strTemp;
	}
	
	// �� ���� ���� �����´� - LSH
	strTemp.Format(_T("%d/%d"), pData->m_nTotalData[nColumn], pData->m_nTotalData[PRODUCT_COLUMN_PRODUCT]);
	// �� ������ �ۼ�Ʈ ���� �����´� - LSH
	strTemp +=  _T("(") + theProductBank.CalcPercent(pData->m_nTotalData[PRODUCT_COLUMN_PRODUCT], pData->m_nTotalData[nColumn]) + _T(")");
	m_strValueList[JIG_ID_MAX][JIG_CH_MAX] = strTemp;
}

void CDlgReportSub::UpdateDisplay()
{
	// �ӽ� ���� ���� ���� - LSH171218
	CString strTemp = _T("");

	// Title �Է� - LSH171218
	strTemp = m_strColumnName + _T(" rate of each Channel at ") + m_strHour + _T(" o'Clock");
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_CONTACTINFO_TITLE, strTemp);

	// List ���� Update - LSH171218
	for(int nJig=0 ; nJig<JIG_ID_MAX+1 ; nJig++)
		for(int nCh=0 ; nCh<JIG_CH_MAX+1 ; nCh++)
			CGxUICtrl::SetStaticString(this, m_nUiIdList[nJig][nCh], m_strValueList[nJig][nCh]);
}


// CDlgReportSub �޽��� ó�����Դϴ�.
BEGIN_EVENTSINK_MAP(CDlgReportSub, CDialogEx)
	ON_EVENT(CDlgReportSub, IDOK, DISPID_CLICK, CDlgReportSub::ClickOk, VTS_NONE)
END_EVENTSINK_MAP()


void CDlgReportSub::ClickOk()
{
	OnCancel();
}