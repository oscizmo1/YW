#pragma once


#include <atlimage.h>			// for CImage

#include "GausGUI.h"

// CGxImage
class _GAUS_GUI CGxImage : public CStatic
{
	DECLARE_DYNAMIC(CGxImage)

public:
	CGxImage();
	virtual ~CGxImage();


private:
	BOOL		m_bStretchMode;							// ��ü ũ�⿡ �µ��� �̹����� �ø��ų� ���̴� ����ΰ� ? �⺻��=FALSE
	BOOL		m_bLoadDone;
	CImage	m_ImgDisplay;
	CString	m_sFilename;								// �о���� ���� �̸�

public:

	BOOL	GetImageLoaded();								// �̹����� Load �Ͽ��� ?
	
	void	SetStretchMode(BOOL bMode);				// ��Ʈ��ġ ��带 �����Ѵ�.
	void	LoadImage(CString strFilename);
	void	ClearImage();									// �̹����� �����Ѵ�.
	CString GetFilename();								// �ҷ����� �̹��� �����̸�

	// 2015-09-22, jhLee, �߰��Լ�
	void DrawImage(CDC* pDC);							// �̹����� ���� �׷��ִ� �Լ�
	void SetText(int x, int y, CString sMsg);		// �̹����� ���ڸ� ����Ѵ�.

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
};


