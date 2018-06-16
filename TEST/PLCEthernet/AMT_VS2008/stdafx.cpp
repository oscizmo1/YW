// stdafx.cpp : ǥ�� ���� ������ �����ϴ� �ҽ� �����Դϴ�.
// MT.pch�� �̸� �����ϵ� ����� �˴ϴ�.
// stdafx.obj�� �̸� �����ϵ� ���� ������ �����մϴ�.

#include "stdafx.h"

LineType g_LineType = A_GROUP_TYPE;		//�ʱ⿡ A������ ������ ������ �����ϵ��� �����. [02/05/2009 JJH]
MachinePCType g_MachineType = A_AMT;
LangueageType g_LanguageType = LAN_ENG;
BOOL g_Simulation = FALSE;
CString g_EngPassWord = "";
CString g_SuperUserPassWord = "";

LineType GetLineType()
{
	return g_LineType;
}

MachinePCType GetMachinePCType()
{
	return g_MachineType;
}

LangueageType GetLanguageType()
{
	return g_LanguageType;
}

void SetLineType( LineType linetype )
{
	g_LineType = linetype;
}

void SetMachinePCType( MachinePCType machinetype )
{
	g_MachineType = machinetype;
}

void SetLanguageType( LangueageType languagetype )
{
	g_LanguageType = languagetype;
}

BOOL IsSimulation()
{
	return g_Simulation;
}

void SetSimulation( BOOL bSimulate )
{
	g_Simulation = bSimulate;
}
void SetEngineerPassWord( CString strEngPassWord )
{
	g_EngPassWord = strEngPassWord;
}

void SetSuperUserPassWord( CString strSuperPassWord )
{
	g_SuperUserPassWord = strSuperPassWord;
}

CString GetSuperUserPassWord()
{
	return g_SuperUserPassWord;
}

CString GetEngineerPassWord()
{
	return g_EngPassWord;
}
