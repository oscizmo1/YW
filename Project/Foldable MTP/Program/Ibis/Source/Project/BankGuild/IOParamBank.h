#pragma once


enum INPUT_ID
{
	// X0000~X000F
	X_AUTO_TEACH_KEY,
	X_OP_RESET_SW,
	X_EMS_SWITCH_1,
	X_EMS_SWITCH_2,
	X_005,
	X_SHUTTLE_1_LIGHT_CURTAIN,
	X_SHUTTLE_2_LIGHT_CURTAIN,
	X_PDT_LIGHT_CURTAIN, //Spare(물류부 3 Light Curtain)
	X_UTIL_BOX_TEMP_ALARM,
	X_UTIL_BOX_FIRE_ALARM,
	X_SNC_SMPS1_ON_CHECK,
	X_SNC_SMPS2_ON_CHECK,
	X_SAFETY_DC_POWER_ON_CHECK, //Safety DC Power ON CHECK
	X_GPS_POWER_DOWN,  //GPS POWER DOWN (TIMER)
	X_MC_ON_CHECK,
	X_00F,

	// X010~X01F
	X_Main_BOX_FAN1_ALARM,
	X_Main_BOX_FAN2_ALARM,
	X_Main_BOX_FAN3_ALARM,
	X_Main_BOX_FAN4_ALARM,
	X_FRONT_DOOR1_SENSOR,
	X_FRONT_DOOR2_SENSOR,
	X_RIGNT_DOOR_SENSOR,
	X_BACK_DOOR1_SENSOR, 
	X_HOT_LINE_PDT_ALIVE,
	X_HOT_LINE_PDT_EMERGENCY,
	X_HOT_LINE_PDT_NET_READY,
	X_HOT_LINE_PDT_RESERVED,
	X_HOT_LINE_PDT_INTERACTIVE_1,
	X_HOT_LINE_PDT_INTERACTIVE_2,
	X_HOT_LINE_PDT_INTERACTIVE_3,
	X_HOT_LINE_PDT_SUB_EMO,

	// X0020~X002F
	X_SHUTTLE_1_VACCUM_ON_SW,
	X_SHUTTLE_1_VACCUM_OFF_SW,
	X_SHUTTLE_1_PATTERN_BACK_SW,
	X_SHUTTLE_1_PATTERN_NEXT_SW,
	X_SHUTTLE_1_PATTERN_ONOFF_SW,
	X_SHUTTLE_1_VACCUM_READY_SW,
	X_SHUTTLE_1_PATTERN_READY_SW,
	X_027,
	X_SHUTTLE_2_VACCUM_ON_SW,
	X_SHUTTLE_2_VACCUM_OFF_SW,
	X_SHUTTLE_2_PATTERN_BACK_SW,
	X_SHUTTLE_2_PATTERN_NEXT_SW,
	X_SHUTTLE_2_PATTERN_ONOFF_SW,
	X_SHUTTLE_2_VACCUM_READY_SW,
	X_SHUTTLE_2_PATTERN_READY_SW,
	X_02F,

	// X0030~X003F
	X_030,
	X_031,
	X_032,
	X_033,
	X_034,
	X_035,
	X_036,
	X_037,
	X_038,
	X_039,
	X_03A,
	X_03B,
	X_03C,
	X_03D,
	X_03E,
	X_03F,

	// X0040~X004F
	X_C_BOX_A_FAN1_ALARM,
	X_C_BOX_A_FAN2_ALARM,
	X_OP_PC_FAN1_ALARM,
	X_OP_PC_FAN2_ALARM,
	X_OP_PC_FAN3_ALARM,
	X_OP_PC_FAN4_ALARM,
	X_C_BOX_A_TEMP_ALARM,
	X_C_BOX_A_FIRE_ALARM,
	X_OP_PC_TEMP_ALARM,
	X_OP_PC_FIRE_ALARM,
	X_EFU_ALARM,
	X_SHUTTLE_TABLE_STO1,
	X_SHUTTLE_TABLE_STO2,
	X_04D,
	X_SHUTTLE_1_TILTING_DOWN,
	X_SHUTTLE_1_TILTING_UP, 

	// X0050~X005F
	X_C_BOX_B_FAN1_ALARM,
	X_C_BOX_B_FAN2_ALARM,
	X_ALIGN_PG_PC_FAN1_ALARM,
	X_ALIGN_PG_PC_FAN2_ALARM,
	X_ALIGN_PG_PC_FAN3_ALARM,
	X_ALIGN_PG_PC_FAN4_ALARM,
	X_C_BOX_B_TEMP_ALARM,
	X_C_BOX_B_FIRE_ALARM,
	X_ALIGN_PG_PC_TEMP_ALARM,
	X_ALIGN_PG_PC_FIRE_ALARM,
	X_05A,
	X_05B,
	X_05C,
	X_05D,
	X_SHUTTLE_2_TILTING_DOWN,
	X_SHUTTLE_2_TILTING_UP, 

	// X0060~X006F
	X_SHUTTLE_1_MANIFOLD_PRESSURE_SENSOR,
	X_SHUTTLE_1_EJECTOR_PRESSURE_SENSOR,
	X_SHUTTLE_1_VACCUM_ON_SENSOR,
	X_SHUTTLE_1_FPCB_VACCUM_ON_SENSOR,
	X_ONIZER_1_PRESSURE_SENSOR,
	X_IONIZER_1_ALARM,
	X_065,
	X_MAIN_AIR_DOWN,
	X_SHUTTLE_2_MANIFOLD_PRESSURE_SENSOR,
	X_SHUTTLE_2_EJECTOR_PRESSURE_SENSOR,
	X_SHUTTLE_2_VACCUM_ON_SENSOR,
	X_SHUTTLE_2_FPCB_VACCUM_ON_SENSOR,
	X_ONIZER_2_PRESSURE_SENSOR,
	X_IONIZER_2_ALARM,
	X_06E,
	X_06F,

