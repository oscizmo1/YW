// CommThread.cpp: implementation of the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CommThread.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//--- Ŭ���� ������
CCommThread::CCommThread()
{
	m_bConnected = FALSE;
}

CCommThread::~CCommThread()
{

}


BOOL CCommThread::OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity )
{

	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	// overlapped structure ���� �ʱ�ȭ.
	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	//--> Read �̺�Ʈ ������ ����..
	if ( !(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)) ) 	
	{
		return FALSE;
	}


	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	//--> Write �̺�Ʈ ������ ����..
	if (! (m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL)))
	{
		return FALSE;
	}

	//--> ��Ʈ�� ����..
	m_sPortName = strPortName;

	//--> ��������...RS 232 ��Ʈ ����..
	m_hComm = CreateFile( m_sPortName, 
		GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
		NULL);


	//--> ��Ʈ ���⿡ �����ϸ�..
	if (m_hComm == (HANDLE) -1)
	{
		//AfxMessageBox("fail Port ofen");
		return FALSE;
	}


	//===== ��Ʈ ���� ����. =====

	// EV_RXCHAR event ����...�����Ͱ� ������.. ���� �̺�Ʈ�� �߻��ϰԲ�..
	//SetCommMask( m_hComm, EV_RXCHAR);	
	SetCommMask( m_hComm, ETX);	

	// InQueue, OutQueue ũ�� ����.
	SetupComm( m_hComm, BUFF_SIZE, BUFF_SIZE);	

	// ��Ʈ ����.
	PurgeComm( m_hComm,					
		PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout ����.
	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 0;

	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_9600 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;

	SetCommTimeouts( m_hComm, &timeouts);

	// dcb ����.... ��Ʈ�� ��������..��� ����ϴ� DCB ����ü�� ����..
	dcb.DCBlength = sizeof(DCB);

	//--> ���� ������ �� �߿���..
	GetCommState( m_hComm, &dcb);	

	//--> ���巹��Ʈ�� �ٲٰ�..
	dcb.BaudRate = dwBaud;


	//--> Data 8 Bit
	dcb.ByteSize = byData;

	//--> Noparity
	dcb.Parity = byParity;

	//--> 1 Stop Bit
	dcb.StopBits = byStop;


	//--> ��Ʈ�� ��..����������.. �����غ���..
	if( !SetCommState( m_hComm, &dcb) )	
	{
		return FALSE;
	}


	// ��Ʈ ���� ������ ����.
	m_bConnected = TRUE;

	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread( NULL, 0, 
		(LPTHREAD_START_ROUTINE)ThreadWatchComm, 
		this, 0, &dwThreadID);

	//--> ������ ������ �����ϸ�..
	if (! m_hThreadWatchComm)
	{
		//--> ���� ��Ʈ�� �ݰ�..
		ClosePort();
		return FALSE;
	}
	return TRUE;
}
	

