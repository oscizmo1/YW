#include "stdafx.h"
#include "CQueue.h"

void CQueue::Clear()
{
	m_iHead = m_iTail = 0;
	memset(buff,0,BUFF_SIZE);
}

CQueue::CQueue()
{
	Clear();
}

int CQueue::GetSize()
{
	return (m_iHead - m_iTail + BUFF_SIZE) % BUFF_SIZE;
}

BOOL CQueue::PutByte(BYTE b)
{
	if(GetSize() == (BUFF_SIZE-1)) return FALSE;
	buff[m_iHead++] = b;
	m_iHead %= BUFF_SIZE;
	return TRUE;
}

BOOL CQueue::GetByte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	*pb = buff[m_iTail++];
	m_iTail %= BUFF_SIZE;
	return TRUE;
}

BOOL CQueue::GetWord(short *pb)
{
	if(GetSize() < 2) return FALSE;
	
	if( m_iTail+1 >= BUFF_SIZE)
	{
		BYTE TempBuff[2];

		memcpy(TempBuff, &buff[m_iTail], 1);
		memcpy(&TempBuff[1], &buff[0], 1);
		memcpy(pb, TempBuff, 2);
	}
	else
		memcpy(pb, &buff[m_iTail], 2);
	
	m_iTail+=2;
	m_iTail %= BUFF_SIZE;

	*pb = ((*pb >> 8) & 0x00ff) | ((*pb << 8) & 0xFF00);
	
	return TRUE;
}

BOOL CQueue::Get8Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 8) return FALSE;

	memcpy(pb, &buff[m_iTail], 8);
//	*pb = buff[m_iTail];

	m_iTail+=8;
	m_iTail %= BUFF_SIZE;
	return TRUE;
}

BOOL CQueue::Get12Byte(BYTE *pb)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < 12) return FALSE;

	memcpy(pb, &buff[m_iTail], 12);
	//	*pb = buff[m_iTail];

	m_iTail+=12;
	m_iTail %= BUFF_SIZE;
	return TRUE;
}


int CQueue::GetData(BYTE* pByte, int nSize)
{
	if(GetSize() == 0) return FALSE;
	if(GetSize() < nSize) return FALSE;

	memcpy(pByte, &buff[m_iTail], nSize);
	//	*pb = buff[m_iTail];

	m_iTail+=nSize;
	m_iTail %= BUFF_SIZE;

	return 0;
}
void CQueue::PutData(BYTE* pByte, int nSize)
{
}
