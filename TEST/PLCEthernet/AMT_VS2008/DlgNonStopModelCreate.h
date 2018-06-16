#pragma once
#include "lbl_blu.h"
#include "prev_port.h"


// CDlgNonStopModelCreate dialog

class CDlgNonStopModelCreate : public CDialog
{
	DECLARE_DYNAMIC(CDlgNonStopModelCreate)

public:
	CDlgNonStopModelCreate(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgNonStopModelCreate();

// Dialog Data
	enum { IDD = IDD_DIALOG_NON_STOP_MODEL_CREATE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	LPCTSTR m_szFPCType[2];
	LPCTSTR m_szPinNum[3];
	LPCTSTR m_szContactNum[3];
	LPCTSTR m_szContactor[4];
	LPCTSTR m_szComment;

	int m_nFPCType, m_nPinNum, m_nContactNum, m_nContactor;

	void ChangeFPCType(BOOL bNext);
	void ChangePinNum(BOOL bNext);
	void ChangeContactNum(BOOL bNext);
	void ChangeContactor(BOOL bNext);
	void InitDialogText();

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedBtnCreate();
	afx_msg void OnBnClickedBtnCancel1();
	DECLARE_EVENTSINK_MAP()
	void OnClickPrevFpcType();
	void OnClickPrevPinNum();
	void OnClickPrevContactNum();
	void OnClickPrevContactor();
	void OnClickNextFpcType();
	void OnClickNextPinNum();
	void OnClickNextContactNum();
	void OnClickNextContactor();
	CLbl_blu m_lblFPCType;
	CLbl_blu m_lblPinNum;
	CLbl_blu m_lblContactNum;
	CLbl_blu m_lblContactor;
	CLbl_blu m_lblComment;
	void OnMouseDownPrevFpcType(short Button, short Shift, long X, long Y);
	void OnMouseDownPrevPinNum(short Button, short Shift, long X, long Y);
	void OnMouseDownPrevContactNum(short Button, short Shift, long X, long Y);
	void OnMouseDownPrevContactor(short Button, short Shift, long X, long Y);
	void OnMouseDownNextFpcType(short Button, short Shift, long X, long Y);
	void OnMouseDownNextPinNum(short Button, short Shift, long X, long Y);
	void OnMouseDownNextContactNum(short Button, short Shift, long X, long Y);
	void OnMouseDownNextContactor(short Button, short Shift, long X, long Y);
	CPrev_port m_lblPrevFPCType;
	CPrev_port m_lblPrevPinNum;
	CPrev_port m_lblPrevContactNum;
	CPrev_port m_lblPrevContactor;
	CPrev_port m_lblNextFPCType;
	CPrev_port m_lblNextPinNum;
	CPrev_port m_lblNextContactNum;
	CPrev_port m_lblNextContactor;
	void OnMouseUpPrevFpcType(short Button, short Shift, long X, long Y);
	void OnMouseUpPrevPinNum(short Button, short Shift, long X, long Y);
	void OnMouseUpPrevContactNum(short Button, short Shift, long X, long Y);
	void OnMouseUpPrevContactor(short Button, short Shift, long X, long Y);
	void OnMouseUpNextFpcType(short Button, short Shift, long X, long Y);
	void OnMouseUpNextPinNum(short Button, short Shift, long X, long Y);
	void OnMouseUpNextContactNum(short Button, short Shift, long X, long Y);
	void OnMouseUpNextContactor(short Button, short Shift, long X, long Y);
	void OnClickIlabelxComment();
};
