#pragma once

// Machine generated IDispatch wrapper class(es) created by Microsoft Visual C++

// NOTE: Do not modify the contents of this file.  If this class is regenerated by
//  Microsoft Visual C++, your modifications will be overwritten.

/////////////////////////////////////////////////////////////////////////////
// CXnetctrl1 wrapper class

class CXnetctrl1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CXnetctrl1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA3297BF4, 0x7819, 0x47AC, { 0x9B, 0x10, 0xB2, 0x43, 0x52, 0x3C, 0x29, 0xC7 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// Attributes
public:


// Operations
public:

// _DXNet

// Functions
//

	long SendString(LPCTSTR szStrVal, long nLen)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0xc, DISPATCH_METHOD, VT_I4, (void*)&result, parms, szStrVal, nLen);
		return result;
	}
	CString GetString(long lMsgID)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lMsgID);
		return result;
	}
	long Start()
	{
		long result;
		InvokeHelper(0xe, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long Stop()
	{
		long result;
		InvokeHelper(0xf, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long PurgeSpool()
	{
		long result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long GetBinary(long lMsgID, short * pByteBuff, long nBuffCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI2 VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgID, pByteBuff, nBuffCount);
		return result;
	}
	long SendBinary(short * pByteBuff, long nCount)
	{
		long result;
		static BYTE parms[] = VTS_PI2 VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pByteBuff, nCount);
		return result;
	}
	long GetLastError()
	{
		long result;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	CString GetLastErrorString()
	{
		CString result;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	CString GetErrorString(long nError)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, nError);
		return result;
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}

// Properties
//

CString GetIP()
{
	CString result;
	GetProperty(0x1, VT_BSTR, (void*)&result);
	return result;
}
void SetIP(CString propVal)
{
	SetProperty(0x1, VT_BSTR, propVal);
}
long GetPort()
{
	long result;
	GetProperty(0x2, VT_I4, (void*)&result);
	return result;
}
void SetPort(long propVal)
{
	SetProperty(0x2, VT_I4, propVal);
}
BOOL GetActive()
{
	BOOL result;
	GetProperty(0x3, VT_BOOL, (void*)&result);
	return result;
}
void SetActive(BOOL propVal)
{
	SetProperty(0x3, VT_BOOL, propVal);
}
BOOL GetSpoolEnabled()
{
	BOOL result;
	GetProperty(0x4, VT_BOOL, (void*)&result);
	return result;
}
void SetSpoolEnabled(BOOL propVal)
{
	SetProperty(0x4, VT_BOOL, propVal);
}
long GetMaxSpoolSize()
{
	long result;
	GetProperty(0x5, VT_I4, (void*)&result);
	return result;
}
void SetMaxSpoolSize(long propVal)
{
	SetProperty(0x5, VT_I4, propVal);
}
CString GetSpoolDir()
{
	CString result;
	GetProperty(0x6, VT_BSTR, (void*)&result);
	return result;
}
void SetSpoolDir(CString propVal)
{
	SetProperty(0x6, VT_BSTR, propVal);
}
BOOL GetLogEvent()
{
	BOOL result;
	GetProperty(0x7, VT_BOOL, (void*)&result);
	return result;
}
void SetLogEvent(BOOL propVal)
{
	SetProperty(0x7, VT_BOOL, propVal);
}
BOOL GetLogMesgSummary()
{
	BOOL result;
	GetProperty(0x8, VT_BOOL, (void*)&result);
	return result;
}
void SetLogMesgSummary(BOOL propVal)
{
	SetProperty(0x8, VT_BOOL, propVal);
}
BOOL GetLogMesgDetail()
{
	BOOL result;
	GetProperty(0x9, VT_BOOL, (void*)&result);
	return result;
}
void SetLogMesgDetail(BOOL propVal)
{
	SetProperty(0x9, VT_BOOL, propVal);
}
BOOL GetEstablished()
{
	BOOL result;
	GetProperty(0xa, VT_BOOL, (void*)&result);
	return result;
}
void SetEstablished(BOOL propVal)
{
	SetProperty(0xa, VT_BOOL, propVal);
}
long GetSendTimeout()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}
void SetSendTimeout(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}


};
