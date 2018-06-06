#pragma once
#include "Device/DeviceAccura.h"
#include "Device/DeviceSerialGMS.h"

enum FDC_DATATYPE
{
	FDC_DATA_INT = 0,
	FDC_DATA_INT_ADD1,
	FDC_DATA_DOUBLE,
	FDC_DATA_STRING
};

class CFDCItem
{
private:
	int				*m_ptrInt;
	double			*m_ptrDouble;
	CString			*m_ptrString;

	CString m_strSection;
	CString m_strKey;
	FDC_DATATYPE m_Type;

public:
	CFDCItem(void);
	CFDCItem(CString strSection, CString strKey, FDC_DATATYPE type, void *ptr1);
	~CFDCItem(void);

	void Init();
	void InitValue();

	CString GetSection();
	CString GetKey();

	void SetValuePtr(FDC_DATATYPE type, void *ptr);
	CString ValueToString();
	void WriteFile(CString strPath);
};

#define FDC_AIR_LIMIT_VALUE		40
#define FDC_GMS_LIMIT_VALUE		10.
#define FDC_PGMS_LIMIT_VALUE	_T("0")

enum FDC_ALARM_STATE
{
	FDC_NORMAL = 0,
	FDC_WARNING,
	FDC_ALARM
};

class CFDCBank
{
public:
	CFDCBank(void);
	~CFDCBank(void);

	CList <CFDCItem, CFDCItem> m_listFDCItem;

	//kjpark 20170929 FDC ������, ������, ���� ����
	CString m_strLoadMainAir;		// 0x0BC, LD-CH17	
		
	CString m_strWorkTableVac[JIG_ID_MAX][JIG_CH_MAX];
	
	// 0 : Normal, 1 : Alarm
	//Fire Alarm
	//kjpark 20170929 FDC ������, ������, ���� ����
	CString m_strUtilBoxFireAlarm;	// X_UTIL_BOX_FIRE_ALARM
	CString m_strETCBoxFireAlarm;	// X_ETC_BOX_FIRE_ALARM
	CString m_strServoBoxFireAlarm;	// X_SERVO_BOX_FIRE_ALARM

	//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���	
	//kjpark 20170929 FDC ������, ������, ���� ����
	CString m_strUtilBox_Temp ;//PC_RACK_TEMP
	CString m_strEtcBox_Temp ;//PG_RACK_TEMP
	CString m_strServoBox_Temp ;//PC_RACK_TEMP
	CString m_strCtrlPCBox_Temp ;//PC_RACK_TEMP
	CString m_strPGPcBox_Temp ;//PG_RACK_TEMP

	CString m_nFrontDoor1Open;
	CString m_nFrontDoor2Open;
	CString m_nBackDoor1Open;
	CString m_nBackDoor2Open;
	CString m_nLeftDoorOpen;
	CString m_nRightDoorOpen;

	// 0 : Normal, 1 : Limit
	//TAccuraData		m_rDataBlock[ACCURA_CLIENT_MAX];
	//kjpark 20170929 FDC ������, ������, ���� ����
	CString m_strbWattNet;
	CString m_strbWattTotal;
	CString	m_strbVoltage_R;
	CString	m_strbCurrent_R;
	void InitItem();

	// ���α׷� ���� �� ù ��� ���� ������ �����
	// Ȥ�ó� �׸� ����Ǹ� �׶����� ����� �������Ƿ�..
	void DeleteFDC_SV_par();	

	void SaveFDC_SV_par();

	FDC_ALARM_STATE m_stateMainAir;
	CStopWatch m_timerMainAirAlarm;
	BOOL m_bUnloadADBoardFirstRead;	// �ѹ��� 2�� ���� ���� ���ؼ�

	FDC_ALARM_STATE m_stateGMS1[GMS_SUB_CH_MAX];
	CStopWatch m_timerGMSAlarm1[GMS_SUB_CH_MAX];
	CString m_strOldGMS1[GMS_SUB_CH_MAX];

	FDC_ALARM_STATE m_stateGMS2[GMS_SUB_CH_MAX];
	CStopWatch m_timerGMSAlarm2[GMS_SUB_CH_MAX];
	CString m_strOldGMS2[GMS_SUB_CH_MAX];

	FDC_ALARM_STATE m_statePGMS;
	CStopWatch m_timerPGMSAlarm;

	// ���� �дٰ� �߸��� ���� ������ �ٷ� �������� �ʰ� ���ʰ� �����Ⱓ �״ٰ� �׷��� ���� �ȵǸ�
	// �׶� ���� �����Ѵ�. �������� ������ ������ ���� ���Ͷ� �ɸ��� �� �����ϴ� ���... [3/25/2017 OSC]
	void CheckLoadADBoardValue();	
	void CheckGMSValue();
	void CheckPGMSValue();
};