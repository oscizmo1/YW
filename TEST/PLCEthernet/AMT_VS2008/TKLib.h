#ifndef __TKLIB_H__
#define	__TKLIB_H__
//nclude "MSFlexGrid.h"
#include "LabelControl.h"
//nclude "CommandButton.h"

//tern "C" __declspec(dllimport) void Use_TKg(CMSFlexGrid &ctlGrid, double dMaxValue, double dMinValue , 
//									  int nLeftPos,int nTopPos,BOOL bSingle) ;
extern "C" __declspec(dllimport) void Use_TK(CLabelControl &ctl, double dMaxValue, double dMinValue,
									 int nLeftPos,int nTopPos);
//tern "C" __declspec(dllimport) void Use_TKc(CCommandButton &ctl, double dMaxValue, double dMinValue,
//									 int nLeftPos,int nTopPos);
extern "C" __declspec(dllimport) void Use_STR(CLabelControl &ctl, CPoint pos);
//extern "C" __declspec(dllimport) void Use_GridStr(CMSFlexGrid &ctl, CPoint pos);

#endif
