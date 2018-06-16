// AlignCtl.cpp: implementation of the CAlignCtl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MT.h"
#include "AlignCtl.h"
#include "MainFrm.h"
#include "frmAuto.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define NO_IMG_SAVE			0
#define EVERY_IMG_SAVE		1
#define ERROR_IMG_SAVE		2


//////////////////////////////////////////////////////////////////////////
// singleton
CAlignCtl* CAlignCtl::_instance=NULL;
CAlignCtl* CAlignCtl::Instance()
{
	if (_instance == NULL) {
		_instance = new CAlignCtl;
	}
	return _instance;
}
void CAlignCtl::Release()
{
	// Release() 함수 안에 추가 코드 구현 금지!!!
	if (_instance != NULL) {
		delete _instance;
	}
}
//////////////////////////////////////////////////////////////////////////

CAlignCtl::CAlignCtl()
{
    m_dScore = 0.0;
	m_ManualAlignResult.Distance.x = 0.0;
	m_ManualAlignResult.Distance.y = 0.0;
	m_ManualAlignResult.FPCPos.x = 0.0;
	m_ManualAlignResult.FPCPos.y = 0.0;
	m_ManualAlignResult.PCBPos.x = 0.0;
	m_ManualAlignResult.PCBPos.y = 0.0;
}

CAlignCtl::~CAlignCtl()
{

}

void CAlignCtl::LoadPatPreview(int nModelIndex)
{
	int nSizeX , nSizeY;
	BYTE* buf = NULL;

	CString strImgPath, strPatternName = _T("");

	nSizeX = GetDoc()->m_pAmtData->m_CurModelInfo.rtPattern[nModelIndex].Width();
	nSizeY = GetDoc()->m_pAmtData->m_CurModelInfo.rtPattern[nModelIndex].Height();
	switch( nModelIndex )
	{
	case CAM_0_FPC_0:
		strPatternName = _T("CAM0_FPC0");
		break;
	case CAM_0_FPC_1:
		strPatternName = _T("CAM0_FPC1");
		break;
	case CAM_0_PCB_0:
		strPatternName = _T("CAM0_PCB0");
		break;
	case CAM_0_PCB_1:
		strPatternName = _T("CAM0_PCB1");
		break;
	case CAM_1_FPC_0:
		strPatternName = _T("CAM1_FPC0");
		break;
	case CAM_1_FPC_1:
		strPatternName = _T("CAM1_FPC1");
		break;
	case CAM_1_PCB_0:
		strPatternName = _T("CAM1_PCB0");
		break;
	case CAM_1_PCB_1:
		strPatternName = _T("CAM1_PCB1");
		break;
	case CAM_2_FPC_0:
		strPatternName = _T("CAM2_FPC0");
		break;
	case CAM_2_FPC_1:
		strPatternName = _T("CAM2_FPC1");
		break;
	case CAM_2_PCB_0:
		strPatternName = _T("CAM2_FPC0");
		break;
	case CAM_2_PCB_1:
		strPatternName = _T("CAM2_PCB1");
		break;
	case CAM_3_FPC_0:
		strPatternName = _T("CAM3_FPC0");
		break;
	case CAM_3_FPC_1:
		strPatternName = _T("CAM3_FPC1");
		break;
	case CAM_3_PCB_0:
		strPatternName = _T("CAM3_PCB0");
		break;
	case CAM_3_PCB_1:
		strPatternName = _T("CAM3_PCB1");
		break;
	}

	strImgPath.Format("%s\\Mark\\%s-%s.tif", GetApp()->m_strAlignPath , GetDoc()->m_pAmtData->m_CurModelInfo.strModelName, strPatternName);
	m_Vision.LoadPatPreview(strImgPath , nSizeX , nSizeY, nModelIndex );
}

void CAlignCtl::DrawCenLine(int iNo, int ixPos, int iyPos, bool bOverClear)
{
	m_Vision.DrawCenLine(iNo, CPoint(ixPos, iyPos), bOverClear);
}

void CAlignCtl::LoadPattern( int iPattern , STRUCT_MODEL_DATA *pModelData )
{
	CString strPtrPath;
	if(GetDoc()->m_bUseBinary)
	{
		strPtrPath.Format("%s\\%s.bmp", GetApp()->m_strRecipePath, GeneratePatternFileName(iPattern));
	}
	else
		strPtrPath.Format("%s\\%s\\%s.bmp", GetApp()->m_strRecipePath , pModelData->strModelName, GeneratePatternFileName(iPattern));
	m_Vision.LoadPattern(strPtrPath, iPattern, pModelData->rtPattern[iPattern], pModelData->ptCenter[iPattern]);
}

