#pragma once

// 컴퓨터에서 Microsoft Visual C++를 사용하여 생성한 IDispatch 래퍼 클래스입니다.

// 참고: 이 파일의 내용을 수정하지 마십시오. Microsoft Visual C++에서
//  이 클래스를 다시 생성할 때 수정한 내용을 덮어씁니다.

/////////////////////////////////////////////////////////////////////////////
// CIphonepadx 래퍼 클래스입니다.

class CIphonepadx : public CWnd
{
protected:
	DECLARE_DYNCREATE(CIphonepadx)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xA8BB3D6D, 0xACF3, 0x4516, { 0x90, 0x90, 0xFF, 0x76, 0xD4, 0xEE, 0xC5, 0xFA } };
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
enum
{
    ilmisRound = 0,
    ilmisRectangle = 1
}TxiLedMatrixIndiatorStyle;
enum
{
    ibsNone = 0,
    ibsRaised = 1,
    ibsLowered = 2
}TxiBevelStyle;
enum
{
    mbLeft = 0,
    mbRight = 1,
    mbMiddle = 2
}TxMouseButton;
enum
{
    ilasRight = 0,
    ilasLeft = 1,
    ilasUp = 2,
    ilasDown = 3,
    ilasLeftRight = 4,
    ilasUpDown = 5
}TxiLedArrowStyle;
enum
{
    ichs24 = 0,
    ichs12 = 1
}TxiClockHourStyle;
enum
{
    ifsDropFrame = 0,
    ifsNonDropFrame = 1
}TxiFrameStyle;
enum
{
    bsSolid = 0,
    bsClear = 1,
    bsHorizontal = 2,
    bsVertical = 3,
    bsFDiagonal = 4,
    bsBDiagonal = 5,
    bsCross = 6,
    bsDiagCross = 7
}TxBrushStyle;
enum
{
    psSolid = 0,
    psDash = 1,
    psDot = 2,
    psDashDot = 3,
    psDashDotDot = 4,
    psClear = 5,
    psInsideFrame = 6
}TxPenStyle;
enum
{
    tidoEllipse = 0,
    tidoRectangle = 1,
    tidoLine = 2,
    tidoText = 3,
    tidoImage = 4,
    tidoArc = 5,
    tidoPolygon = 6
}TxiDrawObjectType;
enum
{
    imcsClickAnyWhere = 0,
    imcsClickIndicator = 1
}TxiMouseControlStyle;
enum
{
    issDown = 0,
    issCenter = 1,
    issUp = 2
}TxiSwitchState;
enum
{
    ioVertical = 0,
    ioHorizontal = 1
}TxiOrientation;
enum
{
    iosBottomRight = 0,
    iosTopLeft = 1
}TxiOrientationSide;
enum
{
    ilgpsPointer = 0,
    ilgpsTriangle = 1,
    ilgpsTLine = 2,
    ilgpsColorBar = 3
}TxiLinearGaugePointerStyle;
enum
{
    illsValue = 0,
    illsScientific = 1
}TxiLogLabelStyle;
enum
{
    itmnaInside = 0,
    itmnaCenter = 1,
    itmnaOutside = 2
}TxiTickMinorAlignment;
enum
{
    iavCenter = 0,
    iavTop = 1,
    iavBottom = 2
}TxiAlignmentVertical;
enum
{
    iahCenter = 0,
    iahLeft = 1,
    iahRight = 2
}TxiAlignmentHorizontal;
enum
{
    iagpsArrowLine = 0,
    iagpsArrow = 1,
    iagpsLine = 2,
    iagpsTriangle = 3
}TxiAngularGaugePointerStyle;
enum
{
    ithaCenterDisplay = 0,
    ithaCenterControl = 1
}TxiTitleHorizontalAlignment;
enum
{
    icpsTriangle = 0,
    icpsCircle = 1,
    icpsLine = 2
}TxiCompassPointerStyle;
enum
{
    tpIdle = 0,
    tpLowest = 1,
    tpLower = 2,
    tpNormal = 3,
    tpHigher = 4,
    tpHighest = 5,
    tpTimeCritical = 6
}TxThreadPriority;
enum
{
    itasCenter = 0,
    itasSide = 1
}TxiScaleAlignStyle;
enum
{
    isspsDualArrow = 0,
    isspsArrow = 1,
    isspsPointer = 2,
    isspsLine = 3
}TxiSlidingScalePointerStyle;
enum
{
    iasvTop = 0,
    iasvBottom = 1
}TxiAlignmentSideVertical;
enum
{
    ikbs104 = 0,
    ikbs87 = 1,
    ikbs74 = 2,
    ikbs61 = 3,
    ikbsNumericKeyPad = 4
}TxiKeyBoardStyle;
enum
{
    htKeyword = 0,
    htContext = 1
}TxHelpType;
enum
{
    itpsBar = 0,
    itpsCutOut1 = 1,
    itpsCutOut2 = 2,
    itpsLargeWindow = 3,
    itpsCustom = 4
}TxiTankPointerStyle;
enum
{
    ipfisCircle = 0,
    ipfisBox = 1,
    ipfisTriangle = 2
}TxiPipeFlowIndicatorStyle;
enum
{
    imbsTrapezoid = 0,
    imbsFeet = 1
}TxiMotorBaseStyle;


