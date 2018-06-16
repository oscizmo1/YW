// ViewFDC.cpp : implementation file
//

#include "stdafx.h"
#include "GaonNuri.h"
#include "ViewFDC.h"


// CViewFDC

IMPLEMENT_DYNCREATE(CViewFDC, CFormView)

CViewFDC::CViewFDC()
	: CFormView(CViewFDC::IDD)
{

}

CViewFDC::~CViewFDC()
{
}

void CViewFDC::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_FDC, m_ListFDC);

}

BEGIN_MESSAGE_MAP(CViewFDC, CFormView)
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CViewFDC diagnostics

#ifdef _DEBUG
void CViewFDC::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CViewFDC::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CViewFDC message handlers

BOOL CViewFDC::PreTranslateMessage(MSG* pMsg)
{
	// TODO: Add your specialized code here and/or call the base class
	if((pMsg->message == WM_SYSKEYDOWN && pMsg->wParam == VK_F4) || pMsg->wParam == VK_ESCAPE || pMsg->wParam == VK_RETURN)
		return TRUE;

	return CFormView::PreTranslateMessage(pMsg);
}


// View ȭ�� ���ٽ� ó�� ����Ǵ� �κ�
void CViewFDC::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	
	// FDC_MAP_INFO.map ���� ����� Ȯ���Ѵ�.
	if (ReadingToFDCMap() != TRUE)
		AfxMessageBox("FDC Map Reading Fail");
	
	// FDC �׸��� List�� �����Ѵ�.
	CreateFDCList();
 		
	// FDC List �ۼ� Timer ����
	SetFdcSV();

	// Data �ʱ�ȭ
	InitData();

	// Thread ����
	CreateThread();	
}


// FDC_MAP_INFO.map ���� ����� Ȯ���Ѵ�.
BOOL CViewFDC::ReadingToFDCMap()
{
	char cFileFullName[256];
	sprintf_s(cFileFullName, 256, "%s\\FDC_INFO_MAP.csv", CGaonNuriApp::GetSetupPath());

	int nFdcInfoCnt;
	nFdcInfoCnt	= CDefNameMap::LoadFdcInfoMap(CDefNameMap::MAP_MAX_COL, &CDefNameMap::cFdcInfoMap[0][0][0], cFileFullName);

	if (nFdcInfoCnt > 0)
	{
		g_pData->m_FDC.m_nFDC_TotalCount = nFdcInfoCnt;
		GetLogCtrl()->AddBuf(LOG_CIM, "ReadFdcList Pass Count(%d), Path(%s)", nFdcInfoCnt, cFileFullName);

		for (int i=0; i<nFdcInfoCnt; i++)
		{
			strcpy_s(g_pData->m_FDC.m_cSetUNIT[i], CDefNameMap::cFdcInfoMap[i][1]);
			strcpy_s(g_pData->m_FDC.m_cSetSVID[i], CDefNameMap::cFdcInfoMap[i][2]);
			strcpy_s(g_pData->m_FDC.m_cSetSVNAME[i], CDefNameMap::cFdcInfoMap[i][3]);
			strcpy_s(g_pData->m_FDC.m_cSetSV[i], CDefNameMap::cFdcInfoMap[i][4]);
			//kjpark 20170929 FDC ������, ������, ���� ����
			strcpy_s(g_pData->m_FDC.m_cSetCV[i], CDefNameMap::cFdcInfoMap[i][5]);
			strcpy_s(g_pData->m_FDC.m_cSetRV[i], CDefNameMap::cFdcInfoMap[i][6]);
			strcpy_s(g_pData->m_FDC.m_cSetSVMAX[i], CDefNameMap::cFdcInfoMap[i][7]);
			strcpy_s(g_pData->m_FDC.m_cSetSVMIN[i], CDefNameMap::cFdcInfoMap[i][8]);
			strcpy_s(g_pData->m_FDC.m_cSetSVTYPE[i], CDefNameMap::cFdcInfoMap[i][9]);
			strcpy_s(g_pData->m_FDC.m_cMappingSVNAME[i], CDefNameMap::cFdcInfoMap[i][10]);	//20141222 sms
		}//end of for
	}//end of if
	else
	{
 		GetLogCtrl()->AddBuf(LOG_CIM, "ReadFdcList Fail Count(%d), Path(%s)", nFdcInfoCnt, cFileFullName);
 		return FALSE;
	}

	return TRUE;
}

