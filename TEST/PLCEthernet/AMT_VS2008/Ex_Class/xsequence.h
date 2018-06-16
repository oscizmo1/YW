#if !defined(AFX_XSEQUENCE_H__88947901_C50F_11D4_BAEA_00600834616E__INCLUDED_)
#define AFX_XSEQUENCE_H__88947901_C50F_11D4_BAEA_00600834616E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// xsequence.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// XSequence thread

class XSequence : public CWinThread
{
	DECLARE_DYNCREATE(XSequence)
protected:
	XSequence();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:
	void SetSeqStop();
	void SetSeqRun();
	void SetSeqPause();
	void SetSeqPauseRelease();
	virtual BOOL Initialize();
	virtual void Finalize();
	BOOL Start( HWND hWnd=NULL );
	void Kill();
	void SetOwner(CWnd *pOwner);
	void Msg(UINT uMsg, WPARAM wParam, LPARAM lParam=0, BOOL bWait=TRUE);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(XSequence)
	public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~XSequence();
	CWnd* m_pOwnerWnd;
	BOOL SeqSleepDone();
	BOOL SeqSleepDone( DWORD & dwCurSleepTime );
	void SeqSleepStart( DWORD dwSleepTime );
	virtual BOOL Sequence();

	HANDLE m_hDead;
	HANDLE m_hKill;
	HANDLE m_hRun;

	DWORD m_dwSleepStartTick;	// sequence sleep timer variable
	DWORD m_dwSleepTime;		// sequence sleep time

	// Generated message map functions
	//{{AFX_MSG(XSequence)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

// COMMUNICATION MACRO
#define EVENT(X)				( WaitForSingleObject( X, 0 ) == WAIT_OBJECT_0 )
#define MSG(M,A,B)				if( m_hWnd ) PostMessage( m_hWnd, M, (UINT)A, (LONG)B );

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XSEQUENCE_H__88947901_C50F_11D4_BAEA_00600834616E__INCLUDED_)
