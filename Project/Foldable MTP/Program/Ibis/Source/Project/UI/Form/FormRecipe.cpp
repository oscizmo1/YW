// FormRecipe.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "FormRecipe.h"
//GxGUIFrame 사용
#include "UI\GausGUI\GxGUIFrame.h"
#include "UI\GausGUI\GxUICtrl.h"
#include "UI\Dialog\DlgBoxKeyBoard.h"
#include <algorithm>
#include "UI\GausGUI\GxNumPadEx.h"
#include "UI\GausGUI\GxMsgBox.h"
#include "UI\GausGUI\GxButtonEx.h"
#include "UI\GausGUI\GxStaticEx.h"
#include "BankGuild\RecipeBank.h"
#include "ETC/FileSupport.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

// CFormRecipe

IMPLEMENT_DYNCREATE(CFormRecipe, CFormView)

CFormRecipe::CFormRecipe()
	: CFormView(CFormRecipe::IDD)
{
	m_iUpDownParam = 0;
	m_iSetVelNumIndex=1;
	m_iSelectedRow=1;
	m_bInit = FALSE;
	m_bEnableUI = FALSE;
}

CFormRecipe::~CFormRecipe()
{
	
}

void CFormRecipe::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRID_RECIPE_MODELLIST, m_GridModelList);
}

BEGIN_MESSAGE_MAP(CFormRecipe, CFormView)
	// 사용자 정의 메세지 처리
	// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
	ON_MESSAGE(GxMSG_GUI_UPDATE_TIMER,		OnGUIUpdateTimer)			// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
	ON_MESSAGE(GxMSG_GUI_CHANGE_LANGUAGE,	OnChangeLanguage)				// 표시 언어가 바뀌었다.
	// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

	ON_NOTIFY(NM_CLICK, IDC_GRID_RECIPE_MODELLIST, OnGridRecipeEndSelChange)
END_MESSAGE_MAP()


BEGIN_EVENTSINK_MAP(CFormRecipe, CFormView)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_CREATE, DISPID_CLICK, CFormRecipe::ClickGxbtnTeach1Create, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_DELETE, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeDelete, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_TEACH1_MOVE_UP, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeMoveUp, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_TEACH1_MOVE_DOWN, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeMoveDown, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_EDIT, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeEdit, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE1, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellWidth, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE2, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellHeight, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_ITEM_VALUE3, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeCellFPC, VTS_NONE)
	ON_EVENT(CFormRecipe, IDC_GXBTN_RECIPE_SAVE, DISPID_CLICK, CFormRecipe::ClickGxbtnRecipeSave, VTS_NONE)
END_EVENTSINK_MAP()

// CFormRecipe 진단입니다.

#ifdef _DEBUG
void CFormRecipe::AssertValid() const
{
	CFormView::AssertValid();
}

#ifndef _WIN32_WCE
void CFormRecipe::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif
#endif //_DEBUG


// CFormRecipe 메시지 처리기입니다.



// GxBaseForm -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --
//
// CGxGUIFrame에서 ShowDialog를 수행하면 아래 순서대로 메세지를 호출한다.
//
// ShowDialog() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, TRUE
//
// HideDlgBar() 호출시
// 1. GxMSG_GUI_UPDATE_TIMER, FALSE
//

// View/Dialog의 화면 갱신용 타이머의 구동 및 정지 지정
LRESULT CFormRecipe::OnGUIUpdateTimer(WPARAM wParam, LPARAM lParam)
{
	BOOL bFlag = (BOOL)wParam;

	if ( bFlag )
	{
		if(m_bInit == FALSE)
		{
			RecipeName_InitUI();
			RecipeItem_InitValue();
			m_bInit = TRUE;
		}
		m_bEnableUI = FALSE;
		RecipeName_Update();
		RecipeBank_LoadFile(m_iSetVelNumIndex-1);
		RecipeItem_Value_to_UI();
		RecipeItem_EnableUI(m_bEnableUI);
		m_GridModelList.SetSelectedRange(m_iSelectedRow,1,m_iSelectedRow,1);
	}
	else
	{

	}

	return 0;
}