// FDC �׸��� List�� �����Ѵ�.
void CViewFDC::CreateFDCList()
{
	m_ListFDC.DeleteAllItems();
	// m_ListFDC.SetColumnHeader(_T("NUM,50; UNIT,200; SVID,60; SVNAME,320; SV,100; MAX,100; MIN,100; SVTYPE,100"));
	//kjpark 20170929 FDC ������, ������, ���� ����
	m_ListFDC.SetColumnHeader(_T("SVID,50; UNIT,180; SVNAME,200; SV,60; CV,60; RV,60;TYPE,60"));
	m_ListFDC.SetGridLines(TRUE); 
	m_ListFDC.SetEditable(FALSE);
	m_ListFDC.SetSortable(TRUE);
		
	COLORREF clr;
	CString str;

	for(int i=0; i<g_pData->m_FDC.m_nFDC_TotalCount; i++)
	{
		// str.Format("%d", i+1);	
		m_ListFDC.InsertItem(i, (CString)g_pData->m_FDC.m_cSetSVID[i]);

		m_ListFDC.SetItemText(i, 1, (CString)g_pData->m_FDC.m_cSetUNIT[i]);
		// m_ListFDC.SetItemText(i, 1, (CString)g_pData->m_FDC.m_cSetSVID[i]);
		m_ListFDC.SetItemText(i, 2, (CString)g_pData->m_FDC.m_cSetSVNAME[i]);
		m_ListFDC.SetItemText(i, 3, (CString)g_pData->m_FDC.m_cSetSV[i]);
		//kjpark 20170929 FDC ������, ������, ���� ����
		m_ListFDC.SetItemText(i, 4, (CString)g_pData->m_FDC.m_cSetCV[i]);
		m_ListFDC.SetItemText(i, 5, (CString)g_pData->m_FDC.m_cSetRV[i]);
		m_ListFDC.SetItemText(i, 6, (CString)g_pData->m_FDC.m_cSetSVTYPE[i]);

		//m_ListFDC.SetItemText(i, 5, (CString)g_pData->m_FDC.m_cSetSVMAX[i]);
		//m_ListFDC.SetItemText(i, 6, (CString)g_pData->m_FDC.m_cSetSVMIN[i]);
		//m_ListFDC.SetItemText(i, 7, (CString)g_pData->m_FDC.m_cSetSVTYPE[i]);


		if (i == 0)
		{
			clr = RGB(rand() % 128+128, rand() % 128+128, rand() % 128+128);
		}
		else if (i>0 && strcmp(g_pData->m_FDC.m_cSetUNIT[i], g_pData->m_FDC.m_cSetUNIT[i-1])!=0)
		{
			clr = RGB(rand() % 128+128, rand() % 128+128, rand() % 128+128);
		}
				
		m_ListFDC.SetItemBkColor(i, 0, clr, TRUE );
		m_ListFDC.SetItemBkColor(i, 1, clr, TRUE );
		m_ListFDC.SetItemBkColor(i, 2, clr, TRUE );
		m_ListFDC.SetItemBkColor(i, 3, clr, TRUE );
		//kjpark 20170929 FDC ������, ������, ���� ����
		m_ListFDC.SetItemBkColor(i, 4, clr, TRUE );
		m_ListFDC.SetItemBkColor(i, 5, clr, TRUE );
	}//end of for
}

// Data �ʱ�ȭ
void CViewFDC::InitData()
{
}


// Thread ����
void CViewFDC::CreateThread()
{
	m_bFdcReadDataThreadRun = TRUE;
	m_pFdcReadDataThread = AfxBeginThread(thrFdcReadDataThread, (LPVOID)this);
	if(m_pFdcReadDataThread!=NULL)
		m_hFdcReadDataThread = m_pFdcReadDataThread->m_hThread;
	else
		AfxMessageBox("!!! Attention !!! \n Cannot created Fdc Read Data Thread");
}


// Thread ����
void CViewFDC::DeleteThread()
{
	if (m_bFdcReadDataThreadRun)
	{
		m_bFdcReadDataThreadRun = false;

		DWORD dwExitCode = 0;
		::GetExitCodeThread(m_hFdcReadDataThread, &dwExitCode);
		if(dwExitCode != 0)
		{
			if(::WaitForSingleObject(m_hFdcReadDataThread, 5000) != WAIT_OBJECT_0)
			{
				::TerminateThread(m_hFdcReadDataThread, 0);
			}
		}
		m_hFdcReadDataThread = NULL;
		m_pFdcReadDataThread = NULL;
	}
}

UINT CViewFDC::thrFdcReadDataThread(LPVOID pParam)
{
	CViewFDC *pOwner = (CViewFDC*)pParam;
	pOwner->m_bFdcReadDataThreadRun = TRUE;

	while ( pOwner->m_bFdcReadDataThreadRun )
	{
		pOwner->ReadFdcSV();
		Sleep(1000);
	}

	return 0;
}

