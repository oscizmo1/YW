#include "StdAfx.h"
#include "DeviceSerialTemperature.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialTemperature::CDeviceSerialTemperature(CString strSerialName)
{
	m_strSerialName = strSerialName;
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	m_serialMode = SERIAL_PC_BOX_TEMP;

	m_Serial.m_QueueRead.SetSTX(STX);
	//kjpark 20170922 온도센서 통신 검증
	m_Serial.m_QueueRead.SetETX(LF);

	for (int i = 0; i < eMAX_TEMPCount; i++) 
	{
		m_dbTempValue[i]	= 0.0;	
	}

	// 튀는 노이즈성 데이터를 방지하기 위한 울타리 경계
	//kjpark 20170922 온도센서 통신 검증
	m_nBoundLimit		= -1;						// 몇회 이상 상/하한 기준으로 반복되어어야 인정할 것인가 ?
	m_nUBoundCount		= 0;						// 상한으로 튀는 값 연속 발생 횟수
	m_nLBoundCount		= 0;						// 하한값 밑으로 튀는 값 연속 발생 횟수
	m_dbUBoundLimit	= 10;						// 상한으로 튀는 값 기준
	m_dbLBoundLimit	= 40;						// 하한으로 튀는 값 기준

	m_nDeviceAddr = 1;
}


CDeviceSerialTemperature::~CDeviceSerialTemperature(void)
{
}

BOOL CDeviceSerialTemperature::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	return m_Serial.InitCommPort(nPortNo,CBR_9600,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialTemperature::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_Serial.ClosePort();
}

