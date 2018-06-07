#pragma once

#include "AxisParamBank.h"

struct TEACH_PARAM
{
	enum ID 
	{
		JIG_SHUTTLE_Y1_to_LOAD,
		JIG_SHUTTLE_Y1_to_INSP,
		JIG_SHUTTLE_Y1_to_MANUAL,

		JIG_SHUTTLE_Y2_to_LOAD,
		JIG_SHUTTLE_Y2_to_INSP,
		JIG_SHUTTLE_Y2_to_MANUAL,

		INSPECTION_X1_to_INSP,
		INSPECTION_X2_to_INSP,

		INSPECTION_Z1_to_UP,
		INSPECTION_Z1_to_INSP,

		INSPECTION_Z2_to_UP,
		INSPECTION_Z2_to_INSP,

		ACTIVE_ALIGN_X1_to_LEFT,
		ACTIVE_ALIGN_X1_to_RIGHT,

		ACTIVE_ALIGN_X2_to_LEFT,
		ACTIVE_ALIGN_X2_to_RIGHT,

		PARAM_MAX
	};

	CString strName;

	AXIS_ID idAxis;
	TEACH_PARAM::ID idTeach;

	double dPos;
	double dSpeed;
	double dAcc;
	double dDec;

	TEACH_PARAM(void);
	TEACH_PARAM(CString name, AXIS_ID axis, TEACH_PARAM::ID teach, double pos, double speed, double acc, double dec);
	~TEACH_PARAM(void);

	void Init();
	void ReadPositionFile(CString strFilePath);
	void ReadSpeedFile(CString strFilePath);
	void SavePositionFile(CString strFilePath);
	void SaveSpeedFile(CString strFilePath);
};



class CTeachBank
{
public:
	CTeachBank(void);
	~CTeachBank(void);

private:
	TEACH_PARAM m_Param[TEACH_PARAM::PARAM_MAX];

public:
	void InitializeTeachParam();

	TEACH_PARAM GetTeachParam(TEACH_PARAM::ID id);
	void SetTeachParam(TEACH_PARAM param);

	void ReadTeachFile();
	void SaveTeachFile();
	void SaveTeachDataPCFile();	// 상위 RMS 보고용 파일
	
	void GetTeachParamList(int iAxis, CArray<TEACH_PARAM,TEACH_PARAM> &arr);
};