void CAlignCtl::ManualSearchPattern(int nModelIndex)
{
	// ThreadLiveGrab 일시정지
	GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;
	GetDoc()->m_pThreadLiveGrap->WaitLivePause();
	SearchPattern(nModelIndex, TRUE);
	// ThreadLiveGrab 일시정지 해제
	GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
}

BOOL CAlignCtl::SearchPattern( int nModelIndex , BOOL bIsManual , int iRetryCnt /*= 0 */ )
{
	m_dwSearchTime = GetTickCount();

	STRUCT_MODEL_DATA * pModelData = &GetDoc()->m_pAmtData->m_CurModelInfo;

	double dCenX , dCenY, dScore ;
	int nFormID = GetMainFrame()->m_nCurrentFormID;
	int iRet;
	int nCamIndex = nModelIndex / MAX_CAM_NUM;
	int nPatternIndex = nModelIndex % MAX_CAM_NUM;
	int nStatus, nBufferIndex;
	CString strPosition;

	switch(nFormID)
	{
	case FORM_AUTO:
		nBufferIndex = DISP_AUTO_0+nCamIndex;
		if(nPatternIndex != 3)
			::PostMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_ALIGN_COUNT, (WPARAM)nCamIndex, NULL);
		break;
	case FORM_TEACH:
		nBufferIndex = (nModelIndex > CAM_1 ? DISP_TEACH_R : DISP_TEACH_L);
		break;
	case FORM_ALIGN:
		nBufferIndex = DISP_ALIGN;
		break;
	default:
		nBufferIndex = DISP_AUTO_0+nCamIndex;
		break;
	}
	if(bIsManual)
		nStatus = STATUS_STOP;
	else
		nStatus = STATUS_RUN;

	// SearchPattern을 위해 새로 Grab
	m_Vision.Grab(nBufferIndex, nCamIndex, nStatus);
	m_Vision.Grab(nBufferIndex, nCamIndex, nStatus);

	int nFPCModelNum;
	// PCB Model일 경우 고정
	if( nPatternIndex == 3)
		nFPCModelNum = 1;
	else
		nFPCModelNum = pModelData->nFPCModelNum[nCamIndex];

	if(GetDoc()->m_bUseBinary && nPatternIndex != 3)
		iRet = m_Vision.SearchBinaryPattern(dCenX, dCenY, dScore, nBufferIndex, nModelIndex, nFPCModelNum, GetDoc()->m_nBinarizeFPC[nCamIndex]);
	else
		iRet = m_Vision.SearchPattern(dCenX, dCenY, dScore, nBufferIndex, nModelIndex, nFPCModelNum);

	m_rtPattern = pModelData->rtPattern[nModelIndex];
	// PCB는 매번하지 않기 때문에 화면 표시 위해 따로 저장해 둔다.
	if(nPatternIndex == 3)		m_dPCBScore = dScore;
	m_dScore = dScore;
	m_dwSearchTime = GetTickCount() - m_dwSearchTime;

	m_ptCenter = CdblPoint(dCenX, dCenY).ToCPoint();
	CSize sizeOffset = m_ptCenter - pModelData->ptCenter[nModelIndex];
	m_rtPattern.OffsetRect(sizeOffset);

	BOOL bPCB = (nPatternIndex == 3) ? TRUE:FALSE;
	// Auto일 경우 PCB와 FPC Serarch 영역을 모두 표시
	if(bIsManual == FALSE && nPatternIndex == CAM_0_FPC_0 && nFormID == FORM_AUTO)	
	{
		CRect pcbRect = pModelData->rtPattern[nCamIndex+3];
		CPoint pcbCenter = GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.ToCPoint();
		CSize pcbSizeOffset = pcbCenter - pModelData->ptCenter[nCamIndex+3];
		pcbRect.OffsetRect(pcbSizeOffset);
		m_Vision.DrawPatRect(TRUE, 1, nBufferIndex, pcbRect, CdblPoint(pcbCenter), m_dPCBScore, 0, m_dwSearchTime, false);
		m_Vision.DrawPatRect(FALSE, 0, nBufferIndex, m_rtPattern, CdblPoint(dCenX, dCenY), m_dScore, nFPCModelNum, m_dwSearchTime, true);
		strPosition.Format("%f , %f", dCenX, dCenY);
	}
	else
		m_Vision.DrawPatRect(bPCB, 1, nBufferIndex, m_rtPattern, CdblPoint(dCenX, dCenY), m_dScore, nFPCModelNum, m_dwSearchTime, true);


	if( iRet == 1 )	//정상적으로 찾았을 경우..
	{
		if( m_ptCenter.x < 0 || m_ptCenter.x >= IMAGE_SIZE_X || m_ptCenter.y < 0 || m_ptCenter.y >= IMAGE_SIZE_Y )
		{
			if(GetDoc()->m_bErrImgSave && bIsManual == FALSE)
			{
				SaveSearchImage(TRUE, nCamIndex, strPosition, m_dScore);
			}
			// 실패했을 경우만 Live 재가동
			// 성공했을 경우 계속 SearchPattern을 할 수 있기 때문에 외부에서 재가동한다.
			//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
			return FALSE;
		}
		if(GetDoc()->m_bSuccessImgSave && bIsManual == FALSE && nPatternIndex != 3)
		{
			// Mark를 모두 찾았으면 Option 설정 여부에 따라 image save
			SaveSearchImage(FALSE, nCamIndex, strPosition, m_dScore);
		}
	}
	else	// 찾지 못했을 경우...
	{
		if(GetDoc()->m_bErrImgSave && bIsManual == FALSE)
			SaveSearchImage(TRUE, nCamIndex, strPosition, m_dScore);

		// 실패했을 경우만 Live 재가동
		// 성공했을 경우 계속 SearchPattern을 할 수 있기 때문에 외부에서 재가동한다.
		//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		return FALSE;
	}

	if(bIsManual)
	{
		// FormAlign 화면 표시 위해...
		if(nPatternIndex == 3)
		{
			m_ManualAlignResult.PCBPos.x = m_ptCenter.x;
			m_ManualAlignResult.PCBPos.y = m_ptCenter.y;
		}
		else
		{
			m_ManualAlignResult.FPCPos.x = m_ptCenter.x;
			m_ManualAlignResult.FPCPos.y = m_ptCenter.y;
		}
		m_ManualAlignResult.Distance.x = ( m_ManualAlignResult.PCBPos.x - m_ManualAlignResult.FPCPos.x ) * GetApp()->m_dFovX;
		m_ManualAlignResult.Distance.y = ( m_ManualAlignResult.PCBPos.y - m_ManualAlignResult.FPCPos.y ) * GetApp()->m_dFovY;
	}
	else
	{
		if(nPatternIndex == 3)
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.x = m_ptCenter.x;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.y = m_ptCenter.y;
		}
		else
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x = m_ptCenter.x;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y = m_ptCenter.y;
		}

		GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.x = ( GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.x - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x ) * GetApp()->m_dFovX;
		GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.y = ( GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.y - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y ) * GetApp()->m_dFovY;
 		GetDoc()->m_pAmtData->m_SearchResult.ContactX[nCamIndex] = GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x ;
 		GetDoc()->m_pAmtData->m_SearchResult.ContactY[nCamIndex] = GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y ;
 		GetDoc()->m_pAmtData->m_SearchResult.ContactScore[nCamIndex] = dScore;
 
 		if( iRet )		// Search 결과...
 		{
 			GetDoc()->m_pAmtData->m_SearchResult.SearchYN[nCamIndex].Format("Y");
 		}
 		else
 		{
 			GetDoc()->m_pAmtData->m_SearchResult.SearchYN[nCamIndex].Format("N");
 		}
	}

	return 1;
}

