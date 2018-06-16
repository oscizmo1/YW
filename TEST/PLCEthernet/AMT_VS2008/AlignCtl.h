#pragma once

#include "MIL71.h"

class CAlignCtl  
{
public:
	CAlignCtl();
	virtual ~CAlignCtl();

public:
	//////////////////////////////////////////////////////////////////////////
	// �⺻ ������ �ܺ� class���� Mil class�� ���� �������� �ʰ� �� class�� ���ؼ�
	// �����ϵ��� �Ǿ� �ִ� (� class ����)
	// �����ϸ� GetAlignCtl()->GetVision()-> �̷������� ���� ȣ������ ����
	// �� class�� �Լ� �ϳ��� �� �߰��Ͽ���
	// GetAlignCtl()-> �̰ɷ� �������� �����Ұ�
	// CAlignCtl �ϰ� CMIL71���� ������ �Լ� ���� ���� ������ �� �����̴�.
	//////////////////////////////////////////////////////////////////////////



	// Contact ���� ��ġ�� ����ϴ� �Լ�
	int ContactPosCalibaration(double dYmmOffset);
	// Contact ���� ��ġ�� ����ϴ� �Լ�
	int ContactPosCalibaration_FFC_Edge(int nCamNum, BOOL bBackY=FALSE);//  [6/10/2009 janghan]
	int ContactPosCalibaration_SortX(int nCamNum);//  [6/15/2009 janghan]
	// Line �׸��� �Լ�
	void DrawLine(int nBufferIndex);
	// Align â���� SearchPattern�� �� ����Ǵ� �Լ�. �ȿ��� SearchPattern�� ȣ���Ѵ�.
	void ManualSearchPattern(int nModelIndex);
	// Pattern�� ã�� �Լ�
	BOOL SearchPattern(int nModelIndex , BOOL bIsManual , int iRetryCnt = 0 );
	// Pattern�� �ҷ����� �Լ�
	void LoadPattern(int iPattern , STRUCT_MODEL_DATA *pModelData);
	// Pattern�� Center point�� �׸��� �Լ�
	void DrawCenLine(int iNo , int ixPos , int iyPos , bool bOverClear);
	// Pattern �̸����� ȭ���� �����ִ� �Լ�
	void LoadPatPreview(int iPtrPos);
	// Pattern�� �����ϴ� �Լ�
	void SavePattern(int nModelIndex, long nCondition);
	// ������ Pattern File name�� �����ϴ� �Լ�
	CString GeneratePatternFileName(int nModelIndex);
	// FormAlign���� Pattern ������ ���� �ش� Pattern ������ �׷��ִ� �Լ�
	void MoveRectToDefault(int nModelIndex);
	// FormAlign���� To Center ��ư ������ ����Ǵ� �Լ�
	void ResetRect(int nModelIndex);
	// Pattern�� ���� �����ϴ� �Լ�
	void PatternAllClear();
	// FPC Pattern�� �����ϴ� �Լ�
	void RemoveFPCPattern(int nBufferIndex);
	// �������̿� �׷��� ������ ����� �Լ�
	void OverlayClear(int nBufferNo);
	// ȭ�鿡 NONE��� �����ִ� �Լ�
	void GrabNotUseTextDisp(int nBufferNo);
	// Success, Error image�� �����ϴ� �Լ�
	void SaveSearchImage(BOOL bErrorImg, int nCamIndex, CString strPosition, double nScore);
	// Contact image�� �����ϴ� �Լ�
	void GrabContactImage();
	// 2��ȭó���ϴ� �Լ�
	void GrabBinarize(int nBufferIndex, long nCondition);

	void AlignLog(LPCTSTR szHPanelID, LPCTSTR szPosition, double dScore, CString strSuccess);

	// Pattern�� ã�� �Լ�
	BOOL SearchPattern2(int nModelIndex , BOOL bIsManual , int iRetryCnt = 0 );//  [6/9/2009 janghan]


	inline CMIL71* GetVision() { return &m_Vision; }
	CMIL71	m_Vision;

	// ��������Ʈ ��ġ. //
	CPoint m_ptCenter;

	double m_dScore;
	double m_dPCBScore;	// PCB ����

	// �簢�� ũ��. //
	CRect m_rtPattern;
	DWORD m_dwSearchTime;
	STRUCT_ALIGN_RESULT		m_ManualAlignResult;	// FormAlign���� Manual�� �ϴ� ���� ���� ����

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
