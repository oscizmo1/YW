#pragma once
#include "msflexgrid_pos.h"
#include "label.h"


// CModelRecipe dialog

class CModelRecipe : public CDialog
{
	DECLARE_DYNAMIC(CModelRecipe)

public:
	CModelRecipe(CWnd* pParent = NULL);   // standard constructor
	virtual ~CModelRecipe();

// Dialog Data
	enum { IDD = IDD_DIALOG_MODEL_LOADING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	CString m_strContactType;
	CString strPanelSize;
	CString m_strContactNum;
	CString m_strContactor;
	CString m_strPCBPinNum;
	CString m_strPCBType;

	BOOL bTiltChk;

	virtual BOOL OnInitDialog();
	void SetGridTitleName(void);
	CMsflexgrid_pos m_gridModel;
	CMsflexgrid_pos m_gridDevice;

	void SelectModel(int nIndex);
	long m_nOldSelRow;

	CLabel m_lblPanelSize;
	DECLARE_EVENTSINK_MAP()
	void ClickLabelPanelSizeEdit();

	CLabel m_lblComment;
	void ClickLabelCommentEdit();
	CLabel m_lblAlignScore;
	void ClickLabelAlignScoreEdit();
	void ClickBtnClose();
	CLabel m_lblContactType;
	void ClickLabelContactTypeEdit();
	CLabel m_lblContactNum;
	void ClickLabelContactNumEdit();
	CLabel m_lblContactor;
	void ClickLabelContactorSelEdit();
	CLabel m_lblFpcendLength;
	CLabel m_lblPCBPinNum;
	CLabel m_lblPCBType;
	void ClickLabelFpcendEdit();
	void ClickLabelPinNumEdit();
	void ClickLabelPcbTypeEdit();
	void ClickGridModelname();
	CLabel m_lblModelName;
	void GetModelInfo(void);
};
