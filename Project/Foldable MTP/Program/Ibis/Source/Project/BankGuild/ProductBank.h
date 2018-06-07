#pragma once

#include <vector>

using namespace std;



enum PRODUCT_COLUMN
{
	PRODUCT_COLUMN_PRODUCT,
	PRODUCT_COLUMN_GOOD,
	PRODUCT_COLUMN_REJECT,
	PRODUCT_COLUMN_RETRY,
	PRODUCT_COLUMN_MCRREADING,
	PRODUCT_COLUMN_FIRSTCONTACT,
	PRODUCT_COLUMN_FINALCONTACT,
	PRODUCT_COLUMN_MAX
};

struct tElement
{
	int	iValue;
	UINT	uID;
};

struct tElement_f
{
	float	fValue;
	UINT	uID;
};

// LSH[17/12/17] 기본 구조 생성
struct ProductInfo
{
	UINT m_nHour;

	// Total data 저장 변수
	UINT m_nTotalData[PRODUCT_COLUMN_MAX];

	// Product data 저장 변수
	std::vector<UINT>m_vetProductData[PRODUCT_COLUMN_MAX][JIG_ID_MAX];

	ProductInfo()
	{
		// 시간 값 초기화
		m_nHour = 0;

		Clear();
	}

	// 초기화 - LSH171217
	void Clear()
	{
		for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
		{
			m_nTotalData[nColumn] = 0;

			// JIG수 만큼 반복 - LSH171218
			for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
			{
				m_vetProductData[nColumn][nJig].clear();
				m_vetProductData[nColumn][nJig].resize(JIG_CH_MAX);
			}
		}
	}

	// Operator = 구현 - LSH171220
	void operator = (const ProductInfo infoTemp)
	{
		// 시간 복사 - LSH171220
		m_nHour = infoTemp.m_nHour;

		// Column수 만큼 반복하여 전체 배열을 복사해 준다 - LSH171220
		for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
		{
			// Total 데이터를 복사 - LSH171220
			m_nTotalData[nColumn] = infoTemp.m_nTotalData[nColumn];

			// 각 Ch별 Data를 복사 - LSH171220
			for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
				for(int nCh=0 ; nCh<JIG_CH_MAX ; nCh++)
					m_vetProductData[nColumn][nJig][nCh] = infoTemp.m_vetProductData[nColumn][nJig][nCh];
		}
	}
};

class CProductBank
{
public:
	CProductBank(void);
	~CProductBank(void);


protected:
	// 데이터 저장 변수 - LSH171218
	map<short, ProductInfo>	m_MapData;

	// 현재 시간 저장 변수 - LSH171218
	CString m_strCurTime;
	UINT m_nCurHour;

public:
	// 데이터 초기화 함수 - LSH171217
	void InitData();

	// 경로 생성 여부 매개변수를 받아 경로가 있는지 확인 및 경로 생성을 한다 - LSH171218
	BOOL DirectoryCheck(BOOL bCreateDirectory = TRUE);

	// 현재 날짜 확인 - LSH171217
	void DateCheck();

	// 저장 할 경로 생성
	CString CreateSavePath(CString strDate);

	// Store/Save - LSH171217
	/* Store */
	BOOL LoadReportFileToday();
	BOOL LoadReprotFileDate(CString strDate);
	BOOL LoadReportFile(CString strPath);
	/* Save */
	BOOL SaveReportFileToday();
	BOOL SaveReportFile(CString strPath);


	// 계산 함수 - LSH171217
	/* 퍼센트 계산 */
	CString CalcPercent(int iEntire, int iValue);
	/* 해당 시간의 값*/
	void GetProductData(ProductInfo* infoProduct, UINT nTime);
	/* 시작시간과 종료시간 사이값 값 */
	void GetProductData(ProductInfo* infoProduct, UINT nStartTime, UINT nEndTime);
	// Column의 이름을 반환한다
	CString GetProductColumnName(PRODUCT_COLUMN clumCloumn);

	// 증가 함수 - LSH171218
	void IncreaseProductCount(int nJigID, int nJigCh);
	void IncreaseBinZoreCount(int nJigID, int nJigCh);
	void IncreaseBinOneCount(int nJigID, int nJigCh);
	void IncreaseBinTwoCount(int nJigID, int nJigCh);
	void IncreaseMcrOkCount(int nJigID, int nJigCh);
	void IncreaseAlignOkCount(int nJigID, int nJigCh);
	void IncreaseFirstContactCount(int nJigID, int nJigCh);
	void IncreaseFinalContactCount(int nJigID, int nJigCh);

public:
	//////////////////////////////////////////////////////////////////////////
	// 시간 저장 변수 - LSH171218
	CTime m_timeMainCount;

	// Main UI Count [12/14/2016 OSC]
	tElement m_MainCountGOOD[JIG_ID_MAX][JIG_CH_MAX];
	tElement m_MainCountNG[JIG_ID_MAX][JIG_CH_MAX];
	tElement m_MainCountMCR[JIG_ID_MAX][JIG_CH_MAX];
	tElement m_MainCountCONTACT[JIG_ID_MAX][JIG_CH_MAX];

	void MainCount_Init();
	void MainCount_Update(CCellInfo *pCell);
	void MainCount_Save();
	void MainCount_Load();
	//////////////////////////////////////////////////////////////////////////
};

