#pragma once


// cContactInfo 대화 상자입니다.

class cContactInfo : public CDialog
{
	DECLARE_DYNAMIC(cContactInfo)

public:
	cContactInfo(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~cContactInfo();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_CONTACT_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