// 표시 언어가 변경되었다.
LRESULT CFormRecipe::OnChangeLanguage(WPARAM wParam, LPARAM lParam)
{
	// 지정한 언어로 표시를 변경하도록 한다.
	CGxUICtrl::BroadcastButtonCpationIndex(this, wParam, IDC_GXBTN_RECIPE_CREATE);

	return 0;
}

// end of GxBaseForm  -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- -- --

// 윈도우 생성전에 모양을 달리 지정한다.
BOOL CFormRecipe::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	cs.style = WS_OVERLAPPED | WS_CHILD;				// 테두리를 없앤다.

	return CFormView::PreCreateWindow(cs);
}



void CFormRecipe::ClickGxbtnTeach1Create()
{
	CDlgBoxKeyBoard dlgKey;
	CString sztemp;
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("NEW CREAT"));
	if ( dlgKey.DoModal() == IDOK )
	{
		CString szInput = dlgKey.m_strReturnText;
		
		// 중복검사
		if ( std::find(m_vecRecipeName.begin(), m_vecRecipeName.end(), szInput) != m_vecRecipeName.end() )
		{
			CGxMsgBox	dlgMsgBox;

			dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
			dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
			dlgMsgBox.SetMessage(FALSE, 
				_T("이미 존재하고 있는 레시피 입니다."), 
				_T("A recipe that already exist."),		
				_T("Đang tồn tại một Recipe.") , GetMainHandler()->m_nLangIdx);

			dlgMsgBox.DoModal();
			return;
		}

		m_RecipeBank.m_strRecipeName=szInput;
		m_RecipeBank.SaveMainRecipeFile();

		RecipeName_Update();
		RecipeName_ResetSelection();

		// Recipe 생성 보고 [11/30/2016 OSC]
		theSocketInterFace.m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_CREATE, szInput);
	}
}

void CFormRecipe::RecipeName_InitUI()
{
	if(m_GridModelList.GetSafeHwnd() == NULL) return;

	//Grid 설정
	m_GridModelList.SetEditable(FALSE);
	m_GridModelList.SetDefCellHeight(30);
	m_GridModelList.SetFixedColumnCount(1); 
	m_GridModelList.SetFixedRowCount(1);
	m_GridModelList.SetRowCount(4);
	m_GridModelList.SetColumnCount(2);
	m_GridModelList.SetRowResize(FALSE);
	m_GridModelList.SetColumnResize(FALSE);		
	m_GridModelList.SetListMode(FALSE);
	

	//간격 설정
	m_GridModelList.SetColumnWidth(0, 25); 
	m_GridModelList.SetColumnWidth(1, 360);	

	//이름설정
	m_GridModelList.SetItemText(0,0,_T("NO"));		
	m_GridModelList.SetItemText(0,1,_T("Name"));	
	
	m_GridModelList.Refresh();

	// 폰트
	m_dwTextStyle = DT_RIGHT|DT_CENTER|DT_SINGLELINE|DT_VCENTER|DT_SINGLELINE;
	
	CFont* pGridFont = m_GridModelList.GetFont();
	pGridFont->GetLogFont(&m_lf);

	memcpy(m_lf.lfFaceName, _T("굴림체"),sizeof(_T("굴림체")));
	m_lf.lfHeight = 14;
	m_lf.lfWeight = FW_BOLD;

	for(int i = 0; i < m_GridModelList.GetRowCount(); i++)
	{	
		for(int j = 0; j<m_GridModelList.GetColumnCount(); j++)
		{
			m_GridModelList.SetItemFont(i, j, &m_lf);
			m_GridModelList.SetItemFormat(i, j, m_dwTextStyle);

			if ( i == 0 )				m_GridModelList.SetRowHeight(i, 35);			//	높이
			else						m_GridModelList.SetRowHeight(i, 50);			//  높이

		}
	}
}

