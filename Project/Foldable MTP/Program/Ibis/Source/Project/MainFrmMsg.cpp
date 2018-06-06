#include "stdafx.h"
#include "MainFrm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

LRESULT CMainFrame::OnMSGEvent(WPARAM wParam, LPARAM lParam)
{
	MSGEvent* pMsg = (MSGEvent*)lParam;
	if(!pMsg)
		return -1;

	SERIAL_MODE mode = (SERIAL_MODE)pMsg->message;

	switch(mode)
	{
	case SERIAL_GMS:
		break;
//kjpark 20161010 �������� PG PC ���� ���		
#ifdef OPERATOR_PC
	
	//kjpark2017/09/01 Serial ���� - Temp �߰�, �� 2 ����, ��ũ ����, ���ο���
	case SERIAL_LABELPRINT:
		break;
#endif	
	default:		
		break;
	}

	if(pMsg)
	{
		delete pMsg;
		pMsg = NULL;
	}
	return 0;
}



