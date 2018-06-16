#include "StdAfx.h"
#include "GxUICtrl.h"

// GxOCX header
#include "GxButtonEx.h"
#include "GxStaticEx.h"


CGxUICtrl::CGxUICtrl()
{
}


CGxUICtrl::~CGxUICtrl()
{
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


//
// GausGUI 에서 생성한 Modal Dialog를 강제로 종료시켜준다. Dialog 응답은 IDABORT로 끝내며, 더이상 Dialog를 찾을 없으면 FALSE를 리턴한다.
//
BOOL CGxUICtrl::AbortModalDialog()
{
	HWND hWnd = ::FindWindow(NULL, _T("GausGUI"));			// GausGUI로 타이틀을 갖는 Window를 찾는다.

	if( hWnd )			// 존재한다면
	{
		::SendMessage(hWnd, WM_COMMAND, IDABORT, 0);			// ABORT 명령 전송

		return TRUE;													// 해당 Window를 찾아서 종료 처리를 시도했다.
	}

	return FALSE;														// 더이상 해당 Window를 찾을 수 없다.
}

//
// 지정한 폼의 모든 GxOCX계열의 Control에게 표시 표시 언어가 변경되었음을 통보한다.
//
void CGxUICtrl::BroadcastButtonCpationIndex(CWnd* pView, UINT nIdx, UINT nID)
{
	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));			

	if ( pBtn )
	{
		pBtn->GxBroadcastChangeCaptionIndex( (OLE_HANDLE)(pView->GetSafeHwnd()), nIdx + 1);
	}
}


// 지정한 폼의 모든 GxOCX계열의 Control에게 표시 표시 언어가 변경되었음을 통보한다.
//
void CGxUICtrl::BroadcastStaticCpationIndex(CWnd* pView, UINT nIdx, UINT nID)
{
	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));			

	if ( pLbl )
	{
		pLbl->GxBroadcastChangeCaptionIndex( (OLE_HANDLE)(pView->GetSafeHwnd()), nIdx + 1);
	}
}


//
// 지정 버튼의 색을 지정한다.
void CGxUICtrl::SetButtonColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		if ( pBtn->GetBackColor() != cColor ) pBtn->SetBackColor( cColor );			// 버튼의 색상을 지정 색으로 변경시켜준다.
	}
}


// 지정 버튼의 글자 색을 지정한다.
void CGxUICtrl::SetButtonTextColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		if ( pBtn->GetTextColor() != cColor ) pBtn->SetTextColor( cColor );			// 버튼의 글자 색상을 지정 색으로 변경시켜준다.
	}
}



// 지정된 상태에따라 버튼의 아이콘을 변경시킨다.
//
//  UINT nIdx : Icon 순번, 0 ~ 
void CGxUICtrl::SetButtonIcon(CWnd* pView, UINT nID, UINT nIdx)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		pBtn->SetIconIndex( nIdx );				// 버튼의 아이콘을 지정한 순번으로 변경시켜준다.
	}
}



// 지정된 문자열을 버튼에 표시해준다.
void CGxUICtrl::SetButtonString(CWnd* pView, UINT nID, CString sMsg)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		pBtn->SetCaption( sMsg );																	// 지정한 문자열을 기록한다.
	}
}


// 지정된 정수형을 버튼에 표시해준다.
void CGxUICtrl::SetButtonInt(CWnd* pView, UINT nID, int nValue)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// 라벨 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		CString sMsg;
		sMsg.Format(_T("%d"), nValue);

		pBtn->SetCaption( sMsg );																	// 지정한 문자열을 기록한다.
	}
}


// 지정된 문자열을 라벨에 표시해준다.
void CGxUICtrl::SetButtonDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint)
{
	if ( nID <= 0 ) return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));					// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		CString sMsg, sFmt;
		sFmt.Format(_T("%%.%df"), nPoint);													// 소숫점 자릿수 표현
		sMsg.Format(sFmt, dbValue);

		pBtn->SetCaption( sMsg );																// 지정한 문자열을 기록한다.
	}
}



// 지정 버튼의 사용/미사용을 지정한다.
void CGxUICtrl::SetButtonEnabled(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));				// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		pBtn->SetEnable( bFlag );
	}
}


// 지정 버튼의 보이기/감추기를 지정한다.
void CGxUICtrl::SetButtonVisible(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));		// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		// Flag에 따라서 객체를 감추거나 보여준다.
		if ( bFlag == TRUE ) 
		{
			pBtn->ShowWindow( SW_SHOW );
		}
		else
			pBtn->ShowWindow( SW_HIDE );
	}
}


// 지정 Static의 Blink 기능을 활성/비활성화 시킨다.
void CGxUICtrl::SetButtonBlink(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));		// 버튼 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		// Flag에 따라서 객체를 감추거나 보여준다.
		if ( bFlag == TRUE ) 
		{
			pBtn->StartBlink();
		}
		else
			pBtn->StopBlink();
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 지정된 라벨에 색상을 변경한다.
void CGxUICtrl::SetStaticColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 버튼 리소스를 참조하여 포인터 획득;

	if ( pLbl )
	{
		pLbl->SetBackColor( cColor );												// 색상을 변경시켜준다.
	}
}


// 지정된 라벨에 글자 색상을 변경한다.
void CGxUICtrl::SetStaticTextColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 버튼 리소스를 참조하여 포인터 획득;

	if ( pLbl )
	{
		pLbl->SetTextColor( cColor );												// 글자 색상을 변경시켜준다.
	}
}


