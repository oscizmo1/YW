#pragma once
#include "DeviceSocket_Base.h"

#define SOCKET_PGPC		_T("PGPC")	
//#define PATERNGENRATOR_PC_IP _T("127.0.0.1")//_T("192.168.10.140")
//#define PATERNGENRATOR_PC_PORT	7000//3000
#define PATERNGENRATOR_PC_IP _T("192.168.10.140")
#define PATERNGENRATOR_PC_PORT	3000

#define CODEPGPC	0x80


#define PG_JUDGE_GOOD		_T("GOOD")
#define PG_JUDGE_NG			_T("NG")

#define	TURN_ON				_T("TURNON")
#define	TURN_OFF			_T("TURNOFF")
#define	RESET				_T("RESET")
#define	NEXT				_T("NEXT")
#define	BACK				_T("BACK")
#define SLEEP_IN			_T("SLEEPIN")
#define SLEEP_OUT			_T("SLEEPOUT")

// 공통
#define CELL_LOADING		_T("CELL_LOADING")
#define MTP_WRITE			_T("MTP_WRITE")
#define MTP_WRITE_PRESCALE	_T("MTP_WRITE_PRESCALE")
#define MTP_VERIFY			_T("MTP_VERIFY")
#define TMD_INFO			_T("TMD_INFO")	// TMD File Name [3/12/2013 OSC]
//kjpark 20180113 Tmd Version 추가
#define HOST_VER			_T("HOST_VER")
#define CLIENT_VER			_T("CLIENT_VER")
#define TSP_START			_T("TSP_START")
//kjpark 20171016 하이페리온 커넷트 체크 CMD send 기능
#define HYPERION_CONNECT_CHK _T("SENSOR_SELECT")
#define ID_CHECK			_T("ID_CHECK")	// WHITE CURRENT 대용인듯?
#define DZONE_OFF			_T("DZONE_OFF")	// D존에서 다 끝나고 Contact 푸는 명령어
#define TE_CHECK			_T("TE_CHECK")	// 2015-03-27, jhLee, TE_Check 기능 추가
#define PATTERN_SELECT		_T("PTRN")		// 2015-06-01, jhLee, Pattern Select 기능 추가
#define OTP_REG_CHECK		_T("OTP_REG_CHECK")		// 2015-06-01, jhLee, Pattern Select 기능 추가
#define ZONE_FIN			_T("ZONE_FIN")

//kjpark 20170907 CellLog Title 및 해당 항목 추가
#define EVT_VERSION_CHECK	_T("EVT_VERSION_CHECK")
#define TE_CHECK			_T("TE_CHECK")
#define COPR_ICT_TEST		 _T("COPR_ICT_TEST")
#define POC_ERROR_CHECK		_T("POC_ERROR_CHECK")
#define DDI_BLOCK_TEST		 _T("DDI_BLOCK_TEST")
//kjpark 20161114 C Zone Foce 추가
// Force검사가 전부 Before, After로 나뉘어졌다 [12/21/2016 OSC]
#define FORCE_START1_BEF			_T("FORCE_START1_BEF")
#define FORCE_START2_BEF			_T("FORCE_START2_BEF")
#define FORCE_START3_BEF			_T("FORCE_START3_BEF")
#define FORCE_START1_AFT			_T("FORCE_START1_AFT")
#define FORCE_START2_AFT			_T("FORCE_START2_AFT")
#define FORCE_START3_AFT			_T("FORCE_START3_AFT")

#define ICT_TEST				_T("ICT_TEST")//_T("MIPI_ICT_TEST") 20161210 BKH 이름 변경
#define OPTION_CHECK1			_T("OPTION_CHECK1")
#define HLPM_CURRENT_CHECK		_T("HLPM_CURRENT_CHECK")
//kjpark 20170925 HLPM CURRENT MTP Vifity 에 들어 옴
#define HLPM_CURRENT_CHECK_AMTP _T("HLPM_CURRENT_CHECK_AMTP")
#define OPTION_CHECK2			_T("OPTION_CHECK2")
//kjpark 20170907 CellLog Title 및 해당 항목 추가
#define OPTION_CHECK3			_T("OPTION_CHECK3")
#define OPTION_CHECK4			_T("OPTION_CHECK4")
#define OPTION_CHECK5			_T("OPTION_CHECK5")
#define OPTION_CHECK6			_T("OPTION_CHECK6")
#define OPTION_CHECK7			_T("OPTION_CHECK7")
#define OPTION_CHECK8			_T("OPTION_CHECK8")
#define OPTION_CHECK9			_T("OPTION_CHECK9")
#define OPTION_CHECK10			_T("OPTION_CHECK10")
//kjpark 20171010 신호기 UI 에 White Circle 추가
#define WHITE_CIRCLE			_T("fun/ptrn_hlpm_v255")


