#pragma once


// CDlgReportSub ��ȭ �����Դϴ�.

class CDlgReportSub : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReportSub)

public:
	CDlgReportSub(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgReportSub();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_REPORT_SUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	// Dlg �ʱ�ȭ �Լ� - LSH171218
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()


protected:
	// ǥ�ÿ� ����� UI ID ������ ����(CH�� Average����) - LSH171217
	UINT m_nUiIdList[JIG_ID_MAX+1][JIG_CH_MAX+1];
	CString m_strValueList[JIG_ID_MAX+1][JIG_CH_MAX+1];

	// Title ���� ���� ���� - LSH171218
	CString m_strHour;
	CString m_strColumnName;

public:
	// �ʱ�ȭ - LSH171218
	void InitData();
	// ������ �Է� - LSH171217
	void SetProductItem(ProductInfo* pData ,UINT nColumn);
	// ȭ�� ���� - LSH171217
	void UpdateDisplay();

	// Button �Լ� - LSH171218
	void ClickOk();
};
