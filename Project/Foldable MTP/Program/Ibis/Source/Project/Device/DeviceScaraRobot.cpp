#include "StdAfx.h"
#include "DeviceScaraRobot.h"
#include "BankGuild\ProcDataBank.h"
#include "GeryonApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// 사용 순서. 
// 1. Offset설정. ( SetZOffset, SetXOffset, SetYOffset, SetTOffset )
// 2. End Position 설정. ( SetEndPosition ) 
// 3. Move Start Bit On. ( MoveStart )
// 4. Move Finish Check. ( CheckFinish )
// 5. Move Finish Bit & Move Start Bit Off. ( OnTimer ) 

cDeviceScaraRobot::cDeviceScaraRobot(void)
{
	m_PLCInfo.m_iNetChanel = INT_MNET_CHANNEL;
	m_PLCInfo.m_iNetMode = INT_MNET_MODE;
	m_PLCInfo.m_iNetWork = INT_MNET_NETWORK;
	m_PLCInfo.m_iStation = INT_MNET_STATION;
	m_PLCInfo.m_lNetPath = INT_MNET_PATH;

	m_strSndBuf = _T("");
	m_strRcvBuf = _T("");

	m_bIsConnected = FALSE;
}

cDeviceScaraRobot::~cDeviceScaraRobot(void)
{

}

BEGIN_MESSAGE_MAP(cDeviceScaraRobot, CMelsecNet)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL cDeviceScaraRobot::Open()
{
	SetConnected(FALSE);
	
	if(MelsecOpen(m_PLCInfo.m_iNetChanel, m_PLCInfo.m_iNetMode, &m_PLCInfo.m_lNetPath) == 0)
	{
		SetConnected(TRUE);
		return TRUE;
	}
	else
	{
	//	AfxMessageBox(_T("Err : SCARA Open."));
	//	theProcBank.AlarmHappen(ALM_SCARA_OPEN);
	//	return FALSE;
	}

	return FALSE; 
}

void cDeviceScaraRobot::Close()
{
	SetConnected(FALSE);
	MelsecClose( m_PLCInfo.m_lNetPath);
}

//void cDeviceScaraRobot::AutoStart()
//{
//	if(IsConnected() == FALSE)
//		return;
//
//	/*
//	IN		AUTO RUN MODE		ON
//	OUT		AUTO RUN MODE DIS	ON
//	IN		AUTO RUN MODE		OFF
//	IN		START #1			ON
//	OUT		RUNNING #1			ON
//	OUT		SERVO ON #1			ON
//	IN		START #1			OFF
//	*/
//
//	Scara_BitOn(0x78);	//AUTO RUN MODE : On
//	SetTimer(BIT_READ, 500, NULL);	
//}

//void cDeviceScaraRobot::Stop()
//{
//	if(IsConnected() == FALSE)
//		return;
//
//	//IN	STOP #1		ON
//	//IN	STOP #1		OFF
//	//OUT	RUNNING #1	OFF
//	Scara_BitOn(0x30);	//STOP #1 : On
//	Sleep(500);
//
//	Scara_BitOff(0x30);	//STOP #1 : Off
//
//	SetTimer(RUNNING_OFF, 500, NULL);
//
//}

short cDeviceScaraRobot::Scara_BitOn(short devno)
{
	return MelsecDevSet(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WRIT, devno);
}

short cDeviceScaraRobot::Scara_BitOff(short devno)
{
	return MelsecDevRst(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WRIT, devno);
}

short cDeviceScaraRobot::Scara_ReadBit(short devno)
{
	BOOL bFlag;
	MelsecDevReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_READ, devno, &bFlag);

	return bFlag;
}

short cDeviceScaraRobot::Scara_Read_USERBit(short devno)
{
	BOOL bFlag;
	MelsecDevReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WRIT, devno, &bFlag);

	return bFlag;
}

void cDeviceScaraRobot::Scara_ReadWord(short devno, short nSizebyByte, PSHORT pData)
{
	if(IsConnected() == FALSE)
		return;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	short nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	short buf[128];
	memset(buf, NULL, sizeof(short)*128);

	short ret = MelsecReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WORDW_READ, devno, &nSize, buf);

	//Return 값 확인해봐야함. 읽으면 어떤 값이 return 되는가 ? 
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(BYTE)*nSize);
	}
}

