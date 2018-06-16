#pragma once


// CMCRDialog ��ȭ �����Դϴ�.

class CMCRDialog : public CDialog
{
	DECLARE_DYNAMIC(CMCRDialog)

public:
	CMCRDialog(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	CMCRDialog(CString* pStr, BOOL* pBool)
	{
		// �Է¹��� Handle�� Null�� �ƴ� ��쿡�� Handle�� �����Ѵ� [4/2/2018 LSH]
		if( pStr != NULL )
			pStr = &m_strMCRID;

		if( pBool != NULL )
			pBool = &m_bReadDone;
	}
	~CMCRDialog();

	// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_MCR};

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();

protected:
	// Edit box�� Cell ID�� ���� ���� [4/2/2018 LSH]
	CString m_strMCRID;
	// Cell ID�� �о����� Ȯ���ϴ� ���� [4/2/2018 LSH]
	BOOL m_bReadDone;

public:
	void MkCreate(CWnd *pParent);
	void MkPos(CWnd *pWnd);
	void MkDistroy();
	void MkShow();
	void MkHide();

	// ���� ��ȯ �Լ� [4/2/2018 LSH]
	CString GetCellId() { m_bReadDone = FALSE; return m_strMCRID; };
	BOOL GetReadDone() { return m_bReadDone; };

	BOOL m_bClickCellID;

	afx_msg void OnBnClickedOk();
};