void CFormRecipe::RecipeName_to_Vector()
{
	m_vecRecipeName.clear();

	CString input;
	CString dir;
	CFileFind finder;
	input.Format(_T("%s\\*"), theConfigBank.m_System.m_strRecipePath);

	BOOL bWorking = finder.FindFile(input);

	while (bWorking)
	{    
		bWorking = finder.FindNextFile();

		if(finder.IsDirectory() && !finder.IsDots())
		{
			dir= finder.GetFileName();
			input.Delete(input.GetLength()-1);
			m_vecRecipeName.push_back(dir);

		}
	}
}

void CFormRecipe::RecipeName_Update()
{
	RecipeName_to_Vector();

	CString sNum;

	m_GridModelList.SetRowCount(10);

	for(int i = 0; i < m_GridModelList.GetRowCount(); i++ )
	{
		if ( i == 0 )	continue;
		else			m_GridModelList.SetRowHeight(i, 35);

		sNum.Format(_T("%d"),i+m_iUpDownParam);
		for(int j = 0; j < m_GridModelList.GetColumnCount(); j++ )
		{
			m_GridModelList.SetItemFont(i, j, &m_lf);
			m_GridModelList.SetItemFormat(i, j, m_dwTextStyle);

			if ( j == 0 )	m_GridModelList.SetItemText(i, j, sNum);
			else			
			{
				CString szText;
				if ( m_vecRecipeName.size() > UINT(i-1 + m_iUpDownParam) )
					m_GridModelList.SetItemText(i, j, m_vecRecipeName[i - 1 + m_iUpDownParam] );
				else
					m_GridModelList.SetItemText(i, j, _T(""));
			}
		}
	}
	m_GridModelList.ShowScrollBar(SB_HORZ,FALSE);		//	횡 스크롤 제거
	m_GridModelList.ShowScrollBar(SB_VERT,FALSE);		//	종 스크롤 제거
	m_GridModelList.Refresh();
}

void CFormRecipe::RecipeName_ResetSelection()
{
	// EnableSelection을 변경하면 기존 선택한게 해제된다 [9/16/2017 OSC]
	m_GridModelList.EnableSelection(FALSE);
	m_GridModelList.EnableSelection(TRUE);
	m_iSetVelNumIndex = 0;

	RecipeItem_UIClear();
}

void CFormRecipe::RecipeBank_LoadFile( int nSelectedItem )
{
	if(m_vecRecipeName.size() <= (UINT)nSelectedItem)
		return;
	m_RecipeBank.ReadRecipeFile(m_vecRecipeName[nSelectedItem]);
}

void CFormRecipe::RecipeItem_InitValue()
{
	CUIValue_Double valueDouble;
	CUIValue_BOOL valueBOOL;

	m_vecItemDouble.clear();
	valueDouble.ValuePtr_Link(IDC_GXBTN_RECIPE_ITEM_VALUE1, &m_RecipeBank.m_Cell.m_dWidth);
	m_vecItemDouble.push_back(valueDouble);
	valueDouble.ValuePtr_Link(IDC_GXBTN_RECIPE_ITEM_VALUE2, &m_RecipeBank.m_Cell.m_dHeight);
	m_vecItemDouble.push_back(valueDouble);

	m_vecItemBOOL.clear();
	valueBOOL.ValuePtr_Link(IDC_GXBTN_RECIPE_ITEM_VALUE3, &m_RecipeBank.m_Cell.m_bFPCBuse);
	m_vecItemBOOL.push_back(valueBOOL);
}

