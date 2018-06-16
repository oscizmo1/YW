#pragma once


// CMCRDialog 대화 상자입니다.

class CMCRDialog : public CDialog
{
	DECLARE_DYNAMIC(CMCRDialog)

public:
	CMCRDialog(CWnd* pParent = NULL);   // 표준 생성자입니다.
	CMCRDialog(CString* pStr, BOOL* pBool)
	{
		// 입력받은 Handle이 Null이 아닐 경우에만 Handle을 연결한다 [4/2/2018 LSH]
		if( pStr != NULL )
			pStr = &m_strMCRID;

		if( pBool != NULL )
			pBool = &m_bReadDone;
	}
	~CMCRDialog();

	// 대화 상자 데이터입니다.
	enum { IDD = IDD_DIALOG_MCR};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	// Edit box와 Cell ID가 저장 변수 [4/2/2018 LSH]
	CString m_strMCRID;
	// Cell ID를 읽었는지 확인하는 변수 [4/2/2018 LSH]
	BOOL m_bReadDone;

public:
	void MkCreate(CWnd *pParent);
	void MkPos(CWnd *pWnd);
	void MkDistroy();
	void MkShow();
	void MkHide();

	// 변수 반환 함수 [4/2/2018 LSH]
	CString GetCellId() { m_bReadDone = FALSE; return m_strMCRID; };
	BOOL GetReadDone() { return m_bReadDone; };

	BOOL m_bClickCellID;

	afx_msg void OnBnClickedOk();
};

