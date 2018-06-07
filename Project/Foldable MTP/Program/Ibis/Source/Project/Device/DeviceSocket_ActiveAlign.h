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

// Align data ����ü [02/01/2018 LSH]
typedef struct align_value
{
	// Align �Ϸ� ���� [1/31/2018 LSH]
	BOOL bAlignFlag;

	// Align suucess ���� Ȯ�� [1/31/2018 LSH]
	BOOL bAlignSuucess;

	// ������ ���� ���� [1/31/2018 LSH]
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
	// ���� Align value [1/31/2018 LSH]
	ALIGN_VALUE m_LastAlignValue[JIG_CH_MAX];

public:
	CDeviceSocket_ActiveAlign(void);
	~CDeviceSocket_ActiveAlign(void);

	// Socket���� [1/30/2018 LSH]
	void CreateSocket();

	// Align PC�� ���� ���� Ȯ�� [1/30/2018 LSH]
	BOOL ConnectSocket() { return m_Socket.ConnectToServer(); };

	// ��� ���� �Ľ��Լ� ���� [1/31/2018 LSH]
	void PreparseMessage(CDataSocket* pSocket = NULL);
	// �޼��� ������ �Ľ� [1/31/2018 LSH]
	CString ParseActiveAlignMessage(CString strReadingText);

	// �޼��� ���� [1/31/2018 LSH]
	BOOL SendMassageToActiveAlign(CString strCmd, int nTeach = 0, int nStage = 0, CString strInnerID = _T(""), CString strCellID = _T(""), CString strRecipe = _T("") );

	// ������ Align grab�� ���� �޴´� [1/31/2018 LSH]
	ALIGN_VALUE GetLastAlignValue(JIG_CH nCh) { return m_LastAlignValue[nCh]; m_LastAlignValue[nCh].clear(); };
	// ��� Request�� �Դ��� Ȯ���Ѵ� [2/1/2018 LSH]
	BOOL GetActiveAlignDone(JIG_CH nCh) { return m_LastAlignValue[nCh].bAlignFlag; };


	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignSave(JIG_CH nCh);
	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignRecipeChange(JIG_CH nCh);
	// Align Save [2/1/2018 LSH]
	BOOL ActiveAlignGrab(JIG_CH nCh);
};