void CFormRecipe::RecipeItem_UIClear()
{	
	int nCount = m_vecItemDouble.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticString(this, m_vecItemDouble[i].m_nID, _T(""));
	}

	nCount = m_vecItemBOOL.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticString(this, m_vecItemBOOL[i].m_nID, _T(""));
	}

	int nIDAZone[10] = 
	{
		IDC_GXLBL_RECIPE_AZONE_SEQ01,
		IDC_GXLBL_RECIPE_AZONE_SEQ02,
		IDC_GXLBL_RECIPE_AZONE_SEQ03,
		IDC_GXLBL_RECIPE_AZONE_SEQ04,
		IDC_GXLBL_RECIPE_AZONE_SEQ05,
		IDC_GXLBL_RECIPE_AZONE_SEQ06,
		IDC_GXLBL_RECIPE_AZONE_SEQ07,
		IDC_GXLBL_RECIPE_AZONE_SEQ08,
		IDC_GXLBL_RECIPE_AZONE_SEQ09,
		IDC_GXLBL_RECIPE_AZONE_SEQ10
	};
	int nIDCZone[10] = 
	{
		IDC_GXLBL_RECIPE_CZONE_SEQ01,
		IDC_GXLBL_RECIPE_CZONE_SEQ02,
		IDC_GXLBL_RECIPE_CZONE_SEQ03,
		IDC_GXLBL_RECIPE_CZONE_SEQ04,
		IDC_GXLBL_RECIPE_CZONE_SEQ05,
		IDC_GXLBL_RECIPE_CZONE_SEQ06,
		IDC_GXLBL_RECIPE_CZONE_SEQ07,
		IDC_GXLBL_RECIPE_CZONE_SEQ08,
		IDC_GXLBL_RECIPE_CZONE_SEQ09,
		IDC_GXLBL_RECIPE_CZONE_SEQ10
	};
	for(int i = 0; i < 10; i++)
	{
		CGxUICtrl::SetStaticString(this, nIDAZone[i], _T(""));
		CGxUICtrl::SetStaticString(this, nIDCZone[i], _T(""));
		CGxUICtrl::SetStaticColor(this, nIDAZone[i], GXCOLOR_WHITE);
		CGxUICtrl::SetStaticColor(this, nIDCZone[i], GXCOLOR_WHITE);
	}
}

void CFormRecipe::RecipeItem_Value_to_UI()
{
	int nCount = m_vecItemDouble.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticString(this, m_vecItemDouble[i].m_nID, m_vecItemDouble[i].ReturnString());
	}

	nCount = m_vecItemBOOL.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticString(this, m_vecItemBOOL[i].m_nID, m_vecItemBOOL[i].ReturnString());
	}

	InspModuleItem_Show();
}

void CFormRecipe::RecipeItem_UI_to_Value()
{
	CGxStaticEx *pStc;
	CString str;

	int nCount = m_vecItemDouble.size();
	for(int i = 0; i < nCount; i++)
	{
		pStc = (CGxStaticEx *)GetDlgItem(m_vecItemDouble[i].m_nID);
		str = pStc->GetCaption();
		m_vecItemDouble[i].SetValueString(str);
	}

	nCount = m_vecItemBOOL.size();
	for(int i = 0; i < nCount; i++)
	{
		pStc = (CGxStaticEx *)GetDlgItem(m_vecItemBOOL[i].m_nID);
		str = pStc->GetCaption();
		m_vecItemBOOL[i].SetValueString(str);
	}
}

void CFormRecipe:: RecipeItem_EnableUI(BOOL flag)
{
	int nCount = m_vecItemDouble.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticColor(this, m_vecItemDouble[i].m_nID, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
	}

	nCount = m_vecItemBOOL.size();
	for(int i = 0; i < nCount; i++)
	{
		CGxUICtrl::SetStaticColor(this, m_vecItemBOOL[i].m_nID, ((flag) ?		GXCOLOR_WHITE : GXCOLOR_OFF));
	}
}


