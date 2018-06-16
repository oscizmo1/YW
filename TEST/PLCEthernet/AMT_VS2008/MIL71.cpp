// MIL71.cpp : implementation file
//

#include "stdafx.h"
#include "MT.h"
#include "MIL71.h"

/////////////////////////////////////////////////////////////////////////////
// CMIL71

CMIL71::CMIL71(void)
{

	MilApplication = M_NULL;			// MIL Application identifier.
	MilSystem = M_NULL;					// MIL System identifier.  

	int i;
	for( i= 0; i< MAX_CAM_NUM; i++) 
	{
		MilDigitizer[i]= M_NULL;
	}

	for( i= 0; i< MAX_BUFFER_NUM; i++) 
	{
		MilDisplay[i]= M_NULL;
		MilDispImage[i]= M_NULL;
		MilOverlayImage[i]= M_NULL;
	}

	for(int i=0; i< MAX_MOD_BUFFER_NUM; i++) 
	{			
		Result[i]= M_NULL;
		Model[i]= M_NULL;
	}

	ScaleFactor= 1.0;
	VISION_SYS_MODE= false;

	ZeroMemory(m_pCamInfo , MAX_CAM_NUM * sizeof(CAM_INFO) );

	MilDisplayDontcare = M_NULL;
	MilDispImageDontcare = M_NULL;
	MilOverlayDontcare = M_NULL;

	MilOriginal = M_NULL;
}

CMIL71::~CMIL71(void)
{
}

int CMIL71::Open( CString strDcfPath )
{
	int i= 0;
	for(i= 0; i< MAX_CAM_NUM ; i++)
	{
		switch(m_pCamInfo[i].iCamNo )
		{
			case 0: m_HEAD_CAM_NO[i]= M_CH0; break;
			case 1: m_HEAD_CAM_NO[i]= M_CH1; break;
			case 2: m_HEAD_CAM_NO[i]= M_CH2; break;
			case 3: m_HEAD_CAM_NO[i]= M_CH3; break;
			case 4: m_HEAD_CAM_NO[i]= M_CH4; break;
			case 5: m_HEAD_CAM_NO[i]= M_CH5; break;
			case 6: m_HEAD_CAM_NO[i]= M_CH6; break;
			case 7: m_HEAD_CAM_NO[i]= M_CH7; break;
			default: m_HEAD_CAM_NO[i]= M_CH0; break;
		}

		switch( (int)(m_pCamInfo[i].iCamX_Mode) )
		{
			case 0: m_HEAD_CAM_X_MODE[i]= M_REVERSE; break;
			case 1: m_HEAD_CAM_X_MODE[i]= M_FORWARD; break;
			default: m_HEAD_CAM_X_MODE[i]= M_REVERSE; break;
		}
		
		switch( (int)(m_pCamInfo[i].iCamY_Mode) ) 
		{
			case 0: m_HEAD_CAM_Y_MODE[i]= M_REVERSE; break;
			case 1: m_HEAD_CAM_Y_MODE[i]= M_FORWARD; break;
			default: m_HEAD_CAM_Y_MODE[i]= M_REVERSE; break;
		}
	}

//////////////////////////////////////////////////////////////////////////////////////////
	// Allocate a MIL application. 
	MappAlloc(M_DEFAULT, &MilApplication);
	MappControl(M_ERROR, M_PRINT_DISABLE);

	// Allocate a MIL digitizer if supported and sets the target image size. 
	// Allocate a MIL system. 
	if (!MsysAlloc(M_SYSTEM_METEOR_II, M_DEFAULT, M_DEFAULT, &MilSystem))
	{
		AfxMessageBox("Fail to initialize the MIL71 board(MSYS)");

		if(MilSystem == NULL) VISION_SYS_MODE= false;
	}
	else 	
		VISION_SYS_MODE = true;

	if(!VISION_SYS_MODE)
		MsysAlloc(M_SYSTEM_VGA, M_DEFAULT, M_NO_DDRAW, &MilSystem);

	for(int i = 0 ; i < MAX_CAM_NUM ; i++)
	{
		if(VISION_SYS_MODE)	
		{
			if (!MdigAlloc(MilSystem, M_DEV0, strDcfPath.GetBuffer(0), M_DEFAULT, &MilDigitizer[i]))
			{
				AfxMessageBox("Fail to initialize the MIL71 board(MDIG)");
				MilDigitizer[i]= M_NULL;
			}
		}
		else
		{
			MilDigitizer[i]= M_NULL;
		}

		if(MilDigitizer[i]) 
		{
			MdigChannel(MilDigitizer[i], m_HEAD_CAM_NO[i]);
			MdigControl(MilDigitizer[i], M_SOURCE_SIZE_X, IMAGE_SIZE_X);
			MdigControl(MilDigitizer[i], M_SOURCE_SIZE_Y, IMAGE_SIZE_Y);
			MdigControl(MilDigitizer[i], M_GRAB_MODE,M_SYNCHRONOUS);
			MdigControl(MilDigitizer[i], M_GRAB_TIMEOUT, 90);					 //mSec
			MdigControl(MilDigitizer[i], M_CAMERA_LOCK, M_ENABLE);
			MdigControl(MilDigitizer[i], M_CAMERA_LOCK_SENSITIVITY, M_DEFAULT);
			MdigControl(MilDigitizer[i], M_CAMERA_UNLOCK_SENSITIVITY, M_DEFAULT);

			MdigControl(MilDigitizer[i], M_GRAB_DIRECTION_X, m_HEAD_CAM_X_MODE[i]);
			MdigControl(MilDigitizer[i], M_GRAB_DIRECTION_Y, m_HEAD_CAM_Y_MODE[i]);

			MdigReference(MilDigitizer[i], M_WHITE_REF, 0L);
		}
	}

	// Allocate process image buffer
	MbufAlloc2d(MilSystem, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &ProcImage);

	MbufAlloc2d(MilSystem, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &MilOriginal);

	return TRUE;
}

