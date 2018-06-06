#pragma once
#include "AxisParam.h"

enum AXIS_ID
{
	//NMC1

	AXIS_INSPECTION_X1,
	AXIS_INSPECTION_Z1,
	AXIS_ACTIVE_ALIGN_X1,
	AXIS_JIG_SHUTTLE_Y1,

	//NMC2
	AXIS_INSPECTION_X2,
	AXIS_INSPECTION_Z2,
	AXIS_ACTIVE_ALIGN_X2,
	AXIS_JIG_SHUTTLE_Y2,

	AXIS_ID_MAX
};

class CAxisParamBank
{
public:
	CAxisParamBank(void);
	~CAxisParamBank(void);

	CAxisParam m_Axis[AXIS_ID_MAX];

	void InitializeAxisType();

	void ReadAxisParam();
	void SaveAxisParam();

	BOOL MotorIsUsed(AXIS_ID id);
	BOOL GetParamNode(AXIS_ID id, CAxisParam& node);

};

