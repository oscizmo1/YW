#pragma once
#include "UI\GridCtrl\GridCtrl.h"

// CDlgOtherDefect ��ȭ �����Դϴ�.
#define DEFECT_GROUP_TIMER 100

class CDlgOtherDefect : public CDialog
{
	DECLARE_DYNAMIC(CDlgOtherDefect)

public:
	CDlgOtherDefect(JIG_CH channelNO, CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgOtherDefect();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DLG_OTHER_DEFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()

protected:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
public:
	void OnGridGroupSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	void OnGridElementSelChange(NMHDR *pNotifyStruct, LRESULT *pResult);
	
	void InitGrid();
	void InitGridElement();
	void UpdateGroup();
	void UpdateGridElement();
	void UpdateGridElementRefersh();

	void HighlightCurGroup( BOOL bRefrash );
	void HighlightCurElemet( BOOL bRefrash );
	int m_nLastGroupGridRow;
	int m_nLastElementGridRow;
	void CurGroupChange( int nIndex);
	CGridCtrl m_GridGroup;
	CGridCtrl m_GridElement;
	JIG_CH m_eChannelNO;


	afx_msg void OnClickedButtonDefectCancle();
	afx_msg void OnClickedButtonDefectConfirm();
	afx_msg void OnBnClickedButtonGood();
};
