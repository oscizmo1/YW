#include "StdAfx.h"
#include "Etc.h"
#include <math.h>

// GetAdaptersInfo()
#include <ipHlpapi.h>	
#pragma comment(lib, "ipHlpapi.lib")

// GetIPAddress()
#include <NetCon.h>	

CEtc::CEtc(void)
{
}

CEtc::~CEtc(void)
{
}

void CEtc::CaptureScreen( HWND hScreen, LPCTSTR szFileName )
{
	/*//////////////////////////////////////////////////////////////////////////
	// 원본 소스 
	int nScreenWidth = GetSystemMetrics(SM_CXSCREEN);
	int nScreenHeight = GetSystemMetrics(SM_CYSCREEN);
	HWND hDesktopWnd = GetDesktopWindow();
	HDC hDesktopDC = GetDC(hDesktopWnd);
	HDC hCaptureDC = CreateCompatibleDC(hDesktopDC);
	HBITMAP hCaptureBitmap =CreateCompatibleBitmap(hDesktopDC, 
		nScreenWidth, nScreenHeight);
	SelectObject(hCaptureDC,hCaptureBitmap); 
	BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,
		hDesktopDC,0,0,SRCCOPY|CAPTUREBLT); 
	SaveCapturedBitmap(hCaptureBitmap); //Place holder - Put your code
	//here to save the captured image to disk
	ReleaseDC(hDesktopWnd,hDesktopDC);
	DeleteDC(hCaptureDC);
	DeleteObject(hCaptureBitmap);
	*///////////////////////////////////////////////////////////////////////////

	CRect rect;
	::GetWindowRect(hScreen, &rect);
	int nScreenWidth = rect.Width();
	int nScreenHeight = rect.Height();

	HDC hScreenDC = ::GetDC(hScreen);
	HDC hCaptureDC = ::CreateCompatibleDC(hScreenDC) ;
	HBITMAP hCaptureBitmap = ::CreateCompatibleBitmap(hScreenDC, nScreenWidth, nScreenHeight);
	::SelectObject(hCaptureDC,hCaptureBitmap); 
	::BitBlt(hCaptureDC,0,0,nScreenWidth,nScreenHeight,	hScreenDC,0,0,SRCCOPY); 

	CImage image;
	image.Attach(hCaptureBitmap);
	image.Save(szFileName);
	::ReleaseDC(hScreen,hScreenDC);
	::DeleteDC(hCaptureDC);
	::DeleteObject(hCaptureBitmap);
}

double CEtc::Round( double dValue, int nPos /*= 1*/ )
{ 
	// 소수점 nPos째 자리를 반올림 한다
	// Round(12.15923, 3)		return value = 12.16
	dValue*=pow(10.0, nPos-1); 
	dValue=(dValue>0) ?  floor(dValue+0.5) : ceil(dValue-0.5); 
	dValue*=pow(10.0, -(nPos-1)); 
	return dValue; 
	// 소숫점을 버리는 반올림 소스
	// 	return ((dValue>0) ? floor(dValue+.5) : ceil(dValue-.5)); 
} 
// 
double CEtc::Logbase(double dValue, double base)
{
	return log(dValue) / log(base);
}

void CEtc::CreateSleepTimeEvent()
{
	hSleepTimeEvent= CreateEvent(NULL, TRUE, FALSE, NULL);
}

void CEtc::SleepTime(DWORD dwMillisecond)
{
	WaitForSingleObject(hSleepTimeEvent, dwMillisecond);
}

void CEtc::CloseSleepTimeEvent()
{
	CloseHandle(hSleepTimeEvent);

	if(hSleepTimeEvent)
		hSleepTimeEvent = NULL;
}

BOOL CEtc::CheckPosition( double dDefaultPos, double dNowPos, double dCheckLimit /*= 0.01*/ )	/* 0.01mm이상 차이나면 틀리다고 간주 */
{
	if(max(dDefaultPos+dCheckLimit, dNowPos) == dNowPos)	return FALSE;
	if(min(dDefaultPos-dCheckLimit, dNowPos) == dNowPos)	return FALSE;

	return TRUE;
}


