#pragma once
#include "label.h"
#include "afxwin.h"
#include "afxcmn.h"

#define WS_EX_LAYERED         0x00080000
#define LWA_COLORKEY         0x00000001
#define LWA_ALPHA              0x00000002
typedef BOOL(WINAPI *SLWA) (HWND, COLORREF, BYTE, DWORD);

// CDlgInitial ��ȭ �����Դϴ�.
class CDlgInitial : public CDialog
{
	DECLARE_DYNAMIC(CDlgInitial)

public:
	CDlgInitial(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CDlgInitial();

// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_DIALOG_INITIAL };


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.
	SLWA m_pSetLayerdWindowAttributes;

	int m_iMoveAction;
	int m_nReadyMoveStep;
	BOOL m_bPosMoving;
	DWORD wTimer;
	BOOL m_bMoveTilt;

	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	virtual BOOL OnInitDialog();
	void CheckMotionMoveHome(void);
	CLabel m_lbHomeEnd[9];
	CLabel	 m_lbMoveHome[9];
	afx_msg void OnCmdInitCancel();
};
