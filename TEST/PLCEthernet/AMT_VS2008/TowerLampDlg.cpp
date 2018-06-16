// TowerLampDlg.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "TowerLampDlg.h"


// CTowerLampDlg dialog
#define 	TOWER_LED_BLINK_CHECK	2

IMPLEMENT_DYNAMIC(CTowerLampDlg, CDialog)

CTowerLampDlg::CTowerLampDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CTowerLampDlg::IDD, pParent)
{
	
}

CTowerLampDlg::~CTowerLampDlg()
{
}

void CTowerLampDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LABEL_STANDBY_RED, m_lblStandbyRed);
	DDX_Control(pDX, IDC_LABEL_STANDBY_YELLOW, m_lblStandbyYellow);
	DDX_Control(pDX, IDC_LABEL_STANDBY_GREEN, m_lblStandbyGreen);
	DDX_Control(pDX, IDC_LABEL_INIT_RED, m_lblInitRed);
	DDX_Control(pDX, IDC_LABEL_INIT_YELLOW, m_lblInitYellow);
	DDX_Control(pDX, IDC_LABEL_INIT_GREEN, m_lblInitGreen);
	DDX_Control(pDX, IDC_LABEL_RUN_RED, m_lblRunRed);
	DDX_Control(pDX, IDC_LABEL_RUN_YELLOW, m_lblRunYellow);
	DDX_Control(pDX, IDC_LABEL_RUN_GREEN, m_lblRunGreen);
	DDX_Control(pDX, IDC_LABEL_STOP_RED, m_lblStopRed);
	DDX_Control(pDX, IDC_LABEL_STOP_YELLOW, m_lblStopYellow);
	DDX_Control(pDX, IDC_LABEL_STOP_GREEN, m_lblStopGreen);
	DDX_Control(pDX, IDC_LABEL_ERROR_RED, m_lblErrorRed);
	DDX_Control(pDX, IDC_LABEL_ERROR_YELLOW, m_lblErrorYellow);
	DDX_Control(pDX, IDC_LABEL_ERROR_GREEN, m_lblErrorGreen);
	DDX_Control(pDX, IDC_LABEL_EMO_RED, m_lblEMORed);
	DDX_Control(pDX, IDC_LABEL_EMO_YELLOW, m_lblEMOYellow);
	DDX_Control(pDX, IDC_LABEL_EMO_GREEN, m_lblEMOGreen);
	DDX_Control(pDX, IDC_LABEL_OPT_RED, m_lblOPTRed);
	DDX_Control(pDX, IDC_LABEL_OPT_YELLOW, m_lblOPTYellow);
	DDX_Control(pDX, IDC_LABEL_OPT_GREEN, m_lblOPTGreen);
}


BEGIN_MESSAGE_MAP(CTowerLampDlg, CDialog)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CTowerLampDlg::OnBnClickedOk)
END_MESSAGE_MAP()


// CTowerLampDlg message handlers

void CTowerLampDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: Add your message handler code here and/or call default
	if(nIDEvent == TOWER_LED_BLINK_CHECK)
	{
		bLampState = !bLampState;
		if(m_lblStandbyRed.get_Caption() == "OFF")
		{
			m_lblStandbyRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblStandbyRed.get_Caption() == "ON")
		{
			m_lblStandbyRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStandbyRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblStandbyRed.put_BackColor(RGB(64,0,0));
			}

		}
		if(m_lblInitRed.get_Caption() == "OFF")
		{
			m_lblInitRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblInitRed.get_Caption() == "ON")
		{
			m_lblInitRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblInitRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblInitRed.put_BackColor(RGB(64,0,0));
			}
		}

		if(m_lblStandbyYellow.get_Caption() == "OFF")
		{
			m_lblStandbyYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblStandbyYellow.get_Caption() == "ON")
		{
			m_lblStandbyYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStandbyYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblStandbyYellow.put_BackColor(RGB(64,64,0));
			}
		}
		if(m_lblStandbyGreen.get_Caption() == "OFF")
		{
			m_lblStandbyGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblStandbyGreen.get_Caption() == "ON")
		{
			m_lblStandbyGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStandbyGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblStandbyGreen.put_BackColor(RGB(0,64,0));
			}
		}

		if(m_lblInitYellow.get_Caption() == "OFF")
		{
			m_lblInitYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblInitYellow.get_Caption() == "ON")
		{
			m_lblInitYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblInitYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblInitYellow.put_BackColor(RGB(64,64,0));
			}
		}
		if(m_lblInitGreen.get_Caption() == "OFF")
		{
			m_lblInitGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblInitGreen.get_Caption() == "ON")
		{
			m_lblInitGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblInitGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblInitGreen.put_BackColor(RGB(0,64,0));
			}
		}

		if(m_lblRunRed.get_Caption() == "OFF")
		{
			m_lblRunRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblRunRed.get_Caption() == "ON")
		{
			m_lblRunRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblRunRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblRunRed.put_BackColor(RGB(64,0,0));

			}
		}
		if(m_lblRunYellow.get_Caption() == "OFF")
		{
			m_lblRunYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblRunYellow.get_Caption() == "ON")
		{
			m_lblRunYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblRunYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblRunYellow.put_BackColor(RGB(64,64,0));
			}
		}
		if(m_lblRunGreen.get_Caption() == "OFF")
		{
			m_lblRunGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblRunGreen.get_Caption() == "ON")
		{
			m_lblRunGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblRunGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblRunGreen.put_BackColor(RGB(0,64,0));
			}
		}
		if(m_lblStopRed.get_Caption() == "OFF")
		{
			m_lblStopRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblStopRed.get_Caption() == "ON")
		{
			m_lblStopRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStopRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblStopRed.put_BackColor(RGB(64,0,0));
			}
		}

		if(m_lblStopYellow.get_Caption() == "OFF")
		{
			m_lblStopYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblStopYellow.get_Caption() == "ON")
		{
			m_lblStopYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStopYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblStopYellow.put_BackColor(RGB(64,64,0));
			}
		}

		if(m_lblStopGreen.get_Caption() == "OFF")
		{
			m_lblStopGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblStopGreen.get_Caption() == "ON")
		{
			m_lblStopGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblStopGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblStopGreen.put_BackColor(RGB(0,64,0));
			}
		}

		if(m_lblErrorRed.get_Caption() == "OFF")
		{
			m_lblErrorRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblErrorRed.get_Caption() == "ON")
		{
			m_lblErrorRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblErrorRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblErrorRed.put_BackColor(RGB(64,0,0));
			}
		}

		if(m_lblErrorYellow.get_Caption() == "OFF")
		{
			m_lblErrorYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblErrorYellow.get_Caption() == "ON")
		{
			m_lblErrorYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblErrorYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblErrorYellow.put_BackColor(RGB(64,64,0));
			}
		}
		if(m_lblErrorGreen.get_Caption() == "OFF")
		{
			m_lblErrorGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblErrorGreen.get_Caption() == "ON")
		{
			m_lblErrorGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblErrorGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblErrorGreen.put_BackColor(RGB(0,64,0));
			}
		}

		if(m_lblEMORed.get_Caption() == "OFF")
		{
			m_lblEMORed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblEMORed.get_Caption() == "ON")
		{
			m_lblEMORed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblEMORed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblEMORed.put_BackColor(RGB(64,0,0));
			}
		}

		if(m_lblEMOYellow.get_Caption() == "OFF")
		{
			m_lblEMOYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblEMOYellow.get_Caption() == "ON")
		{
			m_lblEMOYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblEMOYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblEMOYellow.put_BackColor(RGB(64,64,0));
			}
		}
		if(m_lblEMOGreen.get_Caption() == "OFF")
		{
			m_lblEMOGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblEMOGreen.get_Caption() == "ON")
		{
			m_lblEMOGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblEMOGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblEMOGreen.put_BackColor(RGB(0,64,0));
			}
		}

		if(m_lblOPTRed.get_Caption() == "OFF")
		{
			m_lblOPTRed.put_BackColor(RGB(64,0,0));
		}
		else if(m_lblOPTRed.get_Caption() == "ON")
		{
			m_lblOPTRed.put_BackColor(RGB(255,0,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblOPTRed.put_BackColor(RGB(255,0,0));
			}
			else
			{
				m_lblOPTRed.put_BackColor(RGB(64,0,0));
			}
		}
		if(m_lblOPTYellow.get_Caption() == "OFF")
		{
			m_lblOPTYellow.put_BackColor(RGB(64,64,0));
		}
		else if(m_lblOPTYellow.get_Caption() == "ON")
		{
			m_lblOPTYellow.put_BackColor(RGB(255,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblOPTYellow.put_BackColor(RGB(255,255,0));
			}
			else
			{
				m_lblOPTYellow.put_BackColor(RGB(64,64,0));
			}
		}

		if(m_lblOPTGreen.get_Caption() == "OFF")
		{
			m_lblOPTGreen.put_BackColor(RGB(0,64,0));
		}
		else if(m_lblOPTGreen.get_Caption() == "ON")
		{
			m_lblOPTGreen.put_BackColor(RGB(0,255,0));
		}
		else
		{
			if(bLampState == TRUE)
			{
				m_lblOPTGreen.put_BackColor(RGB(0,255,0));
			}
			else
			{
				m_lblOPTGreen.put_BackColor(RGB(0,64,0));
			}
		}

	}

}

