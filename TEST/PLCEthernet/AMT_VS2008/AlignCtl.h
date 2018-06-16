#pragma once

#include "MIL71.h"

class CAlignCtl  
{
public:
	CAlignCtl();
	virtual ~CAlignCtl();

public:
	//////////////////////////////////////////////////////////////////////////
	// 기본 컨셉은 외부 class에서 Mil class를 직접 접근하지 않고 이 class를 통해서
	// 접근하도록 되어 있다 (몇개 class 제외)
	// 가능하면 GetAlignCtl()->GetVision()-> 이런식으로 직접 호출하지 말고
	// 이 class에 함수 하나를 더 추가하여서
	// GetAlignCtl()-> 이걸로 끝나도록 유지할것
	// CAlignCtl 하고 CMIL71에서 동일한 함수 명이 많은 이유가 이 때문이다.
	//////////////////////////////////////////////////////////////////////////



	// Contact 보정 위치를 계산하는 함수
	int ContactPosCalibaration(double dYmmOffset);
	// Contact 보정 위치를 계산하는 함수
	int ContactPosCalibaration_FFC_Edge(int nCamNum, BOOL bBackY=FALSE);//  [6/10/2009 janghan]
	int ContactPosCalibaration_SortX(int nCamNum);//  [6/15/2009 janghan]
	// Line 그리는 함수
	void DrawLine(int nBufferIndex);
	// Align 창에서 SearchPattern할 때 실행되는 함수. 안에서 SearchPattern을 호출한다.
	void ManualSearchPattern(int nModelIndex);
	// Pattern을 찾는 함수
	BOOL SearchPattern(int nModelIndex , BOOL bIsManual , int iRetryCnt = 0 );
	// Pattern을 불러오는 함수
	void LoadPattern(int iPattern , STRUCT_MODEL_DATA *pModelData);
	// Pattern의 Center point를 그리는 함수
	void DrawCenLine(int iNo , int ixPos , int iyPos , bool bOverClear);
	// Pattern 미리보기 화면을 보여주는 함수
	void LoadPatPreview(int iPtrPos);
	// Pattern을 저장하는 함수
	void SavePattern(int nModelIndex, long nCondition);
	// 저장할 Pattern File name을 선정하는 함수
	CString GeneratePatternFileName(int nModelIndex);
	// FormAlign에서 Pattern 선택할 때에 해당 Pattern 영역을 그려주는 함수
	void MoveRectToDefault(int nModelIndex);
	// FormAlign에서 To Center 버튼 누를때 실행되는 함수
	void ResetRect(int nModelIndex);
	// Pattern을 전부 삭제하는 함수
	void PatternAllClear();
	// FPC Pattern을 삭제하는 함수
	void RemoveFPCPattern(int nBufferIndex);
	// 오버레이에 그려진 낙서를 지우는 함수
	void OverlayClear(int nBufferNo);
	// 화면에 NONE라고 적어주는 함수
	void GrabNotUseTextDisp(int nBufferNo);
	// Success, Error image를 저장하는 함수
	void SaveSearchImage(BOOL bErrorImg, int nCamIndex, CString strPosition, double nScore);
	// Contact image를 저장하는 함수
	void GrabContactImage();
	// 2진화처리하는 함수
	void GrabBinarize(int nBufferIndex, long nCondition);

	void AlignLog(LPCTSTR szHPanelID, LPCTSTR szPosition, double dScore, CString strSuccess);

	// Pattern을 찾는 함수
	BOOL SearchPattern2(int nModelIndex , BOOL bIsManual , int iRetryCnt = 0 );//  [6/9/2009 janghan]


	inline CMIL71* GetVision() { return &m_Vision; }
	CMIL71	m_Vision;

	// 센터포인트 위치. //
	CPoint m_ptCenter;

	double m_dScore;
	double m_dPCBScore;	// PCB 전용

	// 사각형 크기. //
	CRect m_rtPattern;
	DWORD m_dwSearchTime;
	STRUCT_ALIGN_RESULT		m_ManualAlignResult;	// FormAlign에서 Manual로 하는 값은 따로 저장

	//////////////////////////////////////////////////////////////////////////
	// singleton
public:
	static CAlignCtl* Instance();
	static void Release();
private:
	static CAlignCtl* _instance;
	//////////////////////////////////////////////////////////////////////////
};

// inline CAlignCtl* GetAlignCtl() {	return CAlignCtl::Instance();	}
