#pragma once


// cContactInfo ��ȭ �����Դϴ�.

class cContactInfo : public CDialog
{
	DECLARE_DYNAMIC(cContactInfo)

public:
	cContactInfo(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~cContactInfo();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_CONTACT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()

	cCONTACT_DATA* m_pData;
public:
	virtual BOOL OnInitDialog();

	void SetDialogItem(cCONTACT_DATA* pData);
	void SetDialogItemTotal(cCONTACT_DATA* pData);
	void SetUIFirstContact();
	void SetUIFinalContact();

	CString szDay;
	CString szDayCount;
	CString szTitle;

	BOOL m_bContactFirst;
};
