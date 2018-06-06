#pragma once

#include "Serial/CommThread.h"

#define ERR_VALUE	-9999

// enum DEVICE_PRINETR_MODE
// {	
// 	DEVICE_PRINETR_PRESS_VALUE = 0,
// 	DEVICE_PRINETR_REFERENCE_SET,
// 	DEVICE_PRINETR_HOLD_START,	
// 	DEVICE_PRINETR_HOLD_STOP,
// 	DEVICE_PRINETR_MAX
// };

class CDeviceSerialPrinter
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialPrinter(CString strSerialName = _T(""));
	~CDeviceSerialPrinter(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

// 	void SendCommand(DEVICE_PRINETR_MODE mode, BYTE* buf, int nBufSize);
	CCommThread* GetCommThreadHandler() {return &m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
	SERIAL_MODE m_serialMode;

	//kjpark 20161004 Label Printer IF 추가
	BOOL TestPrintOut();
	BOOL TestBarcodePrint(CString sMsg);

	DWORD LabelPrintOut(int nLn, CString sMsg);

	//////////////////////////////////////////////////////////////////////////
	// Label Printer 상태 반환 요청 [7/4/2017 OSC]
	enum PRINTER_STATE
	{
		PRINTER_READY =				0x00,
		PRINTER_NOT_USE_01 =		0x01,
		PRINTER_NOT_USE_02 =		0x02,
		PRINTER_NO_RIBON =			0x04,
		PRINTER_SENSING_GAP_NG =	0x08,
		PRINTER_TPH_OVERHIT =		0x10,
		PRINTER_CUTTER_JAM =		0x20,
		PRINTER_COVER_OPEN =		0x40,
		PRINTER_NO_PAPER =			0x80
	};
	enum WORK_STATE
	{
		WORK_READY =			0x00,
		WORK_NOT_USE_01 =		0x01,
		WORK_NOT_USE_02 =		0x02,
		WORK_NOT_USE_04 =		0x04,
		WORK_NOT_USE_08 =		0x08,
		WORK_NOT_USE_10 =		0x10,
		WORK_LABEL_ON_PEEPER =	0x20,
		WORK_LABEL_PRINTING =	0x40,
		WORK_BUFFER_PROCESS =	0x80
	};
	BOOL m_bStateRecived;	// 한번이라도 응답을 받은 적이 있으면 TRUE. 반응하는지 확인용
	PRINTER_STATE m_PrinterState;
	WORK_STATE m_WorkState;
	void RequirePrinterState();
	void GetPrinterState();
	//////////////////////////////////////////////////////////////////////////
};

