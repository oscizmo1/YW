#pragma once
#include "stdafx.h"
#include "Device\SerialManager.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW

#endif

CSerialManager::CSerialManager(void)
{	
	m_bFirst = TRUE;
	m_bADBoardReadable = FALSE;
}

//20160930 kjpark Serial IF
CSerialManager::~CSerialManager(void)
{		
}

DWORD CSerialManager::SerialSendData(SERIAL_MODE nPort, BYTE *pBuff, DWORD nToWrite)
{
	return 0;
}

int CSerialManager::SerialGetDataBuffer(SERIAL_MODE nPort, BYTE *cstr, int nMaxSize)
{
	return 0;
}

//20160930 kjpark Serial IF
BOOL CSerialManager::SerialOpen(SERIAL_MODE nPort)
{
	BOOL bRet = FALSE;
	
	switch(nPort)
	{
	case SERIAL_GMS:
		{
			if(m_DeviceGMS1.IsOpen() == FALSE) 
			{
				bRet = m_DeviceGMS1.SerialOpen(SerialModeSet[nPort].nPort);
				if(bRet)
					m_DeviceGMS1.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
			}
		}
		break;
	case SERIAL_LABELPRINT:
		{
			if(!m_DeviceLabelPrinter.GetCommThreadHandler()->m_bConnected) 
			{
				bRet = m_DeviceLabelPrinter.SerialOpen(SerialModeSet[nPort].nPort);
				if(bRet)
					m_DeviceLabelPrinter.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
			}
		}
		break;	
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	case SERIAL_PC_BOX_TEMP:
		{
			if(!m_pDevicePCTemperature.GetCommThreadHandler().m_bConnected) 
			{
				bRet = m_pDevicePCTemperature.SerialOpen(SerialModeSet[nPort].nPort);
				if(bRet)
					m_pDevicePCTemperature.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
			}
		}
		break;	
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	case SERIAL_VACUUM_MAIN:
		{
			if(m_DeviceVacuumMain.IsOpen() == FALSE) 
			{
				bRet = m_DeviceVacuumMain.SerialOpen(SerialModeSet[nPort].nPort);
				if(bRet)
					m_DeviceVacuumMain.SetSerialName(SerialModeSet[nPort].m_sPortNickName);
			}
		}
		break;	
	}
	
	return bRet;
	
}

//20160930 kjpark Serial IF
BOOL CSerialManager::SerialClose(SERIAL_MODE nPort)
{
	BOOL bRet = FALSE;

	switch(nPort)
	{
	case SERIAL_GMS:
		{	
			m_DeviceGMS1.SerialClose();			
		}
		break;
	case SERIAL_LABELPRINT:
		{	
			m_DeviceLabelPrinter.SerialClose();			
		}
		break;	
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	case SERIAL_PC_BOX_TEMP:
		{	
			m_pDevicePCTemperature.SerialClose();			
		}
		break;
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	case SERIAL_VACUUM_MAIN:
		{	
			m_DeviceVacuumMain.SerialClose();			
		}
		break;		
	}

	return bRet;
}
BOOL CSerialManager::SerialCodeSend(int nCode, int nPort)
{
	return FALSE;
}

BOOL CSerialManager::SerialAckCheck(SERIAL_MODE nPort)
{
	CString szData;
	BYTE buffer[256];

	int nLength = nLength = SerialGetDataBuffer(SerialModeSet[nPort].SerialMode, buffer, 255);
		
	if(nLength > 0)
	{
		buffer[255] = NULL;
		for(int nBuf=0;nBuf<nLength;nBuf++)
		{
			if(buffer[nBuf] == ACK)	// 0x06	: acknowledge
				return TRUE;
		}
	}

	return FALSE;
}

void CSerialManager::SendCommand()
{
}

//20160930 kjpark Serial IF
void CSerialManager::PacketReceive(SERIAL_MODE serialMode, BYTE *byBuf, int nSize)
{

}


void CSerialManager::SendSetVacRange()
{
	// 	m_nRangeSetCh	// 범위를 설정하고자하는 채널 0 ~ 31
	// 	m_nRangeSetMode	// 범위를 설정하고자 하는 모드, 0 ~ 3까지 해서 양수인 값으로넣자
	// 0 : -100 ~ 100 kPa
	// 1 : 10 ~ -101 Kpa
	// 2 : -10 ~ 101 Kpa
	// 3 : -0.10 ~ 1.00 Mpa
	// 걍 0으로 하고 abs로 다 양수로 만들어주자 ㅋ

	// ... 뭔가 읽으려는 슬롯마다 다 해줘야 할듯

	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//kjpark 20170922 AD 보드 통신 검증
	m_DeviceVacuumMain.SendRangeSetCommand(3, 2);	// LD-CH16
	//kjpark 20170928 Adboard Read
	m_DeviceVacuumMain.SendRangeSetCommand(0, 2);	// LD-CH16
	m_DeviceVacuumMain.SendRangeSetCommand(1, 2);	// LD-CH16


}

//kjpark 20170928 Adboard Read
void CSerialManager::SendVacRead(int nSlot)
{
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//kjpark 20170922 AD 보드 통신 검증

	m_DeviceVacuumMain.m_nReadSlot =nSlot;
	m_DeviceVacuumMain.SendReadCommand(4);

}