BOOL CAlignCtl::SearchPattern2( int nModelIndex , BOOL bIsManual , int iRetryCnt /*= 0 */ )//  [6/9/2009 janghan]
{
	//BOOL m_bReturnValue;
	//GetDoc()->m_pThreadLiveGrap->m_bLivePause = TRUE;//  [7/4/2009 janghan]

	m_dwSearchTime = GetTickCount();

	STRUCT_MODEL_DATA * pModelData = &GetDoc()->m_pAmtData->m_CurModelInfo;

	double dCenX , dCenY, dScore ;
	int nFormID = GetMainFrame()->m_nCurrentFormID;
	int iRet;
	int nCamIndex = nModelIndex / MAX_CAM_NUM;
	int nPatternIndex = nModelIndex % MAX_CAM_NUM;
	int nStatus, nBufferIndex;
	CString strPosition;

	switch(nFormID)
	{
	case FORM_AUTO:
		nBufferIndex = DISP_AUTO_0+nCamIndex;
		if(nPatternIndex != CAM_0_PCB_1)
			::PostMessage(GetMainFrame()->m_pForm[FORM_AUTO]->m_hWnd, WM_UPDATE_ALIGN_COUNT, (WPARAM)nCamIndex, NULL);
		break;
	case FORM_TEACH:
		nBufferIndex = (nModelIndex > CAM_1 ? DISP_TEACH_R : DISP_TEACH_L);
		break;
	case FORM_ALIGN:
		nBufferIndex = DISP_ALIGN;
		break;
	default:
		nBufferIndex = DISP_AUTO_0+nCamIndex;
		break;
	}
	if(bIsManual)
		nStatus = STATUS_STOP;
	else
		nStatus = STATUS_RUN;

	// SearchPattern을 위해 새로 Grab
	m_Vision.Grab(nBufferIndex, nCamIndex, nStatus);
	Sleep(10);
	m_Vision.Grab(nBufferIndex, nCamIndex, nStatus);

	int nFPCModelNum = 1;

	iRet = m_Vision.SearchBinaryPattern_auto(dCenX, dCenY, dScore, nBufferIndex, nModelIndex, nFPCModelNum, GetDoc()->m_nLevelCount);
		
	m_rtPattern = pModelData->rtPattern[nModelIndex];
	
	m_dScore = dScore;
	m_dwSearchTime = GetTickCount() - m_dwSearchTime;

	m_ptCenter = CdblPoint(dCenX, dCenY).ToCPoint();
	CSize sizeOffset = m_ptCenter - pModelData->ptCenter[nModelIndex];
	m_rtPattern.OffsetRect(sizeOffset);

	BOOL bPCB = (nPatternIndex == CAM_0_PCB_1) ? TRUE:FALSE;

	m_Vision.DrawPatRect(bPCB, 1, nBufferIndex, m_rtPattern, CdblPoint(dCenX, dCenY), m_dScore, 1, m_dwSearchTime, true);


	if( iRet == 1 )	//정상적으로 찾았을 경우..
	{
		if( m_ptCenter.x < 0 || m_ptCenter.x >= IMAGE_SIZE_X || m_ptCenter.y < 0 || m_ptCenter.y >= IMAGE_SIZE_Y )
		{
			if(GetDoc()->m_bErrImgSave && bIsManual == FALSE)
			{
				if (!(GetDoc()->m_bAutoTune_RUN)) SaveSearchImage(TRUE, nCamIndex, strPosition, m_dScore);
			}
			// 실패했을 경우만 Live 재가동
			// 성공했을 경우 계속 SearchPattern을 할 수 있기 때문에 외부에서 재가동한다.
			//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
			return FALSE;
			
		}
		if(GetDoc()->m_bSuccessImgSave && bIsManual == FALSE && (nPatternIndex != CAM_0_PCB_1))
		{
			// Mark를 모두 찾았으면 Option 설정 여부에 따라 image save
			if (!(GetDoc()->m_bAutoTune_RUN)) SaveSearchImage(FALSE, nCamIndex, strPosition, m_dScore);
		}

		//m_bReturnValue = TRUE;
	}
	else	// 찾지 못했을 경우...
	{
		if(GetDoc()->m_bErrImgSave && bIsManual == FALSE)
		{
			if (!(GetDoc()->m_bAutoTune_RUN)) SaveSearchImage(TRUE, nCamIndex, strPosition, m_dScore);
		}
		// 실패했을 경우만 Live 재가동
		// 성공했을 경우 계속 SearchPattern을 할 수 있기 때문에 외부에서 재가동한다.
		//GetDoc()->m_pThreadLiveGrap->m_bLivePause = FALSE;
		return FALSE;

		//_bReturnValue = FALSE;
	}

	if(bIsManual)
	{
		// FormAlign 화면 표시 위해...
		if(nPatternIndex == CAM_0_PCB_1)
		{
			m_ManualAlignResult.PCBPos.x = m_ptCenter.x;
			m_ManualAlignResult.PCBPos.y = m_ptCenter.y;
			
		}
		else
		{
			m_ManualAlignResult.FPCPos.x = m_ptCenter.x;
			m_ManualAlignResult.FPCPos.y = m_ptCenter.y;
		}
		if (nPatternIndex == 0)
		{
			m_ManualAlignResult.Distance.x = ( 320. - m_ManualAlignResult.FPCPos.x ) * GetApp()->m_dFovX;
			m_ManualAlignResult.Distance.y = ( 240. - m_ManualAlignResult.FPCPos.y ) * GetApp()->m_dFovY;
		}
		else {
			m_ManualAlignResult.Distance.x = ( m_ManualAlignResult.PCBPos.x - m_ManualAlignResult.FPCPos.x ) * GetApp()->m_dFovX;
			m_ManualAlignResult.Distance.y = ( m_ManualAlignResult.PCBPos.y - m_ManualAlignResult.FPCPos.y ) * GetApp()->m_dFovY;
		}
	}
	else
	{
		if(nPatternIndex == CAM_0_PCB_1)
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.x = m_ptCenter.x;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.y = m_ptCenter.y;
		}
		else
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x = m_ptCenter.x;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y = m_ptCenter.y;
		}

		if (nPatternIndex == 0)
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.x = ( 320. - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x ) * GetApp()->m_dFovX;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.y = ( 240. - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y ) * GetApp()->m_dFovY;
		} 
		else
		{
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.x = ( GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.x - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.x ) * GetApp()->m_dFovX;
			GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].Distance.y = ( GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].PCBPos.y - GetDoc()->m_pAmtData->m_AlignResult[nCamIndex].FPCPos.y ) * GetApp()->m_dFovY;
		}

		
	}

	return 1;
}