CString cDeviceScaraRobot::Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString)
{
	if(IsConnected() == FALSE)
	{
		theLog[LOG_EDGE_ALIGN_DEBUG].AddBuf(_T("Melsec Not Open"));
		return _T("");
	}

	short sendData[256];
	memset(sendData, NULL, 256);

	if(bString)
	{
		char szBuff[256];
		CEtc::ConvertMultibyte((wchar_t*)data, szBuff, 256);
		memcpy(sendData, szBuff, nSizebyByte);
	}
	else
		memcpy(sendData, data, nSizebyByte);

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함 
	if(nSizebyByte%2)	
		nSizebyByte++;

	short ret = MelsecSend(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_WORDW_WRITE, devno, &nSizebyByte,sendData);

	if(ret == 0)
	{
		return m_strSndBuf;
	}

	CString strErrMsg;
	strErrMsg.Format(_T("Scara_WriteWord Error!!! "));
	theLog[LOG_EDGE_ALIGN_DEBUG].AddBuf(strErrMsg);

	return strErrMsg;
}

short	cDeviceScaraRobot::MoveStart()
{
	// Offset, Start Point, End Point, 이동 타입 세팅 완료후 동작. 
	//Start bit를 On해준다. 

	short nRet = -1;

	if(IsConnected() == FALSE)
		return FALSE;

	//nRet = Scara_BitOn(SCARA_START_IN);

	//SetTimer(1, 500, NULL);
	
	return nRet;
}

//void cDeviceScaraRobot::CheckFinish()
//{
//	//이동 명령후 이동이 완료 되었는지 체크 한다. 
//	//이동이 완료 되면 1.
//
//	//BOOL bRet = FALSE;
//	//
//	//bRet = Scara_ReadBit(SCARA_FINISH_OUT);
//	//
//	//if(bRet == TRUE)	//이동 완료되면 Move start Bit와 Finish Bit를 Off 상태로 돌려놓는다. 
//	//	MoveFinish();
//
//	SetTimer(CHECK_FINISH, 500, NULL);	//finish bit 체크. 
//}

//void cDeviceScaraRobot::SetEndPosition(SCARA_G_POINT ePoint)
//{
//	int nStartPos = GetPosition();
//	m_nMoveType = TYPE_SPOS_EPOS;
//
//	switch()
//	{
//	case SCARA_WAIT_POS1:
//		break;
//	case SCARA_WAIT_POS2:
//		break;
//	case SCARA_CELL_REVERSE:
//		break;
//	case SCARA_BUFFER_STAGE:
////SCARA_TRAY1_POS,		//index4
////SCARA_TRAY2_POS			//index5
//	}
//
//
//	//switch(ePoint)
//	//{
//	//case SCARA_WAIT_POS1:
//	//	m_nMoveType = MoveToWaitOnePoint(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//
//	//case SCARA_WAIT_POS2:
//	//	m_nMoveType = MoveToWaitTwoPoint(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//
//	//case SCARA_CELL_REVERSE:
//	//	m_nMoveType = MoveToCellReverse(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//
//	//case SCARA_BUFFER_STAGE:
//	//	m_nMoveType = MoveToBufferStage(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//
//	//case SCARA_TRAY1_POS:
//	//	m_nMoveType = MoveToTrayOne(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//
//	//case SCARA_TRAY2_POS:
//	//	m_nMoveType = MoveToTrayTwo(nStartPos, SCARA_WAIT_POS1);
//	//	break;
//	//}
//
//}
//void cDeviceScaraRobot::SetEndPosition(short dev)
//{
//		int	nData = 10;
//		char szBuf[2];
//		szBuf[1] = NULL;
//	
//		szBuf[0] = nData;
//
//		theDeviceSCARA.Scara_WriteWord(SCARA_OFFSET_Y, szBuf, 1, FALSE);
//
//}

