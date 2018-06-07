#pragma once

#include "GausGUI.h"

// #include "resource.h"

// CNumPadEx ��ȭ �����Դϴ�.

class _GAUS_GUI CGxNumPadEx : public CDialog
{
	DECLARE_DYNAMIC(CGxNumPadEx)

public:
	CGxNumPadEx(CWnd* pParent = NULL);   // ǥ�� �������Դϴ�.
	virtual ~CGxNumPadEx();

	// enum { IDD = IDD_DLG_NUMPAD2 };		

	enum {
		MAX_LENGTH		= 12,					// �Է°����� ���ڼ�
		LEN_DECIMAL		= 3,					// �⺻���� �Ҽ��� ���� �ڸ���		
	};				// �ִ� ���ڼ�

protected:

	CString		m_sTitle;				// ���̾�α� ����
	CString		m_sEdit;					// �������� ����
	BOOL			m_bIsInteger;			// ���� �Է� ���
	BOOL			m_bIsPositive;			// ���� ��ġ�� �Է� ���
	BOOL			m_bCheckLimit;			// �Է� ������ ���� �� ���ΰ� ?
	BOOL			m_bCheckEqual;			// ������ ������ �����Ͽ� �������� ������ �Է°� ��ȿ
	BOOL			m_bValueError;			// �� �Է¿� ������ �־���. ȭ�� ���� �ʱ�ȭ�� �̿�

	UINT			m_nDecimalLen;			// �Ҽ��� ����
	
	double		m_dbOld;					// ������
	double		m_dbMin;					// ���Ѱ�
	double		m_dbMax;					// ���Ѱ�
	// double		m_dbEditValue;			// ���� �Է����� ��

	void DisplayBtnMsg(UINT nID, CString sMsg);			// ���� �������� ���ڸ� ǥ�����ش�.
	void DisplayBtnValue(UINT nID, double dbValue, int nDecimal = LEN_DECIMAL);		// Button�� �� ǥ��
	void DisplayLabelMsg(UINT nID, CString sMsg);		// Static�� ���ڿ��� ǥ���Ѵ�.
	void DisplayLabelValue(UINT nID, double dbValue, int nDecimal = LEN_DECIMAL);		// Static�� �� ǥ��
	void DisplayLabelBlink(UINT nID, BOOL bFlag);		// Static�� Blink�� Ȱ��/��Ȱ�� �����Ѵ�.
	void DisplayLabelColor(UINT nID, COLORREF cColor);	// Static�� ���� �����Ѵ�.

	void ClearBlink();					// �Է¿� ������ �־ �������̴� ���� ����ȭ �����ش�.
	void UpdateEditValue();				// ���� �Է����� ���� ǥ�����ش�.

	// ���ڸ� �����Ѵ�.
	void InputNumKey(CString sKey);		// ���� Ű�� �Է��Ѵ�.

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV �����Դϴ�.

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	void OnAbort();									// ���� ����

	DECLARE_MESSAGE_MAP()


	DECLARE_EVENTSINK_MAP()
	// ���� �Է�
	void ClickBtnxNum0();
	void ClickBtnxNum1();
	void ClickBtnxNum2();
	void ClickBtnxNum3();
	void ClickBtnxNum4();
	void ClickBtnxNum5();
	void ClickBtnxNum6();
	void ClickBtnxNum7();
	void ClickBtnxNum8();
	void ClickBtnxNum9();

	// Ư�� ���
	void ClickBtnxMinus();
	void ClickBtnxDot();
	void ClickBtnxClear();
	void ClickBtnxCancel();
	void ClickBtnxEnter();
	void ClickBtnxNumpadDisplay();


public:

	// �ܺ� ���� ����
	void SetTitle(CString sMsg);						// ���̾�α� ǥ�ó���
	void SetIntegerMode(BOOL bFlag);					// ������ �Է��ϴ� ����ΰ� ?
	void SetPositiveMode(BOOL bFlag);				// ����� �Է��ϴ� ����ΰ� ?
	void SetDecimal(UINT nLen);							// �Ǽ����� ǥ���� �Ҽ����� ������ش�.
	void SetValue(double dbMin, double dbMax, double dOld, UINT nLen = LEN_DECIMAL, BOOL bCheck = TRUE);	// ������ ������ �������ش�.
	void SetValue(int dbMin, int dbMax, int dbOld, BOOL bCheck = TRUE);				// ������ ������ �������ش�.
	void SetMode(BOOL bPositive, BOOL bInteger);	// ����� �Է¿���, ������ �Է� ���� �ϰ� ����

	CString GetValue();									// �Էµ� ���ڿ� �����͸� ��������.
	void GetValue(CString* sValue);					// ���ڿ� �����͸� ��������.
	void GetValue(int* nValue);						// ������ �����͸� ��������.
	void GetValue(double* dbValue);					// �Ǽ��� �����͸� ��������.

};
