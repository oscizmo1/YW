
// stdafx.h : ���� ��������� ���� ��������� �ʴ�
// ǥ�� �ý��� ���� ���� �� ������Ʈ ���� ���� ������ 
// ��� �ִ� ���� �����Դϴ�.

#pragma once



// �� ������Ʈ���� ������ �ɼ� - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
//#pragma warning(disable: 4819)	// MIL ���� C4819 �ڵ����� ��� �ٷ��߻����� �ش� ��� �����ϵ��� ����
		// mil.h : warning C4819: ���� �ڵ� ������(949)���� ǥ���� �� ���� ���ڰ� ���Ͽ� ��� �ֽ��ϴ�. 
		//				�����Ͱ� �սǵ��� �ʰ� �Ϸ��� �ش� ������ �����ڵ� �������� �����Ͻʽÿ�.

// #define _CRT_SECURE_NO_WARNINGS	// C4996 ��� disable, fopen(), strtok() ���� �Լ����� �������������ִ� xxx_s ���� �Լ��� ��ü�ϵ����ϴ� �ǰ� ����
// - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 




#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // ���� ������ �ʴ� ������ Windows ������� �����մϴ�.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // �Ϻ� CString �����ڴ� ��������� ����˴ϴ�.

// MFC�� ���� �κа� ���� ������ ��� �޽����� ���� ����⸦ �����մϴ�.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC �ٽ� �� ǥ�� ���� ����Դϴ�.
#include <afxext.h>         // MFC Ȯ���Դϴ�.


#include <afxdisp.h>        // MFC �ڵ�ȭ Ŭ�����Դϴ�.



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows ���� ��Ʈ�ѿ� ���� MFC �����Դϴ�.
#endif // _AFX_NO_AFXCMN_SUPPORT


#include <afxsock.h>            // MFC ���� Ȯ��



//GDI+ ���
#include <gdiplus.h>
#pragma comment(lib, "gdiplus")
using namespace Gdiplus;


//
// Visual Studio 2005, 2008 ~ ���� TRACE �� ��� �� �ѱ��� ����� ���,
// _CrtDbgReport: String too long or IO Error ������ �߻���,
//
// #include <locale.h>     include �� ��
//
// _tsetlocale(LC_ALL, _T("korean"));  ȣ��
//

// Visual Momory Leak Detect
//�̻�� #include "VLD\vld.h"			// for Memory Detect !!

#include "Etc/Etc.h"
#include "Socket/DataSocket.h"
#include "BankGuild/CommonDefineBank.h"
#include "BankGuild/AlarmDefine.h"
#include "Etc/Token.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//kjpark 20160908 kjpark Test Mode Set
#define CMiLRe 
//kjpark 20161010 �������� PG PC ���� ���
//#define OPERATOR_PC
//#define DISPLAY_INSPECT_CHECK
//#define DRY_RUN
#define NOT_USED_INSPECTION_PC
//kjpark loadcell ��� ����
//#define LOADCELL_NOT_USED
//kjpark 20161019 ��� �۾� �̽��� B<->D ���� �ٲ����
#define B2D_CHANGE//Worktable �� B ���� D ���� ��� �۾� �̽��� �ٲ���
//kjpark 20170922 GMS ��� �Ϸ�
//#define PGMS_USE_DB
#include "GeneralPurposeFunction.h"
#include <afxcontrolbars.h>
