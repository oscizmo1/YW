// ContactInfo.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "ContactInfo.h"
#include "afxdialogex.h"

//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "UI\GausGUI\GxButtonEx.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// cContactInfo 대화 상자입니다.

IMPLEMENT_DYNAMIC(cContactInfo, CDialog)

cContactInfo::cContactInfo(CWnd* pParent /*=NULL*/)
	: CDialog(cContactInfo::IDD, pParent)
{
	m_bContactFirst = TRUE;
}

cContactInfo::~cContactInfo()
{
}

void cContactInfo::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(cContactInfo, CDialog)
END_MESSAGE_MAP()


// cContactInfo 메시지 처리기입니다.
void cContactInfo::SetDialogItem(cCONTACT_DATA* pData)
{
	m_pData = pData;
}

void cContactInfo::SetDialogItemTotal(cCONTACT_DATA* pData)
{
		//사용 안해도 될듯. 
}

BOOL cContactInfo::OnInitDialog()
{
	CDialog::OnInitDialog();
	
	CString	szDisplayTemp;
	int		nCount	= m_pData->tCreatCnt.iValue;
	float	fCalcul	= 0;
	
	//Title 
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_DAYTIME, szTitle);

	//today Count
	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_CONTACT_INFO, szDay);
	//CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_CONTACT_INFO, szDayCount);

	//total count
	CGxUICtrl::SetStaticString(this, m_pData->tCreatCnt.uID, CStringSupport::N2C(nCount));
 
								//First Contact
	if(m_bContactFirst == TRUE)	SetUIFirstContact();
	else						SetUIFinalContact();

								//Final Contact

	////Zone,Channel
	//for(int i=0;i<5;i++)
	//{
	//	//AZone
	//	if(m_pData->vctZoneA_Cnt[i] == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)m_pData->vctZoneA_ContactRatio[i].iValue / (float)m_pData->vctZoneA_Cnt[i];

	//	szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneA_ContactRatio[i].iValue, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, m_pData->vctZoneA_ContactRatio[i].uID, szDisplayTemp);

	//	//BZone
	//	if(m_pData->vctZoneB_Cnt[i] == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)m_pData->vctZoneB_ContactRatio[i].iValue / (float)m_pData->vctZoneB_Cnt[i];

	//	szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneB_ContactRatio[i].iValue, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, m_pData->vctZoneB_ContactRatio[i].uID, szDisplayTemp);

	//	//CZone
	//	if(m_pData->vctZoneA_Cnt[i] == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)m_pData->vctZoneC_ContactRatio[i].iValue / (float)m_pData->vctZoneC_Cnt[i];

	//	szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneC_ContactRatio[i].iValue, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, m_pData->vctZoneC_ContactRatio[i].uID, szDisplayTemp);

	//	//DZone
	//	if(m_pData->vctZoneA_Cnt[i] == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)m_pData->vctZoneD_ContactRatio[i].iValue / (float)m_pData->vctZoneD_Cnt[i];

	//	szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneD_ContactRatio[i].iValue, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, m_pData->vctZoneD_ContactRatio[i].uID, szDisplayTemp);
	//}

	////Channel Total
	//int nCreatTotal		= 0;
	//int nContactTotal	= 0;
	//std::vector<UINT> vctUI_ChannelID;
	//vctUI_ChannelID.resize(5);
	//vctUI_ChannelID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH1;
	//vctUI_ChannelID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH2;
	//vctUI_ChannelID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH3;
	//vctUI_ChannelID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH4;
	//vctUI_ChannelID[4] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH5;

	//for(int i=0;i<5;i++)
	//{
	//	nCreatTotal		= m_pData->vctZoneA_Cnt[i]+
	//		m_pData->vctZoneB_Cnt[i]+
	//		m_pData->vctZoneC_Cnt[i]+
	//		m_pData->vctZoneD_Cnt[i];

	//	nContactTotal	= m_pData->vctZoneA_ContactRatio[i].iValue + 
	//		m_pData->vctZoneB_ContactRatio[i].iValue+
	//		m_pData->vctZoneC_ContactRatio[i].iValue+
	//		m_pData->vctZoneD_ContactRatio[i].iValue;

	//	if(nCreatTotal == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)nContactTotal/(float)nCreatTotal;

	//	szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, vctUI_ChannelID[i], szDisplayTemp);
	//}

	////Zone Total
	//std::vector<UINT> vctUI_ZoneID;
	//vctUI_ZoneID.resize(5);
	//vctUI_ZoneID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEA;
	//vctUI_ZoneID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEB;
	//vctUI_ZoneID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEC;
	//vctUI_ZoneID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONED;

	//for(int i=0;i<4;i++)
	//{
	//	std::vector<int>::iterator iterCnt;
	//	std::vector<tElement>::iterator iterRatioCnt;

	//	if(i == 0)
	//	{
	//		iterCnt		= m_pData->vctZoneA_Cnt.begin();
	//		iterRatioCnt= m_pData->vctZoneA_ContactRatio.begin();
	//	}
	//	else if(i == 1)
	//	{
	//		iterCnt		= m_pData->vctZoneB_Cnt.begin();
	//		iterRatioCnt= m_pData->vctZoneB_ContactRatio.begin();
	//	}
	//	else if(i == 2)
	//	{
	//		iterCnt		= m_pData->vctZoneC_Cnt.begin();
	//		iterRatioCnt= m_pData->vctZoneC_ContactRatio.begin();
	//	}
	//	else 
	//	{
	//		iterCnt		= m_pData->vctZoneD_Cnt.begin();
	//		iterRatioCnt= m_pData->vctZoneD_ContactRatio.begin();
	//	}

	//	nCreatTotal		= 0;
	//	nContactTotal	= 0;
	//	for(int i=0;i<5;i++)
	//	{
	//		nCreatTotal		+= iterCnt[i];
	//		nContactTotal	+= iterRatioCnt[i].iValue;
	//	}

	//	if(nCreatTotal == 0)	
	//		fCalcul = 0; 
	//	else								
	//		fCalcul = (float)nContactTotal/(float)nCreatTotal;

	//	szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
	//	szDisplayTemp += _T("%)");
	//	CGxUICtrl::SetStaticString(this, vctUI_ZoneID[i], szDisplayTemp);
	//}

	return TRUE;  // return TRUE unless you set the focus to a control
}

