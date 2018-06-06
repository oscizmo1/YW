#pragma once


// CDlgReportSub 대화 상자입니다.

class CDlgReportSub : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgReportSub)

public:
	CDlgReportSub(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgReportSub();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_REPORT_SUB };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	// Dlg 초기화 함수 - LSH171218
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
public:
	DECLARE_EVENTSINK_MAP()


protected:
	// 표시에 사용할 UI ID 정보를 저장(CH과 Average포함) - LSH171217
	UINT m_nUiIdList[JIG_ID_MAX+1][JIG_CH_MAX+1];
	CString m_strValueList[JIG_ID_MAX+1][JIG_CH_MAX+1];

	// Title 정보 저장 변수 - LSH171218
	CString m_strHour;
	CString m_strColumnName;

public:
	// 초기화 - LSH171218
	void InitData();
	// 데이터 입력 - LSH171217
	void SetProductItem(ProductInfo* pData ,UINT nColumn);
	// 화면 갱신 - LSH171217
	void UpdateDisplay();

	// Button 함수 - LSH171218
	void ClickOk();
};
