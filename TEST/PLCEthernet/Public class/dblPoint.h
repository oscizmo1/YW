// dblPoint.h: interface for the CdblPoint class.
//
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// 최종 수정 [2/11/2009 OSC]
// 	CPoint ConvertCPoint(); 추가
//////////////////////////////////////////////////////////////////////////

#pragma once


class CdblSize;

typedef struct tagdblPOINT {
	double x;
	double y;
} dblPOINT;

class CdblPoint : public dblPOINT
{
public:
	CdblPoint();
	CdblPoint(const CPoint& point);
	CdblPoint(const CdblPoint& point);
	CdblPoint(double initX, double initY);
	virtual ~CdblPoint();

	CPoint GetCPoint() const {
		return CPoint((long)x, (long)y);
	}
  
	void Offset(double xOffset, double yOffset );
	CPoint ToCPoint();

	CdblPoint operator-();
	CdblPoint operator-() const;
	CdblPoint operator-(const CdblSize& size) const;
	CdblPoint operator-(const CdblPoint& point) const;
	CdblPoint operator+(const CdblSize& size) const;
	CdblPoint operator+(const CdblPoint& point) const;

	void operator=(const CdblPoint &srcPoint);
	void operator=(const CPoint &srcPoint);
	BOOL operator!=(const CPoint& point);
	BOOL operator!=(const CdblPoint& point);
	BOOL operator==(const CPoint &point);
	BOOL operator==(const CdblPoint &point);

	void operator-=(const CdblSize& size);
	void operator-=(const CdblPoint& point);
	void operator+=(const CdblSize& size);
	void operator+=(const CdblPoint& point);
	void operator*=(const double& dValue);
};