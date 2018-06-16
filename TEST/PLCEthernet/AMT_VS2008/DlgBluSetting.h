#pragma once
#include "lbl_blu.h"
#include "prev_port.h"
#include "iswitchledx.h"

enum
{
	param_Port,
	param_Baudrate,
	param_DataBit,
	param_StopBit,
	param_ParityBit,
	param_Max
};

enum 
{
	Log_None,
	Log_Send,
	Log_Receive
};

// CDlgBluSetting dialog

class CDlgBluSetting : public CDialog
{
	DECLARE_DYNAMIC(CDlgBluSetting)

public:
	CDlgBluSetting(CWnd* pParent = NULL);   // standard constructor
	virtual ~CDlgBluSetting();

// Dialog Data
	enum { IDD = IDD_DIALOG_BLUSETTING };

private:
	int m_nSelectedID;
	int m_nSelectedParam;
	int m_nCurParamIndex[id_Max][param_Max];
	int m_nSelectedModel;
	int m_nBluBrightness[BluModel_Max][32];
	int m_nBtmBrightness[BtmLight_ChMax];
	int m_nOmitBrightness;
	COLORREF ActiveBkColor;
	COLORREF NoActiveBkColor;
	COLORREF ActiveParamColor;
	COLORREF NoActiveParamColor;
	COLORREF ActiveBrightnessColor;
	COLORREF NoActiveBrightnessColor;

	void ColorSetting();
	void Active232ID(int nSelectedID, int nParam);
	void Active232Param(int nParam);
	void SelectModel(int nSelectedModel);
	void ShiftParam(int nSelectedID, int nParam, BOOL bNext);
	void ViewAllParam(int nSelectedID);
	void EditBluBrightness(int nBrightIndex);
	void EditBtmBrightness(int nBrightIndex);
	void LoadFromDoc();
	void ApplyToDoc();
	void UpdateBluBrightness(int nModel, BOOL bSaveToParam);
	void UpdateBtmBrightness(BOOL bSaveToParam);
	void UpdateOmitBrightness(BOOL bSaveToParam);
	void AddStatusLog(int nID, int nStatus, LPCTSTR szText);
	void SetBluBrightness(int nCh, BOOL bSave = TRUE);
	void SaveBluBrightness();
	void BluOn();
	void BluOff();
	void OmitOn();
	void OmitOff();
	void BtmOn(int nCh);
	void BtmOff();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CLbl_blu m_lbl232Param[id_Max][param_Max];
	CLbl_blu m_lblParamTitle[param_Max];
	CLbl_blu m_lblBluBackGround;
	CLbl_blu m_lblBtmBackGround;
	CLbl_blu m_lblOmitBackGround;
	CPrev_port m_PrevArrow;
	CPrev_port m_NextArrow;
	DECLARE_EVENTSINK_MAP()
	void OnClickLblBlu();
	void OnClickLblBluPort();
	void OnClickLblBluBaudrate();
	void OnClickLblBluDatabit();
	void OnClickLblBluStopbit();
	void OnClickLblBluParitybit();
	void OnClickLblOmit();
	void OnClickLblOmitPort();
	void OnClickLblOmitBaudrate();
	void OnClickLblOmitDatabit();
	void OnClickLblOmitStopbit();
	void OnClickLblOmitParitybit();
	void OnClickLblPort();
	void OnClickLblBaudrate();
	void OnClickLblDatabit();
	void OnClickLblStopbit();
	void OnClickLblParitybit();
	void OnClickArrowPrev();
	void OnClickArrowNext();
	void OnMouseDownArrowPrev(short Button, short Shift, long X, long Y);
	void OnMouseDownArrowNext(short Button, short Shift, long X, long Y);
	void OnMouseUpArrowPrev(short Button, short Shift, long X, long Y);
	void OnMouseUpArrowNext(short Button, short Shift, long X, long Y);
	CHistoryEdit m_historyStatus;
	afx_msg void OnBnClickedBtnApply();
	afx_msg void OnBnClickedBtnClose();
	CIswitchledxCtrl m_switch[BluModel_Max];
	afx_msg void OnBnClickedBtnClearHistory();
	CLbl_blu m_lblBluBright[32];
	void OnChangeUserSwhWt();
	void OnChangeUserSwhWs();
	void OnChangeUserSwhHd();
	void OnChangeUserSwhEtc();
	void OnClickLblBlu1();
	void OnClickLblBlu2();
	void OnClickLblBlu3();
	void OnClickLblBlu4();
	void OnClickLblBlu5();
	void OnClickLblBlu6();
	void OnClickLblBlu7();
	void OnClickLblBlu8();
	void OnClickLblBlu9();
	void OnClickLblBlu10();
	void OnClickLblBlu11();
	void OnClickLblBlu12();
	void OnClickLblBlu13();
	void OnClickLblBlu14();
	void OnClickLblBlu15();
	void OnClickLblBlu16();
	void OnClickLblBlu17();
	void OnClickLblBlu18();
	void OnClickLblBlu19();
	void OnClickLblBlu20();
	void OnClickLblBlu21();
	void OnClickLblBlu22();
	void OnClickLblBlu23();
	void OnClickLblBlu24();
	void OnClickLblBlu25();
	void OnClickLblBlu26();
	void OnClickLblBlu27();
	void OnClickLblBlu28();
	void OnClickLblBlu29();
	void OnClickLblBlu30();
	void OnClickLblBlu31();
	void OnClickLblBlu32();
	void OnClickLblOmitSetting();
	CLbl_blu m_lblOmitBright;
	void OnClickLblBtm();
	void OnClickLblBtmPort();
	void OnClickLblBtmBaudrate();
	void OnClickLblBtmDatabit();
	void OnClickLblBtmStopbit();
	void OnClickLblBtmParitybit();
	void OnClickLblBtmCh1Setting();
	CLbl_blu m_lblBtmBright[BtmLight_ChMax];
	void OnClickLblBtmCh2Setting();
	void OnClickLblBtmCh3Setting();
	void OnClickLblBtmCh4Setting();
	afx_msg void OnBnClickedBtnBtmOn();
	afx_msg void OnBnClickedBtnBtmOff();
	afx_msg void OnBnClickedBtnOpenPort();
	afx_msg void OnBnClickedBtnClosePort();
	afx_msg LRESULT OnCommRead( WPARAM wParam, LPARAM lParam );
	afx_msg void OnBnClickedBtnBluOn();
	afx_msg void OnBnClickedBtnBluOff();
	afx_msg void OnBnClickedBtnOmitOn();
	afx_msg void OnBnClickedBtnOmitOff();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
};