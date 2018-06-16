// MNetH.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MNetH.h"
#include "math.h"
#include "..\Public class\Mdfunc.h"

// CMNetH


CMNetH::CMNetH()
{

}

CMNetH::~CMNetH()
{
}


BEGIN_MESSAGE_MAP(CMNetH, CWnd)
END_MESSAGE_MAP()

short CMNetH::mnetOpen(short chan, short mode, LPLONG path)
{
	m_shortchan = chan; m_shortmode = -1; m_shortstno = 255; m_lpath = *path;/* size =2; data =10;*/

	short ret = mdOpen(chan, mode, path);
	return ret;
}

short CMNetH::mnetClose(long path)
{
	short ret = mdClose(path);
	return ret;
}

short CMNetH::mnetReceive(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data)
{
	short ret = mdReceive(m_lpath, numStationNo, devtyp, devno, size, data);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdReceive Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}

short CMNetH::mnetSubReceive(long path ,short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data)
{
	short ret = mdReceive(path, numStationNo, devtyp, devno, size, data);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdReceive Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}

short CMNetH::mnetSend( short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data )
{
	short ret = mdSend(m_lpath, numStationNo, devtyp, devno, size, data);
	return ret;
}

short CMNetH::ASCIIReceive(short numNetNo, short numStationNo, short devtyp, short devno, CString* pStrAscii, int nWord)
{
	CString tmpASCII,tmpData;
	short   data, size = 2, ret = 0;
	tmpASCII.Empty(); m_strMnetASCII.Empty(); tmpData.Empty();

	char *stopstring;
	int nLeft=0, nRight=0;

	CString strLeft, strRight, tp1, tp2;
	strLeft.Empty(); strRight.Empty(); tp1.Empty(); tp2.Empty();

	for(int i=0; i < nWord ;i++)																	//word개수 만큼만 받아들인다.
	{
		ret = ret + mdReceive(m_lpath, numStationNo, devtyp, devno, &size, &data);

		tmpData.Format("%04X", data);													// tmpData.Format("%02X", data);  1-word임으로 2byte씩 16bit임으로 다음과 같이 표현되는 것이 맞는 것 같다
		tp1 = tmpData.Mid(0,2);
		tp2 = tmpData.Mid(2,2);

		nLeft = strtol(tp1, &stopstring, 16);
		nRight = strtol(tp2, &stopstring, 16);
		strLeft.Format("%c", nLeft);
		strRight.Format("%c", nRight);

		tmpASCII = strRight + strLeft;
		m_strMnetASCII = m_strMnetASCII + tmpASCII;
		devno++;																		//받는 주소값 하나 더 늘린다.

		if(ret!=0) break;				//Receive fn. Result  
		//		if(tp1=="20") break;			//Left Space charater.		(Num. of odd )		//공백도 Ascii로 포함되서 들어온다.
		//		if(tp2=="20") break;			//Right Space charater.
		if(tmpData=="00") break;		//NULL charater.			(Num. of even)
	}

	*pStrAscii = m_strMnetASCII;
	return ret;	
}

short CMNetH::SubASCIIReceive(long path ,short numNetNo, short numStationNo, short devtyp, short devno, CString* pStrAscii, int nWord)
{
	CString tmpASCII,tmpData;
	short   data, size = 2, ret = 0;
	tmpASCII.Empty(); m_strMnetASCII.Empty(); tmpData.Empty();

	char *stopstring;
	int nLeft=0, nRight=0;

	CString strLeft, strRight, tp1, tp2;
	strLeft.Empty(); strRight.Empty(); tp1.Empty(); tp2.Empty();

	for(int i=0; i < nWord ;i++)																	//word개수 만큼만 받아들인다.
	{
		ret = ret + mdReceive(path, numStationNo, devtyp, devno, &size, &data);

		tmpData.Format("%04X", data);													// tmpData.Format("%02X", data);  1-word임으로 2byte씩 16bit임으로 다음과 같이 표현되는 것이 맞는 것 같다
		tp1 = tmpData.Mid(0,2);
		tp2 = tmpData.Mid(2,2);

		nLeft = strtol(tp1, &stopstring, 16);
		nRight = strtol(tp2, &stopstring, 16);
		strLeft.Format("%c", nLeft);
		strRight.Format("%c", nRight);

		tmpASCII = strRight + strLeft;
		m_strMnetASCII = m_strMnetASCII + tmpASCII;
		devno++;																		//받는 주소값 하나 더 늘린다.

		if(ret!=0) break;				//Receive fn. Result  
		//		if(tp1=="20") break;			//Left Space charater.		(Num. of odd )		//공백도 Ascii로 포함되서 들어온다.
		//		if(tp2=="20") break;			//Right Space charater.
		if(tmpData=="00") break;		//NULL charater.			(Num. of even)
	}

	*pStrAscii = m_strMnetASCII;
	return ret;	
}

