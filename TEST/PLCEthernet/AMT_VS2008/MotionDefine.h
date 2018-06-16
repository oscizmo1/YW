#define		MOTION_CHECK_DONE_INTERVAL		5			// 100
#define		MOTION_CHECK_HOME_TIMEOUT		30			// 60 sec
#define		MOTION_CHECK_MOVE_TIMEOUT		5			// 20 sec

#define		HOME_SENSOR_AND_INDEX				0
#define		HOME_SENSOR_ONLY					1
#define		HOME_ABSOLUTE						2

// ���� ����ϴ� �ִ� ��
#define		MAX_AXIS									GetApp()->m_MaxAxis
// ���α׷����� �Ҵ��ϴ� ���� ����
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

#define		MOTOR_PARAM									24							// �ӽ���..
#define		SOCKET_PARAM								9							// �ӽ���..

//////////////////////////////////////////////////////////////////////////
// ��������
#define HOME_ING	0	// ����������
#define HOME_END	1	// �������� �Ϸ�
#define HOME_ERR	-1	// �������Ͱ� error�� �Ϸ�
#define HOME_INIT	99	// �������� ���� �� (EMO�� Power off�� �Ǹ� ���� �̰����� �ٲ�� �Ѵ�.)
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Motion ���� ���� �Լ� return value
#define MOVE_OK_START		2	// StartMove���� ���������� �������� ��
#define MOVE_OK_END			1	// �̵� �Ϸ���� ���������� ���� ��
#define MOVE_ERR_NONE		0	// ������ return�ϴµ� ������ ������ Error�� �߻��ߴ��� ���ߴ��� ������ ���ϴµ� ����.
#define MOVE_ERR_START		-1	// ���Ͱ� �̵����� ���ϰ� return �� ��(Limit ����)
#define MOVE_ERR_SOFT_LIMIT	-2	// sw limit�̳� stroke interlock�� �ɷ��� ��
#define MOVE_ERR_HW_LIMIT	-3	// limit sensor�� �ɷ��� �� (Ȥ�� �ʿ��ұ� �ؼ�... ����� ����)
#define MOVE_ERR_END		-4	// �̵� �Ϸᰡ ���������� ���� ��
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