void CAlignCtl::DrawLine( int nBufferIndex )
{
	m_Vision.DrawRect( nBufferIndex , m_rtPattern , true );
	m_Vision.DrawCenLine( nBufferIndex ,m_ptCenter , false );
}

int CAlignCtl::ContactPosCalibaration(double dYmmOffset)
{
	if (GetDoc()->m_bAutoTune_Using && GetDoc()->m_bUsed_Tune)
	{
		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_0] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_0].dx + 	/*검증안됨*/
			GetDoc()->m_pAmtData->m_AlignResult[CAM_0].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_1] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_1].dx +  
			GetDoc()->m_pAmtData->m_AlignResult[CAM_1].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_2] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dx - 
			GetDoc()->m_pAmtData->m_AlignResult[CAM_2].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_3] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_3].dx + 
			GetDoc()->m_pAmtData->m_AlignResult[CAM_3].Distance.x ;
	} 
	else
	{
		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_0] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_0].dx + GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[CAM_0] +	/*검증안됨*/
			GetDoc()->m_pAmtData->m_AlignResult[CAM_0].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_1] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_1].dx + GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[CAM_1] + 
			GetDoc()->m_pAmtData->m_AlignResult[CAM_1].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_2] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dx + GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[CAM_2] - 
			GetDoc()->m_pAmtData->m_AlignResult[CAM_2].Distance.x ;

		GetDoc()->m_pAmtData->StageContactPos.dx[CAM_3] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_3].dx + GetDoc()->m_pAmtData->m_CurModelInfo.TV_PCBX[CAM_3] + 
			GetDoc()->m_pAmtData->m_AlignResult[CAM_3].Distance.x ;
	}	

	GetDoc()->m_pAmtData->StageContactPos.dy[CAM_0] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_0].dy + dYmmOffset + GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[CAM_0]
		+ GetDoc()->m_pAmtData->m_AlignResult[CAM_0].Distance.y ;
	
	GetDoc()->m_pAmtData->StageContactPos.dy[CAM_1] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_1].dy + dYmmOffset + GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[CAM_1]
		+ GetDoc()->m_pAmtData->m_AlignResult[CAM_1].Distance.y ;

	GetDoc()->m_pAmtData->StageContactPos.dy[CAM_2] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_2].dy + dYmmOffset + GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[CAM_2]
		+ GetDoc()->m_pAmtData->m_AlignResult[CAM_2].Distance.y ;

	GetDoc()->m_pAmtData->StageContactPos.dy[CAM_3] = GetDoc()->m_pAmtData->m_CurModelInfo.GrapPos[CAM_3].dy + dYmmOffset + GetDoc()->m_pAmtData->m_CurModelInfo.TV_FPCY[CAM_3]
		+ GetDoc()->m_pAmtData->m_AlignResult[CAM_3].Distance.y ;
	
