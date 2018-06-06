#pragma once

//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
struct MTP_VALUE
{
	CString strMTPInspection_L;
	CString strMTPInspection_x;	
	CString strMTPInspection_y;
	CString strMTPInspection_u;
	CString strMTPInspection_v;
	CString strMTPInspection_Tcolor;
	CString strMTPInspection_Mpcd;
	CString strMTPInspection_VBAT;
	CString strMTPInspection_IBAT;
	CString strMTPInspection_VEXT;
	CString strMTPInspection_IEXT;
	CString strMTPInspection_VDD;
	CString strMTPInspection_IDD;
	CString strMTPInspection_VCI;
	CString strMTPInspection_ICI;
	CString strMTPInspection_VBAT2;
	CString strMTPInspection_IBAT2;
	CString strMTPInspection_VDD2;
	CString strMTPInspection_IDD2;
	CString strMTPInspection_R;
	CString strMTPInspection_G;
	CString strMTPInspection_B;
	CString strMTPInspection_ID1;
	CString strMTPInspection_ID2;
	CString strMTPInspection_ID3;
	CString strMTPInspection_GAMMA;
	CString strMTPInspection_Ptime;
	CString strMTPInspection_Ttime;
	CString strMTPInspection_JUDGE;
};

class  PG_LOG_DATA_MTP
{
public:
	CString strKeyword;
	CString m_L;		// 7열(G열) : "S_MID_255_PRE_"의 2번째 열 값
	CString m_X;		// 7열(G열) : "S_MID_255_PRE_"의 3번째 열 값
	CString m_Y;		// 7열(G열) : "S_MID_255_PRE_"의 4번째 열 값
	CString m_TCOLOR;	// 7열(G열) : "S_MID_255_PRE_"의 7번째 열 값
	CString m_MPCD;		// 7열(G열) : "S_MID_255_PRE_"의 8번째 열 값
	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	CString m_IBAT;		// 7열(G열) : "MID_ALPM_255"의 10번째 열 값

	PG_LOG_DATA_MTP ()
	{
		strKeyword.Empty();
		Init();
	}
	~PG_LOG_DATA_MTP() {}

	void Init();
};

class CProcDataPG
{
public:
	CProcDataPG(void);
	~CProcDataPG(void);

private:
	JIG_ID m_Jig;
	JIG_CH m_Ch;

	// \\192.168.10.140\Result\[yyyy]-[mm]-[dd]\[Channel]\MTP\[CellID or InnerID]_[yyyymmdd]_[HHMMSS].csv
	CString m_CORRELATION;				// 7열(G열) : "BG7000_CAL_DATE"의 29번째 열 값
	CString m_BEF_255_MTP;				// 7열(G열) : "BEF_255"의 2번째 열 값

	// S_MID_255
	// 아래 4개 값 중에 1개만 쓴다
	// 우선순위는 위에부터 1,2,3,4... 
	// 1번이 못읽으면 2번 값을 적고, 2번도 없으면 3번값을 적고 그런 식.... SDC 정효상프로
	PG_LOG_DATA_MTP m_S_MID_255_PRE_;
	PG_LOG_DATA_MTP m_S_MID_255_;
	PG_LOG_DATA_MTP m_MID_255_PRE;
	PG_LOG_DATA_MTP m_MID_255;

	// 300CD_V255
	// 아래 2개 값 중에 1개만 쓴다
	// 위에껄 먼저 적용하고 위에께 없으면 아래꺼로 적용한다
	PG_LOG_DATA_MTP m_300CD_V255_PRE;
	PG_LOG_DATA_MTP m_300CD_V255;

	// 우선순위에 의해서 채택된 변수
	PG_LOG_DATA_MTP *m_pS_MID_255;
	PG_LOG_DATA_MTP *m_p300CD_V255;

	//kjpark 20180129 MID ALPM 255 IBAT 항목추가
	CString m_MID_ALPM_255;
	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	map<CString, MTP_VALUE> iMapMTPInspectionValue;
public:
	void Init(JIG_ID jig, JIG_CH ch);
	// Log를 읽어서 변수에 넣는다
	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	BOOL ScanLog(CString strCellID, CString strInnerID, SYSTEMTIME time);
	// S_MID_255하고 300CD_255를 우선순위 판단하여 각각 하나의 값을 체택하여 포인터에 넣는다
	BOOL SelectData();
	// 변수값을 CellInfo에 넣는다
	void SetValueToCellInfo(CCellInfo *pCell);

	//kjpark 20180107 신호기 로그에서 MTP 측정 값 가지고와서 셀로그에 넣기
	BOOL GetMTPInspectFileSearch(CString strPathCellID, CString strPathInnerID);
	BOOL SetAddMTPValue(CString strFilename);
	void SetDataMTPValue();	
};

