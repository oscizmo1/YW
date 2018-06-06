#include "StdAfx.h"
#include "TeachBank.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"
#include "IbisApp.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

TEACH_PARAM::TEACH_PARAM( void )
{
	Init();
}

TEACH_PARAM::TEACH_PARAM( CString name, AXIS_ID axis, TEACH_PARAM::ID teach, double pos, double speed, double acc, double dec )
{
	strName = name;
	idAxis = axis;
	idTeach = teach;
	dPos = pos;
	dSpeed = speed;
	dAcc = acc;
	dDec = dec;
}

TEACH_PARAM::~TEACH_PARAM( void )
{
}

void TEACH_PARAM::Init()
{
	strName = _T("");;
	idAxis = AXIS_ID_MAX;
	idTeach = TEACH_PARAM::PARAM_MAX;
	dPos = 0.;
	dSpeed = 0.;
	dAcc = ACC_300ms;
	dDec = ACC_300ms;
}

void TEACH_PARAM::ReadPositionFile( CString strFilePath )
{
	// 파일 입출력은 티칭값과 속도만 한다.
	CIni ini(strFilePath);
	dPos = ini.GetDouble(strName, _T("POSITION"), 0.);
	dSpeed = ini.GetDouble(strName, _T("SPEED"), 0.);
}

void TEACH_PARAM::ReadSpeedFile(CString strFilePath)
{
	// 속도 관리 편하게 하기위해 속도는 별도 파일로 분류
	if(CFileSupport::FileCheck(strFilePath) == FALSE)
		return;
	CIni ini(strFilePath);
	dSpeed = ini.GetDouble(strName, _T("SPEED"), 0.);

// 	long nValue = ini.GetLong(strName, _T("ACC"), 300);
// 	dAcc = 1000. / (double)nValue;
// 	nValue = ini.GetLong(strName, _T("DEC"), 300);
// 	dDec = 1000. / (double)nValue;
}

void TEACH_PARAM::SavePositionFile( CString strFilePath )
{
	// 파일 입출력은 티칭값과 속도속만 한다.
	CIni ini(strFilePath);
	ini.WriteDouble(strName, _T("POSITION"), dPos);
	ini.WriteDouble(strName, _T("SPEED"), dSpeed);

	// CIM 보고 위해 파일 기록만 하고 읽진 않는다 [12/1/2016 OSC]
	ini.WriteLong(strName, _T("ACC"), (long)CEtc::Round(1000./dAcc));
	ini.WriteLong(strName, _T("DEC"), (long)CEtc::Round(1000./dDec));
}

void TEACH_PARAM::SaveSpeedFile(CString strFilePath)
{
	// 속도 관리 편하게 하기위해 속도는 별도 파일로 분류
	CIni ini(strFilePath);
	ini.WriteDouble(strName, _T("SPEED"), dSpeed);

	// CIM 보고 위해 파일 기록만 하고 읽진 않는다 [12/1/2016 OSC]
	ini.WriteLong(strName, _T("ACC"), (long)CEtc::Round(1000./dAcc));
	ini.WriteLong(strName, _T("DEC"), (long)CEtc::Round(1000./dDec));
}

CTeachBank::CTeachBank(void)
{
}


CTeachBank::~CTeachBank(void)
{
}

void CTeachBank::GetTeachParamList(int iAxis, CArray<TEACH_PARAM,TEACH_PARAM> &arr)
{
	arr.RemoveAll();
	TEACH_PARAM param;
	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++ )
	{
		param = GetTeachParam((TEACH_PARAM::ID)i);
		if (param.idAxis == iAxis )
			arr.Add(param);
	}
}

