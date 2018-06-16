#pragma once

#include "mil.h"
//
/////////////////////////////////////////////////////////////////////////////
// CMIL71 window



class CMIL71
{
// Construction
public:
	CMIL71();

// Attributes
public:
// Operations
public:
	long TransparentColor;        // 오버레이 클리어하는데 쓰는 투명color
	bool VISION_SYS_MODE;		

	/* MIL variables */
	MIL_ID MilApplication,	// MIL Application identifier.
			MilSystem,				// MIL System identifier.  
			MilDigitizer[MAX_CAM_NUM],

			MilDisplay[MAX_BUFFER_NUM],
			MilDispImage[MAX_BUFFER_NUM],
			MilOverlayImage[MAX_BUFFER_NUM],

			ProcImage,	// Search Pattern 및 AddPattern은 이 버퍼로만 한다.
			Model[MAX_MOD_BUFFER_NUM], Result[MAX_MOD_BUFFER_NUM];

	// Don't care 편집용
	MIL_ID MilDisplayDontcare;
	MIL_ID MilDispImageDontcare;
	MIL_ID MilOverlayDontcare;

	// binarize 편집 위해서 백업해두는 원본영상
	MIL_ID MilOriginal;

	CDC m_pDC1[MAX_BUFFER_NUM];

	CAM_INFO	m_pCamInfo[MAX_CAM_NUM];

	double ScaleFactor;

	long m_HEAD_CAM_NO[MAX_CAM_NUM];
	double m_HEAD_CAM_X_MODE[MAX_CAM_NUM];
	double m_HEAD_CAM_Y_MODE[MAX_CAM_NUM];


	CString m_strDirLogPath, m_strDirAlignPath  ; 

// Implementation
public:
	// Image Save & Load
	void LoadImage(CString strPath, int nBufferIndex, BOOL bBinarize = FALSE, long nCondition = BINARIZE_CONDITION);
	void SaveImage(CString strPath, int nBufferndex);
	void SavePatternImage(int nModelNo, CRect rtPattern, CString strPath);
	
	// 화면에 NONE를 표시해 주는 함수
	void GrabNotUseTextDisp(int nBufferNo );

	// 오버레이에 낙서해놓은거 지우는 함수
	void OverlayClear(int nBufferNo);
	// 오버레이에 글자 적는 함수
	void GraTextDisp(int nBufferIndex, int nStartX, int nStartY, CString strText, BOOL bOverlayClyer = FALSE) ;
	// 사각형 그리는 함수
	void DrawRect(int no, CRect rt, bool Over_Clear);
	// Pattern 영역 그리는 함수
	void DrawPatRect(BOOL bIsPCB, int mode, int nBufferIndex, CRect rt, CdblPoint ptCen, double score=0.0, int nModelNo = 0, DWORD dwSearchTime = 0, bool Over_Clear= true);
	// Line 그리는 함수
	void DrawLine(int no, int xPos, int yPos, bool Over_Clear);
	// Center 십자가 그리는 함수
	void DrawCenLine(int nBufferIndex, CPoint centerPoint, bool Over_Clear);
	// Live 영상에서 Teaching 기준선 그리는 함수
	void DefaultGrid(bool Over_Clear, int no);
	// formAlign에서 Pattern 미리보기 화면 보여주는 함수
	void LoadPatPreview(CString strImgName , int iSizeX, int iSizeY,  int iModel);

	// 해당 Index에 model이 등록되어 있는지 알아오는 함수
	BOOL IsPatternEmpty(int nModelIndex);
	// Pattern 추가하는 함수
	void AddPattern(int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern 속성 설정하는 함수(Center point, score 등등...)
	void SetModelParam(int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern 불러오는 함수
	void LoadPattern(CString strPtrPath, int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern 저장하는 함수
	void SavePattern(int nModelNo, CString strPath);
	// 모든 Pattern을 삭제하는 함수
	void PatternAllClear();
	// 해당 Pattern을 삭제하는 함수
	void RemovePattern(int nModelIndex);

	// Pattern을 찾는 함수(2진화 아님)
	int SearchPattern(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt );

	// Pattern을 찾는 함수(2진화 용)
	int SearchBinaryPattern(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition );
	
	int SearchBinaryPattern_auto(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition );
	// Image를 2진화처리하는 함수
	void GrabBinarize(int nBufferIndex, long nCondition);

	// Display 영역에 맞에 화면을 확대/축소하는 함수
	int Zoom(long lScale);
	// Grab하는 함수
	int Grab(int nBufferIndex, int nCamNo , int iCurStatus = STATUS_RUN, BOOL bBackup = FALSE );

	// Auto에서 영상 보여주는 control과 Mil buffer를 연결해주는 함수
	BOOL SelAutoWindow(HWND hCam0, HWND hCam1, HWND hCam2, HWND hCam3);
	// Teach에서 영상 보여주는 control과 Mil buffer를 연결해주는 함수
	BOOL SelTeacWindow(HWND hCam0, HWND hCam1);
	// Align에서 영상 보여주는 control과 Mil buffer를 연결해주는 함수
	BOOL SelAlignWindow(HWND hCam0, HWND hCam1);

	// 연결했던 control과 mil buffer를 해제시켜주는 함수
	void DeselAutoWindow();
	void DeselTeacWindow();
	void DeselAlignWindow();

	// Don't care
	BOOL SelDontcareWindow(HWND hCam0);
	void DeselDontcareWindow();
	void ClearOverlayDontcare();
	void ClearImageDontcare();
	void LoadDontcare(int nModelIndex);
	void DrawDontcareBrush(int nThickness, CPoint ptCenter);
	void MovingDontcareBrush(BOOL bUnmask, int nThickness,CPoint ptCenter, CPoint ptOldCenter);
	void ResizeDontcareBrush(int nThickness, CPoint ptCenter);
	void ApplyDontcare(int nMModelIndex);
	// 

	//  [6/10/2009 janghan]
	void BufCopy(MIL_ID srcBuf, MIL_ID desBuf);

	// MIL 종료
	int Close();
	// MIL Initialize
	int Open(CString strDcfPath);
	virtual ~CMIL71();
};