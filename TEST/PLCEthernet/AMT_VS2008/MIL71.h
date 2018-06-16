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
	long TransparentColor;        // �������� Ŭ�����ϴµ� ���� ����color
	bool VISION_SYS_MODE;		

	/* MIL variables */
	MIL_ID MilApplication,	// MIL Application identifier.
			MilSystem,				// MIL System identifier.  
			MilDigitizer[MAX_CAM_NUM],

			MilDisplay[MAX_BUFFER_NUM],
			MilDispImage[MAX_BUFFER_NUM],
			MilOverlayImage[MAX_BUFFER_NUM],

			ProcImage,	// Search Pattern �� AddPattern�� �� ���۷θ� �Ѵ�.
			Model[MAX_MOD_BUFFER_NUM], Result[MAX_MOD_BUFFER_NUM];

	// Don't care ������
	MIL_ID MilDisplayDontcare;
	MIL_ID MilDispImageDontcare;
	MIL_ID MilOverlayDontcare;

	// binarize ���� ���ؼ� ����صδ� ��������
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
	
	// ȭ�鿡 NONE�� ǥ���� �ִ� �Լ�
	void GrabNotUseTextDisp(int nBufferNo );

	// �������̿� �����س����� ����� �Լ�
	void OverlayClear(int nBufferNo);
	// �������̿� ���� ���� �Լ�
	void GraTextDisp(int nBufferIndex, int nStartX, int nStartY, CString strText, BOOL bOverlayClyer = FALSE) ;
	// �簢�� �׸��� �Լ�
	void DrawRect(int no, CRect rt, bool Over_Clear);
	// Pattern ���� �׸��� �Լ�
	void DrawPatRect(BOOL bIsPCB, int mode, int nBufferIndex, CRect rt, CdblPoint ptCen, double score=0.0, int nModelNo = 0, DWORD dwSearchTime = 0, bool Over_Clear= true);
	// Line �׸��� �Լ�
	void DrawLine(int no, int xPos, int yPos, bool Over_Clear);
	// Center ���ڰ� �׸��� �Լ�
	void DrawCenLine(int nBufferIndex, CPoint centerPoint, bool Over_Clear);
	// Live ���󿡼� Teaching ���ؼ� �׸��� �Լ�
	void DefaultGrid(bool Over_Clear, int no);
	// formAlign���� Pattern �̸����� ȭ�� �����ִ� �Լ�
	void LoadPatPreview(CString strImgName , int iSizeX, int iSizeY,  int iModel);

	// �ش� Index�� model�� ��ϵǾ� �ִ��� �˾ƿ��� �Լ�
	BOOL IsPatternEmpty(int nModelIndex);
	// Pattern �߰��ϴ� �Լ�
	void AddPattern(int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern �Ӽ� �����ϴ� �Լ�(Center point, score ���...)
	void SetModelParam(int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern �ҷ����� �Լ�
	void LoadPattern(CString strPtrPath, int nModelIndex, CRect rtPattern, CPoint ptCenter);
	// Pattern �����ϴ� �Լ�
	void SavePattern(int nModelNo, CString strPath);
	// ��� Pattern�� �����ϴ� �Լ�
	void PatternAllClear();
	// �ش� Pattern�� �����ϴ� �Լ�
	void RemovePattern(int nModelIndex);

	// Pattern�� ã�� �Լ�(2��ȭ �ƴ�)
	int SearchPattern(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt );

	// Pattern�� ã�� �Լ�(2��ȭ ��)
	int SearchBinaryPattern(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition );
	
	int SearchBinaryPattern_auto(double &pdCenX, double &pdCenY, double &pdScore, int nBufferIndex, int nModelIndex, int &nModelCnt, long nCondition );
	// Image�� 2��ȭó���ϴ� �Լ�
	void GrabBinarize(int nBufferIndex, long nCondition);

	// Display ������ �¿� ȭ���� Ȯ��/����ϴ� �Լ�
	int Zoom(long lScale);
	// Grab�ϴ� �Լ�
	int Grab(int nBufferIndex, int nCamNo , int iCurStatus = STATUS_RUN, BOOL bBackup = FALSE );

	// Auto���� ���� �����ִ� control�� Mil buffer�� �������ִ� �Լ�
	BOOL SelAutoWindow(HWND hCam0, HWND hCam1, HWND hCam2, HWND hCam3);
	// Teach���� ���� �����ִ� control�� Mil buffer�� �������ִ� �Լ�
	BOOL SelTeacWindow(HWND hCam0, HWND hCam1);
	// Align���� ���� �����ִ� control�� Mil buffer�� �������ִ� �Լ�
	BOOL SelAlignWindow(HWND hCam0, HWND hCam1);

	// �����ߴ� control�� mil buffer�� ���������ִ� �Լ�
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

	// MIL ����
	int Close();
	// MIL Initialize
	int Open(CString strDcfPath);
	virtual ~CMIL71();
};