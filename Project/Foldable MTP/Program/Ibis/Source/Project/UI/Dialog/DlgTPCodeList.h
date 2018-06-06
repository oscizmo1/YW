#pragma once


// CDlgTPCodeList ��ȭ �����Դϴ�.

class CDlgTPCodeList : public CDialog
{
	DECLARE_DYNAMIC(CDlgTPCodeList)

public:
	CDlgTPCodeList(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgTPCodeList();
	virtual BOOL OnInitDialog();
	void SetLossTime(CSystemTime lossStart, CSystemTime lossEnd, CSystemTimeSpan lossSpan);
	void ChangeLanguage();
	CString m_strTPCode;
	LANGUAGE m_Language;
	CSystemTime m_LossStart;
	CSystemTime m_LossEnd;
	CSystemTimeSpan m_LossSpan;
	BOOL m_bAlarmHappend;
	TLosstimeList		m_rLosstimeList;

	// ��Ҹ� ���� â�� ���� ���ϰ� �ϴ� ��� (�ݵ�� ���� �Է� �ʿ�)
	BOOL	m_bCanNotCancel;

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_LOSSTIME_TP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()
	
	void ClickBtnxLang();
	void ClickGxbtnLosstimeTpcode1();
	void ClickGxbtnLosstimeTpcode2();
	BOOL LoadLosstimeItemList();
	void ClickGxbtnLosstimeConfirm();
	void ClickGxbtnLosstimeTpcode3();
	void ClickGxbtnLosstimeTpcode4();
	void ClickGxbtnLosstimeTpcode5();
	void ClickGxbtnLosstimeTpcode6();
	void ClickGxbtnLosstimeTpcode7();
	void ClickGxbtnLosstimeTpcode8();
	void ClickGxbtnLosstimeTpcode9();
	void ClickGxbtnLosstimeTpcode10();
	void ClickGxbtnLosstimeTpcode11();
	void ClickGxbtnLosstimeCancel();
};