BOOL CCommThread::InitCommPort(int nPortNo, 
							   DWORD dwBaud, 
							   BYTE byData, 
							   BYTE byStop, 
							   BYTE byParity)
{
	if (m_bConnected) {
		ClosePort();
	}

	// Local ����.
	COMMTIMEOUTS	timeouts;
	DCB				dcb;
	DWORD			dwThreadID;

	m_bConnected = FALSE;

	m_osRead.Offset = 0;
	m_osRead.OffsetHigh = 0;
	if (!(m_osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {
		return FALSE;
	}

	m_osWrite.Offset = 0;
	m_osWrite.OffsetHigh = 0;
	if (!(m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL))) {
		CloseHandle(m_osRead.hEvent);
		return FALSE;
	}
	
	m_sPortName.Format(_T("\\\\.\\COM%d"), nPortNo);

	m_hComm = CreateFile(m_sPortName, 
						 GENERIC_READ | GENERIC_WRITE, 0, NULL,
						 OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_OVERLAPPED, 
						 NULL);
	if (m_hComm == (HANDLE) - 1) {
		return FALSE;
	}

	// EV_RXCHAR event ����
	SetCommMask(m_hComm, EV_RXCHAR);	
	//SetCommMask( m_hComm, ETX);	

	// InQueue, OutQueue ũ�� ����.
	SetupComm(m_hComm, 4096, 4096);	

	// ��Ʈ ����.
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);

	// timeout ����.
//	timeouts.ReadIntervalTimeout = 0xFFFFFFFF;
	timeouts.ReadIntervalTimeout = 20;
	timeouts.WriteTotalTimeoutMultiplier = 2*CBR_38400 / dwBaud;
	timeouts.WriteTotalTimeoutConstant = 0;
	
	SetCommTimeouts(m_hComm, &timeouts);

	
	// dcb ����.... ��Ʈ�� ��������..��� ����ϴ� DCB ����ü�� ����..
	dcb.DCBlength = sizeof(DCB);

	//--> ���� ������ �� �߿���..
	GetCommState( m_hComm, &dcb);
	dcb.BaudRate = dwBaud;		//Baud Rate
	dcb.ByteSize = byData;		//Data Bit
	dcb.StopBits = byStop;		//Stop Bit
	dcb.Parity   = byParity;	//Parity
	dcb.fDtrControl = DTR_CONTROL_DISABLE; // flow control none
	dcb.EvtChar = CR;

	//--> ��Ʈ�� ��..����������.. �����غ���..
	if (!SetCommState( m_hComm, &dcb)) {
		ClosePort();
		return FALSE;
	}

	//--> ��Ʈ ���� ������ ����.
	m_hThreadWatchComm = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadWatchComm, this, 0, &dwThreadID);
	if (!m_hThreadWatchComm) {
		ClosePort();
		return FALSE;
	}

	// ��Ʈ ���� ������ ����.
	m_bConnected = TRUE;
	
	return TRUE;
}

void CCommThread::ClosePort()
{
	m_bConnected = FALSE;

	SetCommMask(m_hComm, 0);
	
	PurgeComm(m_hComm,	PURGE_TXABORT | PURGE_TXCLEAR | PURGE_RXABORT | PURGE_RXCLEAR);
	
	CloseHandle(m_hComm);
}

DWORD CCommThread::WriteComm(BYTE *pBuff, DWORD nToWrite)
{
	DWORD	dwWritten, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--> ��Ʈ�� ������� ���� �����̸�..
	if( !m_bConnected )		
	{
		return 0;
	}


	//--> ���ڷ� ���� ������ ������ nToWrite ��ŭ ����.. �� ������.,dwWrite �� �ѱ�.
	if( !WriteFile( m_hComm, pBuff, nToWrite, &dwWritten, &m_osWrite))
	{
		//--> ���� ������ ���ڰ� ������ ���..
		if (GetLastError() == ERROR_IO_PENDING)
		{
			// ���� ���ڰ� ���� �ְų� ������ ���ڰ� ���� ���� ��� Overapped IO��
			// Ư���� ���� ERROR_IO_PENDING ���� �޽����� ���޵ȴ�.
			//timeouts�� ������ �ð���ŭ ��ٷ��ش�.
			while (! GetOverlappedResult( m_hComm, &m_osWrite, &dwWritten, TRUE))
			{
				dwError = GetLastError();
				if (dwError != ERROR_IO_INCOMPLETE)
				{
					ClearCommError( m_hComm, &dwErrorFlags, &comstat);
					break;
				}
			}
		}
		else
		{
			dwWritten = 0;
			ClearCommError( m_hComm, &dwErrorFlags, &comstat);
		}
	}

	//--> ���� ��Ʈ�� ������ ������ ����..
	return dwWritten;
}

DWORD CCommThread::ReadComm(BYTE *pBuff, DWORD nToRead)
{
	DWORD	dwRead, dwError, dwErrorFlags;
	COMSTAT	comstat;

	//--- system queue�� ������ byte���� �̸� �д´�.
	ClearCommError( m_hComm, &dwErrorFlags, &comstat);

	//--> �ý��� ť���� ���� �Ÿ��� ������..
	dwRead = comstat.cbInQue;
	
	if (dwRead > 0)	{
		if (!ReadFile( m_hComm, pBuff, nToRead, &dwRead, &m_osRead)) {
			//--> ���� �Ÿ��� ��������..
			if (GetLastError() == ERROR_IO_PENDING)	{
				//--------- timeouts�� ������ �ð���ŭ ��ٷ��ش�.
				while (!GetOverlappedResult( m_hComm, &m_osRead, &dwRead, TRUE)) {
					dwError = GetLastError();
					if (dwError != ERROR_IO_INCOMPLETE)	{
						ClearCommError( m_hComm, &dwErrorFlags, &comstat);
						break;
					}
				}
			}
			else {
				dwRead = 0;
				ClearCommError( m_hComm, &dwErrorFlags, &comstat);
			}
		}
	}

	return dwRead;
}

