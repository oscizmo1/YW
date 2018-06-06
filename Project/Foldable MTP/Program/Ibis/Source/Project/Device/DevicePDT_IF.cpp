#include "StdAfx.h"
#include "DevicePDT_IF.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDevicePDT_IF::CDevicePDT_IF(void)
{
	m_PLCInfo.m_iNetChanel = INT_MNET_CHANNEL;
	m_PLCInfo.m_iNetMode = INT_MNET_MODE;
	m_PLCInfo.m_iNetWork = INT_MNET_NETWORK;
	m_PLCInfo.m_iStation = INT_MNET_STATION;
	m_PLCInfo.m_lNetPath = INT_MNET_PATH;

	m_bIsConnected = FALSE;
}

CDevicePDT_IF::~CDevicePDT_IF(void)
{

}

BEGIN_MESSAGE_MAP(CDevicePDT_IF, CMelsecNet)
	ON_WM_TIMER()
END_MESSAGE_MAP()


BOOL CDevicePDT_IF::Open()
{
	SetConnected(FALSE);
	
	if(MelsecOpen(m_PLCInfo.m_iNetChanel, &m_PLCInfo.m_lNetPath) == 0)
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

void CDevicePDT_IF::Close()
{
	SetConnected(FALSE);
	MelsecClose( m_PLCInfo.m_lNetPath);
}

BOOL CDevicePDT_IF::LB_Get( long addr )
{
	if(IsConnected() == FALSE)
		return FALSE;
	BOOL bFlag;
// 	MelsecDevReceive(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, (short)addr, &bFlag);
	MelsecDevReceiveEx(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, addr, &bFlag);

	return bFlag;
}

void CDevicePDT_IF::LB_Set( long addr, BOOL bValue )
{
	if(IsConnected() == FALSE)
		return;
	long sRet;
	if(bValue)
// 		sRet = MelsecDevSet(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, (short)addr);
		sRet = MelsecDevSetEx(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, addr);
	else
// 		sRet = MelsecDevRst(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, (short)addr);
		sRet = MelsecDevRstEx(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LB, addr);
}

long CDevicePDT_IF::LW_Read( long addr, PSHORT pData, long nSizebyByte )
{
	if(IsConnected() == FALSE)
		return -1;

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함
	long nSize = nSizebyByte;
	if(nSize%2)	nSize++;

	BYTE buf[256];
	memset(buf, NULL, sizeof(BYTE)*256);

	long ret = MelsecReceiveEx(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LW, addr, &nSize, buf);

	//Return 값 확인해봐야함. 읽으면 어떤 값이 return 되는가 ? 
	if(ret == 0)
	{
		memcpy(pData, buf, sizeof(BYTE)*nSize);
	}
	return ret;
}

long CDevicePDT_IF::LW_Write( long addr, LPVOID data, long nSizebyByte )
{
	if(IsConnected() == FALSE)
		return -1;

	short sendData[256];
	memset(sendData, NULL, 256);

	memcpy(sendData, data, nSizebyByte);

	// WORD단위니까 Byte Size는 전부 짝수로 처리해야 함 
	if(nSizebyByte%2)	
		nSizebyByte++;

	long ret = MelsecSendEx(m_PLCInfo.m_iNetWork, m_PLCInfo.m_iStation, DEV_TYP_LW, addr, &nSizebyByte, sendData);

	return ret;
}

long CDevicePDT_IF::MTP_LB_Calcurate_FullAddr( JIG_ID jig, LB_MTP_ADDR addr )
{
	long addrSum = MTP_ADDR_START_LB;
	addrSum += m_Machine * MTP_ADDR_OFFSET_LB;
	addrSum += jig * SHUTTLE_ADDR_OFFSET_LB;
	return addrSum + addr;
}

long CDevicePDT_IF::PDT_LB_Calcurate_FullAddr( JIG_ID jig, LB_PDT_ADDR addr )
{
	long addrSum = PDT_ADDR_START_LB;
	addrSum += m_Machine * PDT_ADDR_OFFSET_LB;
	addrSum += jig * SHUTTLE_ADDR_OFFSET_LB;
	return addrSum + addr;
}

long CDevicePDT_IF::MTP_LW_Calcurate_FullAddr( JIG_ID jig, LW_MTP_ADDR addr )
{
	long addrSum = MTP_ADDR_START_LW;
	addrSum += m_Machine * MTP_ADDR_OFFSET_LW;
	addrSum += jig * SHUTTLE_ADDR_OFFSET_LW;
	return addrSum + addr;
}

long CDevicePDT_IF::PDT_LW_Calcurate_FullAddr( JIG_ID jig, LW_PDT_ADDR addr )
{
	long addrSum = PDT_ADDR_START_LW;
	addrSum += m_Machine * PDT_ADDR_OFFSET_LW;
	addrSum += jig * SHUTTLE_ADDR_OFFSET_LW;
	return addrSum + addr;
}

CString CDevicePDT_IF::WORD_to_String( PSHORT pData, short nSizebyByte )
{
	// MAX 1024글자까지 처리 가능 [10/3/2017 OSC]
	const int nMaxSize = 1024;
	if(nSizebyByte > nMaxSize) nSizebyByte = nMaxSize-1;
	char cBuff[nMaxSize];
	memset(cBuff, NULL, nMaxSize);
	memcpy(cBuff, pData, nSizebyByte);
	wchar_t wszBuff[nMaxSize];

	CEtc::ConvertUnicode(cBuff, nSizebyByte, wszBuff, nMaxSize);
	wszBuff[nSizebyByte] = NULL;
	CString strText;
	strText.Format(_T("%s\n"), wszBuff);

	// PLC는 Space로 빈 공간을 채우기도 하므로 이건 필수 [10/3/2017 OSC]
	strText.TrimRight(_T(" "));
	strText.TrimRight(_T("\n"));
	strText.TrimRight(_T("\r"));

	return strText;
}

void CDevicePDT_IF::String_to_WORD( CString str, PSHORT pData, short nMaxBuffSizeByBite )
{
	// MAX 1024글자까지 처리 가능 [10/3/2017 OSC]
	const int nMaxSize = 1024;
	if(nMaxBuffSizeByBite > nMaxSize) nMaxBuffSizeByBite = nMaxSize;
	char szbuf[nMaxSize];
	CEtc::ConvertMultibyte((wchar_t*)str.GetString(), szbuf, nMaxSize);
	szbuf[nMaxSize-1] = NULL;

	int nCopySize = str.GetLength();
	if(nCopySize > nMaxBuffSizeByBite) nCopySize = nMaxBuffSizeByBite-1;
	memset(pData, NULL, nMaxBuffSizeByBite);
	memcpy(pData, szbuf, nCopySize);
}

void CDevicePDT_IF::SetMachineIndex( MACHINE_INDEX index )
{
	if( (index < MACHINE_1) || (index > MACHINE_3) )
		return;
	m_Machine = index;
}

BOOL CDevicePDT_IF::MTP_LB_Get( JIG_ID jig, LB_MTP_ADDR addr )
{
	return LB_Get(MTP_LB_Calcurate_FullAddr(jig, addr));
}

void CDevicePDT_IF::MTP_LB_Set( JIG_ID jig, LB_MTP_ADDR addr, BOOL bValue )
{
	if(addr == MTP_ABLE)
		theProcBank.m_bAbleOnStatus[jig] = bValue;
	return LB_Set(MTP_LB_Calcurate_FullAddr(jig, addr), bValue);
}

void CDevicePDT_IF::MTP_LB_Set( JIG_ID jig, JIG_CH ch, LB_MTP_ADDR addr, BOOL bValue )
{
	// CH1 어드레스를 넣어면 알아서 CH2로 변환시켜주자 [10/6/2017 OSC]
	//if( (ch == JIG_CH_2) && (addr < MTP_CH2_SURVIVAL) )
	//	addr = (LB_MTP_ADDR)(addr + MTP_CH2_SURVIVAL);

	MTP_LB_Set(jig, addr, bValue);
}

BOOL CDevicePDT_IF::PDT_LB_Get( JIG_ID jig, LB_PDT_ADDR addr )
{
	return LB_Get(PDT_LB_Calcurate_FullAddr(jig, addr));
}

CString CDevicePDT_IF::MTP_LW_ReadCellID( JIG_ID jig, JIG_CH ch )
{
	short sData[SIZE_CELL_ID+1];
	sData[SIZE_CELL_ID] = NULL;
	memset(sData, NULL, (SIZE_CELL_ID+1) *sizeof(short));
	LW_MTP_ADDR addr = (ch == JIG_CH_1) ? MTP_CH1_CELL_ID:MTP_CH2_CELL_ID;

	long ret =	LW_Read(MTP_LW_Calcurate_FullAddr(jig, addr), sData, SIZE_CELL_ID*2);
	if(ret != 0)
		return _T("COMM ERROR");

	return WORD_to_String(sData, SIZE_CELL_ID*2);
}

void CDevicePDT_IF::MTP_LW_WriteCellID( JIG_ID jig, JIG_CH ch, CString strCellID )
{
	LW_MTP_ADDR addr = ch == JIG_CH_1 ? MTP_CH1_CELL_ID:MTP_CH2_CELL_ID;
	short sData[SIZE_CELL_ID+1];
	String_to_WORD(strCellID, sData, SIZE_CELL_ID*2);

	long ret =	LW_Write(MTP_LW_Calcurate_FullAddr(jig, addr), sData, SIZE_CELL_ID*2);
	if(ret != 0)
	{
// 		return _T("COMM ERROR");
	}
}

CString CDevicePDT_IF::MTP_LW_ReadNGCode( JIG_ID jig, JIG_CH ch )
{
	short sData[SIZE_NG_CODE+1];
	sData[SIZE_NG_CODE] = NULL;
	memset(sData, NULL, (SIZE_NG_CODE+1) *sizeof(short));
	LW_MTP_ADDR addr = (ch == JIG_CH_1) ? MTP_CH1_NG_CODE:MTP_CH2_NG_CODE;

	long ret =	LW_Read(MTP_LW_Calcurate_FullAddr(jig, addr), sData, SIZE_NG_CODE*2);
	if(ret != 0)
		return _T("COMM ERROR");

	return WORD_to_String(sData, SIZE_NG_CODE*2);
}

void CDevicePDT_IF::MTP_LW_WriteNGCode( JIG_ID jig, JIG_CH ch, CString strCode )
{
	LW_MTP_ADDR addr = ch == JIG_CH_1 ? MTP_CH1_NG_CODE:MTP_CH2_NG_CODE;
	short sData[SIZE_NG_CODE+1];
	String_to_WORD(strCode, sData, SIZE_NG_CODE*2);

	long ret =	LW_Write(MTP_LW_Calcurate_FullAddr(jig, addr), sData, SIZE_NG_CODE*2);
	if(ret != 0)
	{
// 		return _T("COMM ERROR");
	}
}

CString CDevicePDT_IF::PDT_LW_ReadCellID(JIG_ID jig, JIG_CH ch)
{
	short sData[SIZE_CELL_ID+1];
	sData[SIZE_CELL_ID] = NULL;
	memset(sData, NULL, (SIZE_CELL_ID+1) *sizeof(short));
	LW_PDT_ADDR addr = (ch == JIG_CH_1) ? PDT_CH1_CELL_ID:PDT_CH2_CELL_ID;

	long ret =	LW_Read(PDT_LW_Calcurate_FullAddr(jig, addr), sData, SIZE_CELL_ID*2);
	if(ret != 0)
		return _T("COMM ERROR");

	return WORD_to_String(sData, SIZE_CELL_ID*2);
}

CDevicePDT_IF::ROBOT_STATUS CDevicePDT_IF::PDT_LW_ReadRobotStatus(JIG_ID jig, JIG_CH ch)
{
	short sData[SIZE_ROBOT_STATUS+1];
	sData[SIZE_ROBOT_STATUS] = NULL;
	memset(sData, NULL, (SIZE_ROBOT_STATUS+1) *sizeof(short));
	LW_PDT_ADDR addr = (ch == JIG_CH_1) ? PDT_CH1_ROBOT_STATUS:PDT_CH2_ROBOT_STATUS;

	long ret =	LW_Read(PDT_LW_Calcurate_FullAddr(JIG_ID_A, addr), sData, SIZE_ROBOT_STATUS*2);
	if(ret != 0)
		return PDT_ROBOT_ERR;

	return (ROBOT_STATUS)sData[0];
}

CString CDevicePDT_IF::PDT_LW_ReadRobotStatusString(JIG_ID jig, JIG_CH ch)
{
	ROBOT_STATUS status = PDT_LW_ReadRobotStatus(JIG_ID_A, ch);

	if(status == PDT_ROBOT_IDLE)
		return _T("IDLE");
	else if(status == PDT_ROBOT_RUN)
		return _T("RUN");
	else if(status == PDT_ROBOT_ERR)
		return _T("ERROR");
	else
		return _T("UNKNOWN");
}

int CDevicePDT_IF::PDT_LW_ReadRobotAlarmCode(JIG_ID jig, JIG_CH ch)
{
	short sData[SIZE_ROBOT_ALARM_CODE+1];
	sData[SIZE_ROBOT_ALARM_CODE] = NULL;
	memset(sData, NULL, (SIZE_ROBOT_ALARM_CODE+1) *sizeof(short));
	LW_PDT_ADDR addr = (ch == JIG_CH_1) ? PDT_CH1_ROBOT_ALARM_CODE:PDT_CH2_ROBOT_ALARM_CODE;

	long ret =	LW_Read(PDT_LW_Calcurate_FullAddr(JIG_ID_A, addr), sData, SIZE_ROBOT_ALARM_CODE*2);
	if(ret != 0)
		return -999;

	return (int)sData[0];
}

CString CDevicePDT_IF::PDT_LW_ReadRobotAlarmCodeString(JIG_ID jig, JIG_CH ch)
{
	int nCode = PDT_LW_ReadRobotAlarmCode(JIG_ID_A, ch);

	return CStringSupport::N2C(nCode);
}
