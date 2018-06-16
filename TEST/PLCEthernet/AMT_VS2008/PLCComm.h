
#pragma once

class CPLCComm
{
public:
	CPLCComm(void);
	~CPLCComm(void);

	virtual BOOL Open()		{ return FALSE; }
	virtual void Close()	{}

	// BIT
	virtual long MelsecNetBitOn(short devno)	{ return 0; }
	virtual long MelsecNetBitOff(short devno)	{ return 0; }
	virtual short MelsecNetBitRecv(short devno)	{ return 0; }

	// WORD
	virtual CString NonStopDeviceRead()	{return _T("");}
	virtual void NonStopFailTypeWrite(LPCTSTR szFailType)	{}
	virtual void GlassDataRead(PANEL_INFO &panelinfo, WORD wStartAddr)	{}
	virtual void GlassDataWrite(PANEL_INFO &panelinfo, WORD wStartAddr)	{}
	virtual void MelsecNetPosRead(short devno, double &dPos) {}
	virtual void MelsecNetWordSend(short devno, short num) {}
	virtual void EthernetProc(){}

	BOOL IsConnected()	{ return m_bIsConnected; }
	void SetConnected(BOOL bIsConnected)	{ m_bIsConnected = bIsConnected; }

	PLC_INFO* m_pPLCInfo;

private:
	BOOL m_bIsConnected;
};
