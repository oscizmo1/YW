
// GaonNuriDoc.h : CGaonNuriDoc Ŭ������ �������̽�
//


#pragma once


class CGaonNuriDoc : public CDocument
{
protected: // serialization������ ��������ϴ�.
	CGaonNuriDoc();
	DECLARE_DYNCREATE(CGaonNuriDoc)

// Ư���Դϴ�.
public:

// �۾��Դϴ�.
public:

// �������Դϴ�.
public:
	virtual BOOL OnNewDocument();
	virtual void Serialize(CArchive& ar);

// �����Դϴ�.
public:
	virtual ~CGaonNuriDoc();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ������ �޽��� �� �Լ�
protected:
	DECLARE_MESSAGE_MAP()
};


