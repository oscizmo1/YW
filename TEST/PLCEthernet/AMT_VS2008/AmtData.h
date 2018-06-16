#pragma once

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Equipment General Infomation...

/////////////////////////////////////////////////////////////////////////////////////////////////////////
class CAmtData  
{
public:
	CAmtData();
	virtual ~CAmtData();


public:
	PLC_INFO					m_Melsec;
	MAIN_FLAG						m_Flag;
	EQ_GEN_INFO					m_EqGenInfo;
	STRUCT_MODEL_DATA			m_CurModelInfo;
	STRUCT_SEARCH_RET			m_SearchResult;
	STRUCT_STAGE_POS			StageContactPos;					// Contact Position.//
	STRUCT_STAGE_POS			PCBChangePos;						// PCB Change Position.//

	STRUCT_ALIGN_RESULT		m_AlignResult[MAX_CAM_NUM];

	double  QUAD_NOTUSEDAXIS_POS;

	enum	_MODEL_TYPE	{ FFC_TYPE = 1 , FPC_TYPE , PIN_TYPE};
	enum	_CONTACT_NUM	{ ONE_CONT = 1 , TWO_CONT, FOUR_CONT = 4 };
	enum	_CONTACTOR	{ CONTACT_0 = 1 , CONTACT_1 , CONTACT_2, CONTACT_3};

};
