//
// CGxUICtrl : GxOCX׾�� ActiveX�� �����ϱ����� ���� ��ƿ��Ƽ �Լ� ���� class
//
// 2015-09-22, jhLee
//
//

#pragma once


////////////////////////////////////////////////////////////////////////////////////////////////////
// ���� Color ���� 

#ifndef _GxCOLOR_
#define _GxCOLOR_

	#define GXCOLOR_ON							RGB(0, 255, 0)				// ���õ� ��ư ��, ���� Lime
	#define GXCOLOR_OFF							RGB(224, 224, 224)			// ���õ��� ���� ��ư ��, ����

	#define GXCOLOR_SILVER						RGB(224, 224, 224)			// ���õ��� ���� ��ư ��, ����
	#define GXCOLOR_DARKGRAY					RGB(128, 128, 128)			// ��ο� ���
	#define GXCOLOR_LIME						RGB(191, 255, 0)			// ���õ� ��ư ��, Lime
	#define GXCOLOR_BLACK						RGB(0, 0, 0)					// ���ڻ�, ������
	#define GXCOLOR_WHITE						RGB(255, 255, 255)			// ���
	#define GXCOLOR_BLUE						RGB(0, 0, 255)				// ���ڻ� �Ķ���
	#define GXCOLOR_RED							RGB(255, 0, 0)				// ���ڻ� �Ķ���

	#define GXCOLOR_SELECT						RGB(192, 255, 255)			// ���� 192, 255, 192)
	#define GXCOLOR_DESELECT					RGB(224, 224, 224)			// ���� �ȵ� 192, 255, 192)

	#define GXCOLOR_INIT_READY					RGB(224, 224, 224)			// �ʱ�ȭ ���� ��
	#define GXCOLOR_INIT_RUN					RGB(192, 192, 255)			// �ʱ�ȭ ���� ��
	#define GXCOLOR_INIT_OK						RGB(192, 255, 192)			// �ʱ�ȭ ���� ����
	#define GXCOLOR_INIT_FAIL					RGB(255, 192, 192)			// �ʱ�ȭ ���� ����


	#define	GXCOLOR_LIGHT_PINK					RGB(255,	182,	193)
	#define	GXCOLOR_PINK						RGB(255,	192,	203)
	#define	GXCOLOR_CRIMSON						RGB(220,	20,		60)
	#define	GXCOLOR_LAVENDER_BLUSH				RGB(255,	240,	245)
	#define	GXCOLOR_PALE_VILOT_RED				RGB(219,	112,	147)
	#define	GXCOLOR_HOT_PINK					RGB(255,	105,	180)
	#define	GXCOLOR_DEEP_PINK					RGB(255,	20,		147)
	#define	GXCOLOR_MEDIUM_VILOT_RED			RGB(199,	21,		133)
	#define	GXCOLOR_ORCHID						RGB(218,	112,	214)
	#define	GXCOLOR_THISTLE						RGB(216,	191,	216)
	#define	GXCOLOR_PLUM						RGB(221,	160,	221)
	#define	GXCOLOR_VIIOLET						RGB(238,	130,	238)
	#define	GXCOLOR_MAGENTA						RGB(255,	0,		255)
	#define	GXCOLOR_FUCHSIA						RGB(255,	0,		255)
	#define	GXCOLOR_PURPLE						RGB(128,	0,		128)
	#define	GXCOLOR_MEDIUM_ORCHID				RGB(186,	85,		211)
	#define	GXCOLOR_DARK_VIOLET					RGB(148,	0,		211)
	#define	GXCOLOR_DARK_ORCHID					RGB(153,	50,		204)
	#define	GXCOLOR_INDIGO						RGB(75,		0,		130)
	#define	GXCOLOR_BLUE_VIOLET					RGB(138,	43,		226)
	#define	GXCOLOR_MEDIUM_PURPLE				RGB(147,	112,	219)
	#define	GXCOLOR_MEDIUM_SLATE_BLUE			RGB(123,	104,	238)
	#define	GXCOLOR_SLATE_BLUE					RGB(106,	90,		205)
	#define	GXCOLOR_DARK_SLATE_BLUE				RGB(72,		61,		139)
	#define	GXCOLOR_LAVENDER					RGB(230,	230,	250)
	#define	GXCOLOR_GHOST_WHITE					RGB(248,	248,	255)
	#define	GXCOLOR_MEDIUM_BLUE					RGB(0,		0,		205)
	#define	GXCOLOR_MIDNIGHT_BLUE				RGB(25,		25,		112)
	#define	GXCOLOR_NAVY						RGB(0,		0,		128)
	#define	GXCOLOR_ROYAL_BLUE					RGB(62,		105,	225)
	#define	GXCOLOR_CORNFLOWER_BLUE				RGB(100,	149,	237)
	#define	GXCOLOR_LIGHT_STEEL_BLUE			RGB(176,	196,	222)
	#define	GXCOLOR_LIGHT_SLATE_GRAY			RGB(119,	136,	153)
	#define	GXCOLOR_SLATE_GRAY					RGB(112,	128,	144)
	#define	GXCOLOR_DODGER_BLUE					RGB(30,		144,	255)
	#define	GXCOLOR_ALICE_BLUE					RGB(240,	248,	255)
	#define	GXCOLOR_STEEL_BLUE					RGB(70,		130,	180)
	#define	GXCOLOR_LIGHT_SKY_BLUE				RGB(135,	206,	250)
	#define	GXCOLOR_SKY_BLUE					RGB(135,	206,	235)
	#define	GXCOLOR_DEEP_SKY_BLUE				RGB(0,		191,	255)
	#define	GXCOLOR_POWEDER_BLUE				RGB(176,	224,	230)
	#define	GXCOLOR_CADET_BLUE					RGB(176,	158,	160)
	#define	GXCOLOR_AZURE						RGB(240,	255,	255)
	#define	GXCOLOR_LIGHT_CYAN					RGB(224,	255,	255)
	#define	GXCOLOR_PALE_TURQUOOISE				RGB(175,	238,	238)
	#define	GXCOLOR_CYAN						RGB(0,		255,	255)
	#define	GXCOLOR_AQUA						RGB(0,		255,	255)
	#define	GXCOLOR_DARK_TURQUOISE				RGB(0,		206,	209)
	#define	GXCOLOR_DARK_SLATE_GRAY				RGB(47,		79,		79)
	#define	GXCOLOR_TEAL						RGB(0,		128,	128)
	#define	GXCOLOR_MEDIUM_TURQUOISE			RGB(72,		2209,	204)
	#define	GXCOLOR_LIGHT_SEA_GREEN				RGB(32,		178,	170)
	#define	GXCOLOR_TURQUOISE					RGB(64,		224,	208)
	#define	GXCOLOR_AQUAMARINE					RGB(127,	255,	212)
	#define	GXCOLOR_MEDIUM_AQUAMARINE			RGB(102,	205,	170)
	#define	GXCOLOR_MEDIUM_SPRING_GREEN			RGB(0,		250,	154)
	#define	GXCOLOR_MINT_CREAM					RGB(245,	255,	250)
	#define	GXCOLOR_SPRING_GREEN				RGB(0,		255,	127)
	#define	GXCOLOR_MEDIUM_SEA_GREEN			RGB(60,		179,	113)
	#define	GXCOLOR_SEA_GREEN					RGB(46,		139,	87)
	#define	GXCOLOR_HONEYDEW					RGB(240,	255,	240)
	#define	GXCOLOR_PALE_GREEN					RGB(152,	251,	152)
	#define	GXCOLOR_DARK_SEA_GREEN				RGB(143,	188,	143)
	#define	GXCOLOR_LIME_GREEN					RGB(50,		205,	50)
	#define	GXCOLOR_FOREST_GREEN				RGB(34,		139,	34)
	#define	GXCOLOR_CHARTREUSE					RGB(127,	255,	0)
	#define	GXCOLOR_LAWN_GREEN					RGB(124,	252,	0)
	#define	GXCOLOR_GREEN_YELLOW				RGB(173,	255,	47)
	#define	GXCOLOR_DARK_OLIVE_GREEN			RGB(85,		107,	47)
	#define	GXCOLOR_YELLOW_GREEN				RGB(154,	205,	50)
	#define	GXCOLOR_OLIVE_DRAB					RGB(107,	142,	35)
	#define	GXCOLOR_BEIGE						RGB(245,	245,	220)
	#define	GXCOLOR_LIGHT_GOLDENROD_YELLOW		RGB(250,	250,	210)
	#define	GXCOLOR_IVORY						RGB(255,	255,	240)
	#define	GXCOLOR_LIGHT_YELLOW				RGB(255,	255,	224)
	#define	GXCOLOR_YELLOW						RGB(255,	255,	0)
	#define	GXCOLOR_OLIVE						RGB(128,	128,	0)
	#define	GXCOLOR_DARK_KHAKI					RGB(128,	128,	0)
	#define	GXCOLOR_LEMON_CHIFFON				RGB(255,	250,	205)
	#define	GXCOLOR_PALE_GOLDENROD				RGB(238,	232,	170)
	#define	GXCOLOR_KHAKI						RGB(240,	230,	170)
	#define	GXCOLOR_GOLD						RGB(255,	215,	0)
	#define	GXCOLOR_CORNSILK					RGB(255,	248,	220)
	#define	GXCOLOR_GOLDENROD					RGB(218,	165,	32)
	#define	GXCOLOR_DARK_GOLDENROD				RGB(184,	134,	11)
	#define	GXCOLOR_FLORAL_WHITE				RGB(255,	250,	240)
	#define	GXCOLOR_OLD_LACE					RGB(253,	245,	230)
	#define	GXCOLOR_WHEAT						RGB(245,	222,	179)
	#define	GXCOLOR_MOCCASIN					RGB(255,	228,	181)
	#define	GXCOLOR_ORANGE						RGB(255,	165,	0)
	#define	GXCOLOR_PAPAYA_WHIP					RGB(255,	239,	213)
	#define	GXCOLOR_BLANCHED_ALMOND				RGB(255,	235,	205)
	#define	GXCOLOR_NAVAJO_WHITE				RGB(255,	222,	173)
	#define	GXCOLOR_ANTIQUE_WHITE				RGB(250,	235,	215)
	#define	GXCOLOR_TAN							RGB(210,	180,	140)
	#define	GXCOLOR_BURLY_WOOD					RGB(222,	180,	135)
	#define	GXCOLOR_BISQUE						RGB(255,	228,	196)
	#define	GXCOLOR_DARK_ORANGE					RGB(255,	140,	230)
	#define	GXCOLOR_LINEN						RGB(250,	240,	230)
	#define	GXCOLOR_PERU						RGB(205,	133,	63)
	#define	GXCOLOR_PEACH_PUFF					RGB(255,	218,	185)
	#define	GXCOLOR_SANDY_BROWN					RGB(244,	164,	96)
	#define	GXCOLOR_CHOCOLATE					RGB(210,	105,	30)
	#define	GXCOLOR_SADDLE_BROWN				RGB(244,	164,	96)
	#define	GXCOLOR_SEASHELL					RGB(255,	245,	238)
	#define	GXCOLOR_SIENNA						RGB(160,	82,		45)
	#define	GXCOLOR_LIGHT_SALMON				RGB(255,	160,	122)
	#define	GXCOLOR_CORAL						RGB(255,	127,	80)
	#define	GXCOLOR_ORANGE_RED					RGB(255,	69,		0)
	#define	GXCOLOR_DARK_SALMON					RGB(233,	150,	122)
	#define	GXCOLOR_TOMATO						RGB(255,	99,		71)
	#define	GXCOLOR_MISTY_ROSE					RGB(255,	228,	225)
	#define	GXCOLOR_SALMON						RGB(250,	128,	114)
	#define	GXCOLOR_SNOW						RGB(255,	250,	250)
	#define	GXCOLOR_LIGHT_CORAL					RGB(240,	128,	128)
	#define	GXCOLOR_ROSY_BROWN					RGB(188,	143,	143)
	#define	GXCOLOR_INDIAN_RED					RGB(205,	92,		92)
	#define	GXCOLOR_BROWN						RGB(165,	42,		42)
	#define	GXCOLOR_FIRE_BRICK					RGB(178,	34,		34)
	#define	GXCOLOR_MAROON						RGB(128,	0,		0)
	#define	GXCOLOR_WHITE						RGB(255,	255,	255)
	#define	GXCOLOR_WHITE_SMOKE					RGB(245,	245,	245)
	#define	GXCOLOR_GAINSBORO					RGB(220,	220,	220)
	#define	GXCOLOR_DARKBLUE					RGB(000,	000,	128)
	#define	GXCOLOR_DARK_GREEN					RGB(000,	064,	000)
	#define	GXCOLOR_DARK_CYAN					RGB(000,	064,	064)	
	#define	GXCOLOR_DARK_RED					RGB(064,	000,	000)
	#define	GXCOLOR_DARK_MAGENTA				RGB(128,	000,	128)
	#define	GXCOLOR_DARK_GRAY					RGB(128,	128,	128)
	#define	GXCOLOR_LIGHT_GRAY					RGB(192,	192,	192)
	#define	GXCOLOR_LIGHT_BLUE					RGB(000,	000,	255)
	#define	GXCOLOR_LIGHT_GREEN					RGB(000,	255,	000)
	#define	GXCOLOR_LIGHT_RED					RGB(255,	000,	000)
	#define	GXCOLOR_LIGHT_MAGENTA				RGB(255,	000,	255)
	#define	GXCOLOR_SCRIPT						RGB(176,	6,		002)
	#define	GXCOLOR_GREEN						RGB(020,	160,	000)
	#define	GXCOLOR_GRAY						RGB(190,	190,	190)

	//-----------------------------------------------------------------------------

	#define clrBlack			0x00000000	// ������
	#define clrWhite			0x00FFFFFF	// ���
	#define clrRed	 			0x000000FF	// ������
	#define clrGreen			0x0000FF00	// �ʷϻ�
	#define clrBlue				0x00FF0000	// �Ķ���
	#define clrMagenta			0x00FF00FF	// ��ȫ��
	#define clrCyan				0x00FFFF00	// û�ϻ�
	#define clrYellow			0x0000FFFF	// �����

	#define clrCrimson			RGB(255,	100,	100)			
	#define	clrGreenForest		RGB(80,		240,	148)
	#define	clrGreenLime		RGB(200,	250,	200)
	#define	clrBlueD			RGB(102,	102,	153)
	#define	ClrBlueRoyal		RGB(180,	180,	218)
	#define ClrBlueWhite		RGB(118,	116,	177)
	#define clrYellowWhite		RGB(255,	255,	181)	
	#define clrOrange			RGB(255,	128,	0)
	#define	clrOrangeD			RGB(255,	128,	64)
	#define	ClrCoralL			RGB(255,	180,	180)

	#define	clrGrayDim			RGB(128,	128,	128)
	#define ClrIndigo			RGB(0x4b,	0x00,	0x82)
	#define ClrRedIndian		RGB(0xcd,	0x5c,	0x5c)
	#define clrGrayLSlate		RGB(0x77,	0x88,	0x99)
	#define	clrnavajowhite		RGB(255,	222,	173)
	#define clrburlywood		RGB(0xde,	0xb8,	0x87)


	// Dk ���� Ŀ������ ���� ��ο��� (%)
	// Lt ���� Ŀ������ ���� ����� (%)
	#define clrRedP(Dk, Lt)		(0x000001*(0xFF*Dk/100) + 0x010100*(Dk*Lt/100))
	#define clrGreenP(Dk, Lt)	(0x000100*(0xFF*Dk/100) + 0x010001*(Dk*Lt/100))
	#define clrBlueP(Dk, Lt)	(0x010000*(0xFF*Dk/100) + 0x000101*(Dk*Lt/100))
	#define clrMagentaP(Dk, Lt)	(0x010010*(0xFF*Dk/100) + 0x000100*(Dk*Lt/100))
	#define clrCyanP(Dk, Lt)	(0x010100*(0xFF*Dk/100) + 0x000001*(Dk*Lt/100))
	#define clrYellowP(Dk, Lt)	(0x000101*(0xFF*Dk/100) + 0x010000*(Dk*Lt/100))
	#define clrGrayP(Lt)		(0x010101*(0xFF*Lt/100))

	// Dk ���� Ŀ������ ���� ��ο��� (0~255)
	// Lt ���� Ŀ������ ���� ����� (0~255)
	#define clrRedV(Dk, Lt)		(0x000001*Dk + 0x010100*Lt)
	#define clrGreenV(Dk, Lt)	(0x000100*Dk + 0x010001*Lt)
	#define clrBlueV(Dk, Lt)	(0x010000*Dk + 0x000101*Lt)
	#define clrMagentaV(Dk, Lt)	(0x010010*Dk + 0x000100*Lt)
	#define clrCyanV(Dk, Lt)	(0x010100*Dk + 0x000001*Lt)
	#define clrYellowV(Dk, Lt)	(0x000101*Dk + 0x010000*Lt)
	#define clrGrayV(Lt)		(0x010101*Lt)
