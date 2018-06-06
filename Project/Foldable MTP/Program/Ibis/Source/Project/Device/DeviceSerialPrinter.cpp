#include "StdAfx.h"
#include "DeviceSerialPrinter.h"
#include "IbisApp.h"
#include "Etc/Etc.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CDeviceSerialPrinter::CDeviceSerialPrinter(CString strSerialName)
{
	m_strSerialName = strSerialName;
//kjpark 20161010 라벨프린터 PG PC 에서 사용	
// 	m_serialMode = SERIAL_LABELPRINT;
}


CDeviceSerialPrinter::~CDeviceSerialPrinter(void)
{
}

BOOL CDeviceSerialPrinter::SerialOpen( short nPortNo )
{
	if(IsOpen()) return TRUE;

	CString str;

	m_bStateRecived = FALSE;
	return m_Serial.InitCommPort(nPortNo,CBR_115200,8,ONESTOPBIT,NOPARITY);
}

void CDeviceSerialPrinter::SerialClose()
{
	if(IsOpen() == FALSE) return;

	m_bStateRecived = FALSE;
	m_Serial.ClosePort();
}

BOOL CDeviceSerialPrinter::IsOpen()
{
	return m_Serial.m_bConnected;
}

// void CDeviceSerialPrinter::SendCommand(DEVICE_PRINETR_MODE mode, BYTE* buf, int nBufSize)
// {
// 	int nCheckSum = 0;
// 	for(int i = 1; i < nBufSize-3; i++)
// 	{
// 		nCheckSum += buf[i];
// 	}
// 	CStringA strCheckSum;
// 	strCheckSum.Format("%X", nCheckSum);
// 	// 	char cCheckSum[4];
// 	// 	_itoa_s(nCheckSum, cCheckSum, 4, 16);
// 	// 	buf[7] = cCheckSum[1];
// 	// 	buf[8] = cCheckSum[2];
// 	buf[nBufSize-3] = strCheckSum.GetAt(1);
// 	buf[nBufSize-2] = strCheckSum.GetAt(2);
// 	m_Serial.WriteComm(buf, nBufSize);
// 	WCHAR a[100];
// 	CEtc::ConvertUnicode((char*)buf, nBufSize, a, 100);
// 	a[nBufSize] = NULL;
// 	theLog[LOG_SERIAL].AddBuf(_T("SEND %s"), a);
// }

void CDeviceSerialPrinter::SetSerialName(CString strValue)
{
	m_strSerialName = strValue;
	m_Serial.m_serialMode = m_serialMode;
}

//kjpark 20161004 Label Printer IF 추가
BOOL CDeviceSerialPrinter::TestBarcodePrint(CString sMsg)
{
	// 포트가 열린 경우에만 출력한다.
	if ( m_Serial.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nSize = 3;
	int nPosX = 160;
	int nPosY = 0;

	// Label Printer에 맞는 명령문을 만들어 준다.
	sSendMsg.Format( _T("B2%d,%d,D,%d,N,\'%s\'%c%c"), nPosX, nPosY, nSize, sMsg, 0x0d, 0x0a);		

	nLen = sSendMsg.GetLength();				// 전송할 글자 수

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR로변환
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char로 변환

	return  m_Serial.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}

//kjpark 20161004 Label Printer IF 추가
// Test출력을 한다.
BOOL CDeviceSerialPrinter::TestPrintOut()
{
	CString sMsg;
	CString sDefect = _T("Test Print"); 

	sMsg.Format(_T("SM%d,%d"), 130, 40);
	LabelPrintOut(-1, sMsg);						// 상/하 마진 설정

	LabelPrintOut(0, sDefect);						// Cell ID 출력
	LabelPrintOut(1, _T("U123456")) ;				// 불량 코드
	LabelPrintOut(2, _T("Label Test Print") );		// 불량명

	CTime dateTm = CTime::GetCurrentTime();
	CString strLineMsg;

	strLineMsg.Format(_T("%04d%02d%02d_LAVEL_PRINTER_TEST"),dateTm.GetYear(), dateTm.GetMonth(),dateTm.GetDay());						// 시간 + LIne명;

	LabelPrintOut(3, strLineMsg );		// 시간+Line명

	TestBarcodePrint(_T("A2EG1S53DOKAD024"));			// 2차원 바코드 출력


	return LabelPrintOut(-1, _T("P1") ) > 0;					// 실제 출력 명령
}

//kjpark 20161004 Label Printer IF 추가
DWORD CDeviceSerialPrinter::LabelPrintOut(int nLn, CString sMsg)
{
	// 포트가 열린 경우에만 출력한다.
	if ( m_Serial.m_bConnected != TRUE ) return 0;

	CString sSendMsg;
	TCHAR cTCCmd[eMAX_PrinterBuffer];
	char	cSendData[eMAX_PrinterBuffer];


	int nLen = 0;
	int nHeight = 0;
	int nGap = 0;

	if ( nLn < 0 )			// 줄 번호가 음수라면, 특수 지정 명령이다.
	{
		sSendMsg.Format( _T("%s%c%c"), sMsg, 0x0d, 0x0a);				// 뒤에 CR/LF를 붙여준다.
	}
	else
	{
		switch ( nLn )
		{
		case 0 : nHeight = 0;		break;
		case 1 : nHeight = 20;		break;
		case 2 : nHeight = 40;		break;
		case 3 : nHeight = 60;		break;
		case 4 : nHeight = 80;		break;
		default :
			nHeight = nLn * 20;
		}//of switch

		// Label Printer에 맞는 명령문을 만들어 준다.
		sSendMsg.Format( _T("T%d,%d,c,1,1,0,0,N,N,\'%s\'%c%c"), nGap, nHeight, sMsg, 0x0d, 0x0a);		
	}//of else


	nLen = sSendMsg.GetLength();				// 전송할 글자 수

	_tcscpy_s(cTCCmd,	_countof(cTCCmd),	sSendMsg);	// CString -> TCHAR로변환
	::WideCharToMultiByte(CP_ACP, NULL, cTCCmd, -1, cSendData, eMAX_PrinterBuffer, NULL, FALSE);		// TCHAR -> char로 변환
	
	return  m_Serial.WriteComm( (BYTE*)cSendData, strlen( cSendData ) );// nLen );
}

void CDeviceSerialPrinter::RequirePrinterState()
{
	m_bStateRecived = FALSE;
	LabelPrintOut(-1, _T("^cp"));
}

void CDeviceSerialPrinter::GetPrinterState()
{
	int nLen = m_Serial.m_QueueRead.GetSize();
	if(nLen > 0)
	{
		nLen = nLen > 99 ? 99:nLen;
		BYTE buffer[100];
		memset(buffer, NULL, 100);
		m_Serial.m_QueueRead.GetData(buffer, nLen);

		// 혹시나 깨지거나 해서 한바이트 남으면 어느건지 모르니 버리자 [7/4/2017 OSC]
		if(nLen >= 2)
		{
			m_PrinterState = (PRINTER_STATE)buffer[nLen-2];
			m_WorkState = (WORK_STATE)buffer[nLen-1];
			m_bStateRecived = TRUE;
		}
	}
}
