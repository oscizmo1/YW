#define		MOTION_CHECK_DONE_INTERVAL		5			// 100
#define		MOTION_CHECK_HOME_TIMEOUT		30			// 60 sec
#define		MOTION_CHECK_MOVE_TIMEOUT		5			// 20 sec

#define		HOME_SENSOR_AND_INDEX				0
#define		HOME_SENSOR_ONLY					1
#define		HOME_ABSOLUTE						2

// 현재 사용하는 최대 축
#define		MAX_AXIS									GetApp()->m_MaxAxis
// 프로그램에서 할당하는 버퍼 갯수
#define		MAX_AXIS_BUFFER								20

#define     AXIS_CCD_X0									0
#define     AXIS_CCD_X1									1
#define     AXIS_CCD_X2									2
#define		AXIS_CCD_X3									3
#define     AXIS_CCD_Y0									4
#define     AXIS_CCD_Y1									5
#define     AXIS_CCD_Y2									6
#define		AXIS_CCD_Y3									7
#define		AXIS_TILT_MOVE								8
#define     AXIS_CONT_PIN_Z								8

#define		MOTOR_PARAM									24							// 임시적..
#define		SOCKET_PARAM								9							// 임시적..

//////////////////////////////////////////////////////////////////////////
// 원점상태
#define HOME_ING	0	// 원점복귀중
#define HOME_END	1	// 원점복귀 완료
#define HOME_ERR	-1	// 원점복귀가 error로 완료
#define HOME_INIT	99	// 원점복귀 실행 전 (EMO나 Power off가 되면 전부 이것으로 바꿔야 한다.)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Motion 구동 관련 함수 return value
#define MOVE_OK_START		2	// StartMove까지 정상적으로 실행됬을 때
#define MOVE_OK_END			1	// 이동 완료까지 정상적으로 됬을 때
#define MOVE_ERR_NONE		0	// 실제로 return하는데 쓰이지 않지만 Error가 발생했는지 않했는지 간단히 비교하는데 쓴다.
#define MOVE_ERR_START		-1	// 모터가 이동하지 못하고 return 될 때(Limit 제외)
#define MOVE_ERR_SOFT_LIMIT	-2	// sw limit이나 stroke interlock에 걸렸을 때
#define MOVE_ERR_HW_LIMIT	-3	// limit sensor에 걸렸을 때 (혹시 필요할까 해서... 사용은 안함)
#define MOVE_ERR_END		-4	// 이동 완료가 정상적이지 않을 때
//////////////////////////////////////////////////////////////////////////

enum actCONT { 
	actNONE,  
	actGRAB_MOVE ,
	actCONTACT_CHECK_MOVE ,
	actREADY_MOVE , 
	actPCB_CHANGE_MOVE,
	actTILT_MOVE_WAIT,
	actTILT_MOVE_TILT,
	actMOVE_OK,
	actMOVE_ERROR,
	actAUTOTUNE_CENTER_MOVE,
	actAUTOTUNE_X_SORT
};