void CFormRecipe::InspModuleItem_Show()
{
	int nIDAZone[10] = 
	{
		IDC_GXLBL_RECIPE_AZONE_SEQ01,
		IDC_GXLBL_RECIPE_AZONE_SEQ02,
		IDC_GXLBL_RECIPE_AZONE_SEQ03,
		IDC_GXLBL_RECIPE_AZONE_SEQ04,
		IDC_GXLBL_RECIPE_AZONE_SEQ05,
		IDC_GXLBL_RECIPE_AZONE_SEQ06,
		IDC_GXLBL_RECIPE_AZONE_SEQ07,
		IDC_GXLBL_RECIPE_AZONE_SEQ08,
		IDC_GXLBL_RECIPE_AZONE_SEQ09,
		IDC_GXLBL_RECIPE_AZONE_SEQ10
	};
	int nIDCZone[10] = 
	{
		IDC_GXLBL_RECIPE_CZONE_SEQ01,
		IDC_GXLBL_RECIPE_CZONE_SEQ02,
		IDC_GXLBL_RECIPE_CZONE_SEQ03,
		IDC_GXLBL_RECIPE_CZONE_SEQ04,
		IDC_GXLBL_RECIPE_CZONE_SEQ05,
		IDC_GXLBL_RECIPE_CZONE_SEQ06,
		IDC_GXLBL_RECIPE_CZONE_SEQ07,
		IDC_GXLBL_RECIPE_CZONE_SEQ08,
		IDC_GXLBL_RECIPE_CZONE_SEQ09,
		IDC_GXLBL_RECIPE_CZONE_SEQ10
	};

	int nIndex = 0, nCount = 0;
	std::vector<CString> *pVector;

	pVector = &m_RecipeBank.m_Module.m_vct_AZone_Bef;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDAZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDAZone[nIndex], GXCOLOR_WHITE);
		nIndex++;
	}
	pVector = &m_RecipeBank.m_Module.m_vct_AZone_Must;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDAZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDAZone[nIndex], GXCOLOR_BLUE_VIOLET);
		nIndex++;
	}
	pVector = &m_RecipeBank.m_Module.m_vct_AZone_Aft;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDAZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDAZone[nIndex], GXCOLOR_WHITE);
		nIndex++;
	}
	// 나머지 빈칸은 공백으로 채운다 [9/16/2017 OSC]
	for(; nIndex < 10; nIndex++)
	{
		CGxUICtrl::SetStaticString(this, nIDAZone[nIndex], _T(""));
		CGxUICtrl::SetStaticColor(this, nIDAZone[nIndex], GXCOLOR_WHITE);
	}

	nIndex = 0;
	pVector = &m_RecipeBank.m_Module.m_vct_CZone_Bef;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDCZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDCZone[nIndex], GXCOLOR_WHITE);
		nIndex++;
	}
	pVector = &m_RecipeBank.m_Module.m_vct_CZone_Must;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDCZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDCZone[nIndex], GXCOLOR_BLUE_VIOLET);
		nIndex++;
	}
	pVector = &m_RecipeBank.m_Module.m_vct_CZone_Aft;
	nCount = pVector->size();
	for(int i = 0; i < nCount; i++)
	{
		if(nIndex >= 10)	break;
		CGxUICtrl::SetStaticString(this, nIDCZone[nIndex], pVector->at(i));
		CGxUICtrl::SetStaticColor(this, nIDCZone[nIndex], GXCOLOR_WHITE);
		nIndex++;
	}

	// 나머지 빈칸은 공백으로 채운다 [9/16/2017 OSC]
	for(; nIndex < 10; nIndex++)
	{
		CGxUICtrl::SetStaticString(this, nIDCZone[nIndex], _T(""));
		CGxUICtrl::SetStaticColor(this, nIDCZone[nIndex], GXCOLOR_WHITE);
	}
}

void CFormRecipe::OnGridRecipeEndSelChange(NMHDR *pNotifyStruct, LRESULT *pResult)
{
	CString sztemp;
	NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;	

	if ( pItem->iRow < 1 )		return;				// 메뉴 정의된 Row Click시 return

	m_iSelectedRow = pItem->iRow;
	sztemp=m_GridModelList.GetItemText(m_iSelectedRow,0);
	m_iSetVelNumIndex= _ttoi(sztemp);

	RecipeBank_LoadFile(m_iSetVelNumIndex-1);
	RecipeItem_Value_to_UI();

	//모델 편집 버튼 실행여부 초기화
	m_bEnableUI=FALSE;
	RecipeItem_EnableUI(m_bEnableUI);
		
}

