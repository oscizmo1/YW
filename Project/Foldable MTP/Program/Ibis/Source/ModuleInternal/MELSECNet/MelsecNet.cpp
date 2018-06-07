#include "stdafx.h"
#include "MelsecNet.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMelsecNet::CMelsecNet()
{
}

CMelsecNet::~CMelsecNet()
{
}

BEGIN_MESSAGE_MAP(CMelsecNet, CWnd)
	ON_WM_TIMER()
END_MESSAGE_MAP()


short CMelsecNet::MelsecOpen(short chan, LPLONG path)
{
	m_shortChannel			= chan;
	m_shortMode				= -1;
	m_shortStationNumber	= 255;
	m_longPath				= *path;

	short ret = mdOpen(m_shortChannel, m_shortMode, &m_longPath);

	return ret;
}

short CMelsecNet::MelsecClose(long path)
{
	short ret = mdClose(path);

	return ret;
}

short CMelsecNet::MelsecDevSet(short numNetNo, short numStationNo, short devtyp, short devno)
{
	//Bit on
	short ret = mdDevSet(m_longPath, numStationNo, devtyp, devno);

	return ret;
}

short CMelsecNet::MelsecDevRst(short numNetNo, short numStationNo, short devtyp, short devno)
{
	//Bit off
	short ret = mdDevRst(m_longPath, numStationNo, devtyp, devno);

	if(ret)		
	{
		CString strMsg;
		strMsg.Format(_T("Check Melsec(mdDevRst Error) Code = %d,"), ret);
	}

	return ret;
}

short CMelsecNet::MelsecDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data)
{
	//Bit Read(Receive)
	short devno2, data2, ibit, temp;
	devno2 = data2 = ibit = temp =0;

	if(devno>7)
	{
		devno2 = short(devno/8) * 8;
	}
	else
	{
		devno2 = 0;
	}

	short size = 1;	//This fn. is received by bit, X, Y etc....(so, size is only 1)

	short ret = MelsecReceive(numNetNo, numStationNo, devtyp, devno2, &size, &data2); //for pointer variable.. 

	ibit = (devno%8) +1;

	switch(ibit)
	{
		case 1:	temp = 0x01; break;
		case 2:	temp = 0x02; break;
		case 3:	temp = 0x04; break;
		case 4:	temp = 0x08; break;
		case 5:	temp = 0x10; break;
		case 6:	temp = 0x20; break;
		case 7:	temp = 0x40; break;
		default:temp = 0x80; break;
	}

	int flags = data2 & temp;
	if(flags) 		flags = 1;
	*data = flags;

	return ret;
}
	
short CMelsecNet::MelsecReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data)
{
	//Word Read
	short ret = mdReceive(m_longPath, numStationNo, devtyp, devno, size, data);

	return ret;
}

short CMelsecNet::MelsecSend(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data)
{
	//Word write
	short ret = mdSend(m_longPath, numStationNo, devtyp, devno, size, data);

	return ret;
}


long CMelsecNet::MelsecDevSetEx( long numNetNo, long numStationNo, long devtyp, long devno )
{
	//Bit on
	long ret = mdDevSetEx(m_longPath, numNetNo, numStationNo, devtyp, devno);

	return ret;
}

long CMelsecNet::MelsecDevRstEx( long numNetNo, long numStationNo, long devtyp, long devno )
{
	//Bit off
	long ret = mdDevRstEx(m_longPath, numNetNo, numStationNo, devtyp, devno);

	return ret;
}

long CMelsecNet::MelsecDevReceiveEx( long numNetNo, long numStationNo, long devtyp, long devno, int* data )
{
	//Bit Read(Receive)
	short devno2, data2, ibit, temp;
	devno2 = data2 = ibit = temp =0;

	if(devno>7)
	{
		devno2 = short(devno/8) * 8;
	}
	else
	{
		devno2 = 0;
	}

	long size = 1;	//This fn. is received by bit, X, Y etc....(so, size is only 1)

	long ret = MelsecReceiveEx(numNetNo, numStationNo, devtyp, devno2, &size, &data2); //for pointer variable.. 

	ibit = (devno%8) +1;

	switch(ibit)
	{
	case 1:	temp = 0x01; break;
	case 2:	temp = 0x02; break;
	case 3:	temp = 0x04; break;
	case 4:	temp = 0x08; break;
	case 5:	temp = 0x10; break;
	case 6:	temp = 0x20; break;
	case 7:	temp = 0x40; break;
	default:temp = 0x80; break;
	}

	int flags = data2 & temp;
	if(flags) 		flags = 1;
	*data = flags;

	return ret;
}

long CMelsecNet::MelsecReceiveEx( long numNetNo, long numStationNo, long devtyp, long devno, LPLONG size, LPVOID data )
{
	long ret = mdReceiveEx(m_longPath, numNetNo, numStationNo, devtyp, devno, size, data);

	return ret;
}

long CMelsecNet::MelsecSendEx( long numNetNo, long numStationNo, long devtyp, long devno, LPLONG size, LPVOID data )
{
	long ret = mdSendEx(m_longPath, numNetNo, numStationNo, devtyp, devno, size, data);

	return ret;
}

void CMelsecNet::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	CWnd::OnTimer(nIDEvent);
}