short CMNetH::ASCIISend(short numNetNo, short numStationNo, short devtyp, short devno, CString strMnetASCII)
{
	int      i = 0, tmpno =0; 
	unsigned char str;
	short size =2, ret = 0, data;
	CString  TempStr,ConvStr,MidStr;
	TempStr.Empty();
	MidStr.Empty();
	ConvStr.Empty();
	tmpno = devno;

	i  = strMnetASCII.GetLength();

	char *stopstring;

	int k;
	for(k=0; k<i; k++)
	{
		str = strMnetASCII.GetAt(k);
		TempStr.Format("%02X",str);
		MidStr = TempStr + MidStr;

		if(!(k == 0))
			if((k%2))
			{
				data = (short)strtol(MidStr, &stopstring, 16);
				ret = ret + mdSend(m_lpath, numStationNo, devtyp, devno, &size, &data);
				devno++;
				MidStr.Empty();
			}// else
			//	{
			//		ConvStr += ConvStr;
			//	}
			ConvStr = TempStr + ConvStr;
	}

	if(k==i)
	{
		if(!(i%2))
		{
			ConvStr = "0000" + ConvStr;
			data = 0;
			ret = ret + mdSend(m_lpath, numStationNo, devtyp, tmpno+(int)(i/2), &size, &data);
		}
		else
		{
			MidStr = "20" + MidStr;
			ConvStr = "20" + ConvStr;
			data = (short)strtol(MidStr, &stopstring, 16);
			ret = ret + mdSend(51, numStationNo, devtyp, tmpno+(int)(i/2), &size, &data);
		}
	}
	return ret;
}

BOOL CMNetH::CheckExtent(short size, short cmode, LONGLONG dec)
{
	LONGLONG Min;
	LONGLONG Max;

	// Word size = 0 : 2 byte
	if(size == 2)
	{
		switch(cmode)
		{
		case 0 :	// dec
			Min = -32767;
			Max = 65535;
			break;
		case 1 :	// hex
			Min = 0;
			Max = 65535;
			break;
		case 2 :	// bin
			Min = -32767;
			Max = 65535;
		}
	}
	else
	{
		switch(cmode)
		{
		case 0 :
			Min = -2147483647;
			Max = 4294967295;
			break;
		case 1 :
			Min = 0;
			Max = 4294967295;
			break;
		case 2 :
			Min = -2147483647;
			Max = 4294967295;
		}
	}

	if(dec < Min || Max < dec)
	{
		m_chkMessage.Format("%d Byte :: %d ~ %u",(size+1)*2,Min,Max);
		return FALSE;
	}
	return TRUE;
}

