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
// GausGUI ���� ������ Modal Dialog�� ������ ��������ش�. Dialog ������ IDABORT�� ������, ���̻� Dialog�� ã�� ������ FALSE�� �����Ѵ�.
//
BOOL CGxUICtrl::AbortModalDialog()
{
	HWND hWnd = ::FindWindow(NULL, _T("GausGUI"));			// GausGUI�� Ÿ��Ʋ�� ���� Window�� ã�´�.

	if( hWnd )			// �����Ѵٸ�
	{
		::SendMessage(hWnd, WM_COMMAND, IDABORT, 0);			// ABORT ��� ����

		return TRUE;													// �ش� Window�� ã�Ƽ� ���� ó���� �õ��ߴ�.
	}

	return FALSE;														// ���̻� �ش� Window�� ã�� �� ����.
}

//
// ������ ���� ��� GxOCX�迭�� Control���� ǥ�� ǥ�� �� ����Ǿ����� �뺸�Ѵ�.
//
void CGxUICtrl::BroadcastButtonCpationIndex(CWnd* pView, UINT nIdx, UINT nID)
{
	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));			

	if ( pBtn )
	{
		pBtn->GxBroadcastChangeCaptionIndex( (OLE_HANDLE)(pView->GetSafeHwnd()), nIdx + 1);
	}
}


// ������ ���� ��� GxOCX�迭�� Control���� ǥ�� ǥ�� �� ����Ǿ����� �뺸�Ѵ�.
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
// ���� ��ư�� ���� �����Ѵ�.
void CGxUICtrl::SetButtonColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		if ( pBtn->GetBackColor() != cColor ) pBtn->SetBackColor( cColor );			// ��ư�� ������ ���� ������ ��������ش�.
	}
}


// ���� ��ư�� ���� ���� �����Ѵ�.
void CGxUICtrl::SetButtonTextColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		if ( pBtn->GetTextColor() != cColor ) pBtn->SetTextColor( cColor );			// ��ư�� ���� ������ ���� ������ ��������ش�.
	}
}



// ������ ���¿����� ��ư�� �������� �����Ų��.
//
//  UINT nIdx : Icon ����, 0 ~ 
void CGxUICtrl::SetButtonIcon(CWnd* pView, UINT nID, UINT nIdx)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		pBtn->SetIconIndex( nIdx );				// ��ư�� �������� ������ �������� ��������ش�.
	}
}



// ������ ���ڿ��� ��ư�� ǥ�����ش�.
void CGxUICtrl::SetButtonString(CWnd* pView, UINT nID, CString sMsg)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		pBtn->SetCaption( sMsg );																	// ������ ���ڿ��� ����Ѵ�.
	}
}


// ������ �������� ��ư�� ǥ�����ش�.
void CGxUICtrl::SetButtonInt(CWnd* pView, UINT nID, int nValue)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));						// �� ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		CString sMsg;
		sMsg.Format(_T("%d"), nValue);

		pBtn->SetCaption( sMsg );																	// ������ ���ڿ��� ����Ѵ�.
	}
}


// ������ ���ڿ��� �󺧿� ǥ�����ش�.
void CGxUICtrl::SetButtonDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint)
{
	if ( nID <= 0 ) return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));					// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		CString sMsg, sFmt;
		sFmt.Format(_T("%%.%df"), nPoint);													// �Ҽ��� �ڸ��� ǥ��
		sMsg.Format(sFmt, dbValue);

		pBtn->SetCaption( sMsg );																// ������ ���ڿ��� ����Ѵ�.
	}
}



// ���� ��ư�� ���/�̻���� �����Ѵ�.
void CGxUICtrl::SetButtonEnabled(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));				// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		pBtn->SetEnable( bFlag );
	}
}


// ���� ��ư�� ���̱�/���߱⸦ �����Ѵ�.
void CGxUICtrl::SetButtonVisible(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));		// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		// Flag�� ���� ��ü�� ���߰ų� �����ش�.
		if ( bFlag == TRUE ) 
		{
			pBtn->ShowWindow( SW_SHOW );
		}
		else
			pBtn->ShowWindow( SW_HIDE );
	}
}


// ���� Static�� Blink ����� Ȱ��/��Ȱ��ȭ ��Ų��.
void CGxUICtrl::SetButtonBlink(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxButtonEx* pBtn = (CGxButtonEx*)(pView->GetDlgItem( nID ));		// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		// Flag�� ���� ��ü�� ���߰ų� �����ش�.
		if ( bFlag == TRUE ) 
		{
			pBtn->StartBlink();
		}
		else
			pBtn->StopBlink();
	}
}





/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ������ �󺧿� ������ �����Ѵ�.
void CGxUICtrl::SetStaticColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pLbl )
	{
		pLbl->SetBackColor( cColor );												// ������ ��������ش�.
	}
}


// ������ �󺧿� ���� ������ �����Ѵ�.
void CGxUICtrl::SetStaticTextColor(CWnd* pView, UINT nID, COLORREF cColor)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pLbl )
	{
		pLbl->SetTextColor( cColor );												// ���� ������ ��������ش�.
	}
}


// ������ ���¸� �󺧿� ǥ�����ش�.
void CGxUICtrl::SetStaticIcon(CWnd* pView, UINT nID, UINT nIdx)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// ��ư ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pLbl )
	{
		pLbl->SetIconIndex( nIdx );				// ��ư�� �������� ������ �������� ��������ش�.
	}
}


// ������ ���ڿ��� �󺧿� ǥ�����ش�.
void CGxUICtrl::SetStaticString(CWnd* pView, UINT nID, CString sMsg)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// �� ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pLbl )
	{
		pLbl->SetCaption( sMsg );													// ������ ���ڿ��� ����Ѵ�.
	}
}


