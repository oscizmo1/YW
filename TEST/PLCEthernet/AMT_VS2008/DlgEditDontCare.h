#pragma once
#include "afxwin.h"

enum
{
	Thickness3,
	Thickness5,
	Thickness10,
	Thickness20,
	ThicknessMax
};
enum
{
	MaskState,
	UnmaskState
};
// CDlgEditDontCare dialog

class CDlgEditDontCare : public CDialog
{
	DECLARE_DYNAMIC(CDlgEditDontCare)

public:
	CDlgEditDontCare(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgEditDontCare();

// Dialog Data
	enum { IDD = IDD_DIALOG_EDIT_DONTCARE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	void SetModelIndex(int nModelIndex) { m_nModelIndex = nModelIndex; }

private:
	CMIL71* m_pVision;
	int m_nModelIndex;
	CRect m_rtImage;
	CPoint m_OldPoint;
	BOOL m_bLButtonDown;

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	virtual BOOL OnInitDialog();
	int m_nThickness;
	int m_nMaskState;
	CStatic m_stcDontcare;
	afx_msg void OnClose();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnBnClickedRadioThick1();
	afx_msg void OnBnClickedRadioThick2();
	afx_msg void OnBnClickedRadioThick3();
	afx_msg void OnBnClickedRadioThick4();
	afx_msg void OnBnClickedRadioMask();
	afx_msg void OnBnClickedRadioUnmask();
	afx_msg void OnBnClickedBtnReset();
};
