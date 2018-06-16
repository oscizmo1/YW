// frmIO.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "frmIO.h"

// CFormIO

IMPLEMENT_DYNCREATE(CFormIO, CFormView)

CFormIO::CFormIO()
	: CFormView(CFormIO::IDD)
{
	for(int i = 0; i < 32; i++)
	{
		bIOState[i] = FALSE;
		bIOStateOld[i] = FALSE;
	}
}

CFormIO::~CFormIO()
{
}

void CFormIO::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLDXGRID_INPUT1, m_InPutGrid1);
	DDX_Control(pDX, IDC_MSFLEXGRID_INPUT2, m_InPutGrid2);
	DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT1, m_OutPutGrid1);
	DDX_Control(pDX, IDC_MSFLEXGRID_OUTPUT2, m_OutPutGrid2);
}

BEGIN_MESSAGE_MAP(CFormIO, CFormView)
	ON_WM_TIMER()
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CFormIO diagnostics

#ifdef _DEBUG
void CFormIO::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormIO::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormIO message handlers

BOOL CFormIO::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext)
{
	// TODO: Add your specialized code here and/or call the base class

	return CFormView::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
}

void CFormIO::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();

	// TODO: Add your specialized code here and/or call the base class
	m_InPutGrid1.put_Cols(2);
	m_InPutGrid1.put_Rows(17);
	m_InPutGrid2.put_Cols(2);
	m_InPutGrid2.put_Rows(17);
	m_OutPutGrid1.put_Cols(2);
	m_OutPutGrid1.put_Rows(17);
	m_OutPutGrid2.put_Cols(2);
	m_OutPutGrid2.put_Rows(17);
	m_InPutGrid1.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);
	m_InPutGrid2.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);
	m_OutPutGrid1.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);
	m_OutPutGrid2.put_ColAlignment(0, CMsflexgrid_pos::flexAlignCenterCenter);

	char buff[32];
	CString strTemp;
	m_InPutGrid1.put_ColWidth(0,400);
	m_InPutGrid1.put_ColWidth(1,3500);
	m_InPutGrid2.put_ColWidth(0,400);
	m_InPutGrid2.put_ColWidth(1,3500);
	for(int i= 0; i<0x10;i++)
	{
		strTemp.Format("INPUT_%02X",i);
		GetPrivateProfileString("MMT_INPUT_NAME", strTemp,"",buff,33,IODEFINE_PATH);
		m_InPutGrid1.put_TextMatrix(i+1,0,strTemp.Right(2));
		m_InPutGrid1.put_TextMatrix(i+1, 1, buff);
		m_InPutGrid1.put_RowHeight(i+1, 500);
	}
	for(int i= 0; i<0x10;i++)
	{
		strTemp.Format("INPUT_%02X",i+0x10);
		GetPrivateProfileString("MMT_INPUT_NAME", strTemp,"",buff,33,IODEFINE_PATH);
		m_InPutGrid2.put_TextMatrix(i+1,0,strTemp.Right(2));
		m_InPutGrid2.put_TextMatrix(i+1, 1, buff);
		m_InPutGrid2.put_RowHeight(i+1, 500);
	}
	m_OutPutGrid1.put_ColWidth(0,400);
	m_OutPutGrid1.put_ColWidth(1,3500);
	m_OutPutGrid2.put_ColWidth(0,400);
	m_OutPutGrid2.put_ColWidth(1,3500);
	for(int i= 0; i<0x10;i++)
	{
		strTemp.Format("OUTPUT_%02X",i);
		GetPrivateProfileString("MMT_OUTPUT_NAME", strTemp,"",buff,33,IODEFINE_PATH);
		m_OutPutGrid1.put_TextMatrix(i+1,0,strTemp.Right(2));
		m_OutPutGrid1.put_TextMatrix(i+1, 1, buff);
		m_OutPutGrid1.put_RowHeight(i+1, 500);
	}
	for(int i= 0; i<0x10;i++)
	{
		strTemp.Format("OUTPUT_%02X",i+0x10);
		GetPrivateProfileString("MMT_OUTPUT_NAME", strTemp,"",buff,33,IODEFINE_PATH);
		m_OutPutGrid2.put_TextMatrix(i+1,0,strTemp.Right(2));
		m_OutPutGrid2.put_TextMatrix(i+1, 1, buff);
		m_OutPutGrid2.put_RowHeight(i+1, 500);
	}

}

