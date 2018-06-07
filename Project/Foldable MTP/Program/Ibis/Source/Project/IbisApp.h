
// Ibis.h : Ibis 응용 프로그램에 대한 주 헤더 파일
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH에 대해 이 파일을 포함하기 전에 'stdafx.h'를 포함합니다."
#endif

#include "resource.h"       // 주 기호입니다.

#include "Thread/Thread.h"
#include "BankGuild/LogUnit.h"
#include "BankGuild/LossTimeLog.h"
#include "BankGuild/AxisParamBank.h"
#include "BankGuild/CellBank.h"
#include "BankGuild/IOParamBank.h"
#include "BankGuild/ConfigBank.h"
#include "BankGuild/ProcDataBank.h"
#include "BankGuild/RecipeBank.h"
#include "BankGuild/TeachBank.h"
#include "BankGuild/FDCBank.h"
#include "BankGuild/UnitCtrlBank.h"
#include "Device/DeviceMotion.h"
#include "Device/DeviceIO.h"
#include "Device/SocketDialog.h"
#include "Device/SerialManager.h"
#include "Device/DevicePDT_IF.h"
#include "BankGuild/ProductBank.h"
#include "BankGuild/TactTimeLog.h"
#include "BankGuild/InspModuleBank/InspModuleBank.h"
#include "BankGuild/ThreadBank/ThreadBank.h"
#include "BankGuild/UnitStatusBank.h"

// CIbisApp:
// 이 클래스의 구현에 대해서는 Ibis.cpp을 참조하십시오.
//
class CIbisApp : public CWinApp
{
public:
	CIbisApp();

protected:
	
	// GDI+ 用
	ULONG_PTR				gdiplusToken;
	GdiplusStartupInput		gdiplusstartupinput;
	// end of GDI+

// 재정의입니다.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void InitializeLog();


// 구현입니다.
	DECLARE_MESSAGE_MAP()
};

extern CIbisApp			theApp;
extern CThreadInitializer	theThreadInitializer;
extern CLogUnit				theLog[LOG_MAX];
extern CAxisParamBank		theAxisBank;
extern CCellBank			theCellBank;
extern CIOParamBank			theIOBank;
extern CConfigBank			theConfigBank;
extern CProcDataBank		theProcBank;
extern CRecipeBank			theRecipeBank;
extern CLossTimeLog			theLossTimeLog;
extern CTeachBank			theTeachBank;
extern CFDCBank				theFDCBank;
extern CUnitCtrlFunc		theUnitFunc;		// 티칭창 등 Main Process에서 호출 위해 만듦. 절대 Thread에서 사용하지 말것!
extern CDeviceMotion		theDeviceMotion;
extern CDeviceIO			theDeviceIO;


//kjpark 20160912 socket test
extern CSocketDialog			theSocketInterFace;
extern CSerialManager			theSerialInterFace;

extern CDevicePDT_IF			theDevicePDT_IF;

//  ProductData [11/7/2016 WHLEE]
extern CProductBank				theProductBank;

extern CTactTimeLog				theTactTimeLog;

extern CInspModuleBank			theInspModuleBank;

extern CThreadBank				theThreadBank;

extern CUnitStatusBank			theUnitStatusBank;