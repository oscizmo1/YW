#pragma once

#include "Serial/CommThread.h"

#define GMS_NICKNAME_JIG_GMS	"FTI_CH001_GMS__ALL_JIG_1234"	// ������ 27����
#define GMS_NICKNAME_BZONE_PGMS	"FTI_CH002_PGMS_BZONE_123456"	// ������ 27����
#define GMS_NICKNAME_DZONE_PGMS	"FTI_CH003_PGMS_DZONE_123456"	// ������ 27����

#define PGMS_BUZZER_ON	'1'
#define PGMS_BUZZER_OFF	'0'

enum GMS_MODULE_CH
{
	GMS_CH001_GMS = 0,
	//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
	GMS_CH002_PGMS,	
	GMS_MODULE_CH_MAX
};

enum GMS_SUB_CH
{
	GMS_SUB_CH1 = 0,	// Device1 : A Jig, Device2 : C Jig
	GMS_SUB_CH2,		// �̻��
	GMS_SUB_CH3,		// B Jig
	GMS_SUB_CH4,		// �̻��
	GMS_SUB_CH5,		// �̻��
	GMS_SUB_CH_MAX
};

#define GMS_VALUE_SIZE	4
#define GMS_VALUE_ERR			"999"
#define GMS_VALUE_CH_DISABLE	"9997"
#define GMS_VALUE_RANGE_OVER	"9998"
#define GMS_VALUE_OPEN			"9999"

class CDeviceSerialGMS
{
private:
	CCommThread m_Serial;
	CString m_strSerialName;
public:
	CDeviceSerialGMS(CString strSerialName = _T(""));
	~CDeviceSerialGMS(void);


	BOOL SerialOpen(short nPortNo);
	void SerialClose();
	BOOL IsOpen();

	void SendCommand_Read(GMS_MODULE_CH ch);	// bGMS is TRUE => GMS, else PGMS
	BYTE GetChecksum(BYTE* pData, int nLen);
	BOOL ParsingReciveValue();

	CString m_LastGMSValue[GMS_SUB_CH_MAX];
	CString m_LastPGMS;	// 0:��NG S/W On
												// 1:��OK S/W On
												// 2:��NG S/W Off
												// 3:��OK S/W Off


	double GetLoadValue();
	CCommThread GetCommThreadHandler() {return m_Serial;};
	CString GetSerialName() {return m_strSerialName;};
	void SetSerialName(CString strValue);
};

