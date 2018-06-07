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

// LSH[17/12/17] �⺻ ���� ����
struct ProductInfo
{
	UINT m_nHour;

	// Total data ���� ����
	UINT m_nTotalData[PRODUCT_COLUMN_MAX];

	// Product data ���� ����
	std::vector<UINT>m_vetProductData[PRODUCT_COLUMN_MAX][JIG_ID_MAX];

	ProductInfo()
	{
		// �ð� �� �ʱ�ȭ
		m_nHour = 0;

		Clear();
	}

	// �ʱ�ȭ - LSH171217
	void Clear()
	{
		for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
		{
			m_nTotalData[nColumn] = 0;

			// JIG�� ��ŭ �ݺ� - LSH171218
			for(int nJig=0 ; nJig<JIG_ID_MAX ; nJig++)
			{
				m_vetProductData[nColumn][nJig].clear();
				m_vetProductData[nColumn][nJig].resize(JIG_CH_MAX);
			}
		}
	}

	// Operator = ���� - LSH171220
	void operator = (const ProductInfo infoTemp)
	{
		// �ð� ���� - LSH171220
		m_nHour = infoTemp.m_nHour;

		// Column�� ��ŭ �ݺ��Ͽ� ��ü �迭�� ������ �ش� - LSH171220
		for(int nColumn=0 ; nColumn<PRODUCT_COLUMN_MAX ; nColumn++)
		{
			// Total �����͸� ���� - LSH171220
			m_nTotalData[nColumn] = infoTemp.m_nTotalData[nColumn];

			// �� Ch�� Data�� ���� - LSH171220
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
	// ������ ���� ���� - LSH171218
	map<short, ProductInfo>	m_MapData;

	// ���� �ð� ���� ���� - LSH171218
	CString m_strCurTime;
	UINT m_nCurHour;

public:
	// ������ �ʱ�ȭ �Լ� - LSH171217
	void InitData();

	// ��� ���� ���� �Ű������� �޾� ��ΰ� �ִ��� Ȯ�� �� ��� ������ �Ѵ� - LSH171218
	BOOL DirectoryCheck(BOOL bCreateDirectory = TRUE);

	// ���� ��¥ Ȯ�� - LSH171217
	void DateCheck();

	// ���� �� ��� ����
	CString CreateSavePath(CString strDate);

	// Store/Save - LSH171217
	/* Store */
	BOOL LoadReportFileToday();
	BOOL LoadReprotFileDate(CString strDate);
	BOOL LoadReportFile(CString strPath);
	/* Save */
	BOOL SaveReportFileToday();
	BOOL SaveReportFile(CString strPath);


	// ��� �Լ� - LSH171217
	/* �ۼ�Ʈ ��� */
	CString CalcPercent(int iEntire, int iValue);
	/* �ش� �ð��� ��*/
	void GetProductData(ProductInfo* infoProduct, UINT nTime);
	/* ���۽ð��� ����ð� ���̰� �� */
	void GetProductData(ProductInfo* infoProduct, UINT nStartTime, UINT nEndTime);
	// Column�� �̸��� ��ȯ�Ѵ�
	CString GetProductColumnName(PRODUCT_COLUMN clumCloumn);

	// ���� �Լ� - LSH171218
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
	// �ð� ���� ���� - LSH171218
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

