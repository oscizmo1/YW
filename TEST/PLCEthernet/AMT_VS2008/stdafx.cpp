// stdafx.cpp : 표준 포함 파일을 포함하는 소스 파일입니다.
// MT.pch는 미리 컴파일된 헤더가 됩니다.
// stdafx.obj는 미리 컴파일된 형식 정보를 포함합니다.

#include "stdafx.h"

LineType g_LineType = A_GROUP_TYPE;		//초기에 A군으로 설정후 변경이 가능하도록 만든다. [02/05/2009 JJH]
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
