// CommThread.h: interface for the CCommThread class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMMTHREAD_H__C828EFB9_57FC_45B8_A63E_E859356AD219__INCLUDED_)
#define AFX_COMMTHREAD_H__C828EFB9_57FC_45B8_A63E_E859356AD219__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "xmlarchive.h"
#include "xmlsettings.h"
#include "CQueue.h"

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

	void Save(CXmlSettings* pXmlSettings) {
		pXmlSettings->SetSettingLong(_T("RS-232"), _T("PortNo"), portNo);
		pXmlSettings->SetSettingLong(_T("RS-232"), _T("BaudRate"), baudRate);
		pXmlSettings->SetSettingLong(_T("RS-232"), _T("ByteSize"), byteSize);
		pXmlSettings->SetSettingLong(_T("RS-232"), _T("Parity"), parity);
		pXmlSettings->SetSettingLong(_T("RS-232"), _T("StopBits"), stopBits);
	}

	void Load(CXmlSettings* pXmlSettings) {
		portNo = pXmlSettings->GetSettingLong(_T("RS-232"), _T("PortNo"), 1);
		baudRate = pXmlSettings->GetSettingLong(_T("RS-232"), _T("BaudRate"), CBR_9600);
		byteSize = pXmlSettings->GetSettingLong(_T("RS-232"), _T("ByteSize"), 8);
		parity = pXmlSettings->GetSettingLong(_T("RS-232"), _T("Parity"), NOPARITY);
		stopBits = pXmlSettings->GetSettingLong(_T("RS-232"), _T("StopBits"), ONESTOPBIT);
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
	BOOL		m_bConnected;			// ��Ʈ�� ���ȴ��� ������ ��Ÿ��.
	OVERLAPPED	m_osRead, m_osWrite;	// ��Ʈ ���� Overlapped structure
	HANDLE		m_hThreadWatchComm;		// Watch�Լ� Thread �ڵ�.
	WORD		m_wPortID;				// WM_COMM_READ�� �Բ� ������ �μ�.
	CQueue      m_QueueRead;			//ť����
	//--------- �ܺ� ��� �Լ� ------------------------------------//
	 BOOL InitCommPort(int nPortNo, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity);
	BOOL	OpenPort(CString strPortName, DWORD dwBaud, BYTE byData, BYTE byStop, BYTE byParity );//��Ʈ ���� 
	void	ClosePort();				//��Ʈ �ݱ�
	DWORD	WriteComm(BYTE *pBuff, DWORD nToWrite);//��Ʈ�� ������ ����

	//--------- ���� ��� �Լ� ------------------------------------//
	DWORD	ReadComm(BYTE *pBuff, DWORD nToRead);//��Ʈ���� ������ �о����

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

#endif // !defined(AFX_COMMTHREAD_H__C828EFB9_57FC_45B8_A63E_E859356AD219__INCLUDED_)
