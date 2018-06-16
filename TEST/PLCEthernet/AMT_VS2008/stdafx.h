// stdafx.h : �� ������� �ʰ� ���� ����ϴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN		// Windows ������� ���� ������ �ʴ� ������ ���ܽ�ŵ�ϴ�.
#endif

// �Ʒ� ������ �÷������� �켱�ϴ� �÷����� ������� �ϴ� ��� ���� ���Ǹ� �����Ͻʽÿ�.
// �ٸ� �÷����� ���Ǵ� �ش� ���� �ֽ� ������ MSDN�� �����Ͻʽÿ�.
#ifndef WINVER				// Windows 95 �� Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define WINVER 0x0400		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_WINNT		// Windows NT 4 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINNT 0x0400		// Windows 98�� Windows 2000 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif						

#ifndef _WIN32_WINDOWS		// Windows 98 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_WINDOWS 0x0410 // Windows Me ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#ifndef _WIN32_IE			// IE 4.0 ���� ���������� ����� ����� �� �ֽ��ϴ�.
#define _WIN32_IE 0x0400	// IE 5.0 ���� ������ �µ��� ������ ������ ������ �ֽʽÿ�.
#endif

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ���
#include <afxext.h>         // MFC �ͽ��ټ�
#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ����

#include <afxdtctl.h>		// Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC ����
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// Windows ���� ��Ʈ�ѿ� ���� MFC ����
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC support for ribbons and control bars

#include <afxsock.h>		// MFC ���� �ͽ��ټ�
#include <direct.h>


#pragma comment(lib, "mil.lib")
#pragma comment(lib, "milim.lib")
#pragma comment(lib, "milpat.lib")
#pragma comment(lib, "milBlob.lib")

#pragma comment(lib, "Lib\\VC60FP39.lib")
#pragma comment(lib, "Lib\\CIF32DLL.lib")
#pragma comment(lib, "Lib\\MdFunc32.lib")
// #pragma comment(lib, "Lib\\MnetEx2.lib")

// Public class
#include "../Public class/dblPoint.h"
//#include "../Public class/dblSize.h"
#include "../Public class/HistoryEdit.h"
#include "../Public class/FileSupport.h"
#include "../Public class/DataSocket.h"
#include "../Public class/Ini.h"
#include "../Public class/StopWatch.h"
#include "../Public class/Etc.h"


#include "ErrorDefine.h"
#include "MotionDefine.h"
#include "NetworkDefine.h"
#include "UserDefine.h"
#include "WorkDefine.h"
#include "StructDefine.h"
#include "MySocketData.h"
#include <afxdhtml.h>

LineType GetLineType();
MachinePCType GetMachinePCType();
LangueageType GetLanguageType();
void SetLineType(LineType linetype);
void SetMachinePCType(MachinePCType machinetype);
void SetLanguageType(LangueageType languagetype);
BOOL IsSimulation();
void SetSimulation(BOOL bSimulate);
void SetEngineerPassWord(CString strEngPassWord);
void SetSuperUserPassWord(CString strSuperPassWord);

CString GetEngineerPassWord();
CString GetSuperUserPassWord();
