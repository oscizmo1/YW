#pragma once
#include "UI\GridCtrl\GridCtrl.h"

// CDlgOtherDefect 대화 상자입니다.
#define DEFECT_GROUP_TIMER 100

class CDlgOtherDefect : public CDialog
{
	DECLARE_DYNAMIC(CDlgOtherDefect)

public:
	CDlgOtherDefect(JIG_CH channelNO, CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgOtherDefect();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_DLG_OTHER_DEFECT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
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