// � PC���� FDC���� �����´�.
void CViewFDC::ReadFdcSV()
{
	CString sFile;
	sFile = CSettingParm::cFDC_SV_PATH;	//FDC_SV.par

	CFileFind finder;
	BOOL fExist = finder.FindFile(sFile);

	if (fExist > 0)
	{
		CIni ini(sFile);
		CCimFDC *pFdc = &g_pData->m_FDC;

		for (int i=0; i<pFdc->m_nFDC_TotalCount; i++)
		{
// 			if ((CSettingParm::iACTUATOR_EXTENT_LOW <= atof(pFdc->m_cSetSVID[i]) && atof(pFdc->m_cSetSVID[i]) <= CSettingParm::iACTUATOR_EXTENT_UP)	//MOTOR�� VACUUM�� �PC���� ���� �´�.
// 				|| (60000 <= atof(pFdc->m_cSetSVID[i]))	// sms 20150210 SEM�� �ø���
// 				|| (CSettingParm::iUtil_EXTENT_LOW <= atof(pFdc->m_cSetSVID[i]) && atof(pFdc->m_cSetSVID[i]) <= CSettingParm::iUtil_EXTENT_UP))	// 2015-12-23, jhLee, Utility �׸� �߰�
			{
				if (strcmp(pFdc->m_cSetSVTYPE[i], "STRING")!=0) // ���ڿ� �� ( SVTYPE == "STRING" �ΰ�?)
				{
					//kjpark 20170929 FDC ������, ������, ���� ����
					char cSetSV[SVID_MAX] = {0};
					strcpy_s(cSetSV, ini.GetString(pFdc->m_cSetUNIT[i], pFdc->m_cSetSVNAME[i], "0").GetBuffer());
					CString strValue;
					strValue.Format(_T("%s"), cSetSV);
					CTokenizer t(strValue);
					CString strTemp = t.GetNextToken(_T(","));
					strcpy_s(pFdc->m_cSetSV[i], strTemp.GetBuffer());
					strTemp = t.GetNextToken(_T(","));
					strcpy_s(pFdc->m_cSetCV[i], strTemp.GetBuffer());
					strTemp = t.GetNextToken(_T(","));
					strcpy_s(pFdc->m_cSetRV[i], strTemp.GetBuffer());
				}
				else
				{
					strcpy_s(pFdc->m_cSetSV[i], ini.GetString(pFdc->m_cSetUNIT[i], pFdc->m_cSetSVNAME[i], " ").GetBuffer());
				}

			}
		}//end of for
	}//end of if
}


void CViewFDC::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);

	if(nIDEvent == VIEW_FDC_SV_TIMER)
	{
		ViewFdcListSVDip();
	
		SetTimer(nIDEvent, 1000, NULL);	//SettingPara�� ����
	}

}

BOOL CViewFDC::SetFdcSV()
{
	CString settingFile, str;
	CFileFind finder;
	settingFile = CSettingParm::cFDC_SV_PATH;
	BOOL fExist = finder.FindFile(settingFile);
	if (fExist > 0)
	{
		SetTimer(VIEW_FDC_SV_TIMER, 1000, NULL); //SettingPara�� ����
		return TRUE;
	}
	else
	{
		str.Format("There is no FDC SV file(%s)", settingFile);
		GetLogCtrl()->AddBuf(LOG_SYSTEM, str);
		return FALSE;
	}
}


BOOL CViewFDC::ViewFdcListSVDip()
{
	if(g_pData == NULL)
		return FALSE;

	int nCnt=0;
	for(int i=0;i<g_pData->m_FDC.m_nFDC_TotalCount; i++)
	{
		//if (atof(g_pData->m_FDC.m_cSetSV[i]) < atof(g_pData->m_FDC.m_cSetSVMIN[i]) || atof(g_pData->m_FDC.m_cSetSV[i]) > atof(g_pData->m_FDC.m_cSetSVMAX[i]))
		if ((atof(g_pData->m_FDC.m_cSetSV[i]) < atof(g_pData->m_FDC.m_cSetSVMIN[i])) && strcmp(g_pData->m_FDC.m_cSetSVTYPE[i], "STRING")!=0
			|| (atof(g_pData->m_FDC.m_cSetSV[i]) > atof(g_pData->m_FDC.m_cSetSVMAX[i])) && strcmp(g_pData->m_FDC.m_cSetSVTYPE[i], "STRING")!=0)
		{
			//m_lstFdcList.SetItemBkColor(i, 4, RGB(255,0,0), TRUE);	//�Ѱ��� CELL���� ���� �ְ� ������...
			nCnt++;
		}
		// m_ListFDC.SetItemText(i, 4, (CString)g_pData->m_FDC.m_cSetSV[i]);		
		m_ListFDC.SetItemText(i, 3, (CString)g_pData->m_FDC.m_cSetSV[i]);		
		//kjpark 20170929 FDC ������, ������, ���� ����
		m_ListFDC.SetItemText(i, 4, (CString)g_pData->m_FDC.m_cSetCV[i]);		
		m_ListFDC.SetItemText(i, 5, (CString)g_pData->m_FDC.m_cSetRV[i]);		
	}
	g_pData->m_FDC.m_nSetWarring = nCnt;
	return TRUE;
}