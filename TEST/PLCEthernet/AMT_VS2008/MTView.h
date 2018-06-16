// MTView.h : iCMTView 클래스의 인터페이스
//

#pragma once

#include "MIL71.h"
#include "CIF32.h"
#include "MMC39.h"
#include "ThreadLiveGrap.h"

class CMTView : public CView
{
protected: // serialization에서만 만들어집니다.
	CMTView();
	DECLARE_DYNCREATE(CMTView)

// 특성
public:
	CMTDoc* GetDocument() const;

// 작업
public:


// 재정의
	public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:

// 구현
public:
	virtual ~CMTView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 메시지 맵 함수를 생성했습니다.
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
};

#ifndef _DEBUG  // MTView.cpp의 디버그 버전
inline CMTDoc* CMTView::GetDocument() const
   { return reinterpret_cast<CMTDoc*>(m_pDocument); }
#endif


