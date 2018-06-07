#pragma once

#include "MELSECNet/MelsecNet.h"

typedef struct _tagPLC_INFO 
{

	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

} PLC_INFO;

#define INT_MNET_PATH				151		//melsec ID
#define INT_MNET_MODE				-1		//1: ONLINE, 2: OFFLINE, 3:Forward loop test, 4: Reverse loop test
#define INT_MNET_CHANNEL			151		// Board Channel
#define INT_MNET_NETWORK			0		//Networt Number (Ex함수 쓰려면 0 고정)
#define INT_MNET_STATION			255		//Station Number (Ex함수 쓰려면 255 고정)
//////////////////////////////////////////////////////////////////////////
// MTP 호기별 Station No.
// MTP1 : 2, MTP2 : 3, MTP3 : 4 [8/31/2017 OSC]
//////////////////////////////////////////////////////////////////////////

// 각 어드레스별 시작 주소 (MTP 1호기 기준)
#define MTP_ADDR_START_LB	0x4800
#define MTP_ADDR_START_LW	0x15400
#define PDT_ADDR_START_LB	0x4000
#define PDT_ADDR_START_LW	0x14000

// MTP 호기별 시작주소 Offset (MTP2호기 LB시작주소는 4800+500 = 0x4D00)
#define MTP_ADDR_OFFSET_LB		0x0500
#define MTP_ADDR_OFFSET_LW		0x1400
#define PDT_ADDR_OFFSET_LB		0x0080
#define PDT_ADDR_OFFSET_LW		0x0080

// MTP Shuttle별 시작주소 Offset (Shuttle2 LB시작주소는 4800+20 = 0x4820)
#define SHUTTLE_ADDR_OFFSET_LB		0x0020
#define SHUTTLE_ADDR_OFFSET_LW		0x0080

enum MACHINE_INDEX	// MTP가 물류의 몇번째 위치에 도킹되어 있는지 식별번호. 물류설비 하나당 MTP3대까지 도킹 가능 [9/13/2017 OSC]
{
	MACHINE_1 = 0,
	MACHINE_2,
	MACHINE_3
};

class CDevicePDT_IF : public CMelsecNet
{
public:
	CDevicePDT_IF(void);
	~CDevicePDT_IF(void);

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bIsConnected;

public:

	enum LB_MTP_ADDR
	{
		// Ch1
		MTP_CH1_SURVIVAL				= 0x00,
		MTP_ABLE						= 0x01,
		MTP_START						= 0x02,
		MTP_CH1_CHECK_SENSOR_ON			= 0x03,
		MTP_COMPLETE					= 0x04,
		MTP_CH1_PRODUCT					= 0x05,
		MTP_CH1_INSPECTION_OK			= 0x06,
		MTP_CH1_INSPECTION_NG			= 0x07,
		MTP_CH1_INSPECTION_RETRY_AA		= 0x08,
		MTP_CH1_INSPECTION_RETRY_AB		= 0x09,
		MTP_CH1_INSPECTION_RETRY_AC		= 0x0A,
		MTP_CH1_LOADING_STOP			= 0x0B,
		MTP_CH1_VAC_ON_STATUS			= 0x0C,
		MTP_CH1_VAC_OFF_STATUS			= 0x0D,
		MTP_CH1_SPARE_0E				= 0x0E,
		MTP_CH1_SPARE_0F				= 0x0F,

		// Ch2
		MTP_CH2_SURVIVAL				= 0x10,
// 		MTP_CH2_ABLE					= 0x11,
// 		MTP_CH2_START					= 0x12,
		MTP_CH2_CHECK_SENSOR_ON			= 0x13,
// 		MTP_CH2_COMPLETE				= 0x14,
		MTP_CH2_PRODUCT					= 0x15,
		MTP_CH2_INSPECTION_OK			= 0x16,
		MTP_CH2_INSPECTION_NG			= 0x17,
		MTP_CH2_INSPECTION_RETRY_AA		= 0x18,
		MTP_CH2_INSPECTION_RETRY_AB		= 0x19,
		MTP_CH2_INSPECTION_RETRY_AC		= 0x1A,
		MTP_CH2_LOADING_STOP			= 0x1B,
		MTP_CH2_VAC_ON_STATUS			= 0x1C,
		MTP_CH2_VAC_OFF_STATUS			= 0x1D,
		MTP_CH2_SPARE_1E				= 0x1E,
		MTP_CH2_SPARE_1F				= 0x1F,

		MTP_LB_ADDR_END
	};

	enum LB_PDT_ADDR
	{
		// Ch1
		PDT_SURVIVAL					= 0x00,
		PDT_ABLE						= 0x01,
		PDT_START						= 0x02,
		PDT_ARM_STATUS					= 0x03,
		PDT_COMPLETE					= 0x04,
		PDT_CH1_SPARE_05				= 0x05,
		PDT_CH1_SPARE_06				= 0x06,
		PDT_CH1_SPARE_07				= 0x07,
		PDT_CH1_SPARE_08				= 0x08,
		PDT_CH1_SPARE_09				= 0x09,
		PDT_CH1_SPARE_0A				= 0x0A,
		//kjparkk 20180131 PDT AB RULE 체크 기능 추가
		PDT_RETRY_AB_RLUE				= 0x0B,
		PDT_CH1_VAC_ON_REQ				= 0x0C,
		PDT_CH1_VAC_OFF_REQ				= 0x0D,
		PDT_CH1_SPARE_0E				= 0x0E,
		PDT_CH1_SPARE_0F				= 0x0F,

