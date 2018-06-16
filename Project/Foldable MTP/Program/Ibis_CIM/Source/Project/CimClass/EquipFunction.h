#pragma once

#include "DataStructure.h"

#define MAX_EQUIP_FUNCTION_LIST	10

class CEquipFunction
{
public:
	CEquipFunction(void);
	~CEquipFunction(void);

	EQUIP_FUNCTION m_FunctionList[MAX_EQUIP_FUNCTION_LIST];
	EQUIP_FUNCTION m_FunctionChangeBuffer[MAX_EQUIP_FUNCTION_LIST];

	BOOL m_bSyncStarted;	// ��ϰ� ����ȭ�Ǳ� ���������� TRUE

	void InitFunction();

	BOOL UpdateEquipFunction(tag_Message *pMsg);	// ����� ������ ������ TRUE

};

