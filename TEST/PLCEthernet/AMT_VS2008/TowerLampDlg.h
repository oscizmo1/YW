#pragma once
#include "label.h"


// CTowerLampDlg dialog

class CTowerLampDlg : public CDialog
{
	DECLARE_DYNAMIC(CTowerLampDlg)

public:
	CTowerLampDlg(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTowerLampDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG_TOWERLAMP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_EVENTSINK_MAP()
	void ClickLabel1();
	BOOL bLampState;
	CString strCaption[7][3];
	CString strIniName[7][3];
	int		nTowerLampSet[7][3];

	CLabel m_lblStandbyRed;
	CLabel m_lblStandbyYellow;
	CLabel m_lblStandbyGreen;
	CLabel m_lblInitRed;
	CLabel m_lblInitYellow;
	CLabel m_lblInitGreen;

	void ClickLabelStandbyRed();
	void ClickLabelStandbyYellow();
	void ClickLabelStandbyGreen();
	void ClickLabelInitRed();
	void ClickLabelInitYellow();
	void ClickLabelInitGreen();
	CLabel m_lblRunRed;
	CLabel m_lblRunYellow;
	CLabel m_lblRunGreen;
	CLabel m_lblStopRed;
	CLabel m_lblStopYellow;
	CLabel m_lblStopGreen;
	CLabel m_lblErrorRed;
	CLabel m_lblErrorYellow;
	CLabel m_lblErrorGreen;
	CLabel m_lblEMORed;
	CLabel m_lblEMOYellow;
	CLabel m_lblEMOGreen;
	CLabel m_lblOPTRed;
	CLabel m_lblOPTYellow;
	CLabel m_lblOPTGreen;
	void ClickLabelRunRed();
	void ClickLabelRunYellow();
	void ClickLabelRunGreen();
	void ClickLabelStopRed();
	void ClickLabelStopYellow();
	void ClickLabelStopGreen();
	void ClickLabelErrorRed();
	void ClickLabelErrorYellow();
	void ClickLabelErrorGreen();
	void ClickLabelEmoRed();
	void ClickLabelEmoYellow();
	void ClickLabelEmoGreen();
	void ClickLabelOptRed();
	void ClickLabelOptYellow();
	void ClickLabelOptGreen();
	afx_msg void OnBnClickedOk();
	virtual BOOL OnInitDialog();
	void LampSetting(void);
};