// 작업입니다.
public:

// IiPhonePadX

// Functions
//

	LPDISPATCH get_Font()
	{
		LPDISPATCH result;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
		return result;
	}
	void put_Font(LPDISPATCH newValue)
	{
		static BYTE parms[] = VTS_DISPATCH ;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void putref_Font(LPDISPATCH * newValue)
	{
		static BYTE parms[] = VTS_PDISPATCH ;
		InvokeHelper(DISPID_FONT, DISPATCH_PROPERTYPUTREF, VT_EMPTY, NULL, parms, newValue);
	}
	double get_OuterMargin()
	{
		double result;
		InvokeHelper(0x1, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_OuterMargin(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x1, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	double get_ButtonSpacing()
	{
		double result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_R8, (void*)&result, NULL);
		return result;
	}
	void put_ButtonSpacing(double newValue)
	{
		static BYTE parms[] = VTS_R8 ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ShowFocusRect()
	{
		BOOL result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ShowFocusRect(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	unsigned long get_BackGroundColor()
	{
		unsigned long result;
		InvokeHelper(0x4, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_BackGroundColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x4, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_UpdateFrameRate()
	{
		long result;
		InvokeHelper(0x5, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_UpdateFrameRate(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x5, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Transparent()
	{
		BOOL result;
		InvokeHelper(0x6, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Transparent(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x6, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_BorderStyle()
	{
		long result;
		InvokeHelper(0x7, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_BorderStyle(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x7, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Visible()
	{
		BOOL result;
		InvokeHelper(0xd, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Visible(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xd, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_Enabled()
	{
		BOOL result;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_Enabled(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(DISPID_ENABLED, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void AboutBox()
	{
		InvokeHelper(DISPID_ABOUTBOX, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	unsigned long get_FontColor()
	{
		unsigned long result;
		InvokeHelper(0x21, DISPATCH_PROPERTYGET, VT_UI4, (void*)&result, NULL);
		return result;
	}
	void put_FontColor(unsigned long newValue)
	{
		static BYTE parms[] = VTS_UI4 ;
		InvokeHelper(0x21, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void RepaintAll()
	{
		InvokeHelper(0x22, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void BeginUpdate()
	{
		InvokeHelper(0x23, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void EndUpdate()
	{
		InvokeHelper(0x24, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Lock()
	{
		InvokeHelper(0x25, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void Unlock()
	{
		InvokeHelper(0x26, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	void ShowPropertyEditor()
	{
		InvokeHelper(0x8, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
	}
	BOOL get_ButtonVisible0()
	{
		BOOL result;
		InvokeHelper(0x9, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible0(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x9, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible1()
	{
		BOOL result;
		InvokeHelper(0xa, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible1(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xa, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible2()
	{
		BOOL result;
		InvokeHelper(0xb, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible2(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xb, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible3()
	{
		BOOL result;
		InvokeHelper(0xc, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible3(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xc, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible4()
	{
		BOOL result;
		InvokeHelper(0xe, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible4(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xe, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible5()
	{
		BOOL result;
		InvokeHelper(0xf, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible5(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0xf, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible6()
	{
		BOOL result;
		InvokeHelper(0x10, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible6(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x10, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible7()
	{
		BOOL result;
		InvokeHelper(0x11, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible7(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x11, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible8()
	{
		BOOL result;
		InvokeHelper(0x12, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible8(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x12, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisible9()
	{
		BOOL result;
		InvokeHelper(0x13, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisible9(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x13, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisibleStar()
	{
		BOOL result;
		InvokeHelper(0x14, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisibleStar(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x14, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_ButtonVisiblePound()
	{
		BOOL result;
		InvokeHelper(0x15, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_ButtonVisiblePound(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x15, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption0()
	{
		CString result;
		InvokeHelper(0x16, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption0(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x16, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption1()
	{
		CString result;
		InvokeHelper(0x17, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption1(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x17, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption2()
	{
		CString result;
		InvokeHelper(0x18, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption2(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x18, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption3()
	{
		CString result;
		InvokeHelper(0x19, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption3(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x19, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption4()
	{
		CString result;
		InvokeHelper(0x1a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption4(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption5()
	{
		CString result;
		InvokeHelper(0x1b, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption5(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1b, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption6()
	{
		CString result;
		InvokeHelper(0x1c, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption6(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption7()
	{
		CString result;
		InvokeHelper(0x1d, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption7(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption8()
	{
		CString result;
		InvokeHelper(0x1e, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption8(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaption9()
	{
		CString result;
		InvokeHelper(0x1f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaption9(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaptionStar()
	{
		CString result;
		InvokeHelper(0x20, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaptionStar(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x20, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	CString get_ButtonCaptionPound()
	{
		CString result;
		InvokeHelper(0x27, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_ButtonCaptionPound(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x27, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_ComponentHandle()
	{
		long result;
		InvokeHelper(0x28, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void SaveImageToBitmap(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x29, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveImageToMetaFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void SaveImageToJPEG(LPCTSTR FileName, long Compression, BOOL Progressive)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 VTS_BOOL ;
		InvokeHelper(0x2b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Compression, Progressive);
	}
	long GetObjectPointer()
	{
		long result;
		InvokeHelper(0x1068, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
		return result;
	}
	VARIANT GetBytesJPEG(long Compression, BOOL Progressive)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 VTS_BOOL ;
		InvokeHelper(0x1069, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Compression, Progressive);
		return result;
	}
	void SavePropertiesToFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x106a, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	void LoadPropertiesFromFile(LPCTSTR FileName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x106b, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName);
	}
	BOOL get_OptionSaveAllProperties()
	{
		BOOL result;
		InvokeHelper(0x106d, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_OptionSaveAllProperties(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x106d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	BOOL get_AutoFrameRate()
	{
		BOOL result;
		InvokeHelper(0x106e, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
		return result;
	}
	void put_AutoFrameRate(BOOL newValue)
	{
		static BYTE parms[] = VTS_BOOL ;
		InvokeHelper(0x106e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void iPaintToDC(long X, long Y, BOOL Transparent, long DC)
	{
		static BYTE parms[] = VTS_I4 VTS_I4 VTS_BOOL VTS_I4 ;
		InvokeHelper(0x10cc, DISPATCH_METHOD, VT_EMPTY, NULL, parms, X, Y, Transparent, DC);
	}
	long get_Width()
	{
		long result;
		InvokeHelper(0x2c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Width(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long get_Height()
	{
		long result;
		InvokeHelper(0x2d, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
		return result;
	}
	void put_Height(long newValue)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x2d, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	void OPCItemUpdateResume(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1013, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void OPCItemUpdateSuspend(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1014, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	VARIANT GetOPCItemData(long Index)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1015, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Index);
		return result;
	}
	short GetOPCItemQuality(long Index)
	{
		short result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1016, DISPATCH_METHOD, VT_I2, (void*)&result, parms, Index);
		return result;
	}
	double GetOPCItemTimeStamp(long Index)
	{
		double result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1017, DISPATCH_METHOD, VT_R8, (void*)&result, parms, Index);
		return result;
	}
	void SetOPCItemData(long Index, VARIANT Value)
	{
		static BYTE parms[] = VTS_I4 VTS_VARIANT ;
		InvokeHelper(0x1018, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, &Value);
	}
	BOOL GetOPCItemActive(long Index)
	{
		BOOL result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1019, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms, Index);
		return result;
	}
	CString GetOPCItemGroupName(long Index)
	{
		CString result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x101c, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms, Index);
		return result;
	}
	void SetOPCItemGroupName(long Index, LPCTSTR Value)
	{
		static BYTE parms[] = VTS_I4 VTS_BSTR ;
		InvokeHelper(0x101d, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index, Value);
	}
	void OPCDeleteItem(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x101e, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	void OPCItemActivateSuspend(long Index)
	{
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x101f, DISPATCH_METHOD, VT_EMPTY, NULL, parms, Index);
	}
	VARIANT GetBytesPNG(long Compression)
	{
		VARIANT result;
		static BYTE parms[] = VTS_I4 ;
		InvokeHelper(0x1020, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms, Compression);
		return result;
	}
	void SaveImageToPNG(LPCTSTR FileName, long Compression)
	{
		static BYTE parms[] = VTS_BSTR VTS_I4 ;
		InvokeHelper(0x1021, DISPATCH_METHOD, VT_EMPTY, NULL, parms, FileName, Compression);
	}

// Properties
//



};
