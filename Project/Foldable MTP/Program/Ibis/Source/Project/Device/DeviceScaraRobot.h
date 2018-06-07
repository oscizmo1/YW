#pragma once
#include "../../ModuleInternal/MELSECNet/MelsecNet.h"

typedef struct _tagPLC_INFO 
{

	short						m_iStation;
	short						m_iNetWork;
	long						m_lNetPath;
	short						m_iNetMode;
	short						m_iNetChanel;

} PLC_INFO;

#define INT_MNET_PATH				81		//melsec ID
#define INT_MNET_MODE				-1		//1: ONLINE, 2: OFFLINE, 3:Forward loop test, 4: Reverse loop test
#define INT_MNET_CHANNEL			81
#define INT_MNET_NETWORK			10		//Networt Number
#define INT_MNET_STATION			255		//Station Number

//bit address
#define SCARA_START_IN		0x0F
#define SCARA_START_SERVO	0x06
#define SCARA_START_RUN		0x04
#define SCARA_START_REBOOT	0x0D
#define SCARA_START_ORIGIN	0x1F
#define SCARA_STOP			0x30

#define SCARA_SAFTY_TRAY	0x20
#define SCARA_SAFTY_GP5		0x21
#define SCARA_SAFTY_BUFFER	0x22

#define SCARA_START_BUFFER_WAIT 0x10
#define SCARA_START_REVERSE		0x11	
#define SCARA_START_BUFFER_CH1	0x12
#define SCARA_START_BUFFER_CH2	0x13
#define SCARA_START_BUFFER_CH3	0x14
#define SCARA_START_WAIT		0x15
#define SCARA_START_TRAY1		0x16
#define SCARA_START_TRAY2		0x17
#define SCARA_START_TRAY1_WAIT	0x18
#define SCARA_START_TRAY2_WAIT	0x19

#define SCARA_SAFTY_TRAY1		0x1A
#define SCARA_SAFTY_TRAY2		0x1B
#define SCARA_SAFTY_ONTRAY		0x20

#define SCARA_TYPE_XY			0x38
#define SCARA_TYPE_ANGLE		0x39

#define SCARA_WENABLE_FLAG		0x3C
#define SCARA_RENABLE_FLAG		0x3D

#define SCARA_WCOMPLETE_FLAG	0x70
#define SCARA_RCOMPLETE_FLAG	0x71

//word address
#define SCARA_OFFSET_X	0x00
#define SCARA_OFFSET_Y	0x02
#define SCARA_OFFSET_Z	0x04
#define SCARA_OFFSET_T	0x06

#define SCARA_GPINDEX	0x0B


class cDeviceScaraRobot : public CMelsecNet
{
public:
	cDeviceScaraRobot(void);
	~cDeviceScaraRobot(void);

	DECLARE_MESSAGE_MAP()

private:
	BOOL m_bIsConnected;

public:
	enum SCARA_G_POINT 
	{ 
		SCARA_WAIT_POS1 =0,		//index0
		SCARA_WAIT_POS2,		//index1
		SCARA_CELL_REVERSE,		//index2
		SCARA_BUFFER_STAGE,		//index3
		SCARA_TRAY1_POS,		//index4
		SCARA_TRAY2_POS			//index5
	};

	enum SCARA_MOVE_TYPE
	{
		TYPE_SPOS_EPOS = 0,
		TYPE_W1_EPOS,
		TYPE_W2_EPOS,
		TYPE_SPOS_W1_W2_EPOS,
		TYPE_SPOS_W2_W1_EPOS
	};

	enum SCARA_OFFSET_MODE
	{
		OFFSET_STAGE = 0,
		OFFSET_TRAY
	};

	enum TIMER_ID
	{
		CHECK_STARTBIT=0,

		//BIT_READ = 0,
		//START_READ,
		//RUNNING_OFF,
		//CHECK_FINISH
	};
	int m_nMoveType;
	PLC_INFO m_PLCInfo;

	CString m_strRcvBuf;
	CString m_strSndBuf;



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
	//void AutoStart();
	//void Stop();

	//bit On 	//MelsecNetBitOn
	short Scara_BitOn(short devno);

	//bit Off	//MelsecNetBitOff
	short Scara_BitOff(short devno);

	//bit read	//MelsecNetBitRecv
	short Scara_ReadBit(short devno);
	short Scara_Read_USERBit(short devno);

	//word read	//MelsecNetWordRecvBuf
	void Scara_ReadWord(short devno, short nSizebyByte, PSHORT pData);

	//word write	//MelsecNetWordSend
	CString Scara_WriteWord(short devno, LPVOID data, short nSizebyByte, BOOL bString);
	
	short	MoveStart();
	//void	SetEndPosition(SCARA_G_POINT ePoint);
	//void	SetEndPosition(short dev);

	void	SetZOffset(int nOffset);				// 픽커가 올라오는 높이 설정. 
	void	SetTOffset(int nOffset);
	void	SetYOffset(int nMode, int nOffset);		// Tray나 Stage의 세로 방향의 offset값을 세팅한다. 
	void	SetXOffset(int nMode, int nOffset);		// Tray나 Stage의 가로 방향의 offset값을 세팅한다.  
	//void	SetPickerPosition();

	int	GetPosition();		// 현재 Scara 픽커의 위치를 알려준다 (Position Index)
	double GetPosition_x();
	double GetPosition_y();
	double GetPosition_t();
	double GetPosition_z();

	double GetPosition_TempWrite();

	void	CheckFinish();

	//int	MoveToWaitOnePoint(int nStartPoint, int nEndPoint);
	//int	MoveToWaitTwoPoint(int nStartPoint, int nEndPoint);
	//int	MoveToCellReverse(int nStartPoint, int nEndPoint);
	//int	MoveToBufferStage(int nStartPoint, int nEndPoint);
	//int	MoveToTrayOne(int nStartPoint, int nEndPoint);
	//int	MoveToTrayTwo(int nStartPoint, int nEndPoint);
	afx_msg void OnTimer(UINT_PTR nIDEvent);

	void RebootBitOn();
};