// Send Only
#define SET_ZONE_A			_T("ZONE_A")
#define SET_ZONE_B			_T("ZONE_B")
#define SET_ZONE_C			_T("ZONE_C")
#define SET_ZONE_D			_T("ZONE_D")
#define SLEEP_CURRENT_CHECK	_T("SLEEP_CURRENT_CHECK")
//20170306 kjpark WhiteCurrent ADD in Celllog
#define WHITE_CURRENT		_T("WHITE_CURRENT")	// ID_CHECK로 대체됨 [8/3/2013 OSC]
#define PG_CIRCLE			_T("CIRCLE")
#define PG_MSG				_T("MSG")
#define PG_SQUARE			_T("SQUARE")
#define PG_D0_PTRN			_T("D0_PTRN")		// 이것만 쓰진 못하고 PTRN다음에 패턴 번호가 남는다. ex : D0_PTRN101

// Receive Only
#define REG_PIN				_T("REG_PIN")		// CONTACT 저항 [3/20/2013 OSC]
#define TSP_PIN				_T("TSP_PIN")	// TSP Pin 상태 [3/12/2013 OSC]
#define CURRENT_A_ACK		_T("A_CURRENT")
#define CURRENT_B_ACK		_T("B_CURRENT")
#define CURRENT_C_ACK		_T("C_CURRENT")
#define CURRENT_D_ACK		_T("D_CURRENT")
#define SLEEP_CURRENT_ACK	_T("SLEEP_CURRENT")
#define WHITE_CURRENT_CHECK	_T("WHITE_CURRENT_CHECK")
#define WHITE_PATTERN		_T("FUN/PTRN_V255")
#define CROSS_PATTERN		_T("FUN/PTRN_LAMI_TILT_CROSSTALK")
#define CONTACT_CURRENT		_T("CONTACT_CURRENT")
#define CURRENT_ALRAM		_T("CURRENT_ALRAM")
#define VOLTAGE_ALRAM		_T("VOLTAGE_ALRAM")
#define PG_ACK				_T("ACK")
#define PG_LINECHECK		_T("LineCheck")
#define PG_CHECK			_T("CHECK")
#define PG_PIN_REG			_T("PIN_REG")
#define PG_SYSTEM_ERROR		_T("SYSTEM_ERROR")
#define PG_DOWN				_T("DOWN")
#define TSP_HOVER			_T("TSP_HOVER")
#define PG_RUN				_T("RUN")