void CTeachBank::InitializeTeachParam()
{
	// 여기서 티칭값 명칭과, ID, 가속도, 감속도를 지정해준다.
	// 배열순서 : 티칭위치 이름,	ID, 티칭값,	속도,		가속도,	감속도
	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD			] = TEACH_PARAM(_T("JIG_SHUTTLE_Y1_to_LOAD"			), AXIS_JIG_SHUTTLE_Y1			, TEACH_PARAM::JIG_SHUTTLE_Y1_to_LOAD		, 0., 0., ACC_150ms, ACC_150ms);
	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP			] = TEACH_PARAM(_T("JIG_SHUTTLE_Y1_to_INSP"			), AXIS_JIG_SHUTTLE_Y1			, TEACH_PARAM::JIG_SHUTTLE_Y1_to_INSP		, 0., 0., ACC_150ms, ACC_150ms);
	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL		] = TEACH_PARAM(_T("JIG_SHUTTLE_Y1_to_MANUAL"		), AXIS_JIG_SHUTTLE_Y1			, TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL		, 0., 0., ACC_150ms, ACC_150ms);

	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD			] = TEACH_PARAM(_T("JIG_SHUTTLE_Y2_to_LOAD"			), AXIS_JIG_SHUTTLE_Y2			, TEACH_PARAM::JIG_SHUTTLE_Y2_to_LOAD		, 0., 0., ACC_150ms, ACC_150ms);
	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP			] = TEACH_PARAM(_T("JIG_SHUTTLE_Y2_to_INSP"			), AXIS_JIG_SHUTTLE_Y2			, TEACH_PARAM::JIG_SHUTTLE_Y2_to_INSP		, 0., 0., ACC_150ms, ACC_150ms);
	m_Param[TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL		] = TEACH_PARAM(_T("JIG_SHUTTLE_Y2_to_MANUAL"		), AXIS_JIG_SHUTTLE_Y2			, TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL		, 0., 0., ACC_150ms, ACC_150ms);

	m_Param[TEACH_PARAM::INSPECTION_X1_to_INSP			] = TEACH_PARAM(_T("INSPECTION_X1_to_INSP"			), AXIS_INSPECTION_X1			, TEACH_PARAM::INSPECTION_X1_to_INSP		, 0., 0., ACC_300ms, ACC_300ms);
	m_Param[TEACH_PARAM::INSPECTION_X2_to_INSP			] = TEACH_PARAM(_T("INSPECTION_X2_to_INSP"			), AXIS_INSPECTION_X2			, TEACH_PARAM::INSPECTION_X2_to_INSP		, 0., 0., ACC_300ms, ACC_300ms);
																																															
	m_Param[TEACH_PARAM::INSPECTION_Z1_to_UP			] = TEACH_PARAM(_T("INSPECTION_Z1_to_UP"			), AXIS_INSPECTION_Z1			, TEACH_PARAM::INSPECTION_Z1_to_UP			, 0., 0., ACC_300ms, ACC_300ms);
	m_Param[TEACH_PARAM::INSPECTION_Z1_to_INSP			] = TEACH_PARAM(_T("INSPECTION_Z1_to_INSP"			), AXIS_INSPECTION_Z1			, TEACH_PARAM::INSPECTION_Z1_to_INSP		, 0., 0., ACC_300ms, ACC_300ms);
																																														
	m_Param[TEACH_PARAM::INSPECTION_Z2_to_UP			] = TEACH_PARAM(_T("INSPECTION_Z2_to_UP"			), AXIS_INSPECTION_Z2			, TEACH_PARAM::INSPECTION_Z2_to_UP			, 0., 0., ACC_300ms, ACC_300ms);
	m_Param[TEACH_PARAM::INSPECTION_Z2_to_INSP			] = TEACH_PARAM(_T("INSPECTION_Z2_to_INSP"			), AXIS_INSPECTION_Z2			, TEACH_PARAM::INSPECTION_Z2_to_INSP		, 0., 0., ACC_300ms, ACC_300ms);

	m_Param[TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT_X		] = TEACH_PARAM(_T("ACTIVE_ALIGN_X1_to_LEFT_X"		), AXIS_ACTIVE_ALIGN_X1			, TEACH_PARAM::ACTIVE_ALIGN_X1_to_LEFT_X	, 0., 0., ACC_300ms, ACC_300ms);
	m_Param[TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT_X		] = TEACH_PARAM(_T("ACTIVE_ALIGN_X1_to_RIGHT_X"		), AXIS_ACTIVE_ALIGN_X1			, TEACH_PARAM::ACTIVE_ALIGN_X1_to_RIGHT_X	, 0., 0., ACC_300ms, ACC_300ms);

	m_Param[TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT_X		] = TEACH_PARAM(_T("ACTIVE_ALIGN_X2_to_LEFT_X"		), AXIS_ACTIVE_ALIGN_X2			, TEACH_PARAM::ACTIVE_ALIGN_X2_to_LEFT_X	, 0., 0., ACC_300ms, ACC_300ms);
	m_Param[TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT_X		] = TEACH_PARAM(_T("ACTIVE_ALIGN_X2_to_RIGHT_X"		), AXIS_ACTIVE_ALIGN_X2			, TEACH_PARAM::ACTIVE_ALIGN_X2_to_RIGHT_X	, 0., 0., ACC_300ms, ACC_300ms);

}

