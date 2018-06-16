// DlgTeachTabPattern.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MT.h"
#include "DlgTeachTabPattern.h"
#include "DlgNumPad.h"

// CDlgTeachTabPattern 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgTeachTabPattern, CDialog)

CDlgTeachTabPattern::CDlgTeachTabPattern(CWnd* pParent /*=NULL*/)
	: CDialog(CDlgTeachTabPattern::IDD, pParent)
{

}

CDlgTeachTabPattern::~CDlgTeachTabPattern()
{
}

void CDlgTeachTabPattern::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_MSFLEXGRID_OFFSET, m_cGridOffset);
	DDX_Control(pDX, IDC_LABEL_AUTO_LC_ONLINE3, m_lbGrabPosOffset);
	DDX_Control(pDX, IDC_LABEL_AUTO_LC_ONLINE4, m_lbPinOffset);
}


BEGIN_MESSAGE_MAP(CDlgTeachTabPattern, CDialog)
	ON_WM_SHOWWINDOW()
	ON_BN_CLICKED(IDC_BTN_TEACH_SAVE, &CDlgTeachTabPattern::OnTeachOffsetDataSave)
END_MESSAGE_MAP()


// CDlgTeachTabPattern 메시지 처리기입니다.

BOOL CDlgTeachTabPattern::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.

	m_cGridOffset.put_Rows(3);
	m_cGridOffset.put_Cols(5);

	for( int i =0 ; i < 3 ; i ++ )
	{
		for( int j=0; j < 4 ; j ++ )
		{
			m_cGridOffset.put_Row(i) ;
			m_cGridOffset.put_Col(j) ; 
			m_cGridOffset.put_CellAlignment(4);
		}
	}

	m_cGridOffset.put_ColWidth(0 , 550);
	m_cGridOffset.put_ColWidth(1 , 875);
	m_cGridOffset.put_ColWidth(2 , 875);
	m_cGridOffset.put_ColWidth(3 , 875);
	m_cGridOffset.put_ColWidth(4 , 875);

	m_cGridOffset.put_RowHeight( 0 , 500);
	m_cGridOffset.put_RowHeight( 1 , 500);
	m_cGridOffset.put_RowHeight( 2 , 500);

	m_cGridOffset.put_TextMatrix( 0 , 1 , "CAM0");
	m_cGridOffset.put_TextMatrix( 0 , 2 , "CAM1");
	m_cGridOffset.put_TextMatrix( 0 , 3 , "CAM2");
	m_cGridOffset.put_TextMatrix( 0 , 4 , "CAM3");
	m_cGridOffset.put_TextMatrix( 1 , 0 , "X 축");
	m_cGridOffset.put_TextMatrix( 2 , 0 , "Y 축");


	SetOffsetData();
	SetGridOffsetData();

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

void CDlgTeachTabPattern::SetOffsetData(void)
{
	for( int i=0 ; i < MAX_CAM_NUM ; i ++)
	{
		m_dTV_FPCY[i] = GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[i];
		m_dTV_PCBX[i] = GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[i];
	}

	m_dTV_StageReady = GetDoc()->m_pAmtData->m_CurModelInfo.TV_StageReady;
}

void CDlgTeachTabPattern::SetGridOffsetData(void)
{
	CString strTmpData;

	for( int i = 0 ; i <  MAX_CAM_NUM; i++ )
	{
		strTmpData.Format("%.3f" , m_dTV_PCBX[i] );
		m_cGridOffset.put_TextMatrix( 1 , i+1 , strTmpData );

		strTmpData.Format("%.3f" , m_dTV_FPCY[i] );
		m_cGridOffset.put_TextMatrix( 2 , i+1 , strTmpData );
	}

	strTmpData.Format("%.3f" , m_dTV_StageReady);
	m_lbGrabPosOffset.SetWindowText(strTmpData);
}

void CDlgTeachTabPattern::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);

	if( bShow )
	{
		SetOffsetData();
		SetGridOffsetData();
		// Super user가 아니면 수정 불가 [5/15/2009 OSC]
		this->EnableWindow(GetDoc()->m_bEngLogin);
	}
}
BEGIN_EVENTSINK_MAP(CDlgTeachTabPattern, CDialog)
	ON_EVENT(CDlgTeachTabPattern, IDC_LABEL_AUTO_LC_ONLINE3, DISPID_CLICK, CDlgTeachTabPattern::OnSetLabelReadyOffset, VTS_NONE)
	ON_EVENT(CDlgTeachTabPattern, IDC_LABEL_AUTO_LC_ONLINE4, DISPID_CLICK, CDlgTeachTabPattern::ClickPinZaxisOffset, VTS_NONE)
	ON_EVENT(CDlgTeachTabPattern, IDC_MSFLEXGRID_OFFSET, DISPID_CLICK, CDlgTeachTabPattern::ClickMsflexgridOffset, VTS_NONE)
END_EVENTSINK_MAP()

void CDlgTeachTabPattern::OnTeachOffsetDataSave()
{	
	for( int i=0 ; i < MAX_CAM_NUM ; i ++)
	{
		GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[i] = m_dTV_FPCY[i];
		GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[i] = m_dTV_PCBX[i];
	}

	GetDoc()->m_pAmtData->m_CurModelInfo.TV_StageReady = m_dTV_StageReady;
	GetDoc()->SaveModelFile();
}

void CDlgTeachTabPattern::OnSetLabelReadyOffset()
{
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.

	CDlgNumPad dlg;
	if( dlg.DoModal() == IDOK )
	{
		CString strtmp;
		strtmp = dlg.m_strEditText ;

		m_lbGrabPosOffset.put_Caption(strtmp);
		m_dTV_StageReady = atof( strtmp);
	}
	
}

void CDlgTeachTabPattern::ClickPinZaxisOffset()
{
	CDlgNumPad dlg;
	if( dlg.DoModal() == IDOK )
	{
		CString strtmp;
		strtmp = dlg.m_strEditText ;
		m_lbPinOffset.put_Caption(strtmp);

		GetDoc()->m_pAmtData->m_CurModelInfo.dPinZaixPos =atof( strtmp); 

	}
}

void CDlgTeachTabPattern::ClickMsflexgridOffset()
{
	int Row , Col ;

	Row = m_cGridOffset.get_MouseRow();
	Col = m_cGridOffset.get_MouseCol();

	CDlgNumPad dlg;
	if( dlg.DoModal() == IDOK )
	{
		m_cGridOffset.put_TextMatrix( Row  , Col  , dlg.m_strEditText );
		if(Row == 1)
			m_dTV_PCBX[Col-1] = atof( dlg.m_strEditText );
		else if(Row == 2)
			m_dTV_FPCY[Col-1] = atof( dlg.m_strEditText );
	}
}
