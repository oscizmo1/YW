#pragma once

//kjpark 20180107 ��ȣ�� �α׿��� MTP ���� �� ������ͼ� ���α׿� �ֱ�
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
	CString m_L;		// 7��(G��) : "S_MID_255_PRE_"�� 2��° �� ��
	CString m_X;		// 7��(G��) : "S_MID_255_PRE_"�� 3��° �� ��
	CString m_Y;		// 7��(G��) : "S_MID_255_PRE_"�� 4��° �� ��
	CString m_TCOLOR;	// 7��(G��) : "S_MID_255_PRE_"�� 7��° �� ��
	CString m_MPCD;		// 7��(G��) : "S_MID_255_PRE_"�� 8��° �� ��
	//kjpark 20180129 MID ALPM 255 IBAT �׸��߰�
	CString m_IBAT;		// 7��(G��) : "MID_ALPM_255"�� 10��° �� ��

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
	CString m_CORRELATION;				// 7��(G��) : "BG7000_CAL_DATE"�� 29��° �� ��
	CString m_BEF_255_MTP;				// 7��(G��) : "BEF_255"�� 2��° �� ��

	// S_MID_255
	// �Ʒ� 4�� �� �߿� 1���� ����
	// �켱������ �������� 1,2,3,4... 
	// 1���� �������� 2�� ���� ����, 2���� ������ 3������ ���� �׷� ��.... SDC ��ȿ������
	PG_LOG_DATA_MTP m_S_MID_255_PRE_;
	PG_LOG_DATA_MTP m_S_MID_255_;
	PG_LOG_DATA_MTP m_MID_255_PRE;
	PG_LOG_DATA_MTP m_MID_255;

	// 300CD_V255
	// �Ʒ� 2�� �� �߿� 1���� ����
	// ������ ���� �����ϰ� ������ ������ �Ʒ����� �����Ѵ�
	PG_LOG_DATA_MTP m_300CD_V255_PRE;
	PG_LOG_DATA_MTP m_300CD_V255;

	// �켱������ ���ؼ� ä�õ� ����
	PG_LOG_DATA_MTP *m_pS_MID_255;
	PG_LOG_DATA_MTP *m_p300CD_V255;

	//kjpark 20180129 MID ALPM 255 IBAT �׸��߰�
	CString m_MID_ALPM_255;
	//kjpark 20180107 ��ȣ�� �α׿��� MTP ���� �� ������ͼ� ���α׿� �ֱ�
	map<CString, MTP_VALUE> iMapMTPInspectionValue;
public:
	void Init(JIG_ID jig, JIG_CH ch);
	// Log�� �о ������ �ִ´�
	//kjpark 20180107 ��ȣ�� �α׿��� MTP ���� �� ������ͼ� ���α׿� �ֱ�
	BOOL ScanLog(CString strCellID, CString strInnerID, SYSTEMTIME time);
	// S_MID_255�ϰ� 300CD_255�� �켱���� �Ǵ��Ͽ� ���� �ϳ��� ���� ü���Ͽ� �����Ϳ� �ִ´�
	BOOL SelectData();
	// �������� CellInfo�� �ִ´�
	void SetValueToCellInfo(CCellInfo *pCell);

	//kjpark 20180107 ��ȣ�� �α׿��� MTP ���� �� ������ͼ� ���α׿� �ֱ�
	BOOL GetMTPInspectFileSearch(CString strPathCellID, CString strPathInnerID);
	BOOL SetAddMTPValue(CString strFilename);
	void SetDataMTPValue();	
};

