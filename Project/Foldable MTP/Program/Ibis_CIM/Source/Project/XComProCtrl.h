#pragma once

// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오. Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.

/////////////////////////////////////////////////////////////////////////////
// CXComProCtrl 래퍼 클래스입니다.

class CXComProCtrl : public CWnd
{
protected:
	DECLARE_DYNCREATE(CXComProCtrl)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0x560F8BBF, 0xA69A, 0x4320, { 0x82, 0xA3, 0xEE, 0x16, 0x7D, 0x16, 0x55, 0x9E } };
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

// 특성입니다.
public:


// 작업입니다.
public:

// _DXComPro

// Functions
//

	long Initialize(LPCTSTR pszCfgFile)
	{
		long result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0xd, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pszCfgFile);
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
	long Close()
	{
		long result;
		InvokeHelper(0x10, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	long MakeSecsMsg(long * plMsgId, short nDevId, short nStrm, short nFunc, long lSysByte)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_I2 VTS_I2 VTS_I2 VTS_I4 ;
		InvokeHelper(0x11, DISPATCH_METHOD, VT_I4, (void*)&result, parms, plMsgId, nDevId, nStrm, nFunc, lSysByte);
		return result;
	}
	long Send(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x12, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	long LoadSecsMsg(long * plMsgId, short * pnDevId, short * pnStrm, short * pnFunc, long * plSysByte, short * pnWbit)
	{
		long result;
		static BYTE parms[] = VTS_PI4 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI4 VTS_PI2 ;
		InvokeHelper(0x13, DISPATCH_METHOD, VT_I4, (void*)&result, parms, plMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
		return result;
	}
	void CloseSecsMsg(long lMsgId)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x14, DISPATCH_METHOD, VT_EMPTY, NULL, parms, lMsgId);
	}
	long GetListItem(long lMsgId, long * pnItems)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 ;
		InvokeHelper(0x15, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnItems);
		return result;
	}
	long GetBinaryItem(long lMsgId, unsigned char * pnValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x16, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, plCount, lSize);
		return result;
	}
	long GetBoolItem(long lMsgId, unsigned char * pnValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x17, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, plCount, lSize);
		return result;
	}
	long GetAsciiItem(long lMsgId, signed char * pszValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x18, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pszValue, plCount, lSize);
		return result;
	}
	long GetJis8Item(long lMsgId, signed char * pszValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x19, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pszValue, plCount, lSize);
		return result;
	}
	long GetI1Item(long lMsgId, signed char * pnValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, plCount, lSize);
		return result;
	}
	long GetI2Item(long lMsgId, short * pnValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI2 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, plCount, lSize);
		return result;
	}
	long GetI4Item(long lMsgId, long * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetI8Item(long lMsgId, __int64 * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI8 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetU1Item(long lMsgId, unsigned char * pnValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, plCount, lSize);
		return result;
	}
	long GetU2Item(long lMsgId, unsigned short * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI2 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x1f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetU4Item(long lMsgId, unsigned long * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x20, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetU8Item(long lMsgId, unsigned __int64 * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI8 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x21, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetF4Item(long lMsgId, float * prValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x22, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, prValue, plCount, lSize);
		return result;
	}
	long GetF8Item(long lMsgId, double * prValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x23, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, prValue, plCount, lSize);
		return result;
	}
	long SetListItem(long lMsgId, long nItems)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x24, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, nItems);
		return result;
	}
	long SetBinaryItem(long lMsgId, unsigned char * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x25, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetBoolItem(long lMsgId, unsigned char * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x26, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetAsciiItem(long lMsgId, signed char * pszValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_I4 ;
		InvokeHelper(0x27, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pszValue, lCount);
		return result;
	}
	long SetJis8Item(long lMsgId, signed char * pszValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_I4 ;
		InvokeHelper(0x28, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pszValue, lCount);
		return result;
	}
	long SetI1Item(long lMsgId, signed char * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI1 VTS_I4 ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetI2Item(long lMsgId, short * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI2 VTS_I4 ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetI4Item(long lMsgId, long * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long SetI8Item(long lMsgId, __int64 * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI8 VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long SetU1Item(long lMsgId, unsigned char * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x2d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetU2Item(long lMsgId, unsigned short * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI2 VTS_I4 ;
		InvokeHelper(0x2e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long SetU4Item(long lMsgId, unsigned long * pnValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI4 VTS_I4 ;
		InvokeHelper(0x2f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnValue, lCount);
		return result;
	}
	long SetU8Item(long lMsgId, unsigned __int64 * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI8 VTS_I4 ;
		InvokeHelper(0x30, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long SetF4Item(long lMsgId, float * prValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR4 VTS_I4 ;
		InvokeHelper(0x31, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, prValue, lCount);
		return result;
	}
	long SetF8Item(long lMsgId, double * prValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PR8 VTS_I4 ;
		InvokeHelper(0x32, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, prValue, lCount);
		return result;
	}
	long EnableLog(long nLogType, short nEnabled)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x33, DISPATCH_METHOD, VT_I4, (void*)&result, parms, nLogType, nEnabled);
		return result;
	}
	long GetHeaderBytes(long lMsgId, unsigned char * pnHeader, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x34, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnHeader, lSize);
		return result;
	}
	long GetMsgBytes(long lMsgId, unsigned char * pnMsg, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x35, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnMsg, lSize);
		return result;
	}
	long SetHeaderBytes(long lMsgId, unsigned char * pnHeader, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x36, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnHeader, lSize);
		return result;
	}
	long SetMsgBytes(long lMsgId, unsigned char * pnMsg, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x37, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnMsg, lSize);
		return result;
	}
	long GetAlarmMsgInfo(long lMsgId, short * pnDevId, short * pnStrm, short * pnFunc, long * plSysByte, short * pnWbit)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI4 VTS_PI2 ;
		InvokeHelper(0x38, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
		return result;
	}
	long DebugInfo(short * pnInfo)
	{
		long result;
		static BYTE parms[] = VTS_PI2 ;
		InvokeHelper(0x39, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pnInfo);
		return result;
	}
	long GetSysBytes(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	long SetWBit(long lMsgId, short nWBit)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I2 ;
		InvokeHelper(0x3b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, nWBit);
		return result;
	}
	long GetInvalidMsgInfo(long lMsgId, short * pnDevId, short * pnStrm, short * pnFunc, long * plSysByte, short * pnWbit)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI2 VTS_PI2 VTS_PI2 VTS_PI4 VTS_PI2 ;
		InvokeHelper(0x3c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnDevId, pnStrm, pnFunc, plSysByte, pnWbit);
		return result;
	}
	long MakeSpoolStream(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x3d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	CString GetParam(LPCTSTR szName)
	{
		CString result;
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x3e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, szName);
		return result;
	}
	long SetParam(LPCTSTR pszName, LPCTSTR pszValue)
	{
		long result;
		static BYTE parms[] = VTS_BSTR VTS_BSTR ;
		InvokeHelper(0x3f, DISPATCH_METHOD, VT_I4, (void*)&result, parms, pszName, pszValue);
		return result;
	}
	long GetCurrentItemType(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x40, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	long GetCurrentItemCount(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x41, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	CString GetSecsLog(long lMsgId)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x42, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, lMsgId);
		return result;
	}
	long GetMsgSize(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x43, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	long SetStreamMsg(long lMsgId, unsigned char * pnData, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x44, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnData, lSize);
		return result;
	}
	long GetStreamMsgSize(long lMsgId)
	{
		long result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x45, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId);
		return result;
	}
	long GetStreamMsg(long lMsgId, unsigned char * pnData, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PUI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x46, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, pnData, plCount, lSize);
		return result;
	}
	long MoveSecsItem(long lMsgId, long nIndex)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 ;
		InvokeHelper(0x47, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, nIndex);
		return result;
	}
	long GetU4Item_s1(long lMsgId, long * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x48, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long GetU8Item_s1(long lMsgId, __int64 * plValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI8 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x49, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, plCount, lSize);
		return result;
	}
	long SetU4Item_s1(long lMsgId, long * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x4a, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long SetU8Item_s1(long lMsgId, __int64 * plValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI8 VTS_I4 ;
		InvokeHelper(0x4b, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plValue, lCount);
		return result;
	}
	long GetMbyteItem(long lMsgId, long * plEnCode, unsigned char * pbValue, long * plCount, long lSize)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_PI4 VTS_PUI1 VTS_PI4 VTS_I4 ;
		InvokeHelper(0x4c, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, plEnCode, pbValue, plCount, lSize);
		return result;
	}
	long SetMbyteItem(long lMsgId, long lEnCode, unsigned char * pbValue, long lCount)
	{
		long result;
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_PUI1 VTS_I4 ;
		InvokeHelper(0x4d, DISPATCH_METHOD, VT_I4, (void*)&result, parms, lMsgId, lEnCode, pbValue, lCount);
		return result;
	}
	long EnableHeaderOnlyLog(short bEnabled, short nStrm, long nCount, short * pnFunc)
	{
		long result;
		static BYTE parms[] = VTS_I2 VTS_I2 VTS_I4 VTS_PI2 ;
		InvokeHelper(0x4e, DISPATCH_METHOD, VT_I4, (void*)&result, parms, bEnabled, nStrm, nCount, pnFunc);
		return result;
	}

// Properties
//

short GetDeviceID()
{
	short result;
	GetProperty(0x1, VT_I2, (void*)&result);
	return result;
}
void SetDeviceID(short propVal)
{
	SetProperty(0x1, VT_I2, propVal);
}
short GetStream()
{
	short result;
	GetProperty(0x2, VT_I2, (void*)&result);
	return result;
}
void SetStream(short propVal)
{
	SetProperty(0x2, VT_I2, propVal);
}
short GetFunction()
{
	short result;
	GetProperty(0x3, VT_I2, (void*)&result);
	return result;
}
void SetFunction(short propVal)
{
	SetProperty(0x3, VT_I2, propVal);
}
long GetSystemBytes()
{
	long result;
	GetProperty(0x4, VT_I4, (void*)&result);
	return result;
}
void SetSystemBytes(long propVal)
{
	SetProperty(0x4, VT_I4, propVal);
}
short GetWbit()
{
	short result;
	GetProperty(0x5, VT_I2, (void*)&result);
	return result;
}
void SetWbit(short propVal)
{
	SetProperty(0x5, VT_I2, propVal);
}
short GetRbit()
{
	short result;
	GetProperty(0x6, VT_I2, (void*)&result);
	return result;
}
void SetRbit(short propVal)
{
	SetProperty(0x6, VT_I2, propVal);
}
short GetEbit()
{
	short result;
	GetProperty(0x7, VT_I2, (void*)&result);
	return result;
}
void SetEbit(short propVal)
{
	SetProperty(0x7, VT_I2, propVal);
}
short GetPtype()
{
	short result;
	GetProperty(0x8, VT_I2, (void*)&result);
	return result;
}
void SetPtype(short propVal)
{
	SetProperty(0x8, VT_I2, propVal);
}
short GetStype()
{
	short result;
	GetProperty(0x9, VT_I2, (void*)&result);
	return result;
}
void SetStype(short propVal)
{
	SetProperty(0x9, VT_I2, propVal);
}
short GetBlock()
{
	short result;
	GetProperty(0xa, VT_I2, (void*)&result);
	return result;
}
void SetBlock(short propVal)
{
	SetProperty(0xa, VT_I2, propVal);
}
long GetHeaderSize()
{
	long result;
	GetProperty(0xb, VT_I4, (void*)&result);
	return result;
}
void SetHeaderSize(long propVal)
{
	SetProperty(0xb, VT_I4, propVal);
}
long GetMsgSize()
{
	long result;
	GetProperty(0xc, VT_I4, (void*)&result);
	return result;
}
void SetMsgSize(long propVal)
{
	SetProperty(0xc, VT_I4, propVal);
}


};