short CMNetH::SendTrans(CString InStr, short cmode)
{
	//	size = 2;
	LONGLONG c;
	int i,len;
	//	char buff[33];
	CString Temp , strDec, strHex, strBin;

	//	UpdateData(TRUE);

	InStr.TrimRight();
	if(InStr.IsEmpty() == TRUE)
	{
		strDec.Empty();
		strHex.Empty();
		strBin.Empty();
		m_chkMessage.Empty();
		InStr.Empty();
		return -1;//empty
	}
	InStr.MakeUpper();

	strDec.Empty();
	strHex.Empty();
	strBin.Empty();

	Temp = InStr;
	Temp.Remove(' ');
	len = Temp.GetLength();

	switch(cmode)
	{
	case 0 : 
		for(c=0,i=0;i<len;i++)
		{
			if(Temp[len-(i+1)] != '-') c += (LONGLONG)((Temp[len-(i+1)]-48) *  pow((double) 10,(int) i));
			if(isdigit(Temp[i]) == 0 && (i != 0 || Temp[i] != '-'))
			{
				m_chkMessage.Format("Invalid Code (Desimal is 0 ~ 9)");
				//				UpdateData(FALSE);
				return -2;//not decimal
			}
		}
		if(Temp[0] == '-') c *= -1;
		break;
	case 1 :
		int k;
		for(c=0,i=0;i<len;i++)
		{
			k=0;
			if(64 < Temp[len-(i+1)] && Temp[len-(i+1)] < 71) k=7;
			k = Temp[len-(i+1)]-(48+k);
			c += (int)(k * pow( (double) 16,(int) i ) );

			if(isxdigit(Temp[i]) == 0)
			{
				m_chkMessage.Format("Invalid Code (Hex is 0 ~ 9, A ~ F)");
				//				UpdateData(FALSE);
				return -3;//not hex
			}
		}
		c = (UINT)c;
		break;
	case 2 : 
		for(c=0,i=0;i<len;i++)
		{
			c += (int)((Temp[len-(i+1)]-48) * pow( (double) 2,(int) i ) );
			if(Temp[i] != '0' && Temp[i] != '1')
			{
				m_chkMessage.Format("Invalid Code (Binary is 0 or 1)");
				//				UpdateData(FALSE);
				return -4; //not binary
			}
		}
		break;
	}

	m_chkMessage.Empty();

	short size = 4;
	if(CheckExtent(size,cmode,c) == FALSE)
	{
		strDec.Empty();
		strHex.Empty();
		strBin.Empty();
		//		UpdateData(FALSE);
		return -5; // extent number
	}

	//	if(size == 2)
	//	{
	//		USHORT us;
	//		us = (USHORT)c;
	//		strDec.Format("%d",us);
	//		strHex.Format("%04X",us);

	//		return us;
	/*
	itoa(us,buff,2);
	strBin = "0000000000000000";
	strBin += buff;
	strcpy(buff,strBin.Right(16));

	strBin.Format("%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
	buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],
	buff[8],buff[9],buff[10],buff[11],buff[12],buff[13],buff[14],buff[15]);

	c = us;*/

	//		return us;
	//	}
	//	else
	//	{
	//		UINT us;
	//		us = (UINT)c;
	//		strDec.Format("%u",us);
	//		strHex.Format("%08X",us);
	//		strHex.Insert(4,' ');
	/*
	itoa(us,buff,2);
	strBin = "00000000000000000000000000000000";
	strBin += buff;
	strcpy(buff,strBin.Right(32));

	strBin.Format("%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c",
	buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9],buff[10],
	buff[11],buff[12],buff[13],buff[14],buff[15],buff[16],buff[17],buff[18],buff[19],buff[20],
	buff[21],buff[22],buff[23],buff[24],buff[25],buff[26],buff[27],buff[28],buff[29],buff[30],buff[31]);
	*/
	//		return us;
	//	}

	//	if(cmode == 0) OutStr = strDec;
	//	else if(cmode == 1) OutStr = strHex;
	//	else if(cmode == 2) OutStr = strBin;

	//	return OutStr;
	//	return (short)us;

	//	UpdateData(FALSE);	
	UINT us;
	us = (UINT)c;
	return us;
}