#endif //of #ifndef _GxCOLOR_



class CGxUICtrl
{
public:
	CGxUICtrl();
	~CGxUICtrl();

public:

	static BOOL AbortModalDialog();			// GausGUI ���� ������ Modal Dialog�� ������ ��������ش�. Dialog ������ IDABORT�� ������, ���̻� Dialog�� ã�� ������ FALSE�� �����Ѵ�.
	static void BroadcastButtonCpationIndex(CWnd* pView, UINT nIdx, UINT nID);		// ������ ���� ��� GxOCX�迭�� Control���� ǥ�� ǥ�� �� ����Ǿ����� �뺸�Ѵ�.
	static void BroadcastStaticCpationIndex(CWnd* pView, UINT nIdx, UINT nID);		// ������ ���� ��� GxOCX�迭�� Control���� ǥ�� ǥ�� �� ����Ǿ����� �뺸�Ѵ�.


	// CGxButtonEx ����
	static void SetButtonColor(CWnd* pView, UINT nID, COLORREF cColor);			// ���� ��ư�� �־��� ���� ���� ���� �����Ѵ�.
	static void SetButtonTextColor(CWnd* pView, UINT nID, COLORREF cColor);		// ���� ��ư�� �־��� ���� ���� ���� ���� �����Ѵ�.
	static void SetButtonIcon(CWnd* pView, UINT nID, UINT nIdx);					// ��ư�� �������� �����Ͽ��ش�.
	static void SetButtonString(CWnd* pView, UINT nID, CString sMsg);
	static void SetButtonInt(CWnd* pView, UINT nID, int nValue);
	static void SetButtonDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint);
	static void SetButtonEnabled(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetButtonVisible(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetButtonBlink(CWnd* pView, UINT nID, BOOL bFlag);

	// CGxStaticEx ����
	static void SetStaticColor(CWnd* pView, UINT nID, COLORREF cColor);			// ���� ��� ���� �����Ѵ�.
	static void SetStaticTextColor(CWnd* pView, UINT nID, COLORREF cColor);		// ���� ���ڻ��� �����Ѵ�.
	static void SetStaticIcon(CWnd* pView, UINT nID, UINT nIdx);					// ���� �������� �����Ѵ�.
	static void SetStaticString(CWnd* pView, UINT nID, CString sMsg);
	static void SetStaticInt(CWnd* pView, UINT nID, int nValue);
	static void SetStaticDouble(CWnd* pView, UINT nID, double dbValue, UINT nPoint);
	static void SetStaticVisible(CWnd* pView, UINT nID, BOOL bFlag);
	static void SetStaticBlink(CWnd* pView, UINT nID, BOOL bFlag);


	// ��ġ ���� �Լ�
	static void SetTopPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint=TRUE);				// ��/�� ��ġ�� �������ش�.
	static void SetLeftPosition(CWnd* pOwner, UINT nID, LONG lNewPos, BOOL bRepaint=TRUE);				// ��/�� ��ġ�� �������ش�.


};