void CFormIO::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == IO_RUN_CHECK)
	{
		for(int i=0;i<0x10;i++)
		{
			if(DioInput(i)==TRUE)
			{
				if(bIOState[i] == FALSE)
				{
					m_InPutGrid1.put_Col(1);
					m_InPutGrid1.put_Row(i+1);
					m_InPutGrid1.put_CellBackColor(RGB(0,255,0));
					bIOState[i] = !bIOState[i];
				}
			}
			else
			{
				if(bIOState[i])
				{
					m_InPutGrid1.put_Col(1);
					m_InPutGrid1.put_Row(i+1);
					m_InPutGrid1.put_CellBackColor(RGB(255,255,255));
					bIOState[i] = !bIOState[i];
				}
			}
		}
		for(int i = 0x10; i<0x20;i++)
		{
			if(DioInput(i)==TRUE)
			{
				if(bIOState[i] == FALSE)
				{
					m_InPutGrid2.put_Col(1);
					m_InPutGrid2.put_Row(i+1-0x10);
					m_InPutGrid2.put_CellBackColor(RGB(0,255,0));
					bIOState[i] = !bIOState[i];
				}
			}
			else
			{
				if(bIOState[i])
				{
					m_InPutGrid2.put_Col(1);
					m_InPutGrid2.put_Row(i+1-0x10);
					m_InPutGrid2.put_CellBackColor(RGB(255,255,255));
					bIOState[i] = !bIOState[i];
				}
			}
		}
		for(int i = 0; i<0x10;i++)
		{
			if(DioOutIn(i) == TRUE)
			{
				if(bIOStateOld[i] == FALSE)
				{
					m_OutPutGrid1.put_Col(1);
					m_OutPutGrid1.put_Row(i+1);
					m_OutPutGrid1.put_CellBackColor(RGB(0,255,0));
					bIOStateOld[i] = !bIOStateOld[i];
				}

			}
			else
			{
				if(bIOStateOld[i])
				{
					m_OutPutGrid1.put_Col(1);
					m_OutPutGrid1.put_Row(i+1);
					m_OutPutGrid1.put_CellBackColor(RGB(255,255,255));
					bIOStateOld[i] = !bIOStateOld[i];
				}
			}
		}

		for(int i = 0x10; i<0x20;i++)
		{
			if(DioOutIn(i) == TRUE)
			{
				if(bIOStateOld[i] == FALSE)
				{
					m_OutPutGrid2.put_Col(1);
					m_OutPutGrid2.put_Row(i+1-0x10);
					m_OutPutGrid2.put_CellBackColor(RGB(0,255,0));
					bIOStateOld[i] = !bIOStateOld[i];
				}

			}
			else
			{
				if(bIOStateOld[i])
				{
					m_OutPutGrid2.put_Col(1);
					m_OutPutGrid2.put_Row(i+1-0x10);
					m_OutPutGrid2.put_CellBackColor(RGB(255,255,255));
					bIOStateOld[i] = !bIOStateOld[i];
				}
			}
		}
	}

	CFormView::OnTimer(nIDEvent);
}

void CFormIO::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CFormView::OnShowWindow(bShow, nStatus);

	// TODO: Add your message handler code here
	if(bShow ==TRUE)
		SetTimer(IO_RUN_CHECK, 200, NULL);
	else
		KillTimer(IO_RUN_CHECK);

}
BEGIN_EVENTSINK_MAP(CFormIO, CFormView)
	ON_EVENT(CFormIO, IDC_MSFLEXGRID_OUTPUT1, DISPID_CLICK, CFormIO::ClickMsflexgridOutput1, VTS_NONE)
	ON_EVENT(CFormIO, IDC_MSFLEXGRID_OUTPUT2, DISPID_CLICK, CFormIO::ClickMsflexgridOutput2, VTS_NONE)
END_EVENTSINK_MAP()

void CFormIO::ClickMsflexgridOutput1()
{
	// TODO: Add your message handler code here
	int nCol = m_OutPutGrid1.get_MouseCol();
	int nRow = m_OutPutGrid1.get_MouseRow();

	if((GetDoc()->m_bSuperLogin == TRUE) || (GetDoc()->m_bEngLogin == TRUE))
	{
		if(GetApp()->IsRunStatus() == FALSE)
		{
			if(DioOutIn(nRow-1) == FALSE)
			{
				DioOutput(nRow-1,TRUE);
				m_OutPutGrid1.put_Col(1);
				m_OutPutGrid1.put_Row(nRow);
				m_OutPutGrid1.put_CellBackColor(RGB(0,255,0));
				bIOStateOld[nRow-1] = !bIOStateOld[nRow-1];
			}
			else
			{
				DioOutput(nRow-1,FALSE);
				m_OutPutGrid1.put_Col(1);
				m_OutPutGrid1.put_Row(nRow);
				m_OutPutGrid1.put_CellBackColor(RGB(255,255,255));
				bIOStateOld[nRow-1] = !bIOStateOld[nRow-1];
			}
		}
		else
			AfxMessageBox("The Machine is the behavior. ");
	}
	else
		AfxMessageBox("Please LogIn.");

}

void CFormIO::ClickMsflexgridOutput2()
{
	// TODO: Add your message handler code here
	int nCol = m_OutPutGrid2.get_MouseCol();
	int nRow = m_OutPutGrid2.get_MouseRow();

	if((GetDoc()->m_bSuperLogin == TRUE) || (GetDoc()->m_bEngLogin == TRUE))
	{
		if(GetApp()->IsRunStatus() == FALSE)
		{
			if(DioOutIn(nRow-1+0x10) == FALSE)
			{
				DioOutput(nRow-1+0x10,TRUE);
				m_OutPutGrid2.put_Col(1);
				m_OutPutGrid2.put_Row(nRow);
				m_OutPutGrid2.put_CellBackColor(RGB(0,255,0));
				bIOStateOld[nRow-1+0x10] = !bIOStateOld[nRow-1+0x10];
			}
			else
			{
				DioOutput(nRow-1+0x10,FALSE);
				m_OutPutGrid2.put_Col(1);
				m_OutPutGrid2.put_Row(nRow);
				m_OutPutGrid2.put_CellBackColor(RGB(255,255,255));
				bIOStateOld[nRow-1+0x10] = !bIOStateOld[nRow-1+0x10];
			}
		}
		else
			AfxMessageBox("The Machine is the behavior. ");
	}
	else
		AfxMessageBox("Please LogIn.");
}
