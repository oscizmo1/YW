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

	//kjpark 20170831 MCR ���� ä�� �� ���� �Ϸ�
	//virtual	void CreateSocket()=0;
	BOOL IsConnected();


	virtual	void PreparseMessage(CDataSocket* pSocket = NULL)=0;	// �����ڵ� ��ȯ �� StartChar ���� Ȯ���Ͽ� ParseMessage ȣ��

};

