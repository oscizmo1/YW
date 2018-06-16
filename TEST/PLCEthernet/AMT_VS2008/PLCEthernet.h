#pragma once
#include "actqj71e71tcp.h"
#include "actqj71e71udp.h"


// CPLCEthernet dialog
// BIT 영역은 8bit가 1byte
// 또한 BIT Address 8개가 1WORD와 같다.
// 따라서 B000 ~ BFFF 까지  size는 0xFF WORD다
#define MAX_PLC_BIT_SIZE	0xFF
#define USE_PLC_BIT_SIZE	0x04
// WORD 영역은 Address 1개가 2byte (1 short)
// 따라서 W000 ~ WFFF 까지의 size는 0x0FFF
#define MAX_PLC_WORD_SIZE	0x0FFF
#define USE_PLC_WORD_SIZE	0x0100

class CPLCEthernet : 
	public CDialog,
	public CPLCComm
{
	DECLARE_DYNAMIC(CPLCEthernet)

public:
	CPLCEthernet(CWnd* pParent = NULL);   // standard constructor
	virtual ~CPLCEthernet();

// Dialog Data
	enum { IDD = IDD_PLC_ETHERNET };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

public:
	CActqj71e71tcp m_ActQJ71E71;
	CActqj71e71udp m_ActQJ71E71Udp;
	short m_bitRecv[MAX_PLC_BIT_SIZE];
	short m_bitSend[MAX_PLC_BIT_SIZE];
	short m_WordRecv[MAX_PLC_WORD_SIZE];
	short m_WordSend[MAX_PLC_WORD_SIZE];


	DECLARE_MESSAGE_MAP()

public:
	BOOL Open();
	void Close();

	long MelsecNetBitOn(short devno);
	long MelsecNetBitOff(short devno);
	short MelsecNetBitRecv(short devno);

	
	CString NonStopDeviceRead();
	void NonStopFailTypeWrite(LPCTSTR szFailType);
	void GlassDataRead(PANEL_INFO &panelinfo, WORD wStartAddr);
	void GlassDataWrite(PANEL_INFO &panelinfo, WORD wStartAddr);
	void MelsecNetPosRead(short devno, double &dPos);
	void MelsecNetWordSend(short devno, short num);
	void EthernetProc();
	CString strDevice;
};
