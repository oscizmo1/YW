#include "StdAfx.h"
#include "Queue.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CQueue::CQueue(void)
{
	ClearQueue();
}


CQueue::~CQueue(void)
{
}

void CQueue::ClearQueue()
{
	memset(m_bufQueue, NULL, QUEUE_BUFFER_SIZE);
	m_nStart = 0;
	m_nEnd = 0;
}

void CQueue::Put( char *buf, int nSize )
{
	if(QUEUE_BUFFER_SIZE <= nSize)
	{
		// ����� �� ���� ũ��� Reset [11/15/2016 OSC]
		ClearQueue();
		return;
	}

	// ���� ���� �� �̹� ������ ������ ħ���ع����� Reset  [11/15/2016 OSC]
	if( (m_nStart + nSize) >= (m_nEnd + QUEUE_BUFFER_SIZE) )
	{
		ClearQueue();
	}

	if( (m_nStart + nSize) > QUEUE_BUFFER_SIZE )
	{
		int nCopySize = (QUEUE_BUFFER_SIZE - m_nStart);
		memcpy(&m_bufQueue[m_nStart], buf, nCopySize);
		nSize -= nCopySize;
		buf += nCopySize;
		m_nStart = 0;
	}

	memcpy(&m_bufQueue[m_nStart], buf, nSize);
	m_nStart += nSize;
	m_nStart %= QUEUE_BUFFER_SIZE;
}

BOOL CQueue::Get( char *buf, int nSize )
{
	if(nSize >= QUEUE_BUFFER_SIZE)
		return FALSE;
	
	if(nSize > GetSize())
		return FALSE;

	if( (m_nEnd + nSize) > QUEUE_BUFFER_SIZE)
	{
		int nCopySize = (QUEUE_BUFFER_SIZE - m_nEnd);
		memcpy(buf, &m_bufQueue[m_nEnd], nCopySize);
		nSize -= nCopySize;
		buf += nCopySize;
		m_nEnd = 0;
	}

	memcpy(buf, &m_bufQueue[m_nEnd], nSize);
	m_nEnd += nSize;
	m_nEnd %= QUEUE_BUFFER_SIZE;

	return TRUE;
}

int CQueue::GetSize()
{
	return (m_nStart - m_nEnd + QUEUE_BUFFER_SIZE) % QUEUE_BUFFER_SIZE;
// 	int nTotal = m_nStart - m_nEnd;
// 	return nTotal > 0 ? nTotal:((nTotal+ QUEUE_BUFFER_SIZE) % QUEUE_BUFFER_SIZE);
}