		// Ch2
// 		PDT_CH2_SURVIVAL				= 0x10,
// 		PDT_CH2_ABLE					= 0x11,
// 		PDT_CH2_START					= 0x12,
		PDT_CH2_ARM_STATUS				= 0x13,
// 		PDT_CH2_COMPLETE				= 0x14,
		PDT_CH2_SPARE_15				= 0x15,
		PDT_CH2_SPARE_16				= 0x16,
		PDT_CH2_SPARE_17				= 0x17,
		PDT_CH2_SPARE_18				= 0x18,
		PDT_CH2_SPARE_19				= 0x19,
		PDT_CH2_SPARE_1A				= 0x1A,
		PDT_CH2_SPARE_1B				= 0x1B,
		PDT_CH2_VAC_ON_REQ				= 0x1C,
		PDT_CH2_VAC_OFF_REQ				= 0x1D,
		PDT_CH2_SPARE_1E				= 0x1E,
		PDT_CH2_SPARE_1F				= 0x1F,

		PDT_LB_ADDR_END
	};

	enum LW_MTP_ADDR
	{
		// Ch1
		MTP_CH1_CELL_ID					= 0x00,
		MTP_CH1_NG_CODE					= 0x20,

		MTP_CH2_CELL_ID					= 0x40,
		MTP_CH2_NG_CODE					= 0x60,
		MTP_LW_ADDR_END
	};

	enum LW_PDT_ADDR
	{
		// Ch1
		PDT_CH1_CELL_ID					= 0x00,
		PDT_CH1_ROBOT_STATUS			= 0x20,		// 1=Run, 2=Idle [10/2/2017 OSC]
		PDT_CH1_ROBOT_ALARM_CODE		= 0x21,

		PDT_CH2_CELL_ID					= 0x40,
		// ROBOT은 한개라 다 CH1 어드레스로 한다
		PDT_CH2_ROBOT_STATUS			= 0x20,		// 1=Run, 2=Idle [10/2/2017 OSC]
		PDT_CH2_ROBOT_ALARM_CODE		= 0x21,
		PDT_LW_ADDR_END
	};

	enum ROBOT_STATUS
	{
		PDT_ROBOT_RUN = 1,
		PDT_ROBOT_IDLE = 2,
		PDT_ROBOT_ERR = 3
	};

	enum LW_SIZE
	{
		SIZE_CELL_ID = 0x20,
		SIZE_NG_CODE = 0x6,			// 원래 5이지만 WORD이므로 2
		SIZE_ROBOT_STATUS = 0x2,	// 원래 1이지만 WORD이므로 2
		SIZE_ROBOT_ALARM_CODE = 0x2
	};

	PLC_INFO m_PLCInfo;



	void SetConnected(BOOL bIsConnected)	
	{ 
		m_bIsConnected = bIsConnected; 
	}

	BOOL IsConnected()	
	{ 
		return m_bIsConnected; 
	}

	BOOL Open();
	void Close();


private:
	BOOL LB_Get(long addr);
	void LB_Set(long addr, BOOL bValue);
	long LW_Read(long addr, PSHORT pData, long nSizebyByte);
	long LW_Write(long addr, LPVOID data, long nSizebyByte);

	CString WORD_to_String(PSHORT pData, short nSizebyByte);
	void String_to_WORD(CString str, PSHORT pData, short nMaxBuffSizeByBite);
private:
	MACHINE_INDEX m_Machine;
public:
	void SetMachineIndex(MACHINE_INDEX index);	// 물류 설비에 몇번째 검사기로 도킹되어 있는지 설정

	long MTP_LB_Calcurate_FullAddr(JIG_ID jig, LB_MTP_ADDR addr);
	long PDT_LB_Calcurate_FullAddr(JIG_ID jig, LB_PDT_ADDR addr);
	long MTP_LW_Calcurate_FullAddr(JIG_ID jig, LW_MTP_ADDR addr);
	long PDT_LW_Calcurate_FullAddr(JIG_ID jig, LW_PDT_ADDR addr);

	BOOL MTP_LB_Get(JIG_ID jig, LB_MTP_ADDR addr);
	void MTP_LB_Set(JIG_ID jig, LB_MTP_ADDR addr, BOOL bValue);
	void MTP_LB_Set(JIG_ID jig, JIG_CH ch, LB_MTP_ADDR addr, BOOL bValue);		// CH1 어드레스를 넣어도 알아서 CH2로 변경 [10/6/2017 OSC]
	BOOL PDT_LB_Get(JIG_ID jig, LB_PDT_ADDR addr);

	CString MTP_LW_ReadCellID(JIG_ID jig, JIG_CH ch);
	void MTP_LW_WriteCellID(JIG_ID jig, JIG_CH ch, CString strCellID);
	CString MTP_LW_ReadNGCode(JIG_ID jig, JIG_CH ch);
	void MTP_LW_WriteNGCode(JIG_ID jig, JIG_CH ch, CString strCode);

	CString PDT_LW_ReadCellID(JIG_ID jig, JIG_CH ch);
	ROBOT_STATUS PDT_LW_ReadRobotStatus(JIG_ID jig, JIG_CH ch);		// 1=Run, 2=Idle [10/2/2017 OSC]
	CString PDT_LW_ReadRobotStatusString(JIG_ID jig, JIG_CH ch);	// 문자열로 변환해서 리턴
	int PDT_LW_ReadRobotAlarmCode(JIG_ID jig, JIG_CH ch);
	CString PDT_LW_ReadRobotAlarmCodeString(JIG_ID jig, JIG_CH ch);	// 문자열로 변환해서 리턴
};