// 
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LX[CAM_0] = GetDoc()->m_pAmtData->m_AlignResult[CAM_0].Distance.x;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LX[CAM_1] = GetDoc()->m_pAmtData->m_AlignResult[CAM_1].Distance.x;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LX[CAM_2] = GetDoc()->m_pAmtData->m_AlignResult[CAM_2].Distance.x;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LX[CAM_3] = GetDoc()->m_pAmtData->m_AlignResult[CAM_3].Distance.x;
// 
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LY[CAM_0] = GetDoc()->m_pAmtData->m_AlignResult[CAM_0].Distance.y;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LY[CAM_1] = GetDoc()->m_pAmtData->m_AlignResult[CAM_1].Distance.y;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LY[CAM_2] = GetDoc()->m_pAmtData->m_AlignResult[CAM_2].Distance.y;
// 	GetDoc()->m_pAmtData->m_SearchResult.ContactDist_LY[CAM_3] = GetDoc()->m_pAmtData->m_AlignResult[CAM_3].Distance.y;
	return 1;
}

int CAlignCtl::ContactPosCalibaration_FFC_Edge(int nCamNum, BOOL bBackY)//  [6/10/2009 janghan]
{
	if (nCamNum == CAM_0)
	{
		if (bBackY) GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0); 
		else GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0)  +	/*검증안됨*/
			GetDoc()->m_fAutoDistance_X ;
	} 
	else if(nCamNum == CAM_1)
	{
		if (bBackY) GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1); 
		else GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1)  + 
			GetDoc()->m_fAutoDistance_X ;
	}
	else if(nCamNum == CAM_2)
	{
		if (bBackY) GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2); 
		else GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2)  -
			GetDoc()->m_fAutoDistance_X ;
	}
	else if(nCamNum == CAM_3)
	{
		if (bBackY) GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3); 
		else GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3)  + 
			GetDoc()->m_fAutoDistance_X ;
	}
	
	if (bBackY) GetDoc()->m_fAutoContactPos_Y = GetDoc()->m_pMotion->GetCmdPos(4 + nCamNum) - 0.5f; 
	else GetDoc()->m_fAutoContactPos_Y = GetDoc()->m_pMotion->GetCmdPos(4 + nCamNum)  
	+ GetDoc()->m_fAutoDistance_Y ;

	return 1;
}