void CSerialManager::SendVacReadLD()
{
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//kjpark 20170922 AD 보드 통신 검증
	m_DeviceVacuumMain.m_nReadSlot =3;
	m_DeviceVacuumMain.SendReadCommand(4);
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
BOOL CSerialManager::ParsingLDMainVacuumValue()
{
	return m_DeviceVacuumMain.ParsingReciveValue();
}

void CSerialManager::CopyLoadADBoardValue_to_FDCBank()
{
	//kjpark 20170922 AD 보드 통신 검증
	//kjpark 20170928 Adboard Read
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	theFDCBank.m_strLoadMainAir.Format(_T("%f,%f,%f"), m_DeviceVacuumMain.m_dbVacValue[3][0], m_DeviceVacuumMain.m_dbVacValue[3][0], m_DeviceVacuumMain.m_dbVacValue[3][0]);
	// Vacuum값은 (-)값을 보고한다(고객사 기준) - LSH171207
	theFDCBank.m_strWorkTableVac[JIG_ID_A][JIG_CH_1].Format(_T("%f,%f,%f"), m_DeviceVacuumMain.m_dbVacValue[0][0]*(-1), m_DeviceVacuumMain.m_dbVacValue[0][0]*(-1), m_DeviceVacuumMain.m_dbVacValue[0][0]*(-1));
	theFDCBank.m_strWorkTableVac[JIG_ID_B][JIG_CH_1].Format(_T("%f,%f,%f"), m_DeviceVacuumMain.m_dbVacValue[0][2]*(-1), m_DeviceVacuumMain.m_dbVacValue[0][2]*(-1), m_DeviceVacuumMain.m_dbVacValue[0][2]*(-1));
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
void CSerialManager::SendLabelPrintOut(CCellInfo *pCell)
{
	if(pCell == NULL)
		return;

	CString sMsg;
	CString sDefect = _T("Test Print"); 

// 	sMsg.Format(_T("SM%d,%d"), 130, 40);	// D-AMT 마진
	sMsg.Format(_T("SM%d,%d"), 110, 30);	// X 115, Y 40
	m_DeviceLabelPrinter.LabelPrintOut(-1, sMsg);		// 상/하 마진 설정

	m_DeviceLabelPrinter.LabelPrintOut(0, pCell->defaultData.m_strCellID);		// Cell ID 출력
	m_DeviceLabelPrinter.LabelPrintOut(1, pCell->defaultData.MesCode) ;			// 불량 코드
	m_DeviceLabelPrinter.LabelPrintOut(2, pCell->defaultData.m_strLastResult);		// 불량명

	CTime dateTm = CTime::GetCurrentTime();
	CString strLineMsg;

	strLineMsg.Format(_T("%04d%02d%02d_%s"),
		dateTm.GetYear(), dateTm.GetMonth(),dateTm.GetDay(), theProcBank.m_strEqipID);						// 시간 + LIne명;

	m_DeviceLabelPrinter.LabelPrintOut(3, strLineMsg );		// 시간+Line명

	m_DeviceLabelPrinter.TestBarcodePrint(pCell->defaultData.m_strCellID);		// 2차원 바코드 출력


	m_DeviceLabelPrinter.LabelPrintOut(-1, _T("P1") );			// 실제 출력 명령
	theProcBank.m_bLabelPrintOut = TRUE;
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
int CSerialManager::GetDeviceAddr()
{
	return m_pDevicePCTemperature.m_nDeviceAddr;
}

BOOL CSerialManager::ParsingTemperatureValue()
{
	return m_pDevicePCTemperature.ParsingReciveValue();

}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
void CSerialManager::CopyTempValue_to_FDCBank(int nAddr)
{
	int nBuff = (int)(nAddr - 1);
	
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	switch(nBuff)
	{
	case eIDX_TEMP_UTIL_BOX:
		theFDCBank.m_strUtilBox_Temp.Format(_T("%f,%f,%f"), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_UTIL_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_UTIL_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_UTIL_BOX));
		break;
	case eIDX_TEMP_ETC_BOX:
		theFDCBank.m_strEtcBox_Temp.Format(_T("%f,%f,%f"), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_ETC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_ETC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_ETC_BOX));
		break;
	case eIDX_TEMP_SERVO_BOX:
		theFDCBank.m_strServoBox_Temp.Format(_T("%f,%f,%f"), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_SERVO_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_SERVO_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_SERVO_BOX));
		break;
	case eIDX_TEMP_CTRL_PC_BOX:
		theFDCBank.m_strCtrlPCBox_Temp.Format(_T("%f,%f,%f"), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_CTRL_PC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_CTRL_PC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_CTRL_PC_BOX));
		break;
	case eIDX_TEMP_PG_PC_BOX:
		theFDCBank.m_strPGPcBox_Temp.Format(_T("%f,%f,%f"), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_PG_PC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_PG_PC_BOX), m_pDevicePCTemperature.GetTempValue(eIDX_TEMP_PG_PC_BOX));
		break;
	}
}

//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
void CSerialManager::SendTempRead(int nAddr)
{
	m_pDevicePCTemperature.SendCommand_TempValue(nAddr);
}