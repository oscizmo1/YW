#pragma once

// #include "../../ModuleExternal/MELSEC CC-Link IE/INCLUDE/Mdfunc.h"
#include "MELSEC CC-Link IE/Mdfunc.h"
 #pragma comment(lib, "MdFunc32.lib")

enum DEVICE_TYPE_DEFINE		// Manual Page 50
{
	DEV_TYP_X	= 1,	
	DEV_TYP_Y	= 2,	
	DEV_TYP_LB	= 23,
	DEV_TYP_LW	= 24,
	DEV_TYP_WORDW_WRITE	= 36,
	DEV_TYP_WORDW_READ	= 37
};

class CMelsecNet : public CWnd
{
public:
	CMelsecNet();
	virtual ~CMelsecNet();

	DECLARE_MESSAGE_MAP()

	short MelsecOpen(short chan, LPLONG path);
	short MelsecClose(long path);

	//Bit on
	//short mnetDevSet(short numNetNo, short numStationNo, short devtyp, short devno);
	short MelsecDevSet(short numNetNo, short numStationNo, short devtyp, short devno);

	//Bit off
	//short mnetDevRst(short numNetNo, short numStationNo, short devtyp, short devno);
	short MelsecDevRst(short numNetNo, short numStationNo, short devtyp, short devno);

	//Bit Read(Receive)
	//short mnetDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data);
	short MelsecDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data);
	
	//Word Read
	//short mnetReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short MelsecReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);

	//Word write
	//short mnetSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short MelsecSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);


	//Bit on
	long MelsecDevSetEx(long numNetNo, long numStationNo, long devtyp, long devno);
	//Bit off
	long MelsecDevRstEx(long numNetNo, long numStationNo, long devtyp, long devno);
	//Bit Read(Receive)
	long MelsecDevReceiveEx(long numNetNo, long numStationNo, long devtyp, long devno, int* data);
	//Word Read
	long MelsecReceiveEx(long numNetNo, long numStationNo, long devtyp, long devno, LPLONG size, LPVOID data);
	//Word write
	long MelsecSendEx(long numNetNo, long numStationNo, long devtyp, long devno, LPLONG size, LPVOID data);

	
	short	m_shortChannel;			//slot number.
	short	m_shortMode;			//on/off line Mode.
	short	m_shortStationNumber;	//station number
	long	m_longPath;				//board address.
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};