CString CMNetH::ReceiveTrans(short data, short cmode)
{
	short size = 2;
	LONGLONG c;
	int i,len;
	char buff[33];
	CString Temp, strDec, strBin, strHex, OutStr, InStr;
	OutStr.Empty();

	//	UpdateData(TRUE);
	InStr.Format("%d", data);
	InStr.TrimRight();
	if(InStr.IsEmpty() == TRUE)
	{
		strDec.Empty();
		strHex.Empty();
		strBin.Empty();
		m_chkMessage.Empty();
		InStr.Empty();
		return m_chkMessage;
	}
	InStr.MakeUpper();

	strDec.Empty();
	strHex.Empty();
	strBin.Empty();

	Temp = InStr;
	Temp.Remove(' ');
	len = Temp.GetLength();



	for(c=0,i=0;i<len;i++)
	{
		if(Temp[len-(i+1)] != '-') c += (LONGLONG)((Temp[len-(i+1)]-48) * pow( (double) 10, (int) i) );
		if(isdigit(Temp[i]) == 0 && (i != 0 || Temp[i] != '-'))
		{
			m_chkMessage.Format("Invalid Code (Desimal is 0 ~ 9)");
			//			UpdateData(FALSE);
			return m_chkMessage;
		}
	}
	if(Temp[0] == '-') c *= -1;



	m_chkMessage.Empty();

	if(CheckExtent(size,0,c) == FALSE)
	{
		strDec.Empty();
		strHex.Empty();
		strBin.Empty();
		//		UpdateData(FALSE);
		return m_chkMessage;
	}

	if(size == 2)
	{
		USHORT us;
		us = (USHORT)c;
		strDec.Format("%d\0",us);
		strHex.Format("%04X\0",us);

		_itoa_s(us,buff,2);
		strBin = "0000000000000000";
		strBin += buff;
		strcpy_s(buff,strBin.Right(16));

		strBin.Format("%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c\0",
			buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],
			buff[8],buff[9],buff[10],buff[11],buff[12],buff[13],buff[14],buff[15]);

		c = us;
	}
	else
	{
		UINT us;
		us = (UINT)c;
		strDec.Format("%u\0",us);
		strHex.Format("%08X\0",us);
		strHex.Insert(4,' ');

		_itoa_s(us,buff,2);
		strBin = "00000000000000000000000000000000";
		strBin += buff;
		strcpy_s(buff,strBin.Right(32));

		strBin.Format("%c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c %c%c%c%c\0",
			buff[0],buff[1],buff[2],buff[3],buff[4],buff[5],buff[6],buff[7],buff[8],buff[9],buff[10],
			buff[11],buff[12],buff[13],buff[14],buff[15],buff[16],buff[17],buff[18],buff[19],buff[20],
			buff[21],buff[22],buff[23],buff[24],buff[25],buff[26],buff[27],buff[28],buff[29],buff[30],buff[31]);
	}

	if(cmode == 0) OutStr = strDec;
	else if(cmode == 1) OutStr = strHex;
	else if(cmode == 2) OutStr = strBin;

	//	UpdateData(FALSE);	
	return OutStr;
}

short CMNetH::DataSend(short numNetNo, short numStationNo, short devtyp, short devno, short size, CString InStr, short cmode)
{
	/*CString temp = ConvertData(Input, Cmode);
	char *stopstring;
	data = (short)strtol(temp, &stopstring, 16);
	ret = mdSend(path, stno, devtyp, devno, &size, &data);
	return ret;*/
	short data;
	data = SendTrans(InStr, cmode);
	short ret = mdSend(m_lpath, m_shortstno, devtyp, devno, &size, &data);
	return ret;
}

CString CMNetH::DataReceive(short numNetNo, short numStationNo, short devtyp, short devno, short size, short cmode)
{
	short data;
	short ret = mdReceive(m_lpath, m_shortstno, devtyp, devno, &size, &data);
	CString OutStr;		OutStr.Empty();
	if(ret != 0) OutStr.Format("%d", ret);
	OutStr = ReceiveTrans(data, cmode);	
	return OutStr;
}

short CMNetH::mnetBdRst(long path)
{
	short ret = mdBdRst(path);
	return ret;
}

