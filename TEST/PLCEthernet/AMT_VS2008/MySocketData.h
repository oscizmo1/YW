//
/*
#define VI_STATUS_INIT			0
#define VI_STATUS_UNLOAD		2
#define VI_STATUS_VI_END		4
#define VI_STATUS_RELEASE       8
#define VI_STATUS_WAIT			16
#define VI_STATUS_EMO			32
#define VI_STATUS_ERR			64
#define VI_STATUS_WARN			128
#define VI_STATUS_START			256
#define VI_STATUS_GRAB_FAIL		512

#define VI_END_OK				-1			// 검사 결과 받은 값.//
#define VI_END_UNLOAD			0
#define VI_END_TIMEOUT			1
#define VI_END_GRABFAIL			2
#define VI_OFF_END_TIMEOUT		3
*/// AMT Vision
#define STX						0x02
#define ETX						0x03		

// PG.//
#define PG_STATUS_INIT			0
#define PG_STATUS_UNLOAD		2
#define PG_STATUS_END			4
#define PG_STATUS_RELEASE       8
#define PG_STATUS_WAIT			16
#define PG_STATUS_EMO			32
#define PG_STATUS_ERR			64
#define PG_STATUS_WARN			128
#define PG_STATUS_START			256
#define PG_STATUS_GRAB_FAIL		512

#define PG_END_OK				-1			// 검사 결과 받은 값.//
#define PG_END_UNLOAD			0
#define PG_END_TIMEOUT			1
#define PG_END_GRABFAIL			2
// Send Command.//
#define CMD_STATUS_INFO			100			// 현재 연결 상태.//
#define CMD_CELL_INFO			101			// CELL 정보.//
#define	CMD_INSPECTION_YES		102			// 검사 허가.//
#define CMD_INSPECTION_NO		103
#define CMD_ERR_MSG				104
#define CMD_FILESERVER_SET		105
#define CMD_CONTACT_COMPLETE	5			// 컨텍 완료.//
#define CMD_MODEL_CHANGE		111			// Model Change//

// Recive Command.//
#define CMD_OP_INFO				2			// 로그인 정보.//
#define CMD_INSPECTION_REQ		3			// 검사 요청.//
#define CMD_DEFECT_CODE		4			// 불량 판정.//
#define	CMD_RETEST_REQ			106			// CELL 정보 재전송 요청.//
#define CMD_CONTACT_OFF			108			// 컨텍 해제.//
#define CMD_CONTACT_ON			109			// 컨텍 실행.//
#define CMD_TILTING				110
#define CMD_OMIT_ON				7			// 20080902 Hui : Omit Lamp On.//
#define CMD_OMIT_OFF			8			// 20080902	Hui : Omit Lamp Off.//
#define CMD_CLAMP_ON			23
#define CMD_CLAMP_OFF			24
#define CMD_BLU_BRIGHTNESS1		26			// BLU 휘도 + offset조절
#define CMD_BLU_BRIGHTNESS2		27			// BLU 휘도 - offset조절

// 사용 안함.//
#define CMD_QC_SAMPLE			5			// QC SAMPLE.//
#define CMD_PLC_UNLOAD			6			// 언로드 명령.//

// MT.//
// Send.//
#define CMD_CLEAN_COMPLETE		9			// ACK.// 20081009 Hui : Add
#define CMD_SCAN_WANT			10
#define CMD_RESPONSE_WANT		11			// ACK.//
// Recive.//
#define CMD_SCAN_DATA			20
#define CMD_RESPONSE_DATA		21			// ACK.//
/////////   PG-END  /////////////////////////////////////////////////////    

// Send Command.//
/*
#define CMD_VI_START			0x20		// 검사 시작.//
#define CMD_VI_MANUAL_START		0x30		// Manaul 검사 시작.//
#define CMD_VI_USER_INSP		0x40		// Retry 후 마지막 검사.//
#define CMD_VI_OFFLIGHT_START	0x41		// 비점등 검사 시작.//c
#define CMD_VI_OFFLIGHT_MANUAL	0x42		// 메뉴얼 비점등 검사 시작.//
#define CMD_VI_REQUEST_ALARM	0x44		// 검사부 Alarm 밸생 및 Run 정지.// Protocol Ver 1.5

// Recive Command.//
#define CMD_FILESERVER_SET		0x18
#define CMD_ALARM_SET			0x19
#define CMD_VI_END				0x21
#define CMD_WAIT				0x22
#define CMD_RELEASE				0x23
#define CMD_UNLOAD				0x24
#define CMD_PATH_INFO			0x25
#define CMD_OFFLIGHT_END		0x26		// 비점등 검사 종료.//
#define CMD_INSP_DCALL			0x27
#define CMD_SEND_DCALL			0x28
#define CMD_GRAB_FAIL			0X34
#define CMD_ACK					0X2F
#define CMD_MC_ALARM			0X47	// Model Change Error.//
// 0X47에 대한 분류
#define MC_RCP_ERR				0X52
#define MC_INSP_ERR				0X49
#define MC_OPT_ERR				0X4F

// Ack.//    
#define CMD_GOOD				0x77		// Retry 안한다.//
#define CMD_NG					0x78		// Retry 한다.//
#define CMD_HOLD				0x79		// Hold 판정.//

#define CMD_EMO					0xF0
#define CMD_ERR					0xF1
#define CMD_WARN				0xF2
#define CMD_ALARM_REPORT		0xF3		// Alram Report.//
#define CMD_ERRACK				0xFF
*/	//AMT Vision
// Unloader 방법.//
#define USE_CMD_UNLOAD			0
#define USE_CMD_VI_END			1

// VI Mode.//			수동인지 자동인지.//
#define CMD_VI_MODE_MANUAL		0
#define CMD_VI_MODE_INLINE		1



#define SIZE_AREA				9
#define COMMAND					11
#define DATA_AREA				12
#define TOTAL_DATA				225	

#define CMD_SET_STOP			20
#define CMD_RESET_STOP			21