LPCTSTR CEtc::GetLocalIPAddress()
{
	char name[200];
	char* LocalHostName;
	if( gethostname( name, 200) == 0 )
	{
		PHOSTENT hostinfo = gethostbyname( name );

		if( hostinfo != NULL)
		{
			LocalHostName = inet_ntoa( *( struct in_addr* ) *hostinfo->h_addr_list );
			return LocalHostName;
		}
	}
	return NULL;
}

LPCTSTR CEtc::GetDeviceIPAddress(LPCTSTR szDeviceName)
{
	DWORD dwStatus;

	IP_ADAPTER_INFO *pAdapterInfo = NULL;

	ULONG ulSizeAdapterInfo = 0;



	dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);

	//***********************************************************************
	//버퍼 오버 플로우 일때 ulSizeAdapterInfo 크기로 메모리를 할당하고
	//다시 GetAdaptersInfo를 호출한다.
	if( dwStatus == ERROR_BUFFER_OVERFLOW)
	{
		pAdapterInfo = new IP_ADAPTER_INFO;
// 		if( !(pAdapterInfo = (PIP_ADAPTER_INFO)malloc(ulSizeAdapterInfo)) )
// 		{
// 			AfxMessageBox("Get Gateway Address Error");
// 		}

		dwStatus = GetAdaptersInfo(pAdapterInfo,&ulSizeAdapterInfo);
	}

	char Gateway_Addr[16];

	do
	{
		if(pAdapterInfo->AdapterName == szDeviceName)
		{
			for(int x=0; x<16; x++)
				Gateway_Addr[x] = pAdapterInfo->GatewayList.IpAddress.String[x];
		}

		pAdapterInfo = pAdapterInfo->Next;
	}
	while(pAdapterInfo);
	//	free(pAdapterInfo);
	delete pAdapterInfo;
	LPCTSTR szIPAddress = Gateway_Addr;
	return szIPAddress;
}

HRESULT CEtc::GetConnectionIPAddress( LPCTSTR szConnectionName )
{
	HRESULT hr = E_FAIL;
	CoInitialize(NULL);
	INetConnectionManager *pNetConnectionManager = NULL;
	hr = CoCreateInstance(CLSID_ConnectionManager,
		NULL,
		CLSCTX_LOCAL_SERVER | CLSCTX_NO_CODE_DOWNLOAD,
		IID_INetConnectionManager,
		reinterpret_cast<LPVOID *>(&pNetConnectionManager) );
	if (SUCCEEDED(hr))
	{
		/*Get an enumurator for the set of connections on the system */
		IEnumNetConnection* pEnumNetConnection;
		pNetConnectionManager->EnumConnections(NCME_DEFAULT, &pEnumNetConnection);
		ULONG ulCount = 0;
		BOOL fFound = FALSE;
		hr = HRESULT_FROM_WIN32(ERROR_FILE_NOT_FOUND);
		HRESULT hrT = S_OK;

		/*
		Enumurate through the list of adapters on the system and look for the one we want
		NOTE: To include per-user RAS connections in the list, you need to set the COM Proxy Blanket on all the interfaces. This is not needed for All-user RAS connections or LAN connections.
		*/
		do
		{
			NETCON_PROPERTIES* pProps = NULL;
			INetConnection *pConn;

			/*
			Find the next (or first connection)
			*/
			hrT = pEnumNetConnection->Next(1, &pConn, &ulCount);

			if (SUCCEEDED(hrT) && 1 == ulCount)
			{
				/*
				Get the connection properties
				*/
				hrT = pConn->GetProperties(&pProps);

				if (S_OK == hrT)
				{
					wchar_t  wszConnectionName[255];
					ZeroMemory(wszConnectionName, sizeof(wchar_t)*255);
					MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, szConnectionName, strlen(szConnectionName), wszConnectionName, 255);
					LPWSTR a = wszConnectionName;

					if(pProps->pszwName == a)
					{
						GetDeviceIPAddress((LPCTSTR)pProps->pszwDeviceName);
					}
				}

				pConn->Release();
				pConn = NULL;
			}

		}
		while (SUCCEEDED(hrT) && 1 == ulCount && !fFound);

		if (FAILED(hrT))
		{
			hr = hrT;
		}

		pEnumNetConnection->Release();
	}

	if (FAILED(hr) && hr != HRESULT_FROM_WIN32(ERROR_RETRY))
	{
		printf("Could not enable or disable connection (0x%08x)\r\n", hr);
	}
	pNetConnectionManager->Release();
	CoUninitialize();
	return hr;
}