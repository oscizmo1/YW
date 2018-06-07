#include "StdAfx.h"
#include "ProcDataBank.h"
#include "GeryonApp.h"
#include "UI\GausGUI\GxMsgBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

BOOL CProcDataBank::GetZoneBPG_ON_Check()
{
	//�˻�� PG ON ��ư Ȯ��
	//return (theDeviceIO.ReadInBit(X_CYLINDER_FWD_SEN) == theIOBank.m_In[X_CYLINDER_FWD_SEN].ReturnFrom(vac)) ? TRUE:FALSE;
	return TRUE;
}

//kjpark 20170912 JobProcess�� Invalid�� �Ǹ� �˾� �����
void CProcDataBank::SetInvalidJobPorcessNotMachingID()
{
	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, ON);

	//kjpark 20170711 CIM MESSAGE STATUS ���� ǥ��
	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_JOBPROCESS);

	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// ����� �̸��� ����
	dlgMsgBox.SetTitle(_T("Ȯ��"), _T("Confirm"), _T("Confirm"));
	dlgMsgBox.SetMessage(FALSE, 
		_T("Cell is Not Maching"), 
		_T("Cell Remove?"),		
		_T("Cell Remove?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	theUnitFunc.SetOutPutIO(Y_BUZZER_K1, OFF);

	//kjpark 20170711 CIM MESSAGE STATUS ���� ǥ��
	::PostMessageA(GetMainWnd(), MSG_CIM_MESSAGE_UPDATE, NULL, CIM_MESSAGE_MAX);
}