int CAlignCtl::ContactPosCalibaration_SortX(int nCamNum)
{
	if (nCamNum == CAM_0)
	{
		GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X0)  +	/*검증안됨*/
			GetDoc()->m_fAutoDistance_X ;
	} 
	else if(nCamNum == CAM_1)
	{
		GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X1)  + 
			GetDoc()->m_fAutoDistance_X ;
	}
	else if(nCamNum == CAM_2)
	{
		GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X2)  -
			GetDoc()->m_fAutoDistance_X ;
	}
	else if(nCamNum == CAM_3)
	{
		GetDoc()->m_fAutoContactPos_X = GetDoc()->m_pMotion->GetCmdPos(AXIS_CCD_X3)  + 
			GetDoc()->m_fAutoDistance_X ;
	}

	 GetDoc()->m_fAutoContactPos_Y = GetDoc()->m_pMotion->GetCmdPos(4 + nCamNum);

	return 1;
}

void CAlignCtl::SavePattern( int nModelIndex, long nCondition )
{
	m_Vision.AddPattern(nModelIndex, m_rtPattern, m_ptCenter);
	CString strPtrPath;
	if(GetDoc()->m_bUseBinary)
		strPtrPath.Format("%s\\%s.bmp", GetApp()->m_strRecipePath, GeneratePatternFileName(nModelIndex));
	else
		strPtrPath.Format("%s\\%s\\%s.bmp", GetApp()->m_strRecipePath , GetDoc()->m_pAmtData->m_CurModelInfo.strModelName, GeneratePatternFileName(nModelIndex));
	m_Vision.SavePattern(nModelIndex, strPtrPath);

	// 패턴 영역하고 Center Position Doc에 저장
	GetDoc()->m_pAmtData->m_CurModelInfo.rtPattern[nModelIndex] = m_rtPattern;
	GetDoc()->m_pAmtData->m_CurModelInfo.ptCenter[nModelIndex] = m_ptCenter;

	int nPatternIndex = nModelIndex % MAX_CAM_NUM;
	int nCamIndex = nModelIndex / MAX_CAM_NUM;
	// FPC 갯수 증가
	if((nPatternIndex != 3) && (GetDoc()->m_pAmtData->m_CurModelInfo.nFPCModelNum[nCamIndex] < nPatternIndex+1))
		GetDoc()->m_pAmtData->m_CurModelInfo.nFPCModelNum[nCamIndex] = nPatternIndex+1;
	// File save
	GetDoc()->SaveModelFile();

	// Binarize Condition 증가
	if(GetDoc()->m_bUseBinary)
	{
		if(nPatternIndex == 3)
		{
			//GetDoc()->m_nBinarizePCB[nCamIndex] = nCondition;
		}
		else
			GetDoc()->m_nBinarizeFPC[nCamIndex] = nCondition;
		GetDoc()->SaveBinarizeCondition();
	}
}