//int cDeviceScaraRobot::MoveToWaitOnePoint(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 0;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index wait1"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS2)			//wait2			==> wait1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_CELL_REVERSE)	//Cell Reverse	==> wait1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer stage	==> wait1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1			==> wait1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY2_POS)		//tray2	==> wait2 ==> wait1
//		nMoveType = TYPE_W2_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//	
//	return nMoveType;
//}
//int cDeviceScaraRobot::MoveToWaitTwoPoint(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 1;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index wait2"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS1)			//wait1			==> wait2
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse		==> wait2
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> wait2
//		nMoveType = TYPE_W1_EPOS;
//	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1			==> wait2
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2			==> wait2
//		nMoveType = TYPE_SPOS_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//
//	return nMoveType;
//}
//int cDeviceScaraRobot::MoveToCellReverse(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 2;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index Reverse"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS1)			//wait1		==> Reverse
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2		==> Reverse
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer	==> Reverse
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY1_POS)	//Tray1		==> Reverse
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2 ==> wait2 ==> Reverse
//		nMoveType = TYPE_W2_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//
//	return nMoveType;
//}
//int cDeviceScaraRobot::MoveToBufferStage(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 3;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index Buffer"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS1)			//wait1		==> Buffer
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> wait1 ==> Buffer
//		nMoveType = TYPE_W1_EPOS;
//	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse	==> Buffer
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1	==> wait1 ==> Buffer
//		nMoveType = TYPE_W1_EPOS;
//	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2	==> wait2 ==> wait1 ==> Buffer
//		nMoveType = TYPE_SPOS_W1_W2_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//
//	return nMoveType;
//}
//
//int cDeviceScaraRobot::MoveToTrayOne(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 4;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index Buffer"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS1)			//wait1 ==> Tray1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> Tray1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse ==> Tray1
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> Tray1
//		nMoveType = TYPE_W1_EPOS;
//	else if(nStartPoint==SCARA_TRAY2_POS)		//Tray2	==> wait2 ==> Tray1
//		nMoveType = TYPE_W2_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//
//	return nMoveType;
//}
//
//int cDeviceScaraRobot::MoveToTrayTwo(int nStartPoint, int nEndPoint)
//{
//	if(IsConnected() == FALSE)
//		return FALSE;
//
//	BYTE data = 5;
//	int nMoveType = TYPE_SPOS_EPOS;
//
//	//RWw11 Global Point Index Setting. ( 이동할 Global Point의 Index. )
//	//strData.Format(_T("index Buffer"));	//int형 Position Index
//
//	if(nStartPoint==SCARA_WAIT_POS1)			//wait1 ==> wait2 ==> Tray2
//		nMoveType = TYPE_W2_EPOS;
//	else if(nStartPoint==SCARA_WAIT_POS2)		//wait2 ==> Tray2
//		nMoveType = TYPE_SPOS_EPOS;
//	else if(nStartPoint==SCARA_CELL_REVERSE)	//Reverse ==> wait2 ==> Tray2
//		nMoveType = TYPE_W2_EPOS;
//	else if(nStartPoint==SCARA_BUFFER_STAGE)	//Buffer ==> wait1 ==> wait2 ==>Tray2
//		nMoveType = TYPE_SPOS_W1_W2_EPOS;
//	else if(nStartPoint==SCARA_TRAY1_POS)		//Tray1	==> wait2 ==> Tray2
//		nMoveType = TYPE_W2_EPOS;
//
//	//Scara_WriteWord(SCARA_END_INDEX, (LPVOID)&data, sizeof(BYTE), FALSE);
//
//	return nMoveType;
//
//}

void cDeviceScaraRobot::SetZOffset(int nOffset)
{
	// scara의 픽커가 올라오는 높이 설정. 
	//Global float입력. //RWw0~w7	Offset 입력 ( X, Y, T Offset )
	if(IsConnected() == FALSE)
		return;

	WORD data = nOffset;
	Scara_WriteWord(SCARA_OFFSET_Z, (LPVOID)&data, sizeof(WORD), FALSE);
}

void	cDeviceScaraRobot::SetTOffset(int nOffset)
{
	//Tray나 Stage의 Theta방향의 offset값을 세팅한다. 
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )
	if(IsConnected() == FALSE)
		return;

	WORD data = nOffset;
	Scara_WriteWord(SCARA_OFFSET_T, (LPVOID)&data, sizeof(WORD), FALSE);
}

void cDeviceScaraRobot::SetYOffset(int nMode, int nOffset)
{
	// Tray나 Stage의 세로 방향의 offset값을 세팅한다. 
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )
	if(IsConnected() == FALSE)
		return;

	WORD data = nOffset;

	Scara_WriteWord(SCARA_OFFSET_Y, (LPVOID)&data, sizeof(WORD), FALSE);
}
void cDeviceScaraRobot::SetXOffset(int nMode, int nOffset)
{
	// Tray나 Stage의 가로 방향의 offset값을 세팅한다.  
	//RWw0~w7	Offset 입력 ( X, Y, T Offset )
	if(IsConnected() == FALSE)
		return;

	WORD data = nOffset;
	Scara_WriteWord(SCARA_OFFSET_X, (LPVOID)&data, sizeof(WORD), FALSE);
}

int	cDeviceScaraRobot::GetPosition()
{
	// 현재 Scara 픽커의 위치를 알려준다 (Position Index)
	int nReturn = 0;
	//millimeter로 위치값을 얻어와 티칭 위치와 비교 현재위치의 Index를 반환해주는 기능. 


	//return 0는 대기위치, false(-1)은 오류.
	return nReturn;
}
double cDeviceScaraRobot::GetPosition_TempWrite()
{
	if(IsConnected() == FALSE)
		return FALSE;

	DWORD value;
	CString strTest;

	int	nData = 10;
	char szBuf[2];
	szBuf[1] = NULL;
	
	szBuf[0] = nData;

	Scara_WriteWord(SCARA_OFFSET_X, (LPVOID)strTest.GetString(), (short)strTest.GetLength(), TRUE);
	Scara_WriteWord(SCARA_OFFSET_Y, szBuf, 1, FALSE);

	short Data[2] = {0,0};
	memcpy(&value, Data, sizeof(short)*2);
	double fData = (double)value/1000.;
	// 0. 이하자리 안읽는것 알아봐야함. 

	return fData;
}

