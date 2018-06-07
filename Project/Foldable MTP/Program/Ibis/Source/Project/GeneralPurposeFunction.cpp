#include "StdAfx.h"
#include "GeneralPurposeFunction.h"
#include "MainFrm.h"

CMainFrame* g_pMainWnd = NULL;

void SetMainWnd()
{
	//-- GET MAINFRAME POINT
	g_pMainWnd = (CMainFrame*)AfxGetMainWnd();


}

HWND GetMainWnd()
{
	return g_pMainWnd->GetSafeHwnd();
}

//kjpark 20161019 양수버튼 체크
CMainFrame* GetMainHandler()
{
	return g_pMainWnd;
}