// void CCommThread::ProcessRcvData(BYTE *byBuf, int nSize)
// {
// 	if (!nSize)	return;
// 
// 	CString	strRecvData(byBuf);
// 
// 	
// 	m_pListener->ReceivedData(strRecvData);
// }

// ��Ʈ�� �����ϰ�, ���� ������ ������
// m_ReadData�� ������ �ڿ� MainWnd�� �޽����� ������ Buffer�� ������
// �о��� �Ű��Ѵ�.

DWORD	ThreadWatchComm(CCommThread* pComm)
{
	DWORD           dwEvent;
	OVERLAPPED      os;
	BOOL            bOk = TRUE;
	BYTE            buff[6500];      // �б� ����
	DWORD           dwRead;  // ���� ����Ʈ��.


	// Event, OS ����.
	memset( &os, 0, sizeof(OVERLAPPED));

	//--> �̺�Ʈ ����..
	if( !(os.hEvent = CreateEvent( NULL, TRUE, FALSE, NULL)) )
	{
		bOk = FALSE;
	}

	//--> �̺�Ʈ ����ũ..
	if( !SetCommMask( pComm->m_hComm, EV_RXCHAR) )
	{
		bOk = FALSE;
	}

	//--> �̺�Ʈ��..����ũ ������ ������..
	if( !bOk )
	{
		AfxMessageBox("Error while creating ThreadWatchComm, " + pComm->m_sPortName);
		return FALSE;
	}

	while (pComm ->m_bConnected)//��Ʈ�� ����Ǹ� ���� ������ ��
	{
		dwEvent = 0;

		// ��Ʈ�� ���� �Ÿ��� �ö����� ��ٸ���.
		WaitCommEvent( pComm->m_hComm, &dwEvent, NULL);


		//--> �����Ͱ� ���ŵǾ��ٴ� �޼����� �߻��ϸ�..
		if ((dwEvent & EV_RXCHAR) == EV_RXCHAR)
		{
			// ��Ʈ���� ���� �� �ִ� ��ŭ �д´�.
			//--> buff �� �޾Ƴ���..
			do
			{
				dwRead = pComm->ReadComm( buff, 6500); //���� ������ �о� ���� 
				if(BUFF_SIZE - pComm->m_QueueRead.GetSize() > (int)dwRead)
				{
					for( WORD i = 0; i < dwRead; i++ )
					{
						pComm->m_QueueRead.PutByte(buff[i]);//ť ���ۿ�  ���� ������ �ֱ�
					}
				}
				else{
					pComm->m_QueueRead.Clear();
					GetLogCtrl()->AddBuf(LOG_STATUS, "Comport Buffer is Full and Cleard");
					//AfxMessageBox("buff full"); //ť������ ũ�⸦ �ʰ��ϸ� ��� �޽��� ����
				}
			}while(dwRead);
			//			::PostMessage(hCommWnd, WM_COMM_READ, 0, 0 );//�����Ͱ� ���Դٴ� �޽����� ����
		}
		Sleep(1);	// ���� �����͸� ȭ�鿡 ������ �ð��� ���� ����.
		// �����͸� �������� ������ cpu�������� 100%�� �Ǿ� ȭ�鿡 �ѷ��ִ� �۾��� �� �ȵǰ�. ��������� 
		// ť ���ۿ� �����Ͱ� ���̰� ��
	}

	CloseHandle( os.hEvent);

	//--> ������ ���ᰡ �ǰ���?
	pComm->m_hThreadWatchComm = NULL;

	return TRUE;

}
