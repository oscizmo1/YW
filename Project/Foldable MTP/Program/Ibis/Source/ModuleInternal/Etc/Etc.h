//////////////////////////////////////////////////////////////////////////
// �̰� ���� ����� �Լ� ��Ƴ��� class
//
// static void CircleCenter() �Լ� �߰� [2/24/2016 OSC]
// 
// static LPCTSTR GetLocalIpAddress() �Լ� �߰� [4/9/2013 OSC]
//////////////////////////////////////////////////////////////////////////

#pragma once

#include "dblPoint.h"

static HANDLE hSleepTimeEvent = NULL;

class CEtc
{
public:
	CEtc(void);
	~CEtc(void);

	//////////////////////////////////////////////////////////////////////////
	// Ư�� ��Ʈ�� ���� Capture �Լ�(Screen Capture ����)
	// ���� Ȯ��� ���� �ڵ����� File Format �����
	// *.bmp(BMP), *.png(PNG), *.jpg(JPEG), *.gif(GIF)
	// ex) CEtc::CaptureScreen(this.m_hWnd, _T("D:\\a.jpg"));
	static void CaptureScreen(HWND hScreen, LPCTSTR szFileName );
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �ݿø� �Լ�
	// �Ҽ��� nPos° �ڸ��� �ݿø� �Ѵ�
	//#define round(dTemp) (long(dTemp+(dTemp>0? .5:-.5)))
	static double Round(double dValue, int nPos = 1);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// �α��Լ�	
	// base�� ����� �� dValue�� �Ǵ��� ���� �˾ƿ��� �Լ�
	static double Logbase(double dValue, double base);
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Sleep() ��� �Լ�
	// ������
	// CreateSleepTimeEvent();�� �̺�Ʈ ������
	// ���� Sleep()�� �� ���� SleepTime()�� ��� ���
	// ���� ClCloseSleepTimeEvent()�� �̺�Ʈ�� �����Ѵ�.
	static void CreateSleepTimeEvent();
	static void SleepTime(DWORD dwMillisecond);
	static void CloseSleepTimeEvent();
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Encoder Position Check�� �Լ�
	static BOOL CheckPosition(double dDefaultPos, double dNowPos, double dCheckLimit = 0.05); /* ��0.01mm�̻� ���̳��� Ʋ���ٰ� ���� */
	//////////////////////////////////////////////////////////////////////////

	//////////////////////////////////////////////////////////////////////////
	// Local IP Address ������ �Լ�
	static LPCTSTR GetLocalIPAddress();
	//////////////////////////////////////////////////////////////////////////

	static LPCTSTR GetDeviceIPAddress(LPCTSTR szDeviceName);
	static HRESULT GetConnectionIPAddress(LPCTSTR szConnectionName);

	//////////////////////////////////////////////////////////////////////////
	// Unicode, Multibyte ��ȯ
	static void ConvertUnicode( char* szFrom, wchar_t* wszTo, int nToMaxLength );
	static void ConvertMultibyte( wchar_t* wszFrom, char* szTo, int nToMaxLength );
	// Ȥ�� �������� ó�� ���ϴ°� �ִٸ� �Ʒ����� �غ�����
	// wchar Length�� whcar�������� ������ ���̾�� ��. �Ƹ���...
	static void ConvertUnicode( char* szFrom, int nFromLength, wchar_t* wszTo, int nToMaxLength );
	static void ConvertMultibyte( wchar_t* wszFrom, int nFromLength, char* szTo, int nToMaxLength );
	//kjpark 20161110 ETC �����ڵ� ���� �߰�
// 	static CHAR* CStringToChar(CString str);
// 	static CString CharToCString(char* str, BOOL bDelete);
	//////////////////////////////////////////////////////////////////////////

	// �Ϸ縦 07�� �������� ���ų� �� �� ����ϴ� �Լ� [3/15/2014 OSC]
	static void ApplyTimeOffset( CTime &time, int nHour, int nMin );

	//////////////////////////////////////////////////////////////////////////
	// 3�� Point�� �߽��� ���ϴ� �Լ� [2/24/2016 OSC]
	static void CircleCenter(CdblPoint A, CdblPoint B, CdblPoint C, CdblPoint& ptCenter);

};