// 지정된 상태를 라벨에 표시해준다.
void CGxUICtrl::SetStaticIcon(CWnd* pView, UINT nID, UINT nIdx)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 버튼 리소스를 참조하여 포인터 획득;

	if ( pLbl )
	{
		pLbl->SetIconIndex( nIdx );				// 버튼의 아이콘을 지정한 순번으로 변경시켜준다.
	}
}


// 지정된 문자열을 라벨에 표시해준다.
void CGxUICtrl::SetStaticString(CWnd* pView, UINT nID, CString sMsg)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 라벨 리소스를 참조하여 포인터 획득;

	if ( pLbl )
	{
		pLbl->SetCaption( sMsg );													// 지정한 문자열을 기록한다.
	}
}


// 지정된 정수형을 라벨에 표시해준다.
void CGxUICtrl::SetStaticInt(CWnd* pView, UINT nID, int nValue)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 라벨 리소스를 참조하여 포인터 획득;

	if ( pLbl )
	{
		CString sMsg;
		sMsg.Format(_T("%d"), nValue);

		pLbl->SetCaption( sMsg );												// 지정한 문자열을 기록한다.
	}
}


// 지정된 Double형 데이터(소숫점) 라벨에 표시해준다.
void CGxUICtrl::SetStaticDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// 라벨 리소스를 참조하여 포인터 획득;


	if ( pLbl )
	{
		CString sMsg, sFmt;
		sFmt.Format(_T("%%.%df"), nPoint);										// 소숫점 자릿수 표현
		sMsg.Format(sFmt, dbValue);

		pLbl->SetCaption( sMsg );													// 지정한 문자열을 기록한다.
	}
}


// 지정 Static의 보이기/감추기를 지정한다.
void CGxUICtrl::SetStaticVisible(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pBtn = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// Static 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		// Flag에 따라서 객체를 감추거나 보여준다.
		if ( bFlag == TRUE ) 
		{
			pBtn->ShowWindow( SW_SHOW );
		}
		else
			pBtn->ShowWindow( SW_HIDE );
	}
}


// 지정 Static의 Blink 기능을 활성/비활성화 시킨다.
void CGxUICtrl::SetStaticBlink(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pBtn = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// Static 리소스를 참조하여 포인터 획득;

	if ( pBtn )
	{
		// Flag에 따라서 객체를 감추거나 보여준다.
		if ( bFlag == TRUE ) 
		{
			pBtn->StartBlink();
		}
		else
			pBtn->StopBlink();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// 위치 변경 함수
void CGxUICtrl::SetTopPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint)				// 상/하 위치만 변경해준다.
{
	CRect rect;
	pOwner->GetDlgItem( nID )->GetWindowRect( &rect );		// 현재 위치
	pOwner->ScreenToClient( &rect );

	int nHeight		= rect.Height();
	rect.top			= lNewPos;									// 새로운 위치, 
	rect.bottom		= rect.top + nHeight;					// 높이를 새로이 맞추어 준다.
	pOwner->GetDlgItem( nID )->MoveWindow(&rect, bRepaint);
}

void CGxUICtrl::SetLeftPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint)				// 상/하 위치만 변경해준다.
{
	CRect rect;
	pOwner->GetDlgItem( nID )->GetWindowRect( &rect );		// 현재 위치
	pOwner->ScreenToClient( &rect );

	int nWidth		= rect.Width();
	rect.left		= lNewPos;									// 새로운 위치, 
	rect.right		= rect.left + nWidth;					// 너비를 새로이 맞추어 준다.
	pOwner->GetDlgItem( nID )->MoveWindow(&rect, bRepaint);
}






//
// 현재 화면을 저장한다.
//
//void SaveScreen(CString sFile)
//{
//	 // 바탕 화면 윈도우 객체에 대한 포인터를 얻는다. 
//	CWnd* pWndDesktop = GetDesktopWindow(); 
//
//	// 바탕 화면 윈도우 DC 
//	CWindowDC ScrDC(pWndDesktop); 
//
//	// 뷰 윈도우 DC 
//	CClientDC dc(this); 
//
//	// Rect를 사용해서 작업 영역에 대한 좌표를 얻고, 
//	CRect Rect; 
//	GetClientRect( &Rect ); 
//
//	// 그 위치를 현재 윈도우의 절대 좌표로 변경해 주자. 
//	CWnd::GetWindowRect( &Rect ); 
//
//	// CImage를 하나 만들고 
//	CImage Image; 
//
//	// 스캔을 시작할 x, y 위치와 
//	int sx = Rect.left; 
//	int sy = Rect.top; 
//
//	// 작업 영역에 대한 크기를 각각 임시로 저장해 두자. 
//
//	int cx = Rect.Width(); 
//	int cy = Rect.Height(); 
//
//	// 작업 영역의 크기만큼, 현재 바탕화면의 속성과 동일한 image를 생성한다. 
//	Image.Create( cx, cy, ScrDC.GetDeviceCaps( BITSPIXEL ) ); 
//
//	// 이미지 DC에 현재 작업 영역의 절대 좌표를 사용해 그 크기만큼 저장하게 한다. 
//	CDC* pDC = CDC::FromHandle( Image.GetDC() ); 
//	pDC->BitBlt( 0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY ); 
//
//	Image.ReleaseDC(); 
//
//	// 저장된 이미지를 원하는 파일명, 포멧타입을 지정해서 저장한다. 
//	Image.Save( sFile, Gdiplus::ImageFormatJPEG );			// TEXT("Desktop.jpg")
//
//	// 그 파일을 실행해 준다. 
//	// ::ShellExecute( NULL, TEXT("open"), TEXT("Desktop.jpg" ), NULL, NULL, SW_SHOW ); 
//}