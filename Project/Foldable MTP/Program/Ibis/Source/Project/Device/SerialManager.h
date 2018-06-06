#pragma once

#include "Serial\CommThread.h"
#include <vector>
#include "DeviceSerialGMS.h"
#include "DeviceSerialPrinter.h"
#include "DeviceSerialServoTorque.h"
#include "DeviceSerialTemperature.h"
#include "DeviceSerialVacuum.h"


using namespace std;

struct SERIAL_MODE_SET
{
	SERIAL_MODE SerialMode;
	int			nPort;
	CString		m_sPortNickName;
};


static SERIAL_MODE_SET SerialModeSet[] = 
{	
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	//kjpark 20170922 온도센서 통신 검증
	//kjpark 20170922 GMS 통신 완료
	{SERIAL_VACUUM_MAIN				, 8, _T("SERIAL_VACUUM_MAIN")},
	{SERIAL_PC_BOX_TEMP				, 4, _T("SERIAL_PC_BOX_TEMP")},	
	{SERIAL_GMS						, 7, _T("SERIAL_GMS")},
	{SERIAL_LABELPRINT				, 9, _T("SERIAL_LABELPRINT")},
	
};

class CSerialManager
{
public:
	CSerialManager(void);
	~CSerialManager(void);

	std::vector<CCommThread> m_vctNPortReal;
	std::vector<std::vector<unsigned char>> m_vctStrData; 

	DWORD	SerialSendData(SERIAL_MODE nPort, BYTE *pBuff, DWORD nToWrite); 
	int		SerialGetDataBuffer(SERIAL_MODE nPort, BYTE *cstr, int nMaxSize);

	BOOL SerialOpen(SERIAL_MODE nPort);
	BOOL SerialClose(SERIAL_MODE nPort);
	BOOL SerialCodeSend(int nCode, int nPort);
	BOOL SerialAckCheck(SERIAL_MODE nPort);
		
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	CDeviceSerialVacuum* GetVacuumMainHandler() {return &m_DeviceVacuumMain;}
	CDeviceSerialTemperature* GetPCTemperatureHandler() {return &m_pDevicePCTemperature;}
	CDeviceSerialGMS* GetDeviceGMSHandler1() {return &m_DeviceGMS1;}
	CDeviceSerialGMS* GetDeviceGMSHandler2() {return &m_DeviceGMS2;}
	CDeviceSerialPrinter* GetLabelPrinterHandler() {return &m_DeviceLabelPrinter;}	
	
	void SendCommand();		
	void PacketReceive(SERIAL_MODE serialMode, BYTE *byBuf, int nSize);

	//////////////////////////////////////////////////////////////////////////
	// Vacuum [12/11/2016 OSC]
public:
	void SendSetVacRange();

	//kjpark 20170928 Adboard Read
	void SendVacRead(int nSlot);
	void SendVacReadLD();
	void SendVacReadUD();

	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	BOOL ParsingLDMainVacuumValue();
		
	int GetDeviceAddr();
	BOOL ParsingTemperatureValue();
	void CopyTempValue_to_FDCBank(int nAddr);
	void SendTempRead(int nAddr);

private:
	BOOL m_bFirst;
public:
	// 한번에 하나씩 밖에 못 읽기 때문에 프로그램 껏다 키면 전부 다 받기 전 까지는 0인 값이 존재한다.
	// 전부다 받았는지 확인 위해 아래 BOOL 변수 추가
	BOOL m_bADBoardReadable;

	void CopyLoadADBoardValue_to_FDCBank();	
	//////////////////////////////////////////////////////////////////////////
	// Label Printer [12/16/2016 OSC]
	void SendLabelPrintOut(CCellInfo *pCell);
	//////////////////////////////////////////////////////////////////////////
		
private:	
	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어
	CDeviceSerialVacuum m_DeviceVacuumMain;
	CDeviceSerialTemperature m_pDevicePCTemperature;
	CDeviceSerialGMS m_DeviceGMS1;	
	CDeviceSerialGMS m_DeviceGMS2;	
	CDeviceSerialPrinter m_DeviceLabelPrinter;
	
	FDC_ALARM_STATE m_stateMainAir;
	CStopWatch m_timerMainAirAlarm;

};