void cContactInfo::SetUIFirstContact()
{
	CString	szDisplayTemp;
	int		nCount	= m_pData->tCreatCnt.iValue;
	float	fCalcul	= 0;

	//Zone,Channel
	for(int i=0;i<5;i++)
	{
		//AZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneA_ContactRatio[i].iValue / (float)m_pData->vctZoneA_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneA_ContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneA_ContactRatio[i].uID, szDisplayTemp);

		//BZone
		if(m_pData->vctZoneB_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneB_ContactRatio[i].iValue / (float)m_pData->vctZoneB_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneB_ContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneB_ContactRatio[i].uID, szDisplayTemp);

		//CZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneC_ContactRatio[i].iValue / (float)m_pData->vctZoneC_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneC_ContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneC_ContactRatio[i].uID, szDisplayTemp);

		//DZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneD_ContactRatio[i].iValue / (float)m_pData->vctZoneD_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneD_ContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneD_ContactRatio[i].uID, szDisplayTemp);
	}

	//Channel Total
	int nCreatTotal		= 0;
	int nContactTotal	= 0;
	std::vector<UINT> vctUI_ChannelID;
	vctUI_ChannelID.resize(5);
	vctUI_ChannelID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH1;
	vctUI_ChannelID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH2;
	vctUI_ChannelID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH3;
	vctUI_ChannelID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH4;
	vctUI_ChannelID[4] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH5;

	for(int i=0;i<5;i++)
	{
		nCreatTotal		= m_pData->vctZoneA_Cnt[i]+
			m_pData->vctZoneB_Cnt[i]+
			m_pData->vctZoneC_Cnt[i]+
			m_pData->vctZoneD_Cnt[i];

		nContactTotal	= m_pData->vctZoneA_ContactRatio[i].iValue + 
			m_pData->vctZoneB_ContactRatio[i].iValue+
			m_pData->vctZoneC_ContactRatio[i].iValue+
			m_pData->vctZoneD_ContactRatio[i].iValue;

		if(nCreatTotal == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)nContactTotal/(float)nCreatTotal;

		szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, vctUI_ChannelID[i], szDisplayTemp);
	}

	//Zone Total
	std::vector<UINT> vctUI_ZoneID;
	vctUI_ZoneID.resize(5);
	vctUI_ZoneID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEA;
	vctUI_ZoneID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEB;
	vctUI_ZoneID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEC;
	vctUI_ZoneID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONED;

	int nCreatTotalAll = 0;
	int nContactTotalAll = 0;

	for(int i=0;i<4;i++)
	{
		std::vector<int>::iterator iterCnt;
		std::vector<tElement>::iterator iterRatioCnt;

		if(i == 0)
		{
			iterCnt		= m_pData->vctZoneA_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneA_ContactRatio.begin();
		}
		else if(i == 1)
		{
			iterCnt		= m_pData->vctZoneB_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneB_ContactRatio.begin();
		}
		else if(i == 2)
		{
			iterCnt		= m_pData->vctZoneC_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneC_ContactRatio.begin();
		}
		else 
		{
			iterCnt		= m_pData->vctZoneD_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneD_ContactRatio.begin();
		}

		nCreatTotal		= 0;
		nContactTotal	= 0;
		for(int j=0;j<5;j++)
		{
			nCreatTotal		+= iterCnt[j];
			nContactTotal	+= iterRatioCnt[j].iValue;
		}

		if(nCreatTotal == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)nContactTotal/(float)nCreatTotal;

		szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, vctUI_ZoneID[i], szDisplayTemp);

		nCreatTotalAll		+= nCreatTotal;
		nContactTotalAll	+= nContactTotal;
	}

	if(nCreatTotalAll == 0)	
		fCalcul = 0; 
	else								
		fCalcul = (float)nContactTotalAll/(float)nCreatTotalAll;

	szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotalAll, fCalcul*100);
	szDisplayTemp += _T("%)");

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_CONTACT_TOTAL_ALL, szDisplayTemp);
}