	MAX_IN_ID
};

enum OUTPUT_ID
{
	// Y0000~X000F
	Y_FRONT_DOOR1_LOCK_ONOFF,
	Y_FRONT_DOOR2_LOCK_ONOFF,
	Y_RIGNT_DOOR_LOCK_ONOFF,
	Y_BACK_DOOR1_LOCK_ONOFF,
	Y_SHUTTLE_1_LIGHT_CURTAIN_MUTING_LAMP,
	Y_SHUTTLE_1_LIGHT_MUTING_ON_TO_SNC,
	Y_SHUTTLE_2_LIGHT_CURTAIN_MUTING_LAMP,
	Y_SHUTTLE_2_LIGHT_MUTING_ON_TO_SNC,
	Y_PDT_3_LIGHT_CURTAIN_MUTING_LAMP,
	Y_PDT_3_LIGHT_MUTING_ON_TO_SNC,
	Y_OP_RESET_LAMP,
	Y_SHUTTLE_1_VACCUM_ON_SW_LAMP,
	Y_SHUTTLE_1_VACCUM_OFF_SW_LAMP,
	Y_SHUTTLE_1_PATTERN_BACK_SW_LAMP,
	Y_SHUTTLE_1_PATTERN_NEXT_SW_LAMP,
	Y_SHUTTLE_1_PATTERN_ONOFF_SW_LAMP,

	// Y0010~Y001F
	Y_SHUTTLE_2_VACCUM_ON_SW_LAMP,
	Y_SHUTTLE_2_VACCUM_OFF_SW_LAMP,
	Y_SHUTTLE_2_PATTERN_BACK_SW_LAMP,
	Y_SHUTTLE_2_PATTERN_NEXT_SW_LAMP,
	Y_SHUTTLE_2_PATTERN_ONOFF_SW_LAMP,
	Y_015,
	Y_016,
	Y_017,
	Y_HOT_LINE_MTP_ALIVE,
	Y_HOT_LINE_MTP_EMERGENCY,
	Y_HOT_LINE_MTP_NET_READY,
	Y_HOT_LINE_PDT_RESERVED,
	Y_HOT_LINE_MTP_INTERACTIVE_1,
	Y_HOT_LINE_MTP_INTERACTIVE_2,
	Y_HOT_LINE_MTP_INTERACTIVE_3,
	Y_HOT_LINE_MTP_SUB_EMO,

	// Y0020~X002F
	Y_TOWER_LAMP_RED,
	Y_TOWER_LAMP_YELLOW,
	Y_TOWER_LAMP_GREEN,
	Y_BUZZER_K1,
	Y_BUZZER_K2,
	Y_BUZZER_K3,
	Y_BUZZER_K4,
	Y_BUZZER_K5,
	Y_028,
	Y_029,
	Y_02A,
	Y_02B,
	Y_02C,
	Y_02D,
	Y_SHUTTLE_1_TILTING_DOWN,  //Shuttle1 Table Tilting On
	Y_SHUTTLE_1_TILTING_UP,    //Shuttle1 Table Tilting OFF


	// Y0030~X003F
	Y_SHUTTLE1_VACCUM_ONOFF,
	Y_SHUTTLE1_BLOW_ONOFF,
	Y_SHUTTLE1_FPCB_VACCUM_ONOFF,
	Y_SHUTTLE1_FPCB_BLOW_ONOFF,
	Y_IONIZER_1_AIR_ONOFF,
	Y_IONIZER_1_POWER_ONOFF,
	Y_SIGNAL_PC1_RESET,
	Y_SIGNAL_PC2_RESET,
	Y_SHUTTLE2_VACCUM_ONOFF,
	Y_SHUTTLE2_BLOW_ONOFF,
	Y_SHUTTLE2_FPCB_VACCUM_ONOFF,
	Y_SHUTTLE2_FPCB_BLOW_ONOFF,
	Y_IONIZER_2_AIR_ONOFF,
	Y_IONIZER_2_POWER_ONOFF,
	Y_SHUTTLE_2_TILTING_DOWN,  //Shuttle1 Table Tilting On
	Y_SHUTTLE_2_TILTING_UP,    //Shuttle1 Table Tilting OFF

	MAX_OUT_ID
};

class CIOParam
{
public:

	enum CHECK_TYPE
	{
		CHECK_TYPE_B = 0,		// Input일 경우 B접은 Off되어야 감지. Output일 경우 Off해야 동작
		CHECK_TYPE_A = 1,		// Input일 경우 A접은 On되어야 감지. Output일 경우 On해야 동작
		CHECK_TYPE_MAX
	};

	CIOParam(void);
	~CIOParam(void);

	CString m_strIOAddress;
	CString m_strIOName;
	CString m_strIORemark;
	
	CHECK_TYPE m_Check;	// A접인지 B접인지에 따라 TRUE, FALSE로 구분된다

	BOOL ReturnOn();
	BOOL ReturnOff();

	BOOL ReturnFrom(VAC_STATE vac);
	BOOL ReturnFrom(CYL_STATE cyl);
	BOOL GetIOCheck(ONOFF value);
	BOOL GetIOCheck(BOOL bValue);
};

class CIOParamBank
{
public:
	CIOParamBank(void);
	~CIOParamBank(void);

	CIOParam m_In[MAX_IN_ID];
	CIOParam m_Out[MAX_OUT_ID];

	void InitializeIOParam();
	void InitializeIOParam_IN();
	void InitializeIOParam_OUT();

	CIOParam GetInputIOParma(INPUT_ID ID);
	CIOParam GetOutputIOParma(OUTPUT_ID ID);
};