short CMNetH::mnetDevSet(short numNetNo, short numStationNo/*long path, short stno*/, short devtyp, short devno)
{
	short ret = mdDevSet(m_lpath, numStationNo, devtyp, devno);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdDevSet Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}
short CMNetH::mnetSubDevSet(long path , short numNetNo, short numStationNo/*long path, short stno*/, short devtyp, short devno)
{
	short ret = mdDevSet(path, numStationNo, devtyp, devno);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdDevSet Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}


short CMNetH::mnetDevRst(short numNetNo, short numStationNo/*long path, short stno*/, short devtyp, short devno)
{
	short ret = mdDevRst(m_lpath, numStationNo, devtyp, devno);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdDevRst Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}

short CMNetH::mnetSubDevRst(long path , short numNetNo, short numStationNo/*long path, short stno*/, short devtyp, short devno)
{
	short ret = mdDevRst(path, numStationNo, devtyp, devno);
	if(ret)		
	{
		CString strMsg;
		strMsg.Format("Check Melsec(mdDevRst Error) Code = %d,", ret);
		//		AfxMessageBox(strMsg);
	}
	return ret;
}
short CMNetH::mnetBdModRead(long path, PSHORT mode)
{
	short ret = mdBdModRead(path, mode);
	return ret;
}

short CMNetH::mnetBdModSet(long path, short mode)
{
	short ret = mdBdModSet(path, mode);
	return ret;
}

short CMNetH::mnetInit(long path)
{
	short ret = mdInit(path);
	return ret;
}

short CMNetH::mnetControl(short numNetNo, short numStationNo, short buf)
{
	short ret = mdControl(m_lpath, numStationNo, buf);
	return ret;
}

short CMNetH::mnetTypeRead(short numNetNo, short numStationNo, PSHORT buf)
{
	short ret = mdTypeRead(m_lpath, numStationNo, buf);
	return ret;
}

short CMNetH::mnetBdLedRead(long path, PSHORT buf)
{
	short ret = mdBdLedRead(path, buf);
	return ret;
}

short CMNetH::mnetBdSwRead(long path, PSHORT buf)
{
	short ret = mdBdSwRead(path, buf);
	return ret;
}

short CMNetH::mnetBdVerRead(long path, PSHORT buf)
{
	short ret = mdBdVerRead(path, buf);
	return ret;
}

short CMNetH::mnetRandR(long path, short stno, LPVOID dev, LPVOID buf, short bufsize)
{
	short ret = mdRandR(path, stno, dev, buf, bufsize);
	return ret;
}

short CMNetH::mnetRandW(long path, short stno, LPVOID dev, LPVOID buf, short bufsize)
{
	short ret = mdRandW(path, stno, dev, buf, bufsize);
	return ret;
}

//DEL short CMNetH::mnetReceive2(short numNetNo, short numStationNo, short devtyp, short devno, PSHORT size, LPVOID data)
//DEL {
//DEL 
//DEL //	numNetNo = 0; numStationNo = 255;
//DEL 	stno = numNetNo * 256 + numStationNo; // stand alone stno=>255.
//DEL 	ret = mdReceive(path, stno, devtyp, devno, size, data);
//DEL 	return ret;
//DEL }

