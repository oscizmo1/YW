#include "StdAfx.h"
#include "AxisParam.h"
#include "Etc/Ini.h"
#include "Etc/FileSupport.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CAxisParam::CAxisParam(void)
{
	Init();
}


CAxisParam::~CAxisParam(void)
{
}

void CAxisParam::Init()
{
// 	m_strAxisName.Empty();
	m_bUse = FALSE;		// �������� �űԷ� �߰��Ǵ� ���� �⺻�� �̻�� [6/1/2017 OSC]
// 	m_DeviceType = DEVICE_TYPE_NMC;
// 	m_nModuleNo = 0;
// 	m_nInnerNo = 0;
// 	m_OriginType = ORIGIN_TYPE_HOME;
// 	m_MotorType = MOTOR_TYPE_SERVO;
	m_bInfiniteMove = FALSE;
	m_dJogSpeedHigh = 0.;
	m_dJogSpeedMiddle = 0.;
	m_dJogSpeedLow = 0.;
	m_dHomeSpeed = 0.;
	m_dDefaultMoveSpeed = 0.;
	m_dPositiveLimit = 0.;
	m_dNegativeLimit = 0.;
	m_dGearRatio = 0.;
	m_dDefaultAccel = ACC_300ms;
	m_dDefaultDecel = ACC_300ms;
}

void CAxisParam::ReadFile( CString strFilePath )
{
	// �� �̸��� ���� �ȵ� ������ return
	if(m_strAxisName.GetLength() < 1)
		return;


	if(CFileSupport::FileCheck(strFilePath) == FALSE)
	{
		// ������ ���� ��� ����ó��. �ϴ� returnó�� ������ �ʴ´�
	}

	CIni ini(strFilePath);
	// ���� ���� �ʱ�ȭ �� �� �ҷ��´�
	Init();

	m_bUse					= ini.GetBoolean(m_strAxisName, _T("m_bUse"), m_bUse);
	// �Ʒ� ������ ���󰡸� �����ϴ� �ϴ� File ������� �ּ�
// 	m_DeviceType			= (DEVICE_TYPE)ini.GetLong(m_strAxisName, _T("m_DeviceType"), m_DeviceType);
// 	m_nModuleNo				= ini.GetLong(m_strName, _T("m_nModuleNo"), m_nModuleNo);
// 	m_nInnerNo				= ini.GetLong(m_strName, _T("m_nInnerNo"), m_nInnerNo);
// 	m_OriginType			= (ORIGIN_TYPE)ini.GetLong(m_strAxisName, _T("m_OriginType"), (long)m_OriginType);
// 	m_MotorType				= (MOTOR_TYPE)ini.GetLong(m_strAxisName, _T("m_MotorType"), (long)m_MotorType);
// 	m_bInfiniteMove			= ini.GetBoolean(m_strAxisName, _T("m_bInfiniteMove"), m_bInfiniteMove);
	m_dJogSpeedHigh			= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedHigh"), m_dJogSpeedHigh);
	m_dJogSpeedMiddle		= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedMiddle"), m_dJogSpeedMiddle);
	m_dJogSpeedLow			= ini.GetDouble(m_strAxisName, _T("m_dJogSpeedLow"), m_dJogSpeedLow);
	m_dHomeSpeed			= ini.GetDouble(m_strAxisName, _T("m_dHomeSpeed"), m_dHomeSpeed);
	m_dDefaultMoveSpeed		= ini.GetDouble(m_strAxisName, _T("m_dDefaultMoveSpeed"), m_dDefaultMoveSpeed);
	m_dPositiveLimit		= ini.GetDouble(m_strAxisName, _T("m_dPositiveLimit"), m_dPositiveLimit);
	m_dNegativeLimit		= ini.GetDouble(m_strAxisName, _T("m_dNegativeLimit"), m_dNegativeLimit);
	m_dGearRatio			= ini.GetDouble(m_strAxisName, _T("m_dGearRatio"), m_dGearRatio);
	m_dDefaultAccel			= ini.GetDouble(m_strAxisName, _T("m_dDefaultAccel"), m_dDefaultAccel);
	m_dDefaultDecel			= ini.GetDouble(m_strAxisName, _T("m_dDefaultDecel"), m_dDefaultDecel);
}

void CAxisParam::SaveFile( CString strFilePath )
{
	CIni ini(strFilePath);

	ini.WriteBoolean(m_strAxisName, _T("m_bUse"), m_bUse);
	// �Ʒ� ������ ���󰡸� �����ϴ� �ϴ� File ������� �ּ�
// 	ini.WriteLong(m_strName, _T("m_DeviceType"),					(long)m_DeviceType		);
// 	ini.WriteLong(m_strName, _T("m_nModuleNo"),				(long)m_nModuleNo	);
// 	ini.WriteLong(m_strName, _T("m_nInnerNo"),					(long)m_nInnerNo		);
// 	ini.WriteLong(m_strAxisName, _T("m_OriginType"),					(long)m_OriginType		);
// 	ini.WriteLong(m_strAxisName, _T("m_MotorType"),					(long)m_MotorType		);
// 	ini.WriteLong(m_strAxisName, _T("m_bInfiniteMove"),				(long)m_bInfiniteMove		);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedHigh"),			m_dJogSpeedHigh		);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedMiddle"),		m_dJogSpeedMiddle	);
	ini.WriteDouble(m_strAxisName, _T("m_dJogSpeedLow"),			m_dJogSpeedLow		);
	ini.WriteDouble(m_strAxisName, _T("m_dHomeSpeed"),			m_dHomeSpeed			);
	ini.WriteDouble(m_strAxisName, _T("m_dDefaultMoveSpeed "),	m_dDefaultMoveSpeed );
	ini.WriteDouble(m_strAxisName, _T("m_dPositiveLimit"),			m_dPositiveLimit			);
	ini.WriteDouble(m_strAxisName, _T("m_dNegativeLimit"),			m_dNegativeLimit		);
	ini.WriteDouble(m_strAxisName, _T("m_dGearRatio"),				m_dGearRatio			);
	ini.WriteDouble(m_strAxisName, _T("m_dDefaultAccel"),			m_dDefaultAccel			);
	ini.WriteDouble(m_strAxisName, _T("m_dDefaultDecel"),			m_dDefaultDecel			);
}
