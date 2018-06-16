
// Description		
// Usage :: 기존에 사용하던 Sequence Thread base class를 정리함.
//	1. 이를 선조 Class로 하여 하나의 Class를 더 생성한다. (CMainSequence : public XSequence)
//	2. C***View에서 이 Sequence에 대한 Instance를 얻는다.
//---------------------------------------------------------------------------

// Include Files
#include "stdafx.h"
#include "..\MT.h"	// It will be changed if this file is used by any other application.
#include "xsequence.h"

/////////////////////////////////////////////////////////////////////////////
// XSequence

IMPLEMENT_DYNCREATE(XSequence, CWinThread)

// Constructor
XSequence::XSequence()
{
	// CWinThread::m_bAutoDelete : FALSE => not be automatically deleted at thread termination
	m_bAutoDelete	= FALSE;
	m_pOwnerWnd		= NULL;

	m_hDead     = CreateEvent( NULL, TRUE, FALSE, NULL );	// MANUAL Reset, Initial state = non-signal
	m_hKill     = CreateEvent( NULL, TRUE, FALSE, NULL );
	m_hRun		 = CreateEvent( NULL, TRUE, FALSE, NULL );	
}

// Destructor
XSequence::~XSequence()
{
	CloseHandle( m_hDead );
	CloseHandle( m_hKill );
	CloseHandle( m_hRun );
}

BOOL XSequence::InitInstance()
{
	if( !Initialize() )		// Virtual function : It will be initialized at upper class of this
	{
		VERIFY( SetEvent( m_hDead ) );
		Finalize();
		return FALSE;
	}

	while( !EVENT( m_hKill ) )	// while the m_hKill is not signaled, it will be run forever.
	{
		// if m_hRun is signaled, it returns WAIT_OBJECT_0 value
		VERIFY( WaitForSingleObject( m_hRun, INFINITE ) == WAIT_OBJECT_0 );
		Sequence();	// virtural function
	}

	VERIFY( SetEvent( m_hDead ) );
	Finalize();		// Virtual function : It will be finalize at upper class of this

	// InitInstance()의 return 값이 FALSE일 경우 ExitInstance()를 Call하고
	// Thread를 종료하게 된다.
	return FALSE;	
}

int XSequence::ExitInstance()
{
	// TODO:  perform any per-thread cleanup here
	return CWinThread::ExitInstance();
}

BEGIN_MESSAGE_MAP(XSequence, CWinThread)
	//{{AFX_MSG_MAP(XSequence)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// XSequence Class Member functions

//---------------------------------------------------------------------------
// virtual functions.
BOOL XSequence::Initialize()
{
	return TRUE;
}

void XSequence::Finalize()
{
}

BOOL XSequence::Sequence()
{
	return TRUE;
}

//---------------------------------------------------------------------------
// You have to call this function at first.
// 
BOOL XSequence::Start( HWND hWnd )
{
	if( hWnd == NULL ) return FALSE;

	// get the pointer of the parent window
	m_pOwnerWnd = CWnd::FromHandle( hWnd );

	// create the thread
	if( !CWinThread::CreateThread() ) return FALSE;

	return TRUE;
}

//---------------------------------------------------------------------------
// You have to call this function when you want to end the thread.
// 
void XSequence::Kill()
{
	if( m_hThread )
	{
		SuspendThread();
		DWORD dwResume = ResumeThread();		// 자는 Thread는 깨우고 (죽일라고)

		VERIFY( SetEvent( m_hRun ) );
		VERIFY( SetEvent( m_hKill ) );

		// check the exit of while loop
//		VERIFY( WaitForSingleObject( m_hDead, INFINITE ) == WAIT_OBJECT_0 );
//		VERIFY( WaitForSingleObject( m_hThread, INFINITE ) == WAIT_OBJECT_0 );
		// check the exit of while loop	2003/08/26
		// ( thread 가 죽지 않는 경우 일정 시간이 지나면 Timeout을 걸어 시스템 종료를 한다.)
		if( WaitForSingleObject( m_hDead, 2000 ) == WAIT_TIMEOUT )
		{
			TRACE("EqMainSequence time out");
			TerminateThread( m_hThread, -1);
//			AfxMessageBox( "Do you want to exit the Thread ?", MB_ICONSTOP | MB_SYSTEMMODAL | MB_OK );
			Sleep( 200 );
			return;
		}
		WaitForSingleObject( m_hThread, 100 );
		Sleep( 200 );
	}

	DWORD dwExitCode;
	if( !GetExitCodeThread( m_hThread, &dwExitCode ) )
	{
		if( GetLastError() == STILL_ACTIVE )
		{
			TerminateThread( m_hThread, -1 );
			Sleep( 200 );
			if( !GetExitCodeThread( m_hThread, &dwExitCode ) )
				AfxMessageBox( "Windows를 종료하고 시스템 전원을 OFF하여주십시오.", MB_ICONSTOP | MB_SYSTEMMODAL | MB_OK );
		}
	}
}

void XSequence::SetOwner(CWnd *pOwner)
{
	m_pOwnerWnd = pOwner;	
}

//---------------------------------------------------------------------------
// Sequence sleep function
void XSequence::SeqSleepStart( DWORD dwSleepTime )
{
	m_dwSleepTime = dwSleepTime;
	m_dwSleepStartTick = GetTickCount();
}

BOOL XSequence::SeqSleepDone()
{
	if( GetTickCount() - m_dwSleepStartTick >  m_dwSleepTime ) return TRUE;
	return FALSE;
}

BOOL XSequence::SeqSleepDone( DWORD &dwCurSleepTime )
{
	dwCurSleepTime = GetTickCount() - m_dwSleepStartTick;
	if( dwCurSleepTime > m_dwSleepTime ) return TRUE;
	return FALSE;
}

//---------------------------------------------------------------------------
// Sequence control functions
void XSequence::SetSeqRun()
{
	SetEvent( m_hRun );
}

void XSequence::SetSeqStop()
{
	ResetEvent( m_hRun );
}

//---------------------------------------------------------------------------
// Send the message to parent window
// wait for the termination of message process : SendMessage()
void XSequence::Msg(UINT uMsg,  WPARAM wParam, LPARAM lParam, BOOL bWait)
{
	if( m_pOwnerWnd == NULL ) return;
	if( bWait ) m_pOwnerWnd->SendMessage( uMsg, wParam, lParam );
	else m_pOwnerWnd->PostMessage( uMsg, wParam, lParam );
}