short CMNetH::mnetDevReceive(short numNetNo, short numStationNo, short devtyp, short devno, int* data)
{
	short devno2, data2, ibit, temp;

	devno2 = data2 = ibit = temp =0;
	// 	char tbuf[9]="0";

	if(devno>7)
	{
		devno2 = short(devno/8) * 8;
	}
	else
	{
		devno2 = 0;
	}

	/*short res[5];
	res[0] = pMNeth.mnetReceive(0, 255, 23, 0x10, &m_shMnetSize, &data2);  //for array variable.. 
	res[1] = pMNeth.mnetReceive(0, 255, 23, 0x14, &m_shMnetSize, &data2); 
	res[2] = pMNeth.mnetReceive(0, 255, 23, 0x15, &m_shMnetSize, &data2); 
	res[3] = pMNeth.mnetReceive(0, 255, 23, 0x20, &m_shMnetSize, &data2);
	res[4] = pMNeth.mnetReceive(0, 255, 23, 0x17, &m_shMnetSize, &data2); */

	short size = 1;	//This fn. is received by bit, X, Y etc....(so, size is only 1)

	short ret = mnetReceive(numNetNo, numStationNo, devtyp, devno2, &size, &data2); //for pointer variable.. 
	// ret = mnetReceive(Net.No, Stat.No, DEV_TYPE, DEVNO, &SIZE, &DATA);
	// 	itoa(data2, tbuf, 2);
	ibit = (devno%8) +1;

	switch(ibit)
	{
	case 1:	temp = 0x01; break;
	case 2:	temp = 0x02; break;
	case 3:	temp = 0x04; break;
	case 4:	temp = 0x08; break;
	case 5:	temp = 0x10; break;
	case 6:	temp = 0x20; break;
	case 7:	temp = 0x40; break;
	default:temp = 0x80; break;
	}
	int flags = data2 & temp;
	if(flags) 		flags = 1;
	*data = flags;
	// 	data = (PSHORT)flags;

	/*
	m_shMnetRet = pMNeth.mnetDevSet(0, 255, DEV_TYP_BIT, 0x11);
	m_shMnetRet = pMNeth.mnetDevRst(0, 255, DEV_TYP_BIT, 0x10);
	*/
	/*if(data < 0x00) {ret = -1;} 
	else if(flags > 0x80) {ret = -3;}
	else if((flags%2)==1) {ret = -2;}
	ret = 0;*/

	return ret;
}

short CMNetH::mnetSubDevReceive(long path , short numNetNo, short numStationNo, short devtyp, short devno, int* data)
{
	short devno2, data2, ibit, temp;

	devno2 = data2 = ibit = temp =0;
	// 	char tbuf[9]="0";

	if(devno>7)
	{
		devno2 = short(devno/8) * 8;
	}
	else
	{
		devno2 = 0;
	}

	/*short res[5];
	res[0] = pMNeth.mnetReceive(0, 255, 23, 0x10, &m_shMnetSize, &data2);  //for array variable.. 
	res[1] = pMNeth.mnetReceive(0, 255, 23, 0x14, &m_shMnetSize, &data2); 
	res[2] = pMNeth.mnetReceive(0, 255, 23, 0x15, &m_shMnetSize, &data2); 
	res[3] = pMNeth.mnetReceive(0, 255, 23, 0x20, &m_shMnetSize, &data2);
	res[4] = pMNeth.mnetReceive(0, 255, 23, 0x17, &m_shMnetSize, &data2); */

	short size = 1;	//This fn. is received by bit, X, Y etc....(so, size is only 1)

	short ret = mnetSubReceive( path, numNetNo, numStationNo, devtyp, devno2, &size, &data2); //for pointer variable.. 
	// ret = mnetReceive(Net.No, Stat.No, DEV_TYPE, DEVNO, &SIZE, &DATA);
	// 	itoa(data2, tbuf, 2);
	ibit = (devno%8) +1;

	switch(ibit)
	{
	case 1:	temp = 0x01; break;
	case 2:	temp = 0x02; break;
	case 3:	temp = 0x04; break;
	case 4:	temp = 0x08; break;
	case 5:	temp = 0x10; break;
	case 6:	temp = 0x20; break;
	case 7:	temp = 0x40; break;
	default:temp = 0x80; break;
	}
	int flags = data2 & temp;
	if(flags) 		flags = 1;
	*data = flags;
	// 	data = (PSHORT)flags;

	/*
	m_shMnetRet = pMNeth.mnetDevSet(0, 255, DEV_TYP_BIT, 0x11);
	m_shMnetRet = pMNeth.mnetDevRst(0, 255, DEV_TYP_BIT, 0x10);
	*/
	/*if(data < 0x00) {ret = -1;} 
	else if(flags > 0x80) {ret = -3;}
	else if((flags%2)==1) {ret = -2;}
	ret = 0;*/

	return ret;
}

// CMNetH 메시지 처리기입니다.