BOOL CDeviceSerialTemperature::IsOpen()
{
	return m_Serial.m_bConnected;
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
//kjpark2017/09/21 온도센서 변경으로 통신 프로토콜 변경 테스트 중
void CDeviceSerialTemperature::SendCommand_TempValue(int nDeviceNum)
{
	// 필요한 데이터만 전송한다.
	BYTE bySend[32];
	BYTE byChecksum;
	int		nLen = 0;

	//char szToStr[8] = { 0x00, };
	//sprintf(szToStr, "%02d", nDeviceNum);

	CString sAddr;
	sAddr.Format(_T("%02d"),  nDeviceNum);

	bySend[nLen++]	= STX;		// STX
	bySend[nLen++]	= (BYTE)sAddr.GetAt(0);		// Address, 2
	bySend[nLen++]	= (BYTE)sAddr.GetAt(1);		

	bySend[nLen++]	= 'D';				// Command, 연속데이터 읽기 DRS
	bySend[nLen++]	= 'R';
	bySend[nLen++]	= 'S';

	bySend[nLen++]	= ',';				// Fixed

	bySend[nLen++]	= '0';				// Data count
	bySend[nLen++]	= '2';

	bySend[nLen++]	= ',';				// Fixed

	bySend[nLen++]	= '0';				// Read-Addr, 4, 데이터를 읽어 올 주소
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '0';
	bySend[nLen++]	= '1';

	byChecksum =  GetChecksum(bySend, nLen);		// Checksum을 만든다.
	CString strCheckSum;
	strCheckSum.Format(_T("%02X"),  byChecksum);

	bySend[nLen++]	= (BYTE)strCheckSum.GetAt(0);		// Cehcksum, 2
	bySend[nLen++]	= (BYTE)strCheckSum.GetAt(1);

	bySend[nLen++]	= CR;			//	0x0D	// carriage return
	bySend[nLen++]	= LF;			//	0x0A	// NL line feed, new line


	// COM Port로 전송한다.
	int nSendLen =  m_Serial.WriteComm( bySend, nLen );

	//m_nSendCount = (m_nSendCount + 1) % eMAX_Count;		// 데이터 전송 카운터 증가

	//return nSendLen;

}
// Checksum을 구한다.
// XOR한 값에 2의 보수를 만들어준다.
//kjpark 20170922 온도센서 통신 검증
BYTE CDeviceSerialTemperature::GetChecksum(BYTE* pData, int nLen)
{

	DWORD dwSum = 0;

	for(int i = 1 ; i < nLen ; i++)
	{
		dwSum = (dwSum + pData[i]);
	}
	return BYTE(dwSum & 0xFF);
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
//kjpark2017/09/21 온도센서 변경으로 통신 프로토콜 변경 테스트 중
BOOL CDeviceSerialTemperature::ParsingReciveValue()
{
	if(IsOpen() == FALSE) return FALSE;

	BYTE buf[200];
	BYTE buf1[200];
	BYTE buf2[200];
	int nReciveLength = m_Serial.m_QueueRead.GetDataSTX_To_ETX(buf2, 199);
	int nStartPos = 0;
	int nEndPos = 0;
	BOOL bCheckFlag = FALSE;

	if(nReciveLength < 1)
	{	
		return FALSE;
	}

	buf[nReciveLength] = NULL;	

	//           1         2
	// 012345678901234567890123
	//[ 0100000101000000000019 ]
	if ( nReciveLength > 0 )			// 수신된 데이터가 있는가 ?
	{
		int nPos = 0;
		long lReadValue = 0;
	
		for (int i = 0; i < nReciveLength; i++)
		{
			//kjpark 20170922 온도센서 통신 검증
			if (buf2[i] == STX )		// 종료 문자 LF가 존재하는가 ?
			{
				nStartPos = 0;			// 종료 문자 위치
			}
			buf1[nStartPos++] = buf2[i];

			//kjpark 20170922 온도센서 통신 검증
			if(nStartPos > 0 && buf2[i] == LF)
			{
				for(int j = 0; j < nStartPos; j++)
				{
					buf[j] = buf1[j];

				}
				nPos = nStartPos;
				break;
			}

		}	
			
		if ( nPos > 0 )
		{
			// 초기 문자가 STX이고, 데이터길이가  10바이트가 넘는가 ?
			if ( (nPos >= 23) && (buf[0] == 0x02) )		// 정상적인 데이터인가 ?
			{
				char szToStr[8] = { 0x00, };
				int nAddr = 0;
	
				// 주소 취득
				szToStr[0] = buf[1];
				szToStr[1] = buf[2];
				szToStr[2] = 0x00;
	
				nAddr = atoi( szToStr );										// 주소를 정수형으로 변경
	
				// 정상적인 응답인지 확인
				if (	(buf[7]	= 'O') &&		// 응답이 'OK' 인가 ?
					(buf[8]	= 'K') )
				{
					char szValue[8] = { 0x00, };			// 문자로 변활 할 공간 할당
					TCHAR szMsg[8] = { 0, };
					double dbValue = 0.0;		// 임시 값
	
	
					// Checksum을 점검한다.
					// Checksum 채취
					szValue[0] = buf[nPos - 3];
					szValue[1] = buf[nPos - 2];
					szValue[2] = 0x00;
	
					long lReadValue = strtol(szValue, NULL, 16);													// 16진수문자열을 정수형으로 변환
					BYTE byRcvChecksum = (BYTE)(lReadValue & 0xFF);												// 수신받은 Checksum
					//kjpark 20170922 온도센서 통신 검증
					BYTE byLocalChecksum = GetChecksum( buf, nPos - 4 );	// 재계산된 Checksum
	
					//TRACE(_T("- Checksum, Rcv:%c%c,%02X,    %02d,%02X\n"), szValue[0], szValue[1], byRcvChecksum, byLocalChecksum, byLocalChecksum);
	
					// Checksum이 일치하면 해당 데이터 사용 가능
					//if ( byRcvChecksum == byLocalChecksum )
					{
						// Data 부분만 취한다.
						szValue[0] = buf[10];
						szValue[1] = buf[11];
						szValue[2] = buf[12];
						szValue[3] = buf[13];
						szValue[4] = NULL;					// 널 문자
	
						lReadValue = strtol(szValue, NULL, 16);								// 16진수문자열을 정수형으로 변환
	
						for(int i = 0; i < 5; i++) szMsg[i] = szValue[i];
						szMsg[4] = 0x00;
						m_sRcvData[0].Format(_T("%s"), szMsg);							// 수신된 데이터 자체를 표시
	
						dbValue = lReadValue * 0.1;										// 온도값 취득
	
						// 튀는 값 잡아주는 부분

						// 높은값으로 튀는 경우
						if ( dbValue >= m_dbUBoundLimit )								
						{
							if ( m_nUBoundCount > m_nBoundLimit )						// 지정된 횟수이상 연속으로 들어왔다면 
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;								// 해당값을 인정하고 받아들인다.
								bCheckFlag = TRUE;
							}
							else
							{
								m_nUBoundCount++;
							}// 연속으로 들어 온 횟수를 증가시킨다.
						}
						else
						{
							if ( dbValue < m_dbLBoundLimit )							// 지정 하한값 아래로 들어왔다.
							{
								if ( m_nLBoundCount > m_nBoundLimit )					// 지정된 횟수이상 연속으로 들어왔다면 
								{
									m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// 해당값을 인정하고 받아들인다.
									bCheckFlag = TRUE;
								}
								else
								{
									m_nLBoundCount++;									// 연속으로 들어 온 횟수를 증가시킨다.
								}
							}
							else		// 범위안에 들어오는 값이다.
							{
								m_dbTempValue[m_nDeviceAddr-1] = dbValue;							// 해당값을 인정하고 받아들인다.
								bCheckFlag = TRUE;
								m_nUBoundCount = 0;
								m_nLBoundCount = 0;
							}	
						}						
	
					} //of if Checksum
				}//of if 응답코드가 정상적이라면
			}//of Correct Data			
		}//of if CR Received ?
	
		// 처리완료 
	
		if(bCheckFlag == TRUE)
		{
			bCheckFlag = FALSE;
			//IncDeviceAddr();
		}
	}
	// 더이상 처리할게 없으면 TRUE [12/7/2016 OSC]
	if(m_Serial.m_QueueRead.GetDataSTX_To_ETX() <= 0)
		return TRUE;

	return FALSE;
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
//주소값 증가
void CDeviceSerialTemperature::IncDeviceAddr(void)
{
	//Receive를 먼저하고  Send 를 하기때문에 1-6까지 증가하고
	if(m_nDeviceAddr < eMAX_TEMPCount)
	{
		m_nDeviceAddr++;
	}
	else
	{
		m_nDeviceAddr = 1;  //6에서 Receive하면 1로 초기화
	}

}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
// 지정 순번의 공압 값을 읽어온다.
double CDeviceSerialTemperature::GetTempValue(int nIdx)
{
	if ( (nIdx < 0) || (nIdx >= eMAX_TEMPCount) ) return 0.0;

	return m_dbTempValue[nIdx];	
}
void CDeviceSerialTemperature::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}