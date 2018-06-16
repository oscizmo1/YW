#pragma once


// CMNetH
enum DEV_TYPE_DEF
{
	DEV_TYP_INPUT = 1,//DevX = 1, //	X
	DEV_TYP_OUT = 2, //DevY = 2, //	Y
	DEV_TYP_BIT = 23,//DevB = 23,//	B
	DEV_TYP_WORD = 24//	W
	//	DEV_TYP_M = 4, //DevM = 4,//	M
	//	DEV_TYP_DEVD =13, //DevD = 13,//	D
	//	DEV_TYP_DEVSD = 14,//	SpecialDSW(link special W for MELSECNET/10,/H or CC-Link)
	//	DevL = 3,//	L
	//	DevSM = 5,//	Special MSB(link special B for MELSECNET/10,/H or CC-Link)
	//	DevF = 6,//	F
	//	DevTT = 7,//	T(contact)
	//	DevTC = 8,//	T(coil)
	//	DevCT = 9,//	C(contact)
	//	DevCC = 10,//	C(coil)
	//	DevTN = 11,//	T(current value)
	//	DevCN = 12,//	C(current value)
	//	DevTM =15,//	T(setting value main)
	//	DevTS =16,//	T(setting value sub1)
	//	DevTS2 = 16002, //	T(setting value sub2)
	//	DevTS3 = 16003, //	T(setting value sub3)
	//	DevCM = 17,//	C(setting value main)
	//	DevTS = 18,//	C(setting value sub1)
	//	DevTS2 = 18002,//	C(setting value sub2)
	//	DevTS3 = 18003,//	C(setting value sub3)
	//	DevA = 19,//	A
	//	DevZ = 20,//	Z
	//	DevV = 21,//	V(index register)
	//	DevR = 22,//	R
	//	DevQSB = 25,//	Q/QnA link special relay(Q/QnACPUup)
	//	DevSTT = 26,//	Retentive timer
	//	DevSTC = 27,//	Retentive timer
	//	DevQSW = 28,//	Q/QnA link special register(Q/QnACPUup)
	//	DevQV = 30 //	Q/QnA edge relay(Q/QnACPUup)
};

class CMNetH : public CWnd
{

public:
	CMNetH();

	short mnetDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data);
	short mnetRandW(long path, short stno, LPVOID dev, LPVOID buf, short bufsize);
	short mnetRandR(long path, short stno, LPVOID dev, LPVOID buf, short bufsize);
	short mnetBdVerRead(long path, PSHORT buf);
	short mnetBdSwRead(long path, PSHORT buf);
	short mnetBdLedRead(long path, PSHORT buf);
	short mnetTypeRead(short numNetNo, short numStationNo, PSHORT buf);
	short mnetControl(short numNetNo, short numStationNo, short buf);
	short mnetInit(long path);
	short mnetBdModSet(long path, short mode);
	short mnetBdModRead(long path, PSHORT mode);
	short mnetDevRst(short numNetNo, short numStationNo, short devtyp, short devno);
	short mnetDevSet(short numNetNo, short numStationNo, short devtyp, short devno);
	short mnetBdRst(long path);
	short mnetSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short mnetReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short mnetClose(long path);
	short mnetOpen(short chan, short mode, LPLONG path);

	short mnetSubReceive(long path ,short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data);
	short mnetSubDevReceive(long path , short numNetNo, short numStationNo, short devtyp, short devno, int* data);
	short mnetSubDevSet(long path , short numNetNo, short numStationNo, short devtyp, short devno);
	short mnetSubDevRst(long path , short numNetNo, short numStationNo, short devtyp, short devno);
	CString DataReceive(short numNetNo, short numStationNo, short devtyp, short devno, short size,short cmode);
	short DataSend(short numNetNo, short numStationNo, short devtyp, short devno, short size, CString InStr, short cmode);
	short ASCIISend(short numNetNo, short numStationNo, short devtyp, short devno, CString strMnetASCII);
	short ASCIIReceive(short numNetNo, short numStationNo, short devtyp, short devno, CString* pStrAscii, int nWord);
	short SubASCIIReceive(long path ,short numNetNo, short numStationNo, short devtyp, short devno, CString* pStrAscii, int nWord);

	short m_shortret, m_shortchan, m_shortmode, m_shortstno, m_shortdata, m_shortsize, m_shortdevtyp, m_shortdevno;
	long m_lpath;
	CString m_strMnetASCII;
	CString m_strOutStr, m_strInStr, m_chkMessage, m_Hex, m_Bin, m_Dec;

	virtual ~CMNetH();

protected:
	DECLARE_MESSAGE_MAP()
	BOOL CheckExtent(short size, short cmode, LONGLONG dec);
	CString ReceiveTrans(short data, short cmode);
	short SendTrans(CString InStr, short cmode);
};


