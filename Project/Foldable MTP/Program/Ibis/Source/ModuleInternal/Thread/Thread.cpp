#include "stdafx.h"
#include "Thread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif
CThreadInitializer::CThreadInitializer()
{
	for(int i = 0; i < THREAD_INDEX_MAX; i++)
	{
		m_pThr[i] = FALSE;
		m_hThr[i] = FALSE;
		m_bThrExist[i] = TRUE;
	}
}
CThreadInitializer::~CThreadInitializer()
{
}

BOOL CThreadInitializer::CreateThread( THREAD_INDEX index, AFX_THREADPROC pThr, LPVOID pParam )
{
	m_pThr[index] = ::AfxBeginThread(pThr, pParam, THREAD_PRIORITY_ABOVE_NORMAL, 0, CREATE_SUSPENDED);
	if(m_pThr[index]==NULL)
		return FALSE;

	m_bThrExist[index] = FALSE;
	m_hThr[index] = m_pThr[index]->m_hThread;
	m_pThr[index]->m_bAutoDelete = FALSE;
	m_pThr[index]->ResumeThread();

	return TRUE;
}

BOOL CThreadInitializer::ReleaseThread( THREAD_INDEX index )
{
	if(m_hThr[index])
	{
		m_bThrExist[index] = TRUE;

		DWORD dwExitCode = 0;
		::GetExitCodeThread(m_hThr[index], &dwExitCode);
		if(dwExitCode != 0)
		{
			if(::WaitForSingleObject(m_hThr[index], 10000) == WAIT_TIMEOUT)
			{
				if(::TerminateThread(m_hThr[index], 0))
				{
				}
			}
		}
		delete m_pThr[index];
	}

	return TRUE;
}
