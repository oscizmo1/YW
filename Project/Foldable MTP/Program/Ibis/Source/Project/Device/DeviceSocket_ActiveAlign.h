#pragma once
#include "DeviceSocket_Base.h"

#define AVTIVE_ALIGN_ALIGN_REQ			_T("ALIGN_REQ")
#define AVTIVE_ALIGN_RECIPE_REQ			_T("RECIPE_REQ")
#define AVTIVE_ALIGN_TEACHING_SAVE		_T("ALIGN_TEACHING_SAVE")

#define AVTIVE_ALIGN_CURRENT_STATE		_T("CURRENT_STATE")
#define AVTIVE_ALIGN_ALIGN_RES			_T("ALIGN_RES")
#define AVTIVE_ALIGN_RECIPE_RES			_T("RECIPE_RES")
#define AVTIVE_ALIGN_TEACHING_RES		_T("ALIGN_TEACHING_RES")


#define SOCKET_ACTIVEALIGN_PC	_T("ACTIVEALIGN")
#define ALIGN_PC_IP				_T("192.168.10.130")
#define ALIGN_PC_ACTIVE_PORT	7777

// Align data 구조체 [02/01/2018 LSH]
typedef struct align_value
{
	// Align 완료 여부 [1/31/2018 LSH]
	BOOL bAlignFlag;

	// Align suucess 여부 확인 [1/31/2018 LSH]
	BOOL bAlignSuucess;

	// 보상값을 저장 변수 [1/31/2018 LSH]
	DOUBLE dT;
	DOUBLE dX;
	DOUBLE dY;

	align_value(){
		clear();
	}

	void clear(){
		bAlignFlag = FALSE;
		bAlignSuucess = FALSE;
		dT = 0;
		dX = 0;
		dY = 0;
	}
} ALIGN_VALUE ;

class CDeviceSocket_ActiveAlign : 	public CDeviceSocket_Base
{
protected:
	// 이전 Align value [1/31/2018 LSH]
	ALIGN_VALUE m_LastAlignValue[JIG_CH_MAX];

public:
	CDeviceSocket_ActiveAlign(void);
	~CDeviceSocket_ActiveAlign(void);

	// Socket생성 [1/30/2018 LSH]
	void CreateSocket();

	// Align PC와 연결 상태 확인 [1/30/2018 LSH]
	BOOL ConnectSocket() { return m_Socket.ConnectToServer(); };

	// 상속 받은 파싱함수 정의 [1/31/2018 LSH]
	void PreparseMessage(CDataSocket* pSocket = NULL);
	// 메세지 데이터 파싱 [1/31/2018 LSH]
	CString ParseActiveAlignMessage(CString strReadingText);

	// 메세지 전송 [1/31/2018 LSH]
	BOOL SendMassageToActiveAlign(CString strCmd, int nTeach = 0, int nStage = 0, CString strInnerID = _T(""), CString strCellID = _T(""), CString strRecipe = _T("") );

	// 마지막 Align grab한 값을 받는다 [1/31/2018 LSH]
	ALIGN_VALUE GetLastAlignValue(JIG_CH nCh) { return m_LastAlignValue[nCh]; m_LastAlignValue[nCh].clear(); };
	// 통신 Request가 왔는지 확인한다 [2/1/2018 LSH]
	BOOL GetActiveAlignDone(JIG_CH nCh) { return m_LastAlignValue[nCh].bAlignFlag; };


	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignSave(JIG_CH nCh);
	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignRecipeChange(JIG_CH nCh);
	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignGrab(JIG_CH nCh);
};