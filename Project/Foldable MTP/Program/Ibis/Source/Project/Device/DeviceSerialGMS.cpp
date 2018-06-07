#include "StdAfx.h"
#include "DeviceSerialGMS.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialGMS::CDeviceSerialGMS(CString strSerialName)
{
	m_strSerialName = strSerialName;
	m_Serial.m_QueueRead.SetStartChar(DEL);
	m_Serial.m_QueueRead.SetEndChar(ETX);

	m_LastPGMS = _T("2");
	for(int i = 0; i < GMS_MODULE_CH_MAX; i++)
		//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
		m_LastGMSValue[i] = _T("0.501000,0.501000,0.501000");
}


CDeviceSerialGMS::~CDeviceSerialGMS(void)
{
}

BOOL CDeviceSerialGMS::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	CString str;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialGMS::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialGMS::IsOpen()
{
	return m_Serial.m_bConnected;
}

void CDeviceSerialGMS::SendCommand_Read(GMS_MODULE_CH ch)
{
	// LOADWS   [0x10][0x02]FORCE_TOUCH_CH001_GMS__123402[0x03]
	// LOADWS   [0x10][0x02]FORCE_TOUCH_CH003_PGMS_12340?[0x03]
	CStringA strBody, strPacket;
	switch(ch)
	{
	case GMS_CH001_GMS: 
		strBody.Format("%c%c%s%s%c", DEL, STX, "LOADWS   ", GMS_NICKNAME_JIG_GMS, PGMS_BUZZER_OFF);
		break;
	case GMS_CH002_PGMS: 
		strBody.Format("%c%c%s%s%c", DEL, STX, "LOADWS   ", GMS_NICKNAME_BZONE_PGMS, PGMS_BUZZER_OFF);
		break;
	}
	strPacket.Format("%s%c%c", strBody, GetChecksum((BYTE*)strBody.GetString(), strBody.GetLength()), ETX);
	int nLength = strPacket.GetLength();

	m_Serial.WriteComm((BYTE*)strPacket.GetString(), nLength);

// 	WCHAR a[100];
// 	CEtc::ConvertUnicode((char*)strPacket.GetString(), nLength, a, 100);
// 	a[nLength] = NULL;
// 	theLog[LOG_SERIAL_GMS].AddBuf(_T("SEND %s"), a);
}

// Checksum을 구한다.
// XOR한 값에 2의 보수를 만들어준다.
BYTE CDeviceSerialGMS::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = pData[2];

	// DLE, STX를 제외하고 계산한다.
	for (int i = 3; i < nLen; i++) 
	{
		bySum = bySum ^ pData[i];			// XOR 수행
	}

	// 만약 Check sum 값이 특정한 값을 가지면 0xff로 대체한다.
	if( (bySum == 0x00) || (bySum == 0x10) || (bySum == 0x0d) )
	{
		bySum = 0xff;
	}

	return bySum;							// 계산된 데이터 전송
}

BOOL CDeviceSerialGMS::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[100];
	int nReciveLength = m_Serial.m_QueueRead.GetDataStartChar_To_EndChar(buf, 99);
	if(nReciveLength < 1)	return FALSE;
	buf[nReciveLength] = NULL;
	if(nReciveLength != 60)
	{
		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECIVE Length ERR %d [%s]"), nReciveLength, buf);
		return FALSE;
	}


	// NickName 확인 [12/7/2016 OSC]
	int nSize = strlen(GMS_NICKNAME_JIG_GMS);
	BYTE bufNickName[sizeof(GMS_NICKNAME_JIG_GMS)];
	memcpy(bufNickName, &buf[11], nSize);
	bufNickName[nSize] = NULL;
	if(strcmp((char *)bufNickName, GMS_NICKNAME_JIG_GMS) == 0)
	{
		// GMS
		char GMS_VALUE[GMS_SUB_CH_MAX][GMS_VALUE_SIZE+1];
		CStringA strValue;
		for(int i = 0; i < GMS_SUB_CH_MAX; i++)
		{
			memcpy(GMS_VALUE[i], &buf[ 38 + (i*GMS_VALUE_SIZE) ], GMS_VALUE_SIZE);
			GMS_VALUE[i][GMS_VALUE_SIZE] = NULL;
			strValue.Format("%s", GMS_VALUE[i]);
			// 999로 시작하면 저항값이 아님
			if(strValue.Left(3) == GMS_VALUE_ERR)
			{
				m_LastGMSValue[i] = strValue;
			}
			else
			{
				m_LastGMSValue[i].Format(_T("%f"), atoi(GMS_VALUE[i])/100.);	// 100을 나눠야 단위가 맞다
			}
		}

		// GMS 5번채널은 사용하지 않는다. 하지만 CIM에는 무조건 5개씩 보고해야 한다. [12/16/2016 OSC]
		m_LastGMSValue[GMS_SUB_CH5] = GMS_VALUE_CH_DISABLE;
// 		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECV GMS %s, %s, %s, %s"), 
// 			m_LastGMSValue[GMS_A_JIG], m_LastGMSValue[GMS_B_JIG],
// 			m_LastGMSValue[GMS_C_JIG], m_LastGMSValue[GMS_D_JIG] );
		theFDCBank.CheckGMSValue();
	}
	else if(strcmp((char *)bufNickName, GMS_NICKNAME_BZONE_PGMS) == 0)
	{
		// PGMS
		CStringA str;
		str.Format("%c", buf[57]);
		m_LastPGMS.Format(_T("%d"), atoi(str));
// 		theLog[LOG_SERIAL_GMS].AddBuf(_T("RECV B ZONE PGMS %s"), m_LastBZonePGMS);
		theFDCBank.CheckPGMSValue();
	}	

	// 더이상 처리할게 없으면 TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetSizeStartChar_To_EndChar() <= 0)
		return TRUE;

	return FALSE;
}

void CDeviceSerialGMS::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
}