static const unsigned short crc16tab[256]= {
	0x0000,0x1021,0x2042,0x3063,0x4084,0x50a5,0x60c6,0x70e7,
	0x8108,0x9129,0xa14a,0xb16b,0xc18c,0xd1ad,0xe1ce,0xf1ef,
	0x1231,0x0210,0x3273,0x2252,0x52b5,0x4294,0x72f7,0x62d6,
	0x9339,0x8318,0xb37b,0xa35a,0xd3bd,0xc39c,0xf3ff,0xe3de,
	0x2462,0x3443,0x0420,0x1401,0x64e6,0x74c7,0x44a4,0x5485,
	0xa56a,0xb54b,0x8528,0x9509,0xe5ee,0xf5cf,0xc5ac,0xd58d,
	0x3653,0x2672,0x1611,0x0630,0x76d7,0x66f6,0x5695,0x46b4,
	0xb75b,0xa77a,0x9719,0x8738,0xf7df,0xe7fe,0xd79d,0xc7bc,
	0x48c4,0x58e5,0x6886,0x78a7,0x0840,0x1861,0x2802,0x3823,
	0xc9cc,0xd9ed,0xe98e,0xf9af,0x8948,0x9969,0xa90a,0xb92b,
	0x5af5,0x4ad4,0x7ab7,0x6a96,0x1a71,0x0a50,0x3a33,0x2a12,
	0xdbfd,0xcbdc,0xfbbf,0xeb9e,0x9b79,0x8b58,0xbb3b,0xab1a,
	0x6ca6,0x7c87,0x4ce4,0x5cc5,0x2c22,0x3c03,0x0c60,0x1c41,
	0xedae,0xfd8f,0xcdec,0xddcd,0xad2a,0xbd0b,0x8d68,0x9d49,
	0x7e97,0x6eb6,0x5ed5,0x4ef4,0x3e13,0x2e32,0x1e51,0x0e70,
	0xff9f,0xefbe,0xdfdd,0xcffc,0xbf1b,0xaf3a,0x9f59,0x8f78,
	0x9188,0x81a9,0xb1ca,0xa1eb,0xd10c,0xc12d,0xf14e,0xe16f,
	0x1080,0x00a1,0x30c2,0x20e3,0x5004,0x4025,0x7046,0x6067,
	0x83b9,0x9398,0xa3fb,0xb3da,0xc33d,0xd31c,0xe37f,0xf35e,
	0x02b1,0x1290,0x22f3,0x32d2,0x4235,0x5214,0x6277,0x7256,
	0xb5ea,0xa5cb,0x95a8,0x8589,0xf56e,0xe54f,0xd52c,0xc50d,
	0x34e2,0x24c3,0x14a0,0x0481,0x7466,0x6447,0x5424,0x4405,
	0xa7db,0xb7fa,0x8799,0x97b8,0xe75f,0xf77e,0xc71d,0xd73c,
	0x26d3,0x36f2,0x0691,0x16b0,0x6657,0x7676,0x4615,0x5634,
	0xd94c,0xc96d,0xf90e,0xe92f,0x99c8,0x89e9,0xb98a,0xa9ab,
	0x5844,0x4865,0x7806,0x6827,0x18c0,0x08e1,0x3882,0x28a3,
	0xcb7d,0xdb5c,0xeb3f,0xfb1e,0x8bf9,0x9bd8,0xabbb,0xbb9a,
	0x4a75,0x5a54,0x6a37,0x7a16,0x0af1,0x1ad0,0x2ab3,0x3a92,
	0xfd2e,0xed0f,0xdd6c,0xcd4d,0xbdaa,0xad8b,0x9de8,0x8dc9,
	0x7c26,0x6c07,0x5c64,0x4c45,0x3ca2,0x2c83,0x1ce0,0x0cc1,
	0xef1f,0xff3e,0xcf5d,0xdf7c,0xaf9b,0xbfba,0x8fd9,0x9ff8,
	0x6e17,0x7e36,0x4e55,0x5e74,0x2e93,0x3eb2,0x0ed1,0x1ef0
};


class CDeviceSocket_PGHost : 	public CDeviceSocket_Base
{
public:
	CDeviceSocket_PGHost(void);
	~CDeviceSocket_PGHost(void);

	void CreateSocket();
	void PreparseMessage(CDataSocket* pSocket = NULL);	// 유니코드 변환 및 StartChar 유무 확인하여 ParseMessage 호출
	CString	ParseMessage(CString strMsg);				// 실제 Parsing

	CString	PacketClear(CString strMsg);				// 2개 이상 연달아 붙어 나올 것을 대비해 맨 앞 하나를 지운다.
	void SetAddLog( CString strLog );
	void PatternGeneratorACK(CCellInfo* pCell, CString strJudge);
	BOOL SendMassageToPatternGenerator(CString strCmdType, int jig, int ch, CString strExtraData = _T("") );
	USHORT GetCRC16_Checksum( const void *buf, int len );

	CStringList m_listMsg;
	void AddPGMsg(CString strMsg);
	void ClearPGMsg();
	void DrawPGMsg(JIG_ID jig, JIG_CH ch, COLORREF colorBack, COLORREF colorFont, int nX, int nY, int nSize);


	CList <SQUARE_POINT, SQUARE_POINT> m_listSquare;
	void AddPGSquare(SQUARE_POINT sqpoint);
	void ClearPGSqueare();
	void DrawPGSquare(JIG_ID jig, JIG_CH ch);

	void ShowDZonePattern(int nPatternNo, JIG_ID jig, JIG_CH ch);

};

