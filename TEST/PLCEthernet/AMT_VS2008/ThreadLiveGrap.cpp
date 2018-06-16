// ThreadLiveGrap.cpp: implementation of the CThreadLiveGrap class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "ThreadLiveGrap.h"
#include "MainFrm.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CThreadLiveGrap::CThreadLiveGrap()
{
	m_bLiveGrapThreadRun = FALSE;
	m_bLiveGrabThreadStart = FALSE;
	m_bSetTeachCenter = TRUE;	// Center를 기본값으로 설정 [2/9/2009 OSC]
	m_bLivePause = FALSE;
	m_Resume = FALSE;
}

CThreadLiveGrap::~CThreadLiveGrap()
{

}

void CThreadLiveGrap::ThreadLiveGrapFunction()
{
	CMIL71* pVision = GetAlignCtl()->GetVision();
	m_bLiveGrabThreadStart = TRUE;

	int nCurStatus =  STATUS_READY ;
	int nCamNo = CAM_0;
	int nFormID = FORM_AUTO;

	while( m_bLiveGrapThreadRun )
	{
		if(m_bLivePause == FALSE)
		{
			if(m_Resume == FALSE) m_Resume = TRUE;

			nFormID = GetMainFrame()->m_nCurrentFormID;
			if(nFormID == FORM_AUTO)
			{
				for (nCamNo = CAM_0; nCamNo < MAX_CAM_NUM; nCamNo++)
				{
					if(GetDoc()->m_bLiveGrapFlag[nCamNo])
						pVision->Grab(nCamNo, nCamNo, nCurStatus);
				}
			}
			else if(nFormID == FORM_TEACH)
			{
				//////////////////////////////////////////////////////////////////////////
				// Center / Side별 Grab 구분 [2/9/2009 OSC]
				if(m_bSetTeachCenter)
				{
					if(GetDoc()->m_bLiveGrapFlag[CAM_1])
						pVision->Grab(DISP_TEACH_L, CAM_1, nCurStatus);
					if(GetDoc()->m_bLiveGrapFlag[CAM_2])
						pVision->Grab(DISP_TEACH_R, CAM_2, nCurStatus);
				}
				else
				{
					if(GetDoc()->m_bLiveGrapFlag[CAM_0])
						pVision->Grab(DISP_TEACH_L, CAM_0, nCurStatus);
					if(GetDoc()->m_bLiveGrapFlag[CAM_3])
						pVision->Grab(DISP_TEACH_R, CAM_3, nCurStatus);				}
			}
			else if(nFormID == FORM_ALIGN)
			{
				for (nCamNo = CAM_0; nCamNo < MAX_CAM_NUM; nCamNo++)
				{
					if(GetDoc()->m_bLiveGrapFlag[nCamNo])
					{
						pVision->Grab(DISP_ALIGN, nCamNo, nCurStatus);
					}
				}
			}
			Sleep(50);
		}
		else
		{
			if(m_Resume) m_Resume = FALSE;
		}
	}

	m_bLiveGrabThreadStart = FALSE;

	return;
}

void CThreadLiveGrap::SetTeachCam( BOOL bSetCenter )
{
	m_bSetTeachCenter = bSetCenter;
}

void CThreadLiveGrap::WaitLivePause()
{
	Sleep(100);
// 	// 1000ms동안 기다린다
// 	int i = 0;
// 	while(m_Resume)
// 	{
// 		ASSERT(i < 50);
// 		i++;
// 		Sleep(20);
// 	}
}

BOOL CThreadLiveGrap::CheckLivePause()
{
	return m_Resume ? FALSE:TRUE;
}