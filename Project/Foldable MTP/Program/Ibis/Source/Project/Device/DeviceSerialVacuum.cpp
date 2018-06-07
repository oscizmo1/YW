#include "StdAfx.h"
#include "DeviceSerialVacuum.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialVacuum::CDeviceSerialVacuum()
{
	m_Serial.m_QueueRead.SetStartChar(STX);
	m_Serial.m_QueueRead.SetEndChar(ETX);
	m_nReadCount	= 4;
	m_nReadSlot = 0;

	memset(m_dbVacValue, 0, sizeof(double)*eMAX_ADSlot*eMAX_ADChannel);

	m_dOffset = 0.;
}


CDeviceSerialVacuum::~CDeviceSerialVacuum(void)
{
}

BOOL CDeviceSerialVacuum::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_19200,8,ONESTOPBIT,EVENPARITY);
}

void CDeviceSerialVacuum::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialVacuum::IsOpen()
{
	return m_Serial.m_bConnected;
}

BOOL CDeviceSerialVacuum::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[100];
	int nReciveLength = m_Serial.m_QueueRead.GetDataStartChar_To_EndChar(buf, 99);
	if(nReciveLength < 1)	return FALSE;
	buf[nReciveLength] = NULL;

	int nSizeLimit = 8 + (m_nReadCount * 5);		// 최소 데이터 길이 = 해더 + 수신요청 데이터 수량 * 데이터당 길이
	char szValue[6];			// 필요한 만큼만 할당
	TCHAR szMsg[256] = { 0, };

	if(nReciveLength < nSizeLimit)
	{
		theLog[LOG_SERIAL].AddBuf(_T("RECIVE Length ERR %d/%d [%s]"), nReciveLength, nSizeLimit, buf);
		return FALSE;
	}

	if (	(buf[ ePOS_ResCode ]	= '0') &&		// Response Code가 '00' 인가 ?
			(buf[ ePOS_ResCode + 1 ]	= '0') )
	{
		//9, 14, 19, 24,
		// 9 + (0 * 5) = 9
		// 9 + (1 * 5) = 14
		// 9 + (2 * 5) = 19
		// 9 + (3 * 5) = 24
		int nStart = ePOS_DataStart;	// 9;				// 데이터 시작위치

		for (int i = 0; i < m_nReadCount; i++)
		{
			// nStart = 9 + (i * 5);
			nStart = ePOS_DataStart + (i * (eLEN_Data + eLEN_Delimiter));		// 데이터를 추출할 위치

			for (int j = 0; j < eLEN_Data; j++)
			{
				szValue[j]	= buf[ nStart + j ];			// 4Bytes를 추출한다.
				szMsg[j]	= szValue[j];
			}
			szValue[eLEN_Data]	= 0x00;				// 널문자로 종료 시킨다.
			szMsg[eLEN_Data]	= 0x00;

			m_sRcvData[m_nReadSlot][i].Format(_T("%s"), szMsg);					// 수신된 데이터 자체를 표시

			if ( szValue[0] != 'L' )		// 센서 연결이 끊어지지 않았다면, "LLLL"
			{
				m_dbVacValue[m_nReadSlot][i] = abs(atof(szValue) + m_dOffset);		// 실수형으로 전환한다.
			}
			else
				m_dbVacValue[m_nReadSlot][i] = 0.;		// 읽기 실패, 센서 입력 불량 으로 판정
		}//of for i
	}

	// 더이상 처리할게 없으면 TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetSizeStartChar_To_EndChar() <= 0)
		return TRUE;

	return FALSE;

}

void CDeviceSerialVacuum::ClearQueue()
{
	m_Serial.m_QueueRead.Clear();
}

// Checksum을 구한다.
// XOR한 값에 2의 보수를 만들어준다.
BYTE CDeviceSerialVacuum::GetChecksum(BYTE* pData, int nLen)
{
	BYTE bySum = pData[0];

	for (int i = 1; i < nLen; i++) 
	{
		// XOR 연산이 아님 bySum = bySum ^ pData[i];			// XOR 수행
		bySum = (bySum + pData[i]) & 0xFF;			// 합산 수행
	}

	return (( ~bySum + 1 ) & 0xFF);		// Byte size에 맞게 바꾸어 준다.
}
// 명령을 전송한다.
// Step은 0 ~
// Count는 1 ~
DWORD CDeviceSerialVacuum::SendReadCommand(int nCount)
{
	// 필요한 데이터만 전송한다.
	BYTE	bySend[32] = { 0x00, };
	int		nLen = 0;
	CString sCheck;

	int nBaseAddr =1000;
	int nNewAddr = nBaseAddr + (nCount * m_nReadSlot);			// 읽어올 주소를 기입


	CString sAddr;
	sAddr.Format(_T("%04d"),  nNewAddr);


	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';

	bySend[nLen++]	= 'R';				// 데이터 읽기
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '1';				// 데이터를 읽어 올 주소
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= 'W';

	// 데이터를 읽어 올 주소
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';
	bySend[nLen++]	= '1' + (nCount - 1);				// 최대 4개의 데이터를 읽어온다. -? 2015-07-31, jhLee, 5 - 8개를 읽을때 통신 수신이 안됨
	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum을 만든다.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum을 BYTE 2자리로 만들어 준다.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL

	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

// 입력 범위 설정 명령을 전송한다.
//
// [in] int nCh : 설정을 변경하고자 하는 채널 순번, 0 ~ 31
// [in] int nMode : 설정하고자 하는 범위 모드값
//
DWORD CDeviceSerialVacuum::SendRangeSetCommand(int nCh, int nMode)
{
	// 필요한 데이터만 전송한다.
	BYTE	bySend[32];
	int		nLen = 0;
	CString sCheck;

	int nNewAddr = 3000 + nCh;			// 설정을 변경할 채널 주소, 0 ~ 31

	CString sAddr, sValue;
	sAddr.Format(_T("%04d"),  nNewAddr);		// 주소를 문자열로 만들어준다.
	sValue.Format(_T("%04d"), nMode);			// 설정 모드를 만들어 준다. "0003"

	bySend[nLen++]	= 0x02;				// STX
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';
	bySend[nLen++]	= 'X';


	bySend[nLen++]	= 'W';				// 데이터 쓰기
	bySend[nLen++]	= 'S';
	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '3';				// 데이터를 쓸 주소
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '0';
	//bySend[nLen++]	= '4';

	// 데이터를 읽어 올 주소
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);				
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(2);	
	bySend[nLen++]	= (BYTE)sAddr.GetAt(3);	
	bySend[nLen++]	= 'W';

	bySend[nLen++]	= ',';

	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				
	//bySend[nLen++]	= '0';				// 설정값
	//bySend[nLen++]	= '3';				

	bySend[nLen++]	= (BYTE)sValue.GetAt(0);				
	bySend[nLen++]	= (BYTE)sValue.GetAt(1);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(2);	
	bySend[nLen++]	= (BYTE)sValue.GetAt(3);	

	bySend[nLen++]	= 0x03;				// ETX

	sCheck.Format(_T("%02X"), GetChecksum(bySend, nLen));		// Checksum을 만든다.

	bySend[nLen++]	= (BYTE)sCheck.GetAt(0);					// Checksum을 BYTE 2자리로 만들어 준다.
	bySend[nLen++]	= (BYTE)sCheck.GetAt(1);

	bySend[nLen++]	= 0x0D;		// CR
	bySend[nLen++]	= 0x0A;		// LF
	bySend[nLen]	= 0x00;		// NULL


	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	return nSendLen;
}

void CDeviceSerialVacuum::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
// 	m_Serial.m_serialMode = m_serialMode;
}