TEACH_PARAM CTeachBank::GetTeachParam( TEACH_PARAM::ID id )
{
	TEACH_PARAM param = m_Param[id];

	//////////////////////////////////////////////////////////////////////////
	// Recipe에 따라 자동으로 가변하는 수치 처리부분
	CRecipeTeach *pRecipeTeach = &theRecipeBank.m_Teach;
	switch (id)
	{

	case TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL:	if(pRecipeTeach->m_dShuttleY1_MCR != 0.)	
												{
													param.dPos = pRecipeTeach->m_dShuttleY1_MCR;	
												}
		break;
	case TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL:	if(pRecipeTeach->m_dShuttleY2_MCR != 0.)	
												{
													param.dPos = pRecipeTeach->m_dShuttleY2_MCR;	
												}
		
		break;

	default:																break;
	}
	//////////////////////////////////////////////////////////////////////////

	return param;
}

void CTeachBank::SetTeachParam( TEACH_PARAM param )
{
	//////////////////////////////////////////////////////////////////////////
	// Recipe로 가변했던 값 원복
	int nTeachID = param.idTeach;
	CRecipeTeach *pRecipeTeach = &theRecipeBank.m_Teach;
	switch (nTeachID)
	{
	case TEACH_PARAM::JIG_SHUTTLE_Y1_to_MANUAL:	pRecipeTeach->m_dShuttleY1_MCR = param.dPos;	break;
	case TEACH_PARAM::JIG_SHUTTLE_Y2_to_MANUAL:	pRecipeTeach->m_dShuttleY2_MCR = param.dPos;	break;
	default:																break;
	}
	//////////////////////////////////////////////////////////////////////////
	m_Param[nTeachID] = param;
}

void CTeachBank::ReadTeachFile()
{
	CString strPathPosition, strPathSpeed;
	strPathPosition.Format(_T("%s\\Teach.ini"), theConfigBank.m_System.m_strConfigPath);
	strPathSpeed.Format(_T("%s\\TeachSpeed.ini"), theConfigBank.m_System.m_strConfigPath);
	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++)
	{
		m_Param[i].ReadPositionFile(strPathPosition);
		m_Param[i].ReadSpeedFile(strPathSpeed);
	}
}

void CTeachBank::SaveTeachFile()
{
	CTime time = CTime::GetCurrentTime();
	CString strPathPosition, strBackupPathPosition, strPathDataPC;
	strPathPosition.Format(_T("%s\\Teach.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPathPosition.Format(_T("%s\\%d_%d\\Teach_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));

	CString strPathSpeed, strBackupPathSpeed;
	strPathSpeed.Format(_T("%s\\TeachSpeed.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPathSpeed.Format(_T("%s\\%d_%d\\TeachSpeed_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));


	CString strFolderPath = CFileSupport::GetPathName(strPathPosition);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPathPosition);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPathPosition))
	{
		// 기존 파일 백업
		::CopyFile(strPathPosition, strBackupPathPosition, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPathPosition);
	}

	if(CFileSupport::FileCheck(strPathSpeed))
	{
		// 기존 파일 백업
		::CopyFile(strPathSpeed, strBackupPathSpeed, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPathSpeed);
	}

	for(int i = 0; i < TEACH_PARAM::PARAM_MAX; i++)
	{
		m_Param[i].SavePositionFile(strPathPosition);
		m_Param[i].SaveSpeedFile(strPathSpeed);
	}

// 	// 상위보고용 파일 별도 저장
// 	SaveTeachDataPCFile();

	// Recipe에 저장하는 티칭값들 때문에 레시피도 Save
	theRecipeBank.SaveMainRecipeTeachFile();
}

void CTeachBank::SaveTeachDataPCFile()
{
	// 상위 RMS 보고용 파일
	// 전부 올리지 않고 필요한 것만 올린다
	// 앞으로 이 파일 내용은 절대 바꾸지 말것
	// 여기 항목 추가하면 MES에서도 추가해야 한다 [6/22/2017 OSC]
	CString strPathDataPC;
	strPathDataPC.Format(_T("%s\\TeachDataPC.ini"), theConfigBank.m_System.m_strConfigPath);

	if(CFileSupport::FileCheck(strPathDataPC))
	{
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPathDataPC);
	}

// 	m_Param[TEACH_PARAM::CELL_REVERSE_THETA_CV].SavePositionFile(strPathDataPC);
// 	m_Param[TEACH_PARAM::CELL_REVERSE_THETA_SCARA].SavePositionFile(strPathDataPC);
}
