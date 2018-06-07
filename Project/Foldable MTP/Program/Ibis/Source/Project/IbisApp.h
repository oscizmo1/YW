
// Ibis.h : Ibis ���� ���α׷��� ���� �� ��� ����
//
#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"       // �� ��ȣ�Դϴ�.

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
// �� Ŭ������ ������ ���ؼ��� Ibis.cpp�� �����Ͻʽÿ�.
//
class CIbisApp : public CWinApp
{
public:
	CIbisApp();

protected:
	
	// GDI+ ��
	ULONG_PTR				gdiplusToken;
	GdiplusStartupInput		gdiplusstartupinput;
	// end of GDI+

// �������Դϴ�.
public:
	virtual BOOL InitInstance();
	virtual int ExitInstance();

	void InitializeLog();


// �����Դϴ�.
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
extern CUnitCtrlFunc		theUnitFunc;		// ƼĪâ �� Main Process���� ȣ�� ���� ����. ���� Thread���� ������� ����!
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