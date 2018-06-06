//////////////////////////////////////////////////////////////////////////
// �̰� ���� ����� �Լ� ��Ƴ��� class
// ���� ���� [4/5/2009 OSC]
// static LPCTSTR GetLocalIpAddress() �Լ� �߰�
//////////////////////////////////////////////////////////////////////////

#pragma once

static HANDLE hSleepTimeEvent = NULL;

class CEtc
{
public:
	CEtc(void);
	~CEtc(void);

	//////////////////////////////////////////////////////////////////////////
	// Ư�� ��Ʈ�� ���� Capture �Լ�(Screen Capture ����)
	// ���� Ȯ����� ���� �ڵ����� File Format �����
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
};