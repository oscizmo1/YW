#pragma once
#include "mneth.h"
#include "PLCComm.h"

class CMelsecNet :
	public CMNetH,
	public CPLCComm
{
public:
	CMelsecNet(void);

	BOOL Open();
	void Close();

	long MelsecNetBitOn(short devno);
	long MelsecNetBitOff(short devno);
	short MelsecNetBitRecv(short devno);
	CString MelsecNetWordRecv(short devno, int nSize);
	CString MelsecNetWordSend(short devno, char* sSndBuf);

	CString NonStopDeviceRead();
	void NonStopFailTypeWrite(LPCTSTR szFailType);
	void GlassDataRead(PANEL_INFO &panelinfo, WORD wStartAddr);
	void GlassDataWrite(PANEL_INFO &panelinfo, WORD wStartAddr);

	CString m_strRcvBuf;
	CString m_strSndBuf;

	BOOL m_bInitMelsec;

	~CMelsecNet(void);
};
