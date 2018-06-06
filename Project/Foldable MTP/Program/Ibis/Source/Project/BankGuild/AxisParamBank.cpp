#include "StdAfx.h"
#include "AxisParamBank.h"
#include "IbisApp.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CAxisParamBank::CAxisParamBank(void)
{
}


CAxisParamBank::~CAxisParamBank(void)
{
}

void CAxisParamBank::InitializeAxisType()
{
	// 파일 Load/Save를 하지 않을 항목들을 여기서 정의한다
 	//////////////////////////////////////////////////////////////////////////
	// NMC
	m_Axis[	AXIS_INSPECTION_X1			].m_strAxisName = _T("AXIS_INSPECTION_X1");
	m_Axis[	AXIS_INSPECTION_X1			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_INSPECTION_X1			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_INSPECTION_X1			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_INSPECTION_X1			].m_nModuleNo = 1;
	m_Axis[	AXIS_INSPECTION_X1			].m_nInnerNo = 0;

	m_Axis[	AXIS_INSPECTION_Z1			].m_strAxisName = _T("AXIS_INSPECTION_Z1");
	m_Axis[	AXIS_INSPECTION_Z1			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_INSPECTION_Z1			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_INSPECTION_Z1			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_INSPECTION_Z1			].m_nModuleNo = 1;
	m_Axis[	AXIS_INSPECTION_Z1			].m_nInnerNo = 1;

	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_strAxisName = _T("AXIS_ACTIVE_ALIGN_X1");
	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_nModuleNo = 1;
	m_Axis[	AXIS_ACTIVE_ALIGN_X1		].m_nInnerNo = 2;

	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_strAxisName = _T("AXIS_JIG_SHUTTLE_Y1");
	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_nModuleNo = 1;
	m_Axis[	AXIS_JIG_SHUTTLE_Y1			].m_nInnerNo = 3;

	m_Axis[	AXIS_INSPECTION_X2			].m_strAxisName = _T("AXIS_INSPECTION_X2");
	m_Axis[	AXIS_INSPECTION_X2			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_INSPECTION_X2			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_INSPECTION_X2			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_INSPECTION_X2			].m_nModuleNo = 2;
	m_Axis[	AXIS_INSPECTION_X2			].m_nInnerNo = 0;

	m_Axis[	AXIS_INSPECTION_Z2			].m_strAxisName = _T("AXIS_INSPECTION_Z2");
	m_Axis[	AXIS_INSPECTION_Z2			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_INSPECTION_Z2			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_INSPECTION_Z2			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_INSPECTION_Z2			].m_nModuleNo = 2;
	m_Axis[	AXIS_INSPECTION_Z2			].m_nInnerNo = 1;

	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_strAxisName = _T("AXIS_ACTIVE_ALIGN_X2");
	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_nModuleNo = 2;
	m_Axis[	AXIS_ACTIVE_ALIGN_X2		].m_nInnerNo = 2;

	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_strAxisName = _T("AXIS_JIG_SHUTTLE_Y2");
	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_DeviceType = CAxisParam::DEVICE_TYPE_NMC;
	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_MotorType = CAxisParam::MOTOR_TYPE_SERVO;
	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_OriginType = CAxisParam::ORIGIN_TYPE_LIMIT_N;
	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_nModuleNo = 2;
	m_Axis[	AXIS_JIG_SHUTTLE_Y2			].m_nInnerNo = 3;
}

void CAxisParamBank::ReadAxisParam()
{
	CString strPath;
	strPath.Format(_T("%s\\Axis.ini"), theConfigBank.m_System.m_strConfigPath);
	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		m_Axis[i].ReadFile(strPath);
	}
}

void CAxisParamBank::SaveAxisParam()
{
	CTime time = CTime::GetCurrentTime();
	CString strPath, strBackupPath;
	strPath.Format(_T("%s\\Axis.ini"), theConfigBank.m_System.m_strConfigPath);
	strBackupPath.Format(_T("%s\\%d_%d\\Axis_%02d_%s.ini"), theConfigBank.m_System.m_strConfigBackupPath, 
		time.GetYear(), time.GetMonth(), time.GetDay(), time.Format(_T("%H%M%S")));

	CString strFolderPath = CFileSupport::GetPathName(strPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}
	strFolderPath = CFileSupport::GetPathName(strBackupPath);
	if(CFileSupport::DirectoryCheck(strFolderPath) == FALSE)
	{
		if(CFileSupport::CreateDirectory(strFolderPath) == FALSE)
			return;
	}

	if(CFileSupport::FileCheck(strPath))
	{
		// 기존 파일 백업
		::CopyFile(strPath, strBackupPath, FALSE);
		// File 내용을 깔끔하게 하기 위해 기존껀 지우고 새로 적는다
		::DeleteFile(strPath);
	}

	for(int i = 0; i < AXIS_ID_MAX; i++)
	{
		m_Axis[i].SaveFile(strPath);
	}
}
BOOL CAxisParamBank::MotorIsUsed(AXIS_ID id)
{
	if(id==AXIS_ID_MAX)
		return FALSE;


	CAxisParam	node;
	node = m_Axis[id];

	return node.m_bUse;
}
BOOL CAxisParamBank::GetParamNode(AXIS_ID id, CAxisParam& node)
{
	if(id==AXIS_ID_MAX)
		return FALSE;

	node = m_Axis[id];

	return TRUE;
}