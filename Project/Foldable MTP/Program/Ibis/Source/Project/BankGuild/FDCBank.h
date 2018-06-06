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

	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString m_strLoadMainAir;		// 0x0BC, LD-CH17	
		
	CString m_strWorkTableVac[JIG_ID_MAX][JIG_CH_MAX];
	
	// 0 : Normal, 1 : Alarm
	//Fire Alarm
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString m_strUtilBoxFireAlarm;	// X_UTIL_BOX_FIRE_ALARM
	CString m_strETCBoxFireAlarm;	// X_ETC_BOX_FIRE_ALARM
	CString m_strServoBoxFireAlarm;	// X_SERVO_BOX_FIRE_ALARM

	//kjpark2017/09/01 Serial 정리 - Temp 추가, 라벨 2 삭제, 토크 삭제, 메인에어	
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
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
	//kjpark 20170929 FDC 실제값, 보정값, 보고값 구분
	CString m_strbWattNet;
	CString m_strbWattTotal;
	CString	m_strbVoltage_R;
	CString	m_strbCurrent_R;
	void InitItem();

	// 프로그램 시작 후 첫 기록 전에 이전꺼 지운다
	// 혹시나 항목 변경되면 그때마다 지우기 귀찮으므로..
	void DeleteFDC_SV_par();	

	void SaveFDC_SV_par();

	FDC_ALARM_STATE m_stateMainAir;
	CStopWatch m_timerMainAirAlarm;
	BOOL m_bUnloadADBoardFirstRead;	// 한번에 2개 값을 읽지 못해서

	FDC_ALARM_STATE m_stateGMS1[GMS_SUB_CH_MAX];
	CStopWatch m_timerGMSAlarm1[GMS_SUB_CH_MAX];
	CString m_strOldGMS1[GMS_SUB_CH_MAX];

	FDC_ALARM_STATE m_stateGMS2[GMS_SUB_CH_MAX];
	CStopWatch m_timerGMSAlarm2[GMS_SUB_CH_MAX];
	CString m_strOldGMS2[GMS_SUB_CH_MAX];

	FDC_ALARM_STATE m_statePGMS;
	CStopWatch m_timerPGMSAlarm;

	// 값을 읽다가 잘못된 값이 나오면 바로 보고하지 않고 몇초간 유예기간 뒀다가 그래도 복구 안되면
	// 그때 가서 보고한다. 순간적인 깜빡임 등으로 설비 인터락 걸리는 걸 방지하는 편법... [3/25/2017 OSC]
	void CheckLoadADBoardValue();	
	void CheckGMSValue();
	void CheckPGMSValue();
};