int CMIL71::Close()
{
	PatternAllClear();
	
	if(ProcImage)	MbufFree(ProcImage);	ProcImage=M_NULL;
	if(MilOriginal)	MbufFree(MilOriginal);	MilOriginal=M_NULL;
		
	for(int i = 0 ; i < MAX_CAM_NUM ; i++)
	{
		if(MilDigitizer[i])
		{
			MdigControl(MilDigitizer[i], M_CAMERA_LOCK, M_DISABLE);		Sleep(50);
			MdigFree(MilDigitizer[i]);		MilDigitizer[i]= M_NULL;
		}
	}

	if(MilSystem)			{ MsysFree(MilSystem);		MilSystem= M_NULL; }
	if(MilApplication)		{ MappFree(MilApplication);	MilApplication= M_NULL; }

	return TRUE;
}
BOOL CMIL71::SelAutoWindow( HWND hCam0, HWND hCam1, HWND hCam2, HWND hCam3 )
{
	HWND* phWndArray[MAX_CAM_NUM] = {&hCam0, &hCam1, &hCam2, &hCam3};
	for(int i = DISP_AUTO_0; i <= DISP_AUTO_3; i++)
	{
		if (!MdispAlloc(MilSystem, M_DEV0, M_DEF_DISPLAY_FORMAT, M_WINDOWED , &MilDisplay[i]) )
		{	
			return FALSE;	
		}
		if(VISION_SYS_MODE) 
		{
			if (!MbufAllocColor(MilSystem, 3, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilDispImage[i]))
			{		
				return FALSE;
			}
		}
		else
		{
			if (!MbufAllocColor(MilSystem, 3, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC/*+M_NON_PAGED*/, &MilDispImage[i]))
			{
				return FALSE;
			}
		}
		MbufClear(MilDispImage[i], 0);
		MdispSelectWindow(MilDisplay[i], MilDispImage[i], *(phWndArray[i]));

		if(MilDisplay[i]!= M_NULL) 
		{
			MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_ENABLE);
			MdispInquire(MilDisplay[i], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[i]);
			MdispControl(MilDisplay[i], M_WINDOW_OVR_SHOW, M_ENABLE);
			MdispInquire(MilDisplay[i], M_KEY_COLOR, &TransparentColor); 
			if (MbufInquire(MilOverlayImage[i], M_SIZE_BAND, M_NULL) == 1)
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_RGB888(0, 255, 0));
				MbufControl(MilOverlayImage[i], M_WINDOW_DC_ALLOC, M_DEFAULT);
				m_pDC1[i].Attach((HDC)MbufInquire(MilOverlayImage[i], M_WINDOW_DC, M_NULL));
			}
		}
	}

	return TRUE;
}

BOOL CMIL71::SelTeacWindow( HWND hCam0, HWND hCam1 )
{
	HWND* phWndArray[2] = {&hCam0, &hCam1};
	for(int i = DISP_TEACH_L; i <= DISP_TEACH_R; i++)
	{
		if (!MdispAlloc(MilSystem, M_DEV0, M_DEF_DISPLAY_FORMAT, M_WINDOWED , &MilDisplay[i]) )
		{	
			return FALSE;	
		}
		if(VISION_SYS_MODE) 
		{
			if (!MbufAllocColor(MilSystem, 3, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilDispImage[i]))
			{		
				return FALSE;
			}
		}
		else
		{
			if (!MbufAllocColor(MilSystem, 3, IMAGE_SIZE_X, IMAGE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC/*+M_NON_PAGED*/, &MilDispImage[i]))
			{
				return FALSE;
			}
		}
		MbufClear(MilDispImage[i], 0);
		MdispSelectWindow(MilDisplay[i], MilDispImage[i], *(phWndArray[i-DISP_TEACH_L]));

		if(MilDisplay[i]!= M_NULL) 
		{
			MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_ENABLE);
			MdispInquire(MilDisplay[i], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[i]);
			MdispControl(MilDisplay[i], M_WINDOW_OVR_SHOW, M_ENABLE);
			MdispInquire(MilDisplay[i], M_KEY_COLOR, &TransparentColor); 
			if (MbufInquire(MilOverlayImage[i], M_SIZE_BAND, M_NULL) == 1)
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_RGB888(0, 255, 0));
				MbufControl(MilOverlayImage[i], M_WINDOW_DC_ALLOC, M_DEFAULT);
				m_pDC1[i].Attach((HDC)MbufInquire(MilOverlayImage[i], M_WINDOW_DC, M_NULL));
			}
		}
	}
	return TRUE;
}

BOOL CMIL71::SelAlignWindow( HWND hCam0, HWND hCam1 )
{
	HWND* phWndArray[2] = {&hCam0, &hCam1};
	int nSizeX, nSizeY;
	for(int i = DISP_ALIGN; i <= DISP_ALIGN_PREV; i++)
	{
		nSizeX = (int)(i == DISP_ALIGN ? IMAGE_SIZE_X : PATTERN_SIZE_X);
		nSizeY = (int)(i == DISP_ALIGN ? IMAGE_SIZE_Y : PATTERN_SIZE_Y);
		if (!MdispAlloc(MilSystem, M_DEV0, M_DEF_DISPLAY_FORMAT, M_WINDOWED , &MilDisplay[i]) )
		{	
			return FALSE;	
		}
		if(VISION_SYS_MODE) 
		{
			if (!MbufAllocColor(MilSystem, 3, nSizeX, nSizeY, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_GRAB+M_PROC+M_NON_PAGED, &MilDispImage[i]))
			{		
				return FALSE;
			}
		}
		else
		{
			if (!MbufAllocColor(MilSystem, 3, nSizeX, nSizeY, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC/*+M_NON_PAGED*/, &MilDispImage[i]))
			{
				return FALSE;
			}
		}
		MbufClear(MilDispImage[i], 0);
		MdispSelectWindow(MilDisplay[i], MilDispImage[i], *(phWndArray[i-DISP_ALIGN]));

		if(MilDisplay[i]!= M_NULL) 
		{
			MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_ENABLE);
			MdispInquire(MilDisplay[i], M_WINDOW_OVR_BUF_ID, &MilOverlayImage[i]);
			MdispControl(MilDisplay[i], M_WINDOW_OVR_SHOW, M_ENABLE);
			MdispInquire(MilDisplay[i], M_KEY_COLOR, &TransparentColor); 
			if (MbufInquire(MilOverlayImage[i], M_SIZE_BAND, M_NULL) == 1)
			{
				MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
				MgraColor(M_DEFAULT, M_RGB888(0, 255, 0));
				MbufControl(MilOverlayImage[i], M_WINDOW_DC_ALLOC, M_DEFAULT);
				m_pDC1[i].Attach((HDC)MbufInquire(MilOverlayImage[i], M_WINDOW_DC, M_NULL));
			}
		}
	}
	return TRUE;
}

void CMIL71::DeselAutoWindow()
{
	for(int i = DISP_AUTO_0; i <= DISP_AUTO_3; i++)
	{
		MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_DISABLE);
		MdispDeselect(MilDisplay[i], MilDispImage[i]);

		if(MilOverlayImage[i])		MbufFree(MilOverlayImage[i]);
		if(MilDispImage[i])	MbufFree(MilDispImage[i]);
		if(MilDisplay[i])		MdispFree(MilDisplay[i]);
	}
}

void CMIL71::DeselTeacWindow()
{
	for(int i = DISP_TEACH_L; i <= DISP_TEACH_R; i++)
	{
		MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_DISABLE);
		MdispDeselect(MilDisplay[i], MilDispImage[i]);

		if(MilOverlayImage[i])		MbufFree(MilOverlayImage[i]);
		if(MilDispImage[i])	MbufFree(MilDispImage[i]);
		if(MilDisplay[i])		MdispFree(MilDisplay[i]);
	}
}

void CMIL71::DeselAlignWindow()
{
	for(int i = DISP_ALIGN; i <= DISP_ALIGN_PREV; i++)
	{
		MdispControl(MilDisplay[i], M_WINDOW_OVR_WRITE, M_DISABLE);
		MdispDeselect(MilDisplay[i], MilDispImage[i]);

		if(MilOverlayImage[i])		MbufFree(MilOverlayImage[i]);
		if(MilDispImage[i])	MbufFree(MilDispImage[i]);
		if(MilDisplay[i])		MdispFree(MilDisplay[i]);
	}
}

int CMIL71::Grab( int nBufferIndex, int nCamNo , int iCurStatus /*= STATUS_RUN*/, BOOL bBackup /*= FALSE*/ )
{
	MdigChannel(MilDigitizer[nCamNo], m_HEAD_CAM_NO[nCamNo]);
	Sleep(10);

	if( iCurStatus == STATUS_RUN) MbufClear(MilDispImage[nBufferIndex], 128);

	if(VISION_SYS_MODE)
	{
		MdigGrab(MilDigitizer[nCamNo], MilDispImage[nBufferIndex]);
		if(bBackup)	
		{
			MbufClear(MilOverlayImage[nBufferIndex], TransparentColor);
			MbufClear(MilOriginal, 128);
			MbufCopy(MilDispImage[nBufferIndex], MilOriginal);
		}
	}
	else
	{ 
	}

	return TRUE;
}

int CMIL71::Zoom(long lScale)
{	// 시간없다 그냥 고정 처음 한번만 실행.//
	// 줌설정 안해도 되게끔.//

	if( lScale == 0 )
	{
		MdispZoom(MilDisplay[DISP_AUTO_0], -2, -2);
		MdispZoom(MilDisplay[DISP_AUTO_1], -2, -2);
		MdispZoom(MilDisplay[DISP_AUTO_2], -2, -2);
		MdispZoom(MilDisplay[DISP_AUTO_3], -2, -2);

		MdispZoom(MilDisplay[DISP_TEACH_L], -2, -2);
		MdispZoom(MilDisplay[DISP_TEACH_R], -2, -2);

		MdispZoom(MilDisplay[DISP_ALIGN], -1, -1);
	}

	return TRUE;
}

void CMIL71::AddPattern( int nModelIndex, CRect rtPattern, CPoint ptCenter )
{
	if(Model[nModelIndex] != M_NULL) 
	{ 
		MpatFree(Model[nModelIndex]);  Model[nModelIndex]= M_NULL;
	}
	if(Result[nModelIndex] != M_NULL) 
	{	
		MpatFree(Result[nModelIndex]);  Result[nModelIndex]= M_NULL;
	}

	MbufCopy(MilDispImage[DISP_ALIGN], ProcImage);
	MpatAllocModel(MilSystem, ProcImage, rtPattern.left, rtPattern.top, rtPattern.Width(), rtPattern.Height(), M_NORMALIZED, &Model[nModelIndex]);

	SetModelParam(nModelIndex, rtPattern, ptCenter);

	MpatAllocResult(MilSystem, M_DEFAULT, &Result[nModelIndex]);
}

void CMIL71::SetModelParam( int nModelIndex, CRect rtPattern, CPoint ptCenter )
{
	MpatSetNumber(Model[nModelIndex], 1);
	MpatSetAccuracy(Model[nModelIndex], M_MEDIUM);
	MpatSetSpeed(Model[nModelIndex], M_HIGH);
	MpatSetAcceptance(Model[nModelIndex], 20);	// 나중 디버깅을 위해 낮춰놓는다.
	MpatSetAngle(Model[nModelIndex], M_SEARCH_ANGLE_MODE, M_DISABLE);
	MpatSetCenter(Model[nModelIndex], (double)(ptCenter.x - rtPattern.left), (double)(ptCenter.y - rtPattern.top));
}

void CMIL71::LoadPattern( CString strPtrPath, int nModelIndex, CRect rtPattern, CPoint ptCenter )
{
	if(Model[nModelIndex] != M_NULL) 
	{ 
		MpatFree(Model[nModelIndex]);  Model[nModelIndex]= M_NULL;
	}
	if(Result[nModelIndex] != M_NULL) 
	{	
		MpatFree(Result[nModelIndex]);  Result[nModelIndex]= M_NULL;
	}

	DWORD dwAttr = GetFileAttributes(strPtrPath.GetBuffer(0));
	if(dwAttr != -1 ) 
	{
		MIL_ID	MilImage	= M_NULL;
		MbufRestore(strPtrPath.GetBuffer(0), MilSystem, &MilImage);
		long nsizeX, nSizeY;
		MbufInquire(MilImage, M_SIZE_X, &nsizeX);
		MbufInquire(MilImage, M_SIZE_Y, &nSizeY);
		MpatAllocModel(MilSystem, MilImage, 0, 0, nsizeX, nSizeY, M_NORMALIZED, &Model[nModelIndex]);
		MbufFree(MilImage);	MilImage = M_NULL;
		SetModelParam(nModelIndex, rtPattern, ptCenter);
		MpatAllocResult(MilSystem, 2L, &Result[nModelIndex]);
	}
}

int CMIL71::SearchPattern( double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt )
{
	double final_x, final_y, final_Score;
	pdCenX = pdCenY = pdScore = 0.0;

	int nFindedModelNo = 0;
	CString strName = _T("");
	MbufCopy(MilDispImage[nBufferIndex], ProcImage);
	MpatPreprocModel(ProcImage, Model[nModelIndex], M_DEFAULT);

	// MpatFindMultipleModel() 함수가 잘 안되서 걍 for문으로 하나씩 찾는다.
	for(int i = 0; i < nModelCnt; i++)
	{
		MpatFindModel(ProcImage, Model[nModelIndex+i], Result[nModelIndex+i]);

		// 가장 높은 score를 찾는다.
		if(MpatGetNumber(Result[nModelIndex+i], M_NULL) > 0)
		{
			MpatGetResult(Result[nModelIndex + i], M_SCORE, &final_Score);
			if(pdScore < final_Score)
			{
				pdScore = final_Score;
				nFindedModelNo = i;
			}
		}
	}
	
	if(pdScore > 0.0)
	{
		MpatGetResult(Result[nModelIndex + nFindedModelNo], M_POSITION_X, &final_x);
		MpatGetResult(Result[nModelIndex + nFindedModelNo], M_POSITION_Y, &final_y);
	}
	else
	{
		// 못찾으면 Index 99로 설정
		nFindedModelNo = 99;
		final_x = final_y = final_Score = -1;
	}


	pdCenX = final_x;	// X
	pdCenY = final_y;	// Y
	nModelCnt = nFindedModelNo;

	// 최종 Score로 찾았는지 못찾았는지 판별
	if(pdScore >= GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore)
		return 1;
	else 
		return 0;
}

BOOL CMIL71::IsPatternEmpty( int nModelIndex )
{
	if(Model[nModelIndex])
		return FALSE;
	else
		return TRUE;
}