CString CAlignCtl::GeneratePatternFileName( int nModelIndex )
{
	CString strPtrPath;
	CString strPatternName = _T("");

	switch( nModelIndex )
	{
	case CAM_0_FPC_0:
		strPatternName = _T("CAM0_FPC0");
		break;
	case CAM_0_FPC_1:
		strPatternName = _T("CAM0_FPC1");
		break;
	case 2:
		strPatternName = _T("CAM0_FPC2");
		break;
	case 3:
		strPatternName = _T("CAM0_PCB");
		break;
	case CAM_1_FPC_0:
		strPatternName = _T("CAM1_FPC0");
		break;
	case CAM_1_FPC_1:
		strPatternName = _T("CAM1_FPC1");
		break;
	case 6:
		strPatternName = _T("CAM1_FPC2");
		break;
	case 7:
		strPatternName = _T("CAM1_PCB");
		break;
	case CAM_2_FPC_0:
		strPatternName = _T("CAM2_FPC0");
		break;
	case CAM_2_FPC_1:
		strPatternName = _T("CAM2_FPC1");
		break;
	case 10:
		strPatternName = _T("CAM2_FPC2");
		break;
	case 11:
		strPatternName = _T("CAM2_PCB");
		break;
	case CAM_3_FPC_0:
		strPatternName = _T("CAM3_FPC0");
		break;
	case CAM_3_FPC_1:
		strPatternName = _T("CAM3_FPC1");
		break;
	case 14:
		strPatternName = _T("CAM3_FPC2");
		break;
	case 15:
		strPatternName = _T("CAM3_PCB");
		break;
	}

	strPtrPath.Format("%s", strPatternName);

	return strPtrPath;
}

void CAlignCtl::MoveRectToDefault( int nModelIndex )
{
	// 등록된 모델이 없으면 기본값으로 설정
	if(m_Vision.IsPatternEmpty(nModelIndex))
	{
		m_rtPattern = CRect(CPoint(260, 190), CPoint(380, 290));
		m_ptCenter = m_rtPattern.CenterPoint();
	}
	else
	{
		m_rtPattern = GetDoc()->m_pAmtData->m_CurModelInfo.rtPattern[nModelIndex];
		m_ptCenter = GetDoc()->m_pAmtData->m_CurModelInfo.ptCenter[nModelIndex];
	}
	DrawLine(DISP_ALIGN);
}

void CAlignCtl::ResetRect( int nModelIndex )
{
	m_rtPattern = CRect(CPoint(260, 190), CPoint(380, 290));
	m_ptCenter = m_rtPattern.CenterPoint();
	DrawLine(DISP_ALIGN);
}

void CAlignCtl::PatternAllClear()
{
	m_Vision.PatternAllClear();
}

void CAlignCtl::RemoveFPCPattern( int nModelIndex )
{
	if( (nModelIndex % MAX_CAM_NUM) == 3)
	{
		AfxMessageBox("Can not remove mark");
		return;
	}
	m_Vision.RemovePattern(nModelIndex);
	if(GetDoc()->m_pAmtData->m_CurModelInfo.nFPCModelNum[nModelIndex / MAX_CAM_NUM] != 0)
	{
		GetDoc()->m_pAmtData->m_CurModelInfo.nFPCModelNum[nModelIndex / MAX_CAM_NUM]--;
		GetDoc()->SaveModelFile();
	}
}

void CAlignCtl::OverlayClear( int nBufferNo )
{
	m_Vision.OverlayClear(nBufferNo);
}

void CAlignCtl::GrabNotUseTextDisp( int nBufferNo )
{
	m_Vision.GrabNotUseTextDisp(nBufferNo);
}

