#pragma once
class CDeviceSocket_Base
{
public:
	CDeviceSocket_Base(void);
	~CDeviceSocket_Base(void);

protected:
	HWND m_hParent;
public:
	void SetParentHWnd(HWND hWnd);

	CDataSocket m_Socket;

	//kjpark 20170831 MCR 구현 채널 별 구현 완료
	//virtual	void CreateSocket()=0;
	BOOL IsConnected();


	virtual	void PreparseMessage(CDataSocket* pSocket = NULL)=0;	// 유니코드 변환 및 StartChar 유무 확인하여 ParseMessage 호출

};