void CFormRecipe::ClickGxbtnRecipeDelete()
{
	CGxMsgBox	dlgMsgBox;
	int i=0;
	if ( m_iSetVelNumIndex == 0 )	return;
	if ( m_vecRecipeName.size() < UINT(m_iSetVelNumIndex) )
		return;
	

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("정말로 삭제하시겠습니까?"), 
		_T("Are you sure you want to Delete it?"),		
		_T("Bạn chắc chắn muốn xóa?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	
		
	std::vector<CString>::iterator iter = m_vecRecipeName.begin();
	m_bEnableUI=FALSE;
	CString strRecipeName;
		
	while(iter != m_vecRecipeName.end() )
	{
		if ( iter->GetString() == m_vecRecipeName[m_iSetVelNumIndex-1] )
		{
			if(theRecipeBank.m_strRecipeName== m_vecRecipeName[m_iSetVelNumIndex-1] )
			{
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("DELETE"));
				//현재 사용중이므로 삭제가 불가능 하다는 메세지 출력
				CGxMsgBox	dlgMsgBox;

				dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
				dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
				dlgMsgBox.SetMessage(FALSE, 
					_T("현재 사용중인 레시피 입니다."), 
					_T("The recipe currently in use."),		
					_T("Recipe đang sử dụng hiện tại."), GetMainHandler()->m_nLangIdx );

				dlgMsgBox.DoModal();
				return;
			}

			strRecipeName = iter->GetString();
			CString path;
			path.Format(_T("%s\\"+strRecipeName), theConfigBank.m_System.m_strRecipePath);
			CFileSupport::DeleteDirectory( (LPCTSTR)path); //DeleteDirectory(dir);
			break;
		}
		else
		{
			++iter;
			++i;
		}
	}

	RecipeName_Update();
	RecipeName_ResetSelection();
		
	// Recipe 삭제 보고 [11/30/2016 OSC]
	theSocketInterFace.m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_DELETE, strRecipeName);
	m_iSetVelNumIndex = 0;
}


void CFormRecipe::ClickGxbtnRecipeMoveUp()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("MOVE UP"));
	if ( m_iUpDownParam - 5 < 0  )
		m_iUpDownParam = 0;
	else
		m_iUpDownParam -=5;

	RecipeName_Update();
	RecipeName_ResetSelection();
}


void CFormRecipe::ClickGxbtnRecipeMoveDown()
{
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("MOVE DOWN"));
	int a = m_iUpDownParam + 5;
	int b = m_vecRecipeName.size();

	if (a >= b)		return;
	else
		m_iUpDownParam +=5;

	RecipeName_Update();
	RecipeName_ResetSelection();
}


void CFormRecipe::ClickGxbtnRecipeEdit()
{
	// 레시피 창에서 모델편집이 가능하도록 변경
	if(m_vecRecipeName.size()==0)
	{
		return;
	}
	if(m_iSetVelNumIndex == 0)
		return;

	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("RECIPEEDIT"));
	m_bEnableUI=TRUE;
	RecipeItem_EnableUI(m_bEnableUI);
			
}



void CFormRecipe::ClickGxbtnRecipeCellWidth()
{
	if(m_bEnableUI)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		CGxStaticEx * pStc = (CGxStaticEx *)GetDlgItem(IDC_GXBTN_RECIPE_ITEM_VALUE1);
		double dOldValue = _tstof(pStc->GetCaption());
		dlgNumPad.SetTitle(_T("Cell Setiing width Change"));
		dlgNumPad.SetValue(0.0, 500.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE1, dlgNumPad.GetValue());
				
		}
		else// 모델 편집 버튼 클릭 하지 않았다면
			return;
	}
}





void CFormRecipe::ClickGxbtnRecipeCellHeight()
{
	if(m_bEnableUI)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxNumPadEx	dlgNumPad;
		CGxStaticEx * pStc = (CGxStaticEx *)GetDlgItem(IDC_GXBTN_RECIPE_ITEM_VALUE2);
		double dOldValue = _tstof(pStc->GetCaption());
		dlgNumPad.SetTitle(_T("Cell Setiing height Change"));
		dlgNumPad.SetValue(0.0, 500.0, dOldValue);
		if ( dlgNumPad.DoModal() == IDOK )
		{
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE2, dlgNumPad.GetValue());

		}
		else// 모델 편집 버튼 클릭 하지 않았다면
			return;
	}
}