// ������ �������� �󺧿� ǥ�����ش�.
void CGxUICtrl::SetStaticInt(CWnd* pView, UINT nID, int nValue)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// �� ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pLbl )
	{
		CString sMsg;
		sMsg.Format(_T("%d"), nValue);

		pLbl->SetCaption( sMsg );												// ������ ���ڿ��� ����Ѵ�.
	}
}


// ������ Double�� ������(�Ҽ���) �󺧿� ǥ�����ش�.
void CGxUICtrl::SetStaticDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pLbl = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// �� ���ҽ��� �����Ͽ� ������ ȹ��;


	if ( pLbl )
	{
		CString sMsg, sFmt;
		sFmt.Format(_T("%%.%df"), nPoint);										// �Ҽ��� �ڸ��� ǥ��
		sMsg.Format(sFmt, dbValue);

		pLbl->SetCaption( sMsg );													// ������ ���ڿ��� ����Ѵ�.
	}
}


// ���� Static�� ���̱�/���߱⸦ �����Ѵ�.
void CGxUICtrl::SetStaticVisible(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pBtn = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// Static ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		// Flag�� ���� ��ü�� ���߰ų� �����ش�.
		if ( bFlag == TRUE ) 
		{
			pBtn->ShowWindow( SW_SHOW );
		}
		else
			pBtn->ShowWindow( SW_HIDE );
	}
}


// ���� Static�� Blink ����� Ȱ��/��Ȱ��ȭ ��Ų��.
void CGxUICtrl::SetStaticBlink(CWnd* pView, UINT nID, BOOL bFlag)
{
	if ( nID <= 0 )		return;
	if ( pView == NULL ) return;

	CGxStaticEx* pBtn = (CGxStaticEx*)(pView->GetDlgItem( nID ));		// Static ���ҽ��� �����Ͽ� ������ ȹ��;

	if ( pBtn )
	{
		// Flag�� ���� ��ü�� ���߰ų� �����ش�.
		if ( bFlag == TRUE ) 
		{
			pBtn->StartBlink();
		}
		else
			pBtn->StopBlink();
	}
}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


// ��ġ ���� �Լ�
void CGxUICtrl::SetTopPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint)				// ��/�� ��ġ�� �������ش�.
{
	CRect rect;
	pOwner->GetDlgItem( nID )->GetWindowRect( &rect );		// ���� ��ġ
	pOwner->ScreenToClient( &rect );

	int nHeight		= rect.Height();
	rect.top			= lNewPos;									// ���ο� ��ġ, 
	rect.bottom		= rect.top + nHeight;					// ���̸� ������ ���߾� �ش�.
	pOwner->GetDlgItem( nID )->MoveWindow(&rect, bRepaint);
}

void CGxUICtrl::SetLeftPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint)				// ��/�� ��ġ�� �������ش�.
{
	CRect rect;
	pOwner->GetDlgItem( nID )->GetWindowRect( &rect );		// ���� ��ġ
	pOwner->ScreenToClient( &rect );

	int nWidth		= rect.Width();
	rect.left		= lNewPos;									// ���ο� ��ġ, 
	rect.right		= rect.left + nWidth;					// �ʺ� ������ ���߾� �ش�.
	pOwner->GetDlgItem( nID )->MoveWindow(&rect, bRepaint);
}






//
// ���� ȭ���� �����Ѵ�.
//
//void SaveScreen(CString sFile)
//{
//	 // ���� ȭ�� ������ ��ü�� ���� �����͸� ��´�. 
//	CWnd* pWndDesktop = GetDesktopWindow(); 
//
//	// ���� ȭ�� ������ DC 
//	CWindowDC ScrDC(pWndDesktop); 
//
//	// �� ������ DC 
//	CClientDC dc(this); 
//
//	// Rect�� ����ؼ� �۾� ������ ���� ��ǥ�� ���, 
//	CRect Rect; 
//	GetClientRect( &Rect ); 
//
//	// �� ��ġ�� ���� �������� ���� ��ǥ�� ������ ����. 
//	CWnd::GetWindowRect( &Rect ); 
//
//	// CImage�� �ϳ� ����� 
//	CImage Image; 
//
//	// ��ĵ�� ������ x, y ��ġ�� 
//	int sx = Rect.left; 
//	int sy = Rect.top; 
//
//	// �۾� ������ ���� ũ�⸦ ���� �ӽ÷� ������ ����. 
//
//	int cx = Rect.Width(); 
//	int cy = Rect.Height(); 
//
//	// �۾� ������ ũ�⸸ŭ, ���� ����ȭ���� �Ӽ��� ������ image�� �����Ѵ�. 
//	Image.Create( cx, cy, ScrDC.GetDeviceCaps( BITSPIXEL ) ); 
//
//	// �̹��� DC�� ���� �۾� ������ ���� ��ǥ�� ����� �� ũ�⸸ŭ �����ϰ� �Ѵ�. 
//	CDC* pDC = CDC::FromHandle( Image.GetDC() ); 
//	pDC->BitBlt( 0, 0, cx, cy, &ScrDC, sx, sy, SRCCOPY ); 
//
//	Image.ReleaseDC(); 
//
//	// ����� �̹����� ���ϴ� ���ϸ�, ����Ÿ���� �����ؼ� �����Ѵ�. 
//	Image.Save( sFile, Gdiplus::ImageFormatJPEG );			// TEXT("Desktop.jpg")
//
//	// �� ������ ������ �ش�. 
//	// ::ShellExecute( NULL, TEXT("open"), TEXT("Desktop.jpg" ), NULL, NULL, SW_SHOW ); 
//}