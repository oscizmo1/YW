// MTDoc.h : CMTDoc Ŭ������ �������̽�
//


#pragma once

#include "AmtData.h"
#include "EqMainSequence.h"
#include "CIF32.h"
#include "ThreadLiveGrap.h"
#include "PortMap.h"
#include "../Public class/CommThread.h"
#include "MelsecAddress.h"
#include "MelsecNet.h"


class CMTDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CMTDoc();
	DECLARE_DYNCREATE(CMTDoc)

public:

	CCommThread m_Comm[id_Max];

private:
	CMelsecNet		m_Melsec;
	//////////////////////////////////////////////////////////////////////////
private:
	CPortMap		m_PortMap;
	CPortMapDemo	m_PortMapDemo;
	CPortMapSESL	m_PortMapSESL;
public:
	inline CPortMap* GetPortMap() 
	{ 
		LineType type = GetLineType();
		if(type == A_GROUP_TYPE)
			return &m_PortMapDemo;
		else if(type == SINGLE_ARM_TYPE || type == REMOVE_TILTING_TYPE)
			return &m_PortMapSESL;
		else
			return &m_PortMap;	// �⺻��
	}
	//////////////////////////////////////////////////////////////////////////
private:
	CMelsecAddress	m_MelsecAddress;
	CAdrAMMT1		m_adrAMMT1;
	CAdrAMMT2		m_adrAMMT2;
	CAdrBMMT1		m_adrBMMT1;
	CAdrBMMT2		m_adrBMMT2;
public:
	inline CMelsecAddress* GetMelsecAddress() 
	{
		MachinePCType pcType = GetMachinePCType();
		// Melsec Address ����
		if(pcType == A_AMT)
			return &m_MelsecAddress;
		else if(pcType == A_MMT1)
			return &m_adrAMMT1;
		else if(pcType == A_MMT2)
			return &m_adrAMMT2;
		else if(pcType == B_AMT)
			return &m_MelsecAddress;
		else if(pcType == B_MMT1)
			return &m_adrBMMT1;
		else if(pcType == B_MMT2)
			return &m_adrBMMT2;
		else
			return &m_MelsecAddress;	// �⺻��
	}
	//////////////////////////////////////////////////////////////////////////

// Ư��
public:
	CString		m_strDayLogDirName;
	BOOL		m_bDeviceChange;

	STRUCT_MOTOR_PARA*		m_pMotorParam;
	CAM_INFO				m_CamInfo[MAX_CAM_NUM];

	CEqMainSequence*		m_pEqMainSeq;
	CCIF32*					m_pDeviceNet;
	CMMC39*					m_pMotion;

	CAmtData*				m_pAmtData;

	CThreadLiveGrap*		m_pThreadLiveGrap;
	CWinThread*				m_pLiveThread;

	CStdioFile					m_fileErrorCode;
	CMapStringToString	m_mapErrorCode;

	BOOL			m_bLiveGrapFlag[MAX_CAM_NUM];

	STRUCT_LAST_GLASSDATA m_LastGlass;
	void LoadLastGlassFile();
	void SaveLastGlassFile();

	//////////////////////////////////////////////////////////////////////////
	// Panel Model���� ������ �ʿ� ���� �͵�

	double		m_dOffsetX;
	double		m_dMotionstrokeSingleX;
	double		m_dMotionstrokeSingleY;
	double		m_dMotionstrokeFullX;

	int			m_nCleanSet;		// Clean Check Period
	int			m_iConatctUniType;
	int			m_iCurInitStep;

	int			m_nInsp_Count;				// Inspection Count ���� [12/2/2009 JJH-RomanticBoy]
	
	int			m_nInspection;			// Inspection Count ���� [12/2/2009 JJH-RomanticBoy]
	BOOL		m_bInspCount;			// Inspection Count ��뿩�� [12/2/2009 JJH-RomanticBoy]
	// Tilting Pos
	double	m_dTiltingPos;
	double	m_dUnTiltingPos;


	// User ������ User ���� ���� 
	BOOL			m_bEngLogin;
	BOOL			m_bSuperLogin;
	void			SetSuperUser(BOOL bSet);

	int			m_nUserClass;
	int			m_nUserName;
	int			m_nPassWord;

	CString		m_strUserClass;
	CString		m_strUserName;
	CString		m_strPassWord;

	// Image Save
	BOOL m_bContactImgSave;	// X ���� �Ϸ� �� ����
	BOOL m_bSuccessImgSave;	// PCB, FPC ��� Search ����
	BOOL m_bErrImgSave;		// �ϳ��� ã�� ������ ��

	// Binarize Condition value
	BOOL m_bUseBinary;
	long m_nBinarizeFPC[MAX_CAM_NUM];//
	long m_nBinarizeFPC_Mark[MAX_CAM_NUM]; //  [6/9/2009 janghan]
	long m_nBinarizePCB[MAX_CAM_NUM];
	long m_nBinarizePCB_Mark[MAX_CAM_NUM]; //  [6/9/2009 janghan]
	void SaveBinarizeCondition();
	void SaveBinarizeLevelRange();//  [6/28/2009 janghan]
	void SaveInspectionCount();//  [9/10/2009 janghan]

	// PLC Ethernet Parameter
	BOOL m_bPlcEthernetType;
	CString m_strHostIP;
	int m_nCpuType;
	int m_nNetworkNo;
	int m_nHostStationNo;
	int m_nPcStationNo;

	//  [6/9/2009 janghan] Auto Tune ����///////////////////////////////////////////////////////////////
	int m_nLevelCount;    //2��ȭ �ܰ� ��ȭ
	int m_nTuneLevl;      //ƪ �Ϸᰪ
	double m_fMaxScore;   //�ְ� ��Ī��
	double m_fEdgeFind_X; //FFC Edge X
	double m_fEdgeFind_Y; //FFC Edge Y
	double m_fMarkFind_X; //FFC Mark X
	double m_fPCBmarkFind_X; //PCB Mark X
	BOOL m_bAutoTune_RUN; //ƪ ���� Ȯ�ο�
	BOOL m_bAutoTune[4];  //ƪ ���� ī�޶� Ȯ��
	int m_nMotor_Move_CHK; //�̵� Ȯ��
	int m_nYaxis_Back;     // ��ħȮ���� Y�� ����;
	int m_nCamera_Auto;    // Auto_Tune �۵� ī�޶�
	double m_fAutoDistance_X;//auto Tune ���� �Ÿ�;
	double m_fAutoDistance_Y;//auto Tune ���� �Ÿ�;
	double m_fAutoContactPos_X; //
	double m_fAutoContactPos_Y;
	CString m_strAutoTune_Reason;// AutoTune ���� ����
	//  [6/28/2009 janghan]
	int m_nFFCEdge_Min;  //Tact Time ���̱� ���� Tune Rage�� ���� �ϱ� ���� ������
	int m_nFFCEdge_Max;
	int m_nFFCMark_Min;
	int m_nFFCMark_Max;
	int m_nPCBEdge_Min;
	int m_nPCBEdge_Max;
	int m_nPCBMark_Min;
	int m_nPCBMark_Max;
	int m_nLevelStep;

	DWORD m_nOldTime_Tune; //Auto Tune �ϴµ� �ɸ� �ð� üũ
	DWORD m_nNewTime_Tune;

	BOOL m_bUsed_Tune;//  [7/6/2009 janghan] Auto Tune �۵��߾����� Check �ؼ� ����ȭ �� ��� ���� �� ����
	BOOL m_bAutoTune_Using; //  [7/6/2009 janghan] Auto Tune ��� On/Off Check

	//double m_fPCBMark_Fix_X[4]; // PCB�� �Ź� ã�� �ʿ� ������ ���Ƽ� �ѹ��� ã�� FIX��  [7/3/2009 janghan] - ���� ��
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	long m_nInspectionCount;// SESL ��û���� Zone�� �˻� ī��Ʈ [9/10/2009 janghan]
	WORD m_nHourCheck; //SESL �˻� ī��Ʈ Reset ����  [9/10/2009 janghan] 


	//2009.05.31 Non-stop.
	CString m_NonStopDeviceID;		// PLC�κ��� ���� Device ID
	BOOL m_bUseAMC;					// Non Stop Model Change ��� / ��� ����
	BOOL m_bUseAutoCreate;			// 
	typedef struct sDevice				//yongtae 20081201 
	{
		CString strDeviceName;
		CString strModelName;
		int		iPanelSize;
	}STRUCT_DEVICE;
	CArray<STRUCT_DEVICE,STRUCT_DEVICE> m_AryDevice;
	BOOL m_bPCBChangeComp;
	BOOL m_bModelMatchingComp;
	CString m_CurDeviceID;

	BOOL m_bNonStop_Run;// NonStop ���� ���� Check [8/17/2009 janghan]
	//////////////////////////////////////////////////////////////////////////


	LPCTSTR m_szPortNo[Index_Port_Max];	
	LPCTSTR m_szBaudRate[Index_Baud_Max];	
	LPCTSTR m_szDataBit[Index_DataBit_Max];	
	LPCTSTR m_szStopBit[Index_StopBit_Max];	
	LPCTSTR m_szParityBit[Index_ParityBit_Max];	

	DWORD m_BaudRate[Index_Baud_Max];	
	BYTE m_DataBit[Index_DataBit_Max];		
	BYTE m_StopBit[Index_StopBit_Max];		
	BYTE m_ParityBit[Index_ParityBit_Max];	

	LPCTSTR m_szPanelIDOld;
	//////////////////////////////////////////////////////////////////////////
	// Contact On/Off Delay [12/20/2010 OSC]
	DWORD m_dwContactDelay;
	void SaveContiactDelay();
	//////////////////////////////////////////////////////////////////////////

// �۾�
public:
	void	SetLog(CString strLogMsg, int iMsgID);
// ������
	public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// ����
public:
	virtual ~CMTDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// �޽��� �� �Լ��� �����߽��ϴ�.
protected:
	DECLARE_MESSAGE_MAP()
public:
	CString GetDataFromString(CString inStr, CString startStr, CString endStr);
	void OpenModelFile(CString strModelName);
	void OpenBinarizeModelFile();
	void SaveModelFile(void);
	void SaveBinarizeModelFile(void);
	void SaveCurrentModelName(void);
	void SetMotorParameter(void);
	void ApplyCamParamToVision(void);
	BOOL SetErrorCode(void);
	void LoadConfigFile();
	void SaveConfigFile();

	//////////////////////////////////////////////////////////////////////////
	// ����	���� �־� ���� ���� �켱 Doc�� ����
	CSerialSettingData	m_SerialData[id_Max];
	int m_nBluBrightness[BluModel_Max][32];
	int m_nBtmLightBrightness[BtmLight_ChMax];
	int m_nOmitBrightness;

	BOOL m_bUseOmitIO;			// Omit ������ IO�� �����ϸ� TURE

	BOOL m_bLightOn[id_Max];	// ���� On�������� Off�������� �˱� ����

	void SaveSerialData();
	void SetSerialArray();
	void ReadBrightness();
	void SaveBrightness();
	BOOL SerialPortOpen(int nID);
	void SerialPortClose(int nID);
	void BluOn();
	void BluOff();
	void OmitOn();
	void OmitOff();
	void BtmOn();
	void BtmOff();
	int nMakeAsciiData(int nID, CString strMsg);
	//////////////////////////////////////////////////////////////////////////

	void RunThread(void);
	void Initial(void);
	void GrapAliveFlagAllStop(void);
	virtual void OnCloseDocument();
	void SaveMotorParameter(CString strAxis, int nAxisNum, BOOL bInitParam = FALSE);
	void GetMotionParameter(int nAxisNum);
	void SaveTiltPos(void);
	void ReadTiltPos(void);
	void SaveTowerLamp(void);
	CString strTowerLamp[STATUS_ALL];
	void GetTowerLamp(void);
	void SaveAutoModelChangeOption();
	void LoadAutoModelChangeOption();
	void FileOpenModelAtDevice();
	void FileSaveModelAtDevice();
	void AddModelAtDevice(CString strDeviceID, CString strModelName);
	void SavePCBChangePos();
	BOOL CheckLimit( int nAxis, double dPos );
	BOOL SetAlignPos(int nCamNum);
};
