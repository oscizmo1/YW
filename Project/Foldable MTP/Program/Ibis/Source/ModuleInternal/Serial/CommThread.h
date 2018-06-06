// CommThread.h: interface for the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include "CQueueSerial.h"

struct CCommInfo 
{
	long portNo;
	long baudRate;
	long stopBits;
	long parity;
	long byteSize;

	CCommInfo() {
		portNo		= 1;
		baudRate	= CBR_9600;
		byteSize	= 8;
		parity		= NOPARITY;
		stopBits	= ONESTOPBIT;
	}

	CCommInfo(CCommInfo& commInfo) {
		portNo		= commInfo.portNo;
		baudRate	= commInfo.baudRate;
		byteSize	= commInfo.byteSize;
		parity		= commInfo.parity;
		stopBits	= commInfo.stopBits;
	}

	CString GetInfoStr() {
		CString stopBitsStr[]  = { _T("1"), _T("1.5"), _T("2") };
		CString infoStr = _T("");
		if (portNo > -1)
		{
			infoStr.Format(_T("COM%d/%dbps/%d/%d/%s"), portNo, baudRate, byteSize, parity, stopBitsStr[stopBits]);
		}
		return infoStr;
	}
};


class CCommListener
{
public:
	virtual void ReceivedData(CString strData) = 0;
};

class CCommThread 
{
	CCommListener* m_pListener;
public:
	CCommThread();
	~CCommThread();
	//--------- ȯ�� ���� -----------------------------------------//
	HANDLE		m_hComm;				// ��� ��Ʈ ���� �ڵ�
	CString		m_sPortName;			// ��Ʈ �̸� (COM1 ..)
	CString		m_sPortNickName;			// ��Ʈ �г���
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.
	CQueueSerial      m_QueueRead;			//ť����
	//--------- �ܺ� ��� �Լ� ------------------------------------//
	BOOL InitCommPort(int nPortNo, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity);
	BOOL	OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//��Ʈ ���� 
	void	ClosePort();				//��Ʈ �ݱ�
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//��Ʈ�� ������ ����

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//��Ʈ���� ������ �о����
	SERIAL_MODE m_serialMode;
	BOOL m_bRecvDone;
	
// 	CString byIndexComPort(int xPort);// ��Ʈ�̸� �ޱ� 
// 	DWORD byIndexBaud(int xBaud);// ���۷� �ޱ�
// 	BYTE byIndexData(int xData);//������ ��Ʈ �ޱ�
// 	BYTE byIndexStop(int xStop);// �����Ʈ �ޱ� 
// 	BYTE byIndexParity(int xParity);// �丮Ƽ �ޱ�


// 	void SetListener(CCommListener* pListener) { m_pListener = pListener; };
// 	void ProcessRcvData(BYTE *byBuf, int nSize);


//	BOOL		OpenPort(CString strPortName, DWORD dwBaud, WORD wParam);
};

// Thread�� ����� �Լ� 
DWORD	ThreadWatchComm(CCommThread* pComm);