void CMIL71::SavePattern( int nModelNo, CString strPath )
{
	double dsizeX, dsizeY;
	MpatInquire(Model[nModelNo], M_ALLOC_SIZE_X, &dsizeX);
	MpatInquire(Model[nModelNo], M_ALLOC_SIZE_Y, &dsizeY);
	MIL_ID TempImage;
	TempImage= M_NULL;
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}

	if (!MbufAlloc2d(MilSystem, (long)dsizeX, (long)dsizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
	{
		AfxMessageBox("Fail to initialize the MIL71 board(SavePattern TempImage)");
	}
	MpatCopy(Model[nModelNo], TempImage, M_DEFAULT );
	MbufSave(strPath.GetBuffer(0), TempImage);
}

void CMIL71::SavePatternImage( int nModelNo, CRect rtPattern, CString strPath )
{
	MIL_ID TempImage;
	TempImage= M_NULL;
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}

	if (!MbufAlloc2d(MilSystem, rtPattern.Width(), rtPattern.Height(), 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
	{
		AfxMessageBox("Fail to initialize the MIL71 board(MbufAlloc2d TempImage)");
	}

	MpatCopy(Model[nModelNo], TempImage, M_DEFAULT);
	MbufExport(strPath.GetBuffer(0), M_TIFF , TempImage);
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}
}

void CMIL71::OverlayClear( int nBufferNo )
{
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	MbufClear(MilOverlayImage[nBufferNo], TransparentColor); 
	DefaultGrid(false, nBufferNo) ;//	DefaultGrid(false, 1) ;
	MdispControl(MilDisplay[nBufferNo], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::DrawRect(int no, CRect rt, bool Over_Clear)
{
 	CString str;
	// Clear the overlay buffer with the keying color.           
    if(Over_Clear) 
		MbufClear(MilOverlayImage[no], TransparentColor); 
	// Set graphic text to transparent background.
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
    // Set drawing color to the green index in the pseudo-color LUT.
	if(rt.left<0)	
		rt.left = 0;	
	if(rt.left>IMAGE_SIZE_X)	
		rt.left = IMAGE_SIZE_X;
	if(rt.right<0)	rt.right = 0;	if(rt.right>IMAGE_SIZE_X)	rt.right =	IMAGE_SIZE_X;
	if(rt.top<0)	rt.top = 0;		if(rt.top>IMAGE_SIZE_Y)		rt.top =	IMAGE_SIZE_Y;
	if(rt.bottom<0) rt.bottom = 0;	if(rt.bottom>IMAGE_SIZE_Y)	rt.bottom =	IMAGE_SIZE_Y;
	
	MgraColor(M_DEFAULT, M_RGB888(0,255,0));
    MgraRect(M_DEFAULT, MilOverlayImage[no], rt.left, rt.top, rt.right, rt.bottom);
	MgraRect(M_DEFAULT, MilOverlayImage[no], rt.left-1, rt.top-1, rt.right+1, rt.bottom+1);

	MdispControl(MilDisplay[no], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::DrawPatRect( BOOL bIsPCB, int mode, int nBufferIndex, CRect rt, CdblPoint ptCen, double score/*=0.0*/, int nModelNo /*= 0*/, DWORD dwSearchTime /*= 0*/, bool Over_Clear/*= true*/ )
{
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
 	CString str;
	// Clear the overlay buffer with the keying color.           
    if(Over_Clear) {
		MbufClear(MilOverlayImage[nBufferIndex], TransparentColor); 
		DefaultGrid(false, nBufferIndex) ;//	DefaultGrid(false, 1) ;
	}
	// Set graphic text to transparent background.
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
    // Set drawing color to the green index in the pseudo-color LUT.
	if(rt.left<0)	rt.left = 0;	if(rt.left>IMAGE_SIZE_X)	rt.left =	IMAGE_SIZE_X;
	if(rt.right<0)	rt.right = 0;	if(rt.right>IMAGE_SIZE_X)	rt.right =	IMAGE_SIZE_X;
	if(rt.top<0)	rt.top = 0;		if(rt.top>IMAGE_SIZE_Y)		rt.top =	IMAGE_SIZE_Y;
	if(rt.bottom<0) rt.bottom = 0;	if(rt.bottom>IMAGE_SIZE_Y)	rt.bottom =	IMAGE_SIZE_Y;
	
	MgraColor(M_DEFAULT, M_RGB888(255,30,30));
    MgraRect(M_DEFAULT, MilOverlayImage[nBufferIndex], rt.left, rt.top, rt.right, rt.bottom);
	MgraRect(M_DEFAULT, MilOverlayImage[nBufferIndex], rt.left-1, rt.top-1, rt.right+1, rt.bottom+1);
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	DrawCenLine(nBufferIndex, ptCen.ToCPoint(), false);

	MgraColor(M_DEFAULT, M_RGB888(100,255,100));

	// Auto화면에서 안보이기 때문에 일부러 Search시간을 뺐다
	MgraFont(M_DEFAULT, M_FONT_DEFAULT); //M_FONT_DEFAULT_LARGE);
	MgraFontScale(M_DEFAULT, 2, 2);
// 	if(bIsPCB)
// 		str.Format("PCB Pos:(%4.1f,%4.1f) Score:%3.1f ID[%d] Time[%dms]",ptCen.x, ptCen.y, score, nModelNo, dwSearchTime);
// 	else
// 		str.Format("FPC Pos:(%4.1f,%4.1f) Score:%3.1f ID[%d] Time[%dms]",ptCen.x, ptCen.y, score, nModelNo, dwSearchTime);
	if(bIsPCB)
		str.Format("PCB:(%4.1f,%4.1f) Score:%3.1f ID[%d]",ptCen.x, ptCen.y, score, nModelNo);
	else
		str.Format("FPC:(%4.1f,%4.1f) Score:%3.1f ID[%d]",ptCen.x, ptCen.y, score, nModelNo);
	if(mode>= 10) mode= mode- 10;
	MgraText(M_DEFAULT, MilOverlayImage[nBufferIndex], 10, 5+(mode*20), str.GetBuffer(0));
	MdispControl(MilDisplay[nBufferIndex], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::DrawCenLine( int nBufferIndex, CPoint centerPoint, bool Over_Clear )
{
	if(centerPoint.x < 0) centerPoint.x = 0;
	if(centerPoint.x >  630) centerPoint.x =  630;
	if(centerPoint.y < 0) centerPoint.y = 0;	
	if(centerPoint.y >  470) centerPoint.y =  470;
	// Clear the overlay buffer with the keying color.           
    if(Over_Clear) {
		if(nBufferIndex == DISP_ALIGN)
			DefaultGrid(true, nBufferIndex) ;//	DefaultGrid(false, 1) ;
		else
			MbufClear(MilOverlayImage[nBufferIndex], TransparentColor); 
	}
	// Set graphic text to transparent background.
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
    // Set drawing color to the green index in the pseudo-color LUT.
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	if(nBufferIndex== DISP_ALIGN_PREV) {
		MgraLine(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x-10), (long)(centerPoint.y), (long)(centerPoint.x+10), (long)(centerPoint.y));
		MgraLine(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x), (long)(centerPoint.y-10), (long)(centerPoint.x), (long)(centerPoint.y+10));
		MgraArcFill(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x), (long)(centerPoint.y), 2, 2, 0, 360);
	}
	else {
		MgraLine(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x-10), (long)(centerPoint.y), (long)(centerPoint.x+10), (long)(centerPoint.y));
		MgraLine(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x), (long)(centerPoint.y-10), (long)(centerPoint.x), (long)(centerPoint.y+10));
		MgraArcFill(M_DEFAULT, MilOverlayImage[nBufferIndex], (long)(centerPoint.x), (long)(centerPoint.y), 2, 2, 0, 360);
	}
    MdispControl(MilDisplay[nBufferIndex], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::DrawLine(int no, int cenx, int ceny, bool Over_Clear)
{
	if(cenx< 0) cenx= 0;
	if(cenx>  630) cenx=  630;
	if(ceny< 0) ceny= 0;
	if(ceny>  470) ceny=  470;
	// Clear the overlay buffer with the keying color.           
    if(Over_Clear) MbufClear(MilOverlayImage[no], TransparentColor); 
	// Set graphic text to transparent background.
    MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
    // Set drawing color to the green index in the pseudo-color LUT.
	MgraColor(M_DEFAULT, M_RGB888(255,0,0));
	if(no== DISP_ALIGN) {
		cenx= (int)((double)(cenx) * ScaleFactor);
		ceny= (int)((double)(ceny) * ScaleFactor);
		MgraLine(M_DEFAULT, MilOverlayImage[no], (long)(cenx-10), (long)(ceny), (long)(cenx+10), (long)(ceny));
		MgraLine(M_DEFAULT, MilOverlayImage[no], (long)(cenx), (long)(ceny-10), (long)(cenx), (long)(ceny+10));
	}
	else {
		MgraLine(M_DEFAULT, MilOverlayImage[no], (long)(cenx-10), (long)(ceny), (long)(cenx+10), (long)(ceny));
		MgraLine(M_DEFAULT, MilOverlayImage[no], (long)(cenx), (long)(ceny-10), (long)(cenx), (long)(ceny+10));
	}
    MdispControl(MilDisplay[no], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::LoadPatPreview( CString strImgName , int iSizeX, int iSizeY, int iModel )
{
	MbufClear(MilDispImage[DISP_ALIGN_PREV], 128);
	MbufClear(MilOverlayImage[DISP_ALIGN_PREV], TransparentColor);
	if(Model[iModel] == M_NULL)
	{
		GraTextDisp(DISP_ALIGN_PREV, 0, 0, "NONE", TRUE);
		return;
	}

	// 임시 사용할 buffer 할당
	MIL_ID TempImage;
	TempImage= M_NULL;
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}
	if (!MbufAlloc2d(MilSystem, iSizeX, iSizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
		AfxMessageBox("Fail to initialize the MIL71 board(MbufAlloc2d TempImage)");

	// 화면 비율 설정
	if(iSizeX > iSizeY)
		ScaleFactor = (double)PATTERN_SIZE_X/(double)iSizeX;
	else
		ScaleFactor = (double)PATTERN_SIZE_Y/(double)iSizeY;

	// Pattern draw
	MpatCopy(Model[iModel], TempImage, M_DEFAULT);
	MbufCopy(TempImage, MilDispImage[DISP_ALIGN_PREV]);
	MimResize(MilDispImage[DISP_ALIGN_PREV], MilDispImage[DISP_ALIGN_PREV], ScaleFactor, ScaleFactor, M_DEFAULT);

	// Don't care draw
	MpatCopy(Model[iModel], TempImage, M_DONT_CARE);
	MbufCopy(TempImage, MilOverlayImage[DISP_ALIGN_PREV]);
	MimResize(MilOverlayImage[DISP_ALIGN_PREV], MilOverlayImage[DISP_ALIGN_PREV], ScaleFactor, ScaleFactor, M_DEFAULT);

	MbufFree(TempImage);	TempImage= M_NULL;

	// Center position draw
	CdblPoint pt;
	MpatInquire(Model[iModel], M_CENTER_X, &(pt.x));
	MpatInquire(Model[iModel], M_CENTER_Y, &(pt.y));
	pt *= ScaleFactor;
	DrawCenLine(DISP_ALIGN_PREV, pt.ToCPoint(), false);
	return;
}

void CMIL71::DefaultGrid(bool Over_Clear, int no) 
{ 
	if(Over_Clear) MbufClear(MilOverlayImage[no], TransparentColor); 
	// Set graphic text to transparent background.
	long nYPosition = 240;
	MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
	MgraColor(M_DEFAULT, M_RGB888(255,150,150));
	MgraRectFill(M_DEFAULT, MilOverlayImage[no], 0, nYPosition, 640, nYPosition+1);
	MgraRectFill(M_DEFAULT, MilOverlayImage[no], 318, nYPosition-50, 322, nYPosition);
	MgraLine(M_DEFAULT, MilOverlayImage[no], 320, nYPosition, 310, nYPosition-30);	
	MgraLine(M_DEFAULT, MilOverlayImage[no], 319, nYPosition, 309, nYPosition-30);	

	MgraLine(M_DEFAULT, MilOverlayImage[no], 320, nYPosition, 330, nYPosition-30);	
	MgraLine(M_DEFAULT, MilOverlayImage[no], 321, nYPosition, 331, nYPosition-30);	

	nYPosition += 140;
	MgraColor(M_DEFAULT, M_RGB888(155,255,145));
	MgraRectFill(M_DEFAULT, MilOverlayImage[no], 0, nYPosition, 640, nYPosition);
	MgraRectFill(M_DEFAULT, MilOverlayImage[no], 318, nYPosition, 322, nYPosition+50);
	MgraLine(M_DEFAULT, MilOverlayImage[no], 320, nYPosition, 310, nYPosition+30);	
	MgraLine(M_DEFAULT, MilOverlayImage[no], 319, nYPosition, 309, nYPosition+30);	

	MgraLine(M_DEFAULT, MilOverlayImage[no], 320, nYPosition, 330, nYPosition+30);	
	MgraLine(M_DEFAULT, MilOverlayImage[no], 321, nYPosition, 331, nYPosition+30);	

	MgraColor(M_DEFAULT, M_RGB888(245,250,290));
	MgraFont(M_DEFAULT, M_FONT_DEFAULT); //M_FONT_DEFAULT_LARGE);
	MgraFontScale(M_DEFAULT, 2, 2);
	MdispControl(MilDisplay[no], M_WINDOW_OVR_SHOW, M_ENABLE);
}
void CMIL71::GraTextDisp( int nBufferIndex, int nStartX, int nStartY, CString strText, BOOL bOverlayClear /*= FALSE*/ )
{ 
	if(bOverlayClear)	MbufClear(MilOverlayImage[nBufferIndex], TransparentColor);
	MgraColor(M_DEFAULT, M_RGB888(150,150,150));
	MgraFont(M_DEFAULT, M_FONT_DEFAULT); //M_FONT_DEFAULT_LARGE);
	MgraFontScale(M_DEFAULT, 2, 2);
	MgraText(M_DEFAULT, MilOverlayImage[nBufferIndex], nStartX, nStartY, strText.GetBuffer(0));
}


void CMIL71::SaveImage( CString strPath, int nBufferndex )
{
	MbufExport(strPath.GetBuffer(0), M_JPEG_LOSSY , MilDispImage[nBufferndex]);
}

void CMIL71::GrabNotUseTextDisp( int nBufferNo )
{
	CString strText;
	strText = " NOT USE ";
	OverlayClear(nBufferNo);

	MgraColor(M_DEFAULT, M_RGB888(150,150,150));
	MgraFont(M_DEFAULT, M_FONT_DEFAULT); //M_FONT_DEFAULT_LARGE);
	MgraFontScale(M_DEFAULT, 4, 4);
	MgraText(M_DEFAULT, MilOverlayImage[nBufferNo], 10, 10, strText.GetBuffer(0));
	MdispControl(MilDisplay[nBufferNo], M_WINDOW_OVR_SHOW, M_ENABLE);
}

void CMIL71::LoadImage( CString strPath, int nBufferIndex, BOOL bBinarize /*= FALSE*/, long nCondition )
{
	MbufClear(MilOverlayImage[nBufferIndex], TransparentColor);
	MbufClear(MilDispImage[nBufferIndex], 128);
	MbufLoad(strPath.GetBuffer(0), MilDispImage[nBufferIndex]);
	// 원본 백업
	MbufClear(MilOriginal, 128);
	MbufCopy(MilDispImage[nBufferIndex], MilOriginal);
	if(bBinarize)
	{
		MimBinarize(MilDispImage[nBufferIndex], MilDispImage[nBufferIndex], M_GREATER , nCondition, M_DEFAULT);
		MimDilate(MilDispImage[nBufferIndex], MilDispImage[nBufferIndex], 1, M_GRAYSCALE);
	}
// 	OverlayClear(nBufferIndex);
}

void CMIL71::PatternAllClear()
{
	for(int i=0; i< MAX_MOD_BUFFER_NUM; i++) 
	{			
		if(Result[i]!= M_NULL) MpatFree(Result[i]); 
		if(Model[i]!= M_NULL) MpatFree(Model[i]); 

		Result[i]= M_NULL;
		Model[i]= M_NULL;
	}
}

void CMIL71::RemovePattern( int nModelIndex )
{
	if(Result[nModelIndex]!= M_NULL) MpatFree(Result[nModelIndex]); 
	if(Model[nModelIndex]!= M_NULL) MpatFree(Model[nModelIndex]); 

	Result[nModelIndex]= M_NULL;
	Model[nModelIndex]= M_NULL;
}

BOOL CMIL71::SelDontcareWindow( HWND hCam0 )
{
	if (!MdispAlloc(MilSystem, M_DEV0, M_DEF_DISPLAY_FORMAT, M_WINDOWED , &MilDisplayDontcare) )
		return FALSE;	
	if (!MbufAllocColor(MilSystem, 3, DONTCARE_SIZE_X, DONTCARE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_DISP+M_PROC/*+M_NON_PAGED*/, &MilDispImageDontcare))
		return FALSE;

	MbufClear(MilDispImageDontcare, 0);
	MdispSelectWindow(MilDisplayDontcare, MilDispImageDontcare, hCam0);

	if(MilDisplayDontcare!= M_NULL) 
	{
		MdispControl(MilDisplayDontcare, M_WINDOW_OVR_WRITE, M_ENABLE);
		MdispInquire(MilDisplayDontcare, M_WINDOW_OVR_BUF_ID, &MilOverlayDontcare);
		MdispControl(MilDisplayDontcare, M_WINDOW_OVR_SHOW, M_ENABLE);
		MdispInquire(MilDisplayDontcare, M_KEY_COLOR, &TransparentColor); 
		if (MbufInquire(MilOverlayDontcare, M_SIZE_BAND, M_NULL) == 1)
		{
			MgraControl(M_DEFAULT, M_BACKGROUND_MODE, M_TRANSPARENT);
			MgraColor(M_DEFAULT, M_RGB888(0, 255, 0));
		}
	}

	return TRUE;
}

void CMIL71::DeselDontcareWindow()
{
	MdispControl(MilDisplayDontcare, M_WINDOW_OVR_WRITE, M_DISABLE);
	MdispDeselect(MilDisplayDontcare, MilDispImageDontcare);

	if(MilOverlayDontcare)		MbufFree(MilOverlayDontcare);
	if(MilDispImageDontcare)	MbufFree(MilDispImageDontcare);
	if(MilDisplayDontcare)		MdispFree(MilDisplayDontcare);
}

void CMIL71::ClearOverlayDontcare()
{
	MbufClear(MilOverlayDontcare, TransparentColor);
}

void CMIL71::LoadDontcare( int nModelIndex )
{
	ClearOverlayDontcare();
	ClearImageDontcare();

	MIL_ID TempImage;
	TempImage= M_NULL;
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}
	if (!MbufAlloc2d(MilSystem, DONTCARE_SIZE_X*(int)DONTCARE_SCALEFACTOR, DONTCARE_SIZE_Y*(int)DONTCARE_SCALEFACTOR, 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
		AfxMessageBox("Fail to initialize the MIL71 board(MbufAlloc2d TempImage)");

	// Pattern draw
	MpatCopy(Model[nModelIndex], TempImage, M_DEFAULT);
	MbufCopy(TempImage, MilDispImageDontcare);
	MimResize(MilDispImageDontcare, MilDispImageDontcare, DONTCARE_SCALEFACTOR, DONTCARE_SCALEFACTOR, M_DEFAULT);

	// Don't care draw
	MpatCopy(Model[nModelIndex], TempImage, M_DONT_CARE);
	MbufCopy(TempImage, MilOverlayDontcare);
	MimResize(MilOverlayDontcare, MilOverlayDontcare, DONTCARE_SCALEFACTOR, DONTCARE_SCALEFACTOR, M_DEFAULT);

	MbufFree(TempImage);	TempImage= M_NULL;
}

void CMIL71::ClearImageDontcare()
{
	MbufClear(MilDispImageDontcare, 128);
}

void CMIL71::DrawDontcareBrush( int nThickness, CPoint ptCenter )
{
	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));
	MgraRect(M_DEFAULT, MilOverlayDontcare, ptCenter.x-nThickness, ptCenter.y-nThickness, ptCenter.x+nThickness, ptCenter.y+nThickness);
}

void CMIL71::MovingDontcareBrush( BOOL bUnmask, int nThickness,CPoint ptCenter, CPoint ptOldCenter )
{
	if(ptCenter != CPoint(1000, 1000))
	{
		if(bUnmask)
			MgraColor(M_DEFAULT, TransparentColor);
		else
			MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));

		MgraRectFill(M_DEFAULT, MilOverlayDontcare, ptOldCenter.x-nThickness, ptOldCenter.y-nThickness, ptOldCenter.x+nThickness, ptOldCenter.y+nThickness);
	}
	DrawDontcareBrush(nThickness, ptCenter);
}

void CMIL71::ResizeDontcareBrush( int nThickness, CPoint ptCenter )
{
	MgraColor(M_DEFAULT, M_RGB888(255, 0, 0));
	MgraRect(M_DEFAULT, MilOverlayDontcare, ptCenter.x-nThickness, ptCenter.y-nThickness, ptCenter.x+nThickness, ptCenter.y+nThickness);
}

void CMIL71::ApplyDontcare( int nMModelIndex )
{
	MIL_ID TempImage = M_NULL;
	if (!MbufAlloc2d(MilSystem, DONTCARE_SIZE_X, DONTCARE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
		AfxMessageBox("Fail to initialize the MIL71 board(MbufAlloc2d TempImage)");
	MbufClear(TempImage, 0);

	MIL_ID grayFilterImage = M_NULL;
	if (!MbufAlloc2d(MilSystem, DONTCARE_SIZE_X, DONTCARE_SIZE_Y, 8+M_UNSIGNED, M_IMAGE+M_PROC, &grayFilterImage))
		AfxMessageBox("Fail to initialize the MIL71 board(MbufAlloc2d grayImage)");
	MbufClear(grayFilterImage, 128);

	MbufCopyCond(grayFilterImage, TempImage, MilOverlayDontcare, M_EQUAL, M_RGB888(255, 0, 0));
	MimResize(TempImage, TempImage, 0.5, 0.5, M_DEFAULT);

	MpatSetDontCare(Model[nMModelIndex], TempImage, 0, 0, 128);
	if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}
}

int CMIL71::SearchBinaryPattern( double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition )
{
	double final_x, final_y, final_Score;
	pdCenX = pdCenY = pdScore = 0.0;

	int nFindedModelNo = 0;
	CString strName = _T("");
	MbufCopy(MilDispImage[nBufferIndex], ProcImage);

	MimBinarize(ProcImage, ProcImage, M_GREATER , nCondition, M_DEFAULT);
	MimDilate(ProcImage, ProcImage, 1, M_GRAYSCALE);
	MpatPreprocModel(ProcImage, Model[nModelIndex], M_DEFAULT);

	// MpatFindMultipleModel() 함수가 잘 안되서 걍 for문으로 하나씩 찾는다.
	for(int i = 0; i < nModelCnt; i++)
	{
		MpatFindModel(ProcImage, Model[nModelIndex+i], Result[nModelIndex+i]);

		// 가장 높은 score를 찾는다.
		if(MpatGetNumber(Result[nModelIndex+i], M_NULL) > 0)
		{
			MpatGetResult(Result[nModelIndex + i], M_SCORE, &final_Score);
			if(pdScore < final_Score)
			{
				pdScore = final_Score;
				nFindedModelNo = i;
			}
		}
	}

	if(pdScore > 0.0)
	{
		MpatGetResult(Result[nModelIndex + nFindedModelNo], M_POSITION_X, &final_x);
		MpatGetResult(Result[nModelIndex + nFindedModelNo], M_POSITION_Y, &final_y);
	}
	else
	{
		// 못찾으면 Index 99로 설정
		nFindedModelNo = 99;
		final_x = final_y = final_Score = -1;
	}


	pdCenX = final_x;	// X
	pdCenY = final_y;	// Y
	nModelCnt = nFindedModelNo;

	// 최종 Score로 찾았는지 못찾았는지 판별
	if(pdScore >= (double)GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore)
		return 1;
	else 
		return 0;
}

int CMIL71::SearchBinaryPattern_auto( double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition )
{	//  [6/9/2009 janghan] Auto Teaching 관련 전면 수정//////////////////////////////////////
	double final_x, final_y, final_Score;
	pdCenX = pdCenY = pdScore = 0.0;

	int iBuf_CenterPosX[4], iBuf_CenterPosY[4], iBuf_SizeX[4], iBuf_SizeY[4];

	int nFindedModelNo = 0;
	//CString strName = _T("");
	//char szFileName[128];
	int nModel = nModelIndex % MAX_CAM_NUM;
	MIL_ID TempImage = M_NULL;
	
	MIL_ID	MilIModelBuf[4]; //Model Origin Image
	MIL_ID	Model_Auto = M_NULL;		
	MIL_ID	Result_Auto = M_NULL;


	for (int i=0; i<4; i++)
	{
		MilIModelBuf[i] = M_NULL;
	}

	

	switch (nModelIndex / MAX_CAM_NUM )
	{
	case 0:
		//0번 카메라
		break;
	case 1://1번 카메라
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM1_FPC_0.bmp", MilSystem, &MilIModelBuf[0]); //FFC Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM1_FPC_1.bmp", MilSystem, &MilIModelBuf[1]); //FFC Mark Model
		//MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM1_PCB_0.bmp", MilSystem, &MilIModelBuf[2]); //PCB Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CPCB_BL.bmp", MilSystem, &MilIModelBuf[2]); //PCB Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM1_PCB_1.bmp", MilSystem, &MilIModelBuf[3]); //PCB Mark Model

		//SESL 1층용 이미지 사용 - 
		/*iBuf_SizeX[0] = 111;
		iBuf_SizeX[1] = 160;
		iBuf_SizeX[2] = 111;
		iBuf_SizeX[3] = 169;

		iBuf_SizeY[0] = 85;
		iBuf_SizeY[1] = 85;
		iBuf_SizeY[2] = 85;
		iBuf_SizeY[3] = 55;

		iBuf_CenterPosX[0] = 96;
		iBuf_CenterPosX[1] = 46;
		iBuf_CenterPosX[2] = 96;
		iBuf_CenterPosX[3] = 43;

		iBuf_CenterPosY[0] = 69;
		iBuf_CenterPosY[1] = 73;
		iBuf_CenterPosY[2] = 14;
		iBuf_CenterPosY[3] = 6;*/
		
		//SESL 2층용 이미지 사용 -
		iBuf_SizeX[0] = 111;
		iBuf_SizeX[1] = 111;
		//iBuf_SizeX[2] = 111;
		iBuf_SizeX[2] = 200;
		iBuf_SizeX[3] = 111;

		iBuf_SizeY[0] = 85;
		iBuf_SizeY[1] = 85;
		//iBuf_SizeY[2] = 85;
		iBuf_SizeY[2] = 50;
		iBuf_SizeY[3] = 50;

		iBuf_CenterPosX[0] = 96;
		iBuf_CenterPosX[1] = 31;
		//iBuf_CenterPosX[2] = 96;
		iBuf_CenterPosX[2] = 129;
		iBuf_CenterPosX[3] = 25;

		iBuf_CenterPosY[0] = 69;
		iBuf_CenterPosY[1] = 50;
		//iBuf_CenterPosY[2] = 14;
		iBuf_CenterPosY[2] = 3;
		iBuf_CenterPosY[3] = 14;
		////////////////////////////////////////////

		break;
	case 2://2번 카메라
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM2_FPC_0.bmp", MilSystem, &MilIModelBuf[0]); //FFC Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM2_FPC_1.bmp", MilSystem, &MilIModelBuf[1]); //FFC Mark Model
		//MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM2_PCB_0.bmp", MilSystem, &MilIModelBuf[2]); //PCB Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\PCB_BR.bmp", MilSystem, &MilIModelBuf[2]); //PCB Edge Model
		MbufRestore("C:\\AMT_NEW\\RECIPE\\CAM2_PCB_1.bmp", MilSystem, &MilIModelBuf[3]); //PCB Mark Model

		//SESL 1층//////////////////////////////////////////
		/*iBuf_SizeX[0] = 111;
		iBuf_SizeX[1] = 160;
		iBuf_SizeX[2] = 111;
		iBuf_SizeX[3] = 164;

		iBuf_SizeY[0] = 85;
		iBuf_SizeY[1] = 85;
		iBuf_SizeY[2] = 85;
		iBuf_SizeY[3] = 61;

		iBuf_CenterPosX[0] = 13;
		iBuf_CenterPosX[1] = 118;
		iBuf_CenterPosX[2] = 15;
		iBuf_CenterPosX[3] = 108;

		iBuf_CenterPosY[0] = 70;
		iBuf_CenterPosY[1] = 73;
		iBuf_CenterPosY[2] = 12;
		iBuf_CenterPosY[3] = 6;*/
		////////////////////////////////////////////////////////////

		//SESL 2층//////////////////////////////////////
		//SESL 2층//////////////////////////////////////
		iBuf_SizeX[0] = 111;
		iBuf_SizeX[1] = 111;
		//iBuf_SizeX[2] = 111;
		iBuf_SizeX[2] = 200;
		iBuf_SizeX[3] = 111;

		iBuf_SizeY[0] = 85;
		iBuf_SizeY[1] = 85;
		//iBuf_SizeY[2] = 85;
		iBuf_SizeY[2] = 50;
		iBuf_SizeY[3] = 50;

		iBuf_CenterPosX[0] = 13;
		iBuf_CenterPosX[1] = 79;
		//iBuf_CenterPosX[2] = 15;
		iBuf_CenterPosX[2] = 73;
		iBuf_CenterPosX[3] = 85;

		iBuf_CenterPosY[0] = 70;
		iBuf_CenterPosY[1] = 50;
		//iBuf_CenterPosY[2] = 12;
		iBuf_CenterPosY[2] = 3;
		iBuf_CenterPosY[3] = 14;
		////////////////////////////////////////////////////////////////////////////////
		////////////////////////////////////////////////////////////////////////////////
		break;
	case 3:
		//3번 카메라
		break;
	}
/////
	MpatAllocModel(MilSystem, MilIModelBuf[nModelIndex % 4], 0, 0,
		iBuf_SizeX[nModelIndex % 4], iBuf_SizeY[nModelIndex % 4], M_NORMALIZED, &Model_Auto);

	MpatAllocResult(M_DEFAULT, 1L, &Result_Auto);

	MpatSetCenter(Model_Auto, iBuf_CenterPosX[nModelIndex % 4], iBuf_CenterPosY[nModelIndex % 4]);
	MpatSetPosition(Model_Auto, 0, 0, 640, 480);
  /////
	MbufCopy(MilDispImage[nBufferIndex], ProcImage);

	MimBinarize(ProcImage, ProcImage, M_GREATER , nCondition, M_DEFAULT);
	MimDilate(ProcImage, ProcImage, 1, M_GRAYSCALE);
	MpatPreprocModel(ProcImage, Model_Auto, M_DEFAULT);
////////////////////////////////////////////////////////////////////////////////////////////////////////////
	double dsizeX, dsizeY;
	MpatInquire(Model_Auto, M_ALLOC_SIZE_X, &dsizeX);
	MpatInquire(Model_Auto, M_ALLOC_SIZE_Y, &dsizeY);

	/*if(TempImage) {		MbufFree(TempImage); TempImage= M_NULL;	}

	if (!MbufAlloc2d(MilSystem, (long)dsizeX, (long)dsizeY, 8+M_UNSIGNED, M_IMAGE+M_PROC, &TempImage))
	{
		AfxMessageBox("Fail to initialize the MIL71 board(SavePattern TempImage)");
	}*/

	/*sprintf_s(szFileName,"C:\\Temp_JH\\Binary_%d.bmp", nModelIndex);
	MbufSave(szFileName, ProcImage);

	MpatCopy(Model_Auto, TempImage, M_DEFAULT );
	sprintf_s(szFileName,"C:\\Temp_JH\\Model_%d.bmp", nModelIndex);
	MbufSave(szFileName, TempImage);*/
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//  MbufCopy(ProcImage, MilDispImage[nBufferIndex]);

	MpatFindModel(ProcImage, Model_Auto, Result_Auto);

	if(MpatGetNumber(Result_Auto, M_NULL) == 1L)//찾았을때
	{
		MpatGetResult(Result_Auto, M_POSITION_X, &final_x);
		MpatGetResult(Result_Auto, M_POSITION_Y, &final_y);
		MpatGetResult(Result_Auto, M_SCORE, &final_Score);

		if(Result_Auto != M_NULL)
			MpatFree(Result_Auto);
		if(Model_Auto != M_NULL)
			MpatFree(Model_Auto);

		for (int i=0; i<4; i++)
		{
			if(MilIModelBuf[i] != M_NULL)
				MbufFree(MilIModelBuf[i]);
		}
	}
	else
	{
		if(Result_Auto != M_NULL)
			MpatFree(Result_Auto);
		if(Model_Auto != M_NULL)
			MpatFree(Model_Auto);

		for (int i=0; i<4; i++)
		{
			if(MilIModelBuf[i] != M_NULL)
				MbufFree(MilIModelBuf[i]);
		}
		//sprintf_s(szFileName,"C:\\Temp_JH\\Binary_%d_BAD.bmp", nModelIndex);
		//MbufSave(szFileName, ProcImage);

		return 0;
	}
	pdCenX = final_x;	// X
	pdCenY = final_y;	// Y
	pdScore = final_Score;// 매칭률

	if (pdScore > GetDoc()->m_fMaxScore)
	{
		GetDoc()->m_fMaxScore = final_Score;
		GetDoc()->m_nTuneLevl = GetDoc()->m_nLevelCount;

		//sprintf_s(szFileName,"C:\\Temp_JH\\Binary_%d.bmp", nModelIndex);
		//MbufSave(szFileName, ProcImage);
		 
		switch (nModel)
		{
		case 0:
			GetDoc()->m_fEdgeFind_X = final_x;
			GetDoc()->m_fEdgeFind_Y = final_y;

			GetDoc()->m_fAutoDistance_X =  (320. -  final_x ) * GetApp()->m_dFovX;
			GetDoc()->m_fAutoDistance_Y =  (240. -  final_y ) * GetApp()->m_dFovY;
			break;
		case 1:
			GetDoc()->m_fMarkFind_X = final_x;
			break;
		case 2://PCB Edge X는 겹침 확인용으로 좌표는 필요 없다.
			break;
		case 3:
			GetDoc()->m_fPCBmarkFind_X = final_x;
			//GetDoc()->m_fAutoDistance_X =  GetDoc()->m_fPCBmarkFind_X -  final_x;
			break;
		default:
			AfxMessageBox("Wrong Index");
			break;
		}
	}


	// 최종 Score로 찾았는지 못찾았는지 판별
	if(pdScore >= (double)GetDoc()->m_pAmtData->m_CurModelInfo.iAlignScore)
		return 1;
	else 
		return 0;
}

void CMIL71::GrabBinarize( int nBufferIndex, long nCondition )
{
	// 원본 이미지로부터 Binarize한다.
	MimBinarize(MilOriginal, MilDispImage[nBufferIndex], M_GREATER , nCondition, M_DEFAULT);
	MimDilate(MilDispImage[nBufferIndex], MilDispImage[nBufferIndex], 1, M_GRAYSCALE);
}

void CMIL71::BufCopy(MIL_ID srcBuf, MIL_ID desBuf)
{
	//버퍼 복사
	MbufClear(desBuf, 128);
	MbufCopy(srcBuf, desBuf);
}