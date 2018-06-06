#pragma once


// CDlgTPCodeList 대화 상자입니다.

class CDlgTPCodeList : public CDialog
{
	DECLARE_DYNAMIC(CDlgTPCodeList)

public:
	CDlgTPCodeList(CWnd* pParent = NULL);   // 표준 생성자입니다.
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

	// 취소를 눌러 창을 닫지 못하게 하는 기능 (반드시 정보 입력 필요)
	BOOL	m_bCanNotCancel;

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_LOSSTIME_TP };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

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