void cContactInfo::SetUIFinalContact()
{
	CString	szDisplayTemp;
	int		nCount	= m_pData->tCreatCnt.iValue;
	float	fCalcul	= 0;

	//Zone,Channel
	for(int i=0;i<5;i++)
	{
		//AZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneA_FinalContactRatio[i].iValue / (float)m_pData->vctZoneA_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneA_FinalContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneA_FinalContactRatio[i].uID, szDisplayTemp);

		//BZone
		if(m_pData->vctZoneB_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneB_FinalContactRatio[i].iValue / (float)m_pData->vctZoneB_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneB_FinalContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneB_FinalContactRatio[i].uID, szDisplayTemp);

		//CZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneC_FinalContactRatio[i].iValue / (float)m_pData->vctZoneC_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneC_FinalContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneC_FinalContactRatio[i].uID, szDisplayTemp);

		//DZone
		if(m_pData->vctZoneA_Cnt[i] == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)m_pData->vctZoneD_FinalContactRatio[i].iValue / (float)m_pData->vctZoneD_Cnt[i];

		szDisplayTemp.Format(_T("%d(%0.2f"),m_pData->vctZoneD_FinalContactRatio[i].iValue, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, m_pData->vctZoneD_FinalContactRatio[i].uID, szDisplayTemp);
	}

	//Channel Total
	int nCreatTotal		= 0;
	int nContactTotal	= 0;
	std::vector<UINT> vctUI_ChannelID;
	vctUI_ChannelID.resize(5);
	vctUI_ChannelID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH1;
	vctUI_ChannelID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH2;
	vctUI_ChannelID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH3;
	vctUI_ChannelID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH4;
	vctUI_ChannelID[4] = IDC_GXLBL_REPORT_CONTACT_ToTAL_CH5;

	for(int i=0;i<5;i++)
	{
		nCreatTotal		= m_pData->vctZoneA_Cnt[i]+
			m_pData->vctZoneB_Cnt[i]+
			m_pData->vctZoneC_Cnt[i]+
			m_pData->vctZoneD_Cnt[i];

		nContactTotal	= m_pData->vctZoneA_FinalContactRatio[i].iValue + 
			m_pData->vctZoneB_FinalContactRatio[i].iValue+
			m_pData->vctZoneC_FinalContactRatio[i].iValue+
			m_pData->vctZoneD_FinalContactRatio[i].iValue;

		if(nCreatTotal == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)nContactTotal/(float)nCreatTotal;

		szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, vctUI_ChannelID[i], szDisplayTemp);
	}

	int nCreatTotalAll = 0;
	int nContactTotalAll = 0;

	//Zone Total
	std::vector<UINT> vctUI_ZoneID;
	vctUI_ZoneID.resize(5);
	vctUI_ZoneID[0] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEA;
	vctUI_ZoneID[1] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEB;
	vctUI_ZoneID[2] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONEC;
	vctUI_ZoneID[3] = IDC_GXLBL_REPORT_CONTACT_ToTAL_ZONED;

	for(int i=0;i<4;i++)
	{
		std::vector<int>::iterator iterCnt;
		std::vector<tElement>::iterator iterRatioCnt;

		if(i == 0)
		{
			iterCnt		= m_pData->vctZoneA_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneA_FinalContactRatio.begin();
		}
		else if(i == 1)
		{
			iterCnt		= m_pData->vctZoneB_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneB_FinalContactRatio.begin();
		}
		else if(i == 2)
		{
			iterCnt		= m_pData->vctZoneC_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneC_FinalContactRatio.begin();
		}
		else 
		{
			iterCnt		= m_pData->vctZoneD_Cnt.begin();
			iterRatioCnt= m_pData->vctZoneD_FinalContactRatio.begin();
		}

		nCreatTotal		= 0;
		nContactTotal	= 0;
		for(int j=0;j<5;j++)
		{
			nCreatTotal		+= iterCnt[j];
			nContactTotal	+= iterRatioCnt[j].iValue;
		}

		if(nCreatTotal == 0)	
			fCalcul = 0; 
		else								
			fCalcul = (float)nContactTotal/(float)nCreatTotal;

		szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotal, fCalcul*100);
		szDisplayTemp += _T("%)");
		CGxUICtrl::SetStaticString(this, vctUI_ZoneID[i], szDisplayTemp);

		nCreatTotalAll		+= nCreatTotal;
		nContactTotalAll	+= nContactTotal;
	}

	if(nCreatTotalAll == 0)	
		fCalcul = 0; 
	else								
		fCalcul = (float)nContactTotalAll/(float)nCreatTotalAll;

	szDisplayTemp.Format(_T("%d(%0.2f"),nContactTotalAll, fCalcul*100);
	szDisplayTemp += _T("%)");

	CGxUICtrl::SetStaticString(this, IDC_GXLBL_REPORT_CONTACT_TOTAL_ALL, szDisplayTemp);

}