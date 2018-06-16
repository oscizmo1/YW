#pragma once


// CDlgUserClass dialog

class CDlgUserClass : public CDialog
{
	DECLARE_DYNAMIC(CDlgUserClass)

public:
	CDlgUserClass(CWnd* pParent = NULL);   // standard constructor


	CString		m_strUserClass;
	int			m_nUserClass;


	virtual ~CDlgUserClass();

// Dialog Data
	enum { IDD = IDD_DIALOG_USER_CLASS };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()


	void OnClickLabelEngineer();
	void OnClickLabelOperator();
};