BEGIN_EVENTSINK_MAP(CTowerLampDlg, CDialog)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STANDBY_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelStandbyRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STANDBY_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelStandbyYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STANDBY_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelStandbyGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_INIT_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelInitRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_INIT_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelInitYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_INIT_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelInitGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_RUN_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelRunRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_RUN_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelRunYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_RUN_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelRunGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STOP_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelStopRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STOP_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelStopYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_STOP_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelStopGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_ERROR_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelErrorRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_ERROR_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelErrorYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_ERROR_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelErrorGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_EMO_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelEmoRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_EMO_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelEmoYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_EMO_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelEmoGreen, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_OPT_RED, DISPID_CLICK, CTowerLampDlg::ClickLabelOptRed, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_OPT_YELLOW, DISPID_CLICK, CTowerLampDlg::ClickLabelOptYellow, VTS_NONE)
	ON_EVENT(CTowerLampDlg, IDC_LABEL_OPT_GREEN, DISPID_CLICK, CTowerLampDlg::ClickLabelOptGreen, VTS_NONE)
END_EVENTSINK_MAP()


void CTowerLampDlg::ClickLabelStandbyRed()
{
	// TODO: Add your message handler code here
	 if(m_lblStandbyRed.get_Caption() == "Blink")
		 m_lblStandbyRed.put_Caption("OFF");
	 else if(m_lblStandbyRed.get_Caption() == "OFF")
		 m_lblStandbyRed.put_Caption("ON");
	 else
		 m_lblStandbyRed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelStandbyYellow()
{
	// TODO: Add your message handler code here
	if(m_lblStandbyYellow.get_Caption() == "Blink")
		m_lblStandbyYellow.put_Caption("OFF");
	else if(m_lblStandbyYellow.get_Caption() == "OFF")
		m_lblStandbyYellow.put_Caption("ON");
	else
		m_lblStandbyYellow.put_Caption("Blink");

}

void CTowerLampDlg::ClickLabelStandbyGreen()
{
	// TODO: Add your message handler code here
	if(m_lblStandbyGreen.get_Caption() == "Blink")
		m_lblStandbyGreen.put_Caption("OFF");
	else if(m_lblStandbyGreen.get_Caption() == "OFF")
		m_lblStandbyGreen.put_Caption("ON");
	else
		m_lblStandbyGreen.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelInitRed()
{
	// TODO: Add your message handler code here
	if(m_lblInitRed.get_Caption() == "Blink")
	{
		m_lblInitRed.put_Caption("OFF");
	}
	else if(m_lblInitRed.get_Caption() == "OFF")
	{
		m_lblInitRed.put_Caption("ON");
	}
	else
	{
		m_lblInitRed.put_Caption("Blink");
	}

}

void CTowerLampDlg::ClickLabelInitYellow()
{
	if(m_lblInitYellow.get_Caption() == "Blink")
		m_lblInitYellow.put_Caption("OFF");
	else if(m_lblInitYellow.get_Caption() == "OFF")
		m_lblInitYellow.put_Caption("ON");
	else
		m_lblInitYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelInitGreen()
{
	// TODO: Add your message handler code here
	if(m_lblInitGreen.get_Caption() == "Blink")
		m_lblInitGreen.put_Caption("OFF");
	else if(m_lblInitGreen.get_Caption() == "OFF")
		m_lblInitGreen.put_Caption("ON");
	else
		m_lblInitGreen.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelRunRed()
{
	// TODO: Add your message handler code here
	if(m_lblRunRed.get_Caption() == "Blink")
		m_lblRunRed.put_Caption("OFF");
	else if(m_lblRunRed.get_Caption() == "OFF")
		m_lblRunRed.put_Caption("ON");
	else
		m_lblRunRed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelRunYellow()
{
	// TODO: Add your message handler code here
	if(m_lblRunYellow.get_Caption() == "Blink")
		m_lblRunYellow.put_Caption("OFF");
	else if(m_lblRunYellow.get_Caption() == "OFF")
		m_lblRunYellow.put_Caption("ON");
	else
		m_lblRunYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelRunGreen()
{
	if(m_lblRunGreen.get_Caption() == "Blink")
		m_lblRunGreen.put_Caption("OFF");
	else if(m_lblRunGreen.get_Caption() == "OFF")
		m_lblRunGreen.put_Caption("ON");
	else
		m_lblRunGreen.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelStopRed()
{
	if(m_lblStopRed.get_Caption() == "Blink")
		m_lblStopRed.put_Caption("OFF");
	else if(m_lblStopRed.get_Caption() == "OFF")
		m_lblStopRed.put_Caption("ON");
	else
		m_lblStopRed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelStopYellow()
{
	if(m_lblStopYellow.get_Caption() == "Blink")
		m_lblStopYellow.put_Caption("OFF");
	else if(m_lblStopYellow.get_Caption() == "OFF")
		m_lblStopYellow.put_Caption("ON");
	else
		m_lblStopYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelStopGreen()
{
	if(m_lblStopGreen.get_Caption() == "Blink")
		m_lblStopGreen.put_Caption("OFF");
	else if(m_lblStopGreen.get_Caption() == "OFF")
		m_lblStopGreen.put_Caption("ON");
	else
		m_lblStopGreen.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelErrorRed()
{
	if(m_lblErrorRed.get_Caption() == "Blink")
		m_lblErrorRed.put_Caption("OFF");
	else if(m_lblErrorRed.get_Caption() == "OFF")
		m_lblErrorRed.put_Caption("ON");
	else
		m_lblErrorRed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelErrorYellow()
{
	if(m_lblErrorYellow.get_Caption() == "Blink")
		m_lblErrorYellow.put_Caption("OFF");
	else if(m_lblErrorYellow.get_Caption() == "OFF")
		m_lblErrorYellow.put_Caption("ON");
	else
		m_lblErrorYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelErrorGreen()
{
	if(m_lblErrorGreen.get_Caption() == "Blink")
		m_lblErrorGreen.put_Caption("OFF");
	else if(m_lblErrorGreen.get_Caption() == "OFF")
		m_lblErrorGreen.put_Caption("ON");
	else
		m_lblErrorGreen.put_Caption("Blink");
}


void CTowerLampDlg::ClickLabelEmoRed()
{
	if(m_lblEMORed.get_Caption() == "Blink")
		m_lblEMORed.put_Caption("OFF");
	else if(m_lblEMORed.get_Caption() == "OFF")
		m_lblEMORed.put_Caption("ON");
	else
		m_lblEMORed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelEmoYellow()
{
	if(m_lblEMOYellow.get_Caption() == "Blink")
		m_lblEMOYellow.put_Caption("OFF");
	else if(m_lblEMOYellow.get_Caption() == "OFF")
		m_lblEMOYellow.put_Caption("ON");
	else
		m_lblEMOYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelEmoGreen()
{
	if(m_lblEMOGreen.get_Caption() == "Blink")
		m_lblEMOGreen.put_Caption("OFF");
	else if(m_lblEMOGreen.get_Caption() == "OFF")
		m_lblEMOGreen.put_Caption("ON");
	else
		m_lblEMOGreen.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelOptRed()
{
	if(m_lblOPTRed.get_Caption() == "Blink")
		m_lblOPTRed.put_Caption("OFF");
	else if(m_lblOPTRed.get_Caption() == "OFF")
		m_lblOPTRed.put_Caption("ON");
	else
		m_lblOPTRed.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelOptYellow()
{
	if(m_lblOPTYellow.get_Caption() == "Blink")
		m_lblOPTYellow.put_Caption("OFF");
	else if(m_lblOPTYellow.get_Caption() == "OFF")
		m_lblOPTYellow.put_Caption("ON");
	else
		m_lblOPTYellow.put_Caption("Blink");
}

void CTowerLampDlg::ClickLabelOptGreen()
{
	if(m_lblOPTGreen.get_Caption() == "Blink")
		m_lblOPTGreen.put_Caption("OFF");
	else if(m_lblOPTGreen.get_Caption() == "OFF")
		m_lblOPTGreen.put_Caption("ON");
	else
		m_lblOPTGreen.put_Caption("Blink");
}

void CTowerLampDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	KillTimer(TOWER_LED_BLINK_CHECK);

	// Super userÀÏ ¶§¸¸ Save [5/15/2009 OSC]
	if(GetDoc()->m_bSuperLogin)
	{
		CIni ini;
		strCaption[0][0] = 	    m_lblStandbyRed.get_Caption();
		if(strCaption[0][0] == "ON")
			nTowerLampSet[0][0] = ON_MODE;
		else if(strCaption[0][0] == "Blink")
			nTowerLampSet[0][0] = BLINK_MODE;
		else
			nTowerLampSet[0][0] = OFF_MODE;
		strCaption[0][1] =  	m_lblStandbyYellow.get_Caption();
		if(strCaption[0][1] == "ON")
			nTowerLampSet[0][1] = ON_MODE;
		else if(strCaption[0][1] == "Blink")
			nTowerLampSet[0][1] = BLINK_MODE;
		else
			nTowerLampSet[0][1] = OFF_MODE;
		strCaption[0][2] = 	    m_lblStandbyGreen.get_Caption();
		if(strCaption[0][2] == "ON")
			nTowerLampSet[0][2] = ON_MODE;
		else if(strCaption[0][2] == "Blink")
			nTowerLampSet[0][2] = BLINK_MODE;
		else
			nTowerLampSet[0][2] = OFF_MODE;
		strCaption[1][0] = 	    m_lblInitRed.get_Caption();
		if(strCaption[1][0] == "ON")
			nTowerLampSet[1][0] = ON_MODE;
		else if(strCaption[1][0] == "Blink")
			nTowerLampSet[1][0] = BLINK_MODE;
		else
			nTowerLampSet[1][0] = OFF_MODE;
		strCaption[1][1] =  	m_lblInitYellow.get_Caption();
		if(strCaption[1][1] == "ON")
			nTowerLampSet[1][1] = ON_MODE;
		else if(strCaption[1][1] == "Blink")
			nTowerLampSet[1][1] = BLINK_MODE;
		else
			nTowerLampSet[1][1] = OFF_MODE;
		strCaption[1][2] = 	    m_lblInitGreen.get_Caption();
		if(strCaption[1][2] == "ON")
			nTowerLampSet[1][2] = ON_MODE;
		else if(strCaption[1][2] == "Blink")
			nTowerLampSet[1][2] = BLINK_MODE;
		else
			nTowerLampSet[1][2] = OFF_MODE;
		strCaption[2][0] = 	    m_lblRunRed.get_Caption();
		if(strCaption[2][0] == "ON")
			nTowerLampSet[2][0] = ON_MODE;
		else if(strCaption[2][0] == "Blink")
			nTowerLampSet[2][0] = BLINK_MODE;
		else
			nTowerLampSet[2][0] = OFF_MODE;
		strCaption[2][1] = 		m_lblRunYellow.get_Caption();
		if(strCaption[2][1] == "ON")
			nTowerLampSet[2][1] = ON_MODE;
		else if(strCaption[2][1] == "Blink")
			nTowerLampSet[2][1] = BLINK_MODE;
		else
			nTowerLampSet[2][1] = OFF_MODE;
		strCaption[2][2] = 		m_lblRunGreen.get_Caption();
		if(strCaption[2][2] == "ON")
			nTowerLampSet[2][2] = ON_MODE;
		else if(strCaption[2][2] == "Blink")
			nTowerLampSet[2][2] = BLINK_MODE;
		else
			nTowerLampSet[2][2] = OFF_MODE;
		strCaption[3][0] = 		m_lblStopRed.get_Caption();
		if(strCaption[3][0] == "ON")
			nTowerLampSet[3][0] = ON_MODE;
		else if(strCaption[3][0] == "Blink")
			nTowerLampSet[3][0] = BLINK_MODE;
		else
			nTowerLampSet[3][0] = OFF_MODE;
		strCaption[3][1] = 		m_lblStopYellow.get_Caption();
		if(strCaption[3][1] == "ON")
			nTowerLampSet[3][1] = ON_MODE;
		else if(strCaption[3][1] == "Blink")
			nTowerLampSet[3][1] = BLINK_MODE;
		else
			nTowerLampSet[3][1] = OFF_MODE;
		strCaption[3][2] = 		m_lblStopGreen.get_Caption();
		if(strCaption[3][2] == "ON")
			nTowerLampSet[3][2] = ON_MODE;
		else if(strCaption[3][2] == "Blink")
			nTowerLampSet[3][2] = BLINK_MODE;
		else
			nTowerLampSet[3][2] = OFF_MODE;
		strCaption[4][0] = 		m_lblErrorRed.get_Caption();
		if(strCaption[4][0] == "ON")
			nTowerLampSet[4][0] = ON_MODE;
		else if(strCaption[4][0] == "Blink")
			nTowerLampSet[4][0] = BLINK_MODE;
		else
			nTowerLampSet[4][0] = OFF_MODE;
		strCaption[4][1] = 		m_lblErrorYellow.get_Caption();
		if(strCaption[4][1] == "ON")
			nTowerLampSet[4][1] = ON_MODE;
		else if(strCaption[4][1] == "Blink")
			nTowerLampSet[4][1] = BLINK_MODE;
		else
			nTowerLampSet[4][1] = OFF_MODE;
		strCaption[4][2] = 		m_lblErrorGreen.get_Caption();
		if(strCaption[4][2] == "ON")
			nTowerLampSet[4][2] = ON_MODE;
		else if(strCaption[4][2] == "Blink")
			nTowerLampSet[4][2] = BLINK_MODE;
		else
			nTowerLampSet[4][2] = OFF_MODE;
		strCaption[5][0] = 		m_lblEMORed.get_Caption();
		if(strCaption[5][0] == "ON")
			nTowerLampSet[5][0] = ON_MODE;
		else if(strCaption[5][0] == "Blink")
			nTowerLampSet[5][0] = BLINK_MODE;
		else
			nTowerLampSet[5][0] = OFF_MODE;
		strCaption[5][1] =		m_lblEMOYellow.get_Caption();
		if(strCaption[5][1] == "ON")
			nTowerLampSet[5][1] = ON_MODE;
		else if(strCaption[5][1] == "Blink")
			nTowerLampSet[5][1] = BLINK_MODE;
		else
			nTowerLampSet[5][1] = OFF_MODE;
		strCaption[5][2] =		m_lblEMOGreen.get_Caption();
		if(strCaption[5][2] == "ON")
			nTowerLampSet[5][2] = ON_MODE;
		else if(strCaption[5][2] == "Blink")
			nTowerLampSet[5][2] = BLINK_MODE;
		else
			nTowerLampSet[5][2] = OFF_MODE;
		strCaption[6][0] =		m_lblOPTRed.get_Caption();
		if(strCaption[6][0] == "ON")
			nTowerLampSet[6][0] = ON_MODE;
		else if(strCaption[6][0] == "Blink")
			nTowerLampSet[6][0] = BLINK_MODE;
		else
			nTowerLampSet[6][0] = OFF_MODE;
		strCaption[6][1] =		m_lblOPTYellow.get_Caption();
		if(strCaption[6][1] == "ON")
			nTowerLampSet[6][1] = ON_MODE;
		else if(strCaption[6][1] == "Blink")
			nTowerLampSet[6][1] = BLINK_MODE;
		else
			nTowerLampSet[6][1] = OFF_MODE;
		strCaption[6][2] =		m_lblOPTGreen.get_Caption();
		if(strCaption[6][2] == "ON")
			nTowerLampSet[6][2] = ON_MODE;
		else if(strCaption[6][2] == "Blink")
			nTowerLampSet[6][2] = BLINK_MODE;
		else
			nTowerLampSet[6][2] = OFF_MODE;

		GetDoc()->strTowerLamp[STATUS_READY].Format("R_%d, Y_%d, G_%d", nTowerLampSet[0][0], nTowerLampSet[0][1], nTowerLampSet[0][2]);
		GetDoc()->strTowerLamp[STATUS_INIT].Format("R_%d, Y_%d, G_%d", nTowerLampSet[1][0], nTowerLampSet[1][1], nTowerLampSet[1][2]);
		GetDoc()->strTowerLamp[STATUS_RUN].Format("R_%d, Y_%d, G_%d", nTowerLampSet[2][0], nTowerLampSet[2][1], nTowerLampSet[2][2]);
		GetDoc()->strTowerLamp[STATUS_STOP].Format("R_%d, Y_%d, G_%d", nTowerLampSet[3][0], nTowerLampSet[3][1], nTowerLampSet[3][2]);
		GetDoc()->strTowerLamp[STATUS_ERROR].Format("R_%d, Y_%d, G_%d", nTowerLampSet[4][0], nTowerLampSet[4][1], nTowerLampSet[4][2]);
		GetDoc()->strTowerLamp[STATUS_EMO].Format("R_%d, Y_%d, G_%d", nTowerLampSet[5][0], nTowerLampSet[5][1], nTowerLampSet[5][2]);
		GetDoc()->strTowerLamp[STATUS_OPCALL].Format("R_%d, Y_%d, G_%d", nTowerLampSet[6][0], nTowerLampSet[6][1], nTowerLampSet[6][2]);

		GetDoc()->SaveTowerLamp();
	}
	OnOK();
}

BOOL CTowerLampDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  Add extra initialization here
	
	LampSetting();

	SetTimer(TOWER_LED_BLINK_CHECK, 500, NULL);

	

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CTowerLampDlg::LampSetting(void)
{
	nTowerLampSet[0][0] = (int)(GetDoc()->strTowerLamp[STATUS_READY].GetAt(2) - '0');
	nTowerLampSet[0][1] = (int)(GetDoc()->strTowerLamp[STATUS_READY].GetAt(7) - '0');
	nTowerLampSet[0][2] = (int)(GetDoc()->strTowerLamp[STATUS_READY].GetAt(12) - '0');
	nTowerLampSet[1][0] = (int)(GetDoc()->strTowerLamp[STATUS_INIT].GetAt(2) - '0');
	nTowerLampSet[1][1] = (int)(GetDoc()->strTowerLamp[STATUS_INIT].GetAt(7) - '0');
	nTowerLampSet[1][2] = (int)(GetDoc()->strTowerLamp[STATUS_INIT].GetAt(12) - '0');
	nTowerLampSet[2][0] = (int)(GetDoc()->strTowerLamp[STATUS_RUN].GetAt(2) - '0');
	nTowerLampSet[2][1] = (int)(GetDoc()->strTowerLamp[STATUS_RUN].GetAt(7) - '0');
	nTowerLampSet[2][2] = (int)(GetDoc()->strTowerLamp[STATUS_RUN].GetAt(12) - '0');
	nTowerLampSet[3][0] = (int)(GetDoc()->strTowerLamp[STATUS_STOP].GetAt(2) - '0');
	nTowerLampSet[3][1] = (int)(GetDoc()->strTowerLamp[STATUS_STOP].GetAt(7) - '0');
	nTowerLampSet[3][2] = (int)(GetDoc()->strTowerLamp[STATUS_STOP].GetAt(12) - '0');
	nTowerLampSet[4][0] = (int)(GetDoc()->strTowerLamp[STATUS_ERROR].GetAt(2) - '0');
	nTowerLampSet[4][1] = (int)(GetDoc()->strTowerLamp[STATUS_ERROR].GetAt(7) - '0');
	nTowerLampSet[4][2] = (int)(GetDoc()->strTowerLamp[STATUS_ERROR].GetAt(12) - '0');
	nTowerLampSet[5][0] = (int)(GetDoc()->strTowerLamp[STATUS_EMO].GetAt(2) - '0');
	nTowerLampSet[5][1] = (int)(GetDoc()->strTowerLamp[STATUS_EMO].GetAt(7) - '0');
	nTowerLampSet[5][2] = (int)(GetDoc()->strTowerLamp[STATUS_EMO].GetAt(12) - '0');
	nTowerLampSet[6][0] = (int)(GetDoc()->strTowerLamp[STATUS_OPCALL].GetAt(2) - '0');
	nTowerLampSet[6][1] = (int)(GetDoc()->strTowerLamp[STATUS_OPCALL].GetAt(7) - '0');
	nTowerLampSet[6][2] = (int)(GetDoc()->strTowerLamp[STATUS_OPCALL].GetAt(12) - '0');

	for(int i=0;i<7;i++)
	{
		for(int j=0;j<3;j++)
		{
			if(nTowerLampSet[i][j] == 0)
				strCaption[i][j] = "ON";
			else if(nTowerLampSet[i][j] == 1)
				strCaption[i][j] = "Blink";
			else 
				strCaption[i][j] = "OFF";
		}
	}

	m_lblStandbyRed.put_Caption(strCaption[0][0]);
	m_lblStandbyYellow.put_Caption(strCaption[0][1]);
	m_lblStandbyGreen.put_Caption(strCaption[0][2]);

	m_lblInitRed.put_Caption(strCaption[1][0]);
	m_lblInitYellow.put_Caption(strCaption[1][1]);
	m_lblInitGreen.put_Caption(strCaption[1][2]);

	m_lblRunRed.put_Caption(strCaption[2][0]);
	m_lblRunYellow.put_Caption(strCaption[2][1]);
	m_lblRunGreen.put_Caption(strCaption[2][2]);

	m_lblStopRed.put_Caption(strCaption[3][0]);
	m_lblStopYellow.put_Caption(strCaption[3][1]);
	m_lblStopGreen.put_Caption(strCaption[3][2]);

	m_lblErrorRed.put_Caption(strCaption[4][0]);
	m_lblErrorYellow.put_Caption(strCaption[4][1]);
	m_lblErrorGreen.put_Caption(strCaption[4][2]);

	m_lblEMORed.put_Caption(strCaption[5][0]);
	m_lblEMOYellow.put_Caption(strCaption[5][1]);
	m_lblEMOGreen.put_Caption(strCaption[5][2]);

	m_lblOPTRed.put_Caption(strCaption[6][0]);
	m_lblOPTYellow.put_Caption(strCaption[6][1]);
	m_lblOPTGreen.put_Caption(strCaption[6][2]);
}