void CFormRecipe::ClickGxbtnRecipeCellFPC()
{
	if(m_bEnableUI)// 모델 편집 버튼 클릭이 되었다면
	{
		CGxStaticEx * pStc = (CGxStaticEx *)GetDlgItem(IDC_GXBTN_RECIPE_ITEM_VALUE3);
		if(pStc->GetCaption()==_T("Y"))
		{
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3,_T("N"));
		}
		else
		{
			CGxUICtrl::SetStaticString(this, IDC_GXBTN_RECIPE_ITEM_VALUE3,_T("Y"));
		}
	}
}


void CFormRecipe::ChangeOnOffValue( int nBtnID, int nIndex )
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	if(pOnOff)
	{
		if(pOnOff->GetCaption() ==_T("OFF"))
		{
			pOnOff->SetCaption(_T("ON"));
			pOnOff->SetBackColor(GXCOLOR_ON);
		}
		else
		{
			pOnOff->SetCaption(_T("OFF"));
			pOnOff->SetBackColor(GXCOLOR_OFF);
		}
	}
}

void CFormRecipe::GetUI_to_OnOffValue( BOOL &bValue, int nBtnID, CString strName /*= _T("")*/ )
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	BOOL bUIValue;
	if(pOnOff)
	{
		pOnOff->GetCaption()==(_T("ON")) ? (bUIValue = TRUE):(bUIValue = FALSE);
		if(bValue != bUIValue)
		{
			if(strName.GetLength() > 0)
				theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	%s : %d -> %d"), _T("RECIPE"), strName, bValue, bUIValue);
			bValue = bUIValue;
		}
	}
}

void CFormRecipe::UpdateOnoffValue_to_UI(CString strOnOff, int nBtnID)
{
	CGxButtonEx* pOnOff = (CGxButtonEx*)GetDlgItem(nBtnID);
	if(pOnOff)
	{
		if(strOnOff == _T("ON"))
		{
			pOnOff->SetCaption(_T("ON"));
			pOnOff->SetBackColor(GXCOLOR_ON);
		}
		else
		{
			pOnOff->SetCaption(_T("OFF"));
			pOnOff->SetBackColor(GXCOLOR_OFF);
		}
	}
}

void CFormRecipe::ClickGxbtnRecipeSave()
{
	if(m_iSetVelNumIndex == 0)
		return;
	if(m_bEnableUI == FALSE)// 모델 편집 버튼 클릭이 되었다면
		return;
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	CString sname;
	// 변경된 임시 저장값들을 실제 저장값에 적용
	// 레시피 창에서 모델편집이 가능하도록 변경


	CGxMsgBox	dlgMsgBox;

	dlgMsgBox.SetLangName(2, _T("VTM"));		// 언어의 이름을 변경
	dlgMsgBox.SetTitle(_T("경고"), _T("Warning"), _T("Warning"));
	dlgMsgBox.SetMessage(TRUE, 
		_T("수정하신 작업을 적용하겠습니까?"), 
		_T("Would you like to apply your modifications?"),		
		_T("Bạn có muốn áp dụng các thay đổi?"), GetMainHandler()->m_nLangIdx );

	int nRet = dlgMsgBox.DoModal();
	if(nRet != IDOK)
		return;
	theLog[LOG_OPERATION].AddBuf(_T("[%-15s]	Click '%s'"), _T("RECIPE"), _T("MODIFYSAVE"));

	sname.Format(_T("%s"),m_GridModelList.GetItemText(m_iSelectedRow,1));
	RecipeItem_UI_to_Value();
	m_RecipeBank.m_strRecipeName=sname;
	m_RecipeBank.SaveMainRecipeFile();

	m_bEnableUI=FALSE;
	RecipeItem_EnableUI(m_bEnableUI);

	// 현재 Recipe를 수정한 거라면 새로 불러온다 [11/30/2016 OSC]
	if(m_RecipeBank.m_strRecipeName == theRecipeBank.m_strRecipeName)
		theRecipeBank.ReadRecipeFile(theRecipeBank.m_strRecipeName);

	// Recipe 수정 보고 [11/30/2016 OSC]
	theSocketInterFace.m_CIM.SendCmdRecipeToDataPC(EQUIP_SET_PPID_MODIFY, m_RecipeBank.m_strRecipeName);
}
