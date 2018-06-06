#pragma once

class CCellTactTime
{
public:
	CCellTactTime(void);
	~CCellTactTime(void);

public:
	SYSTEMTIME m_timeStart;	// ���۽ð�
	SYSTEMTIME m_timeEnd;	// ����ð�

	void SetTimeStart(SYSTEMTIME time = SYSTEMTIME());
	void SetTimeEnd(SYSTEMTIME time = SYSTEMTIME());

	void	Init();
};


class CCellDefectInfo : public CCellTactTime
{
public:
	CCellDefectInfo(void);
	~CCellDefectInfo(void);

public:
// 	INSP_STATE m_State;	
	//kjpark 20170907 CellLog Title �� �ش� �׸� �߰�
	CString m_strCommand;
	CString m_strWorkZone;
	CLASS_CELL m_Class;		// ���� ���
	CString m_DefectName;	// �ҷ���
	CString m_strMTPType;
	CONST_JUDGE_NAME::ID m_Judge;

	void	Init();
	void    FindWorkZone();

	BOOL IsStarted();
	BOOL IsEnded();

};

enum DEFECTINFO_CURRENT
{
	IBAT = 0,
	IVSS,
	IVDD,
	IVCI,
	IBAT2,
	IDD2,
	DEFECTINFO_CURRENT_MAX
};

class CCellDefectInfo_Current : public CCellDefectInfo
{
public:
	CCellDefectInfo_Current(void);
	~CCellDefectInfo_Current(void);

	CString	m_CurrentText[DEFECTINFO_CURRENT_MAX];	
	double	m_CurrentValue[DEFECTINFO_CURRENT_MAX];

	void Text_to_Value();	// ���ڿ� ���� double ������ �ִ´�

	void	Init();
};

class CCellDefectInfo_CellLoading : public CCellDefectInfo_Current
{
public:
	CCellDefectInfo_CellLoading(void);
	~CCellDefectInfo_CellLoading(void);

	BOOL m_CellLoadingSend;
	int m_ContactCount;
	CLASS_CELL m_FirstClass;	// ù �õ��� ���

	void	Init();
};

class CCellDefectInfo_Zone : public CCellDefectInfo
{
public:
	CCellDefectInfo_Zone(void);
	~CCellDefectInfo_Zone(void);

	BOOL m_RecvSetZone;
	SYSTEMTIME m_timeWait;	// Wait�ð�

	void SetTimeWait();

	void	Init();
};

#define MAX_TSP_RESULT	12
class CCellDefectInfo_Tsp : public CCellDefectInfo
{
public:
	CCellDefectInfo_Tsp(void);
	~CCellDefectInfo_Tsp(void);

	CString		TspTitle[MAX_TSP_RESULT];
	CString		TspName[MAX_TSP_RESULT];

	void	Init();
};