double cDeviceScaraRobot::GetPosition_x()
{
	if(IsConnected() == FALSE)
		return FALSE;

	long value;

	short Data[2];
	memset(Data,NULL,sizeof(BYTE)*4);
	Scara_ReadWord(SCARA_OFFSET_X, sizeof(BYTE)*4, Data);
	
	memcpy(&value, Data, sizeof(BYTE)*4);
	double fData = value/1000.;

	return fData;

	//short Data[2] = {0,0};
	//Scara_ReadWord(SCARA_OFFSET_X, sizeof(short)*2, Data);
	//
	//return *Data;
}
double cDeviceScaraRobot::GetPosition_y()
{
	if(IsConnected() == FALSE)
		return FALSE;

	long value;

	short Data[2];
	Scara_ReadWord(SCARA_OFFSET_Y, sizeof(short)*2, Data);
	
	memcpy(&value, Data, sizeof(short)*2);
	double fData = (double)value/1000.;

	return fData;
}
double cDeviceScaraRobot::GetPosition_t()
{
	if(IsConnected() == FALSE)
		return FALSE;

	long value;

	short Data[2] = {0,0};
	Scara_ReadWord(SCARA_OFFSET_T, sizeof(short)*2, Data);
	
	memcpy(&value, Data, sizeof(short)*2);
	double fData = (double)value/1000.;

	return fData;

	//short Data[2] = {0,0};
	//Scara_ReadWord(SCARA_OFFSET_T, sizeof(short)*2, Data);
	//
	//return *Data;
}
double cDeviceScaraRobot::GetPosition_z()
{
	if(IsConnected() == FALSE)
		return FALSE;

	long value;

	short Data[2] = {0,0};
	Scara_ReadWord(SCARA_OFFSET_Z, sizeof(short)*2, Data);
	
	memcpy(&value, Data, sizeof(short)*2);
	double fData = (double)value/1000.;

	return fData;

	//short Data[2] = {0,0};
	//Scara_ReadWord(SCARA_OFFSET_Z, sizeof(short)*2, Data);
	//
	//return *Data;
}

void cDeviceScaraRobot::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//switch(nIDEvent)
	//{
	////case 1:
	////	{
	////		if(Scara_ReadBit(SCARA_START_OUT) == 1)
	////		{
	////			KillTimer(CHECK_STARTBIT);
	////			Scara_BitOff(SCARA_START_IN);
	////		}
	////	}
	////	break;
	//	//case BIT_READ:
	//	//{
	//	//	if(Scara_ReadBit(0x78) == TRUE)
	//	//	{
	//	//		//Auto Run Mode DIS : On

	//	//		Scara_BitOff(0x78);		//AUTO RUN MODE : OFF
	//	//		KillTimer(BIT_READ);
	//	//		Sleep(500);

	//	//		Scara_BitOn(0x0f);		//START #1 : On
	//	//		SetTimer(START_READ, 500, NULL);
	//	//	}
	//	//}
	//	//break;

	//	//case START_READ:
	//	//{
	//	//	if(Scara_ReadBit(0x04) == TRUE && Scara_ReadBit(0x04)==TRUE)
	//	//	{
	//	//		//RUNNING #1 : ON && SERVO ON #1 : ON
	//	//		KillTimer(START_READ);
	//	//		Scara_BitOff(0x0f);		//START #1 : OFF
	//	//	}
	//	//}
	//	//break;

	//	//case RUNNING_OFF:
	//	//{
	//	//	if(Scara_ReadBit(0x04) == FALSE)
	//	//	{
	//	//		KillTimer(RUNNING_OFF);		//RUNNING #1 : OFF
	//	//	}
	//	//}
	//	//break;

	//	//case CHECK_FINISH:
	//	//{
	//	//	if(Scara_ReadBit(SCARA_FINISH_OUT) == TRUE)
	//	//	{
	//	//		KillTimer(CHECK_FINISH);
	//	//		Scara_BitOff(SCARA_START_OUT);
	//	//	}
	//	//}
	//	//break;
	//}
	CMelsecNet::OnTimer(nIDEvent);
}

void cDeviceScaraRobot::RebootBitOn()
{
	Scara_BitOn(SCARA_STOP);
	Sleep(50);
	Scara_BitOff(SCARA_STOP);
	Sleep(50);
	Scara_BitOn(SCARA_STOP);
	Sleep(50);
	Scara_BitOff(SCARA_STOP);
	Scara_BitOn(SCARA_START_REBOOT);	
}