void CAlignCtl::SaveSearchImage( BOOL bErrorImg, int nCamIndex , CString strPosition, double nScore)
{
	CString strDirPath, strFilePath, strTime;
	SYSTEMTIME	time;
	GetLocalTime(&time);
	strTime.Format("%02u-%02u-%02u", time.wHour, time.wMinute, time.wSecond);
	strDirPath.Format("%s\\%4u%02u%02u" , IMAGE_PATH , time.wYear , time.wMonth , time.wDay );
	CreateDirectory(strDirPath, NULL);

	CFormAuto* pAuto = (CFormAuto*)GetMainFrame()->m_pForm[FORM_AUTO];
	if(bErrorImg)
	{
		strDirPath.Format("%s\\%4u%02u%02u\\Error" , IMAGE_PATH , time.wYear , time.wMonth , time.wDay );
		CreateDirectory(strDirPath, NULL);

		strFilePath.Format("%s\\%s_%d_%s(%.2f).jpg", strDirPath, strTime, nCamIndex, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, nScore);
		CEtc::CaptureScreen(pAuto->m_pctImageGrap[nCamIndex].m_hWnd, strFilePath);
		strFilePath.Format("%s\\%s_%d_%s_Original(%.2f).jpg", strDirPath, strTime, nCamIndex, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, nScore);
		m_Vision.SaveImage(strFilePath, nCamIndex);
		AlignLog(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, strPosition, nScore, "Error");
	}
	else
	{
		strDirPath.Format("%s\\%4u%02u%02u\\Success" , IMAGE_PATH , time.wYear , time.wMonth , time.wDay );
		CreateDirectory(strDirPath, NULL);
		if(CFileSupport::DirectoryCheck(strDirPath) == FALSE)
			AfxMessageBox("Create?Directory Fail");
		// 성공 이미지는 별로 중요하지 않으니 Capture image만 남겨놓는다.
		strFilePath.Format("%s\\%s_%d_%s(%.2f).jpg", strDirPath, strTime, nCamIndex, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, nScore);
		CEtc::CaptureScreen(pAuto->m_pctImageGrap[nCamIndex].m_hWnd, strFilePath);
		//m_Vision.SaveImage(strFilePath, nCamIndex);
		AlignLog(GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID, strPosition, nScore, "Success");

	}
}

void CAlignCtl::GrabContactImage()
{
	// Contact Image 저장 옵션이 설정되어 있으면 저장도 한다.
	CString strDirPath, strFilePath, strTime;
	if(GetDoc()->m_bContactImgSave)
	{
		SYSTEMTIME	time;
		GetLocalTime(&time);
		strTime.Format("%02u-%02u-%02u", time.wHour, time.wMinute, time.wSecond);
		strDirPath.Format("%s\\%4u%02u%02u" , IMAGE_PATH , time.wYear , time.wMonth , time.wDay );
		CreateDirectory(strDirPath, NULL);
		strDirPath.Format("%s\\%4u%02u%02u\\Contact" , IMAGE_PATH , time.wYear , time.wMonth , time.wDay );
		CreateDirectory(strDirPath, NULL);
	}
	BOOL bGrab[MAX_CAM_NUM] = {FALSE, FALSE, FALSE, FALSE};

	if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::ONE_CONT )
	{
		if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_0 )
		{
			bGrab[CAM_0] = TRUE;
		}
		else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_1 )
		{
			bGrab[CAM_1] = TRUE;
		}
		else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactor == CAmtData::CONTACT_2 )
		{
			bGrab[CAM_2] = TRUE;
		}
		else
		{
			bGrab[CAM_3] = TRUE;
		}
	}
	else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::TWO_CONT )
	{
		bGrab[CAM_1] = TRUE;
		bGrab[CAM_2] = TRUE;
	}
	else if( GetDoc()->m_pAmtData->m_CurModelInfo.iContactNum == CAmtData::FOUR_CONT )
	{
		bGrab[CAM_0] = TRUE;
		bGrab[CAM_1] = TRUE;
		bGrab[CAM_2] = TRUE;
		bGrab[CAM_3] = TRUE;
	}

	for(int i = 0; i < MAX_CAM_NUM; i++)
	{
		if(bGrab[i])
		{
			m_Vision.Grab(i, i);
			m_Vision.Grab(i, i);
		}
	}

	if(GetDoc()->m_bContactImgSave)
	{
		for(int i = 0; i < MAX_CAM_NUM; i++)
		{
			if(bGrab[i])
			{
				strFilePath.Format("%s\\%s_%d_%s.jpg", strDirPath, strTime, i, GetDoc()->m_pEqMainSeq->m_PanelInfo.m_sHPanelID);
				m_Vision.SaveImage(strFilePath, i);
			}
		}
	}
}

void CAlignCtl::GrabBinarize( int nBufferIndex, long nCondition )
{
	m_Vision.GrabBinarize(nBufferIndex, nCondition);
}

void CAlignCtl::AlignLog(LPCTSTR szHPanelID, LPCTSTR szPosition, double dScore, CString strSuccess)
{
	CString strLog;
	strLog.Format("[%s]\t[%s]\t[%s]\t[%.2f]", strSuccess, szHPanelID, szPosition, dScore);
	GetDoc()->SetLog(strLog, LOG_ALIGN);
}
