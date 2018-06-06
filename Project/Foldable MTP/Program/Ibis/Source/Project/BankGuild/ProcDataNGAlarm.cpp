#include "StdAfx.h"
#include "ProcDataNGAlarm.h"


CProcDataNGAlarm::CProcDataNGAlarm(void)
{
}


CProcDataNGAlarm::~CProcDataNGAlarm(void)
{
}

BOOL CProcDataNGAlarm::AlarmCheck(NGAlarmItem *pItem, NGaAlarmLimitItem *pLimit)
{
	if(pLimit->bUse == FALSE)
		return FALSE;
	//////////////////////////////////////////////////////////////////////////
	// Alarm 여부 확인

	// 연속발생여부 확인
	if(pItem->bContinue)
	{
		if(pItem->nContinueNGCount >= pLimit->nCount)
		{
			pItem->Clear();
			return TRUE;
		}
	}

	// 누적 발생율 확인
	// 최소 10개는 쌓아놓고 %를 계산하자...
	int nCount = pItem->m_NGCount.GetCount();
	if(nCount < 10)
		return FALSE;

	double dRate = ((double)pItem->nTotalNGCount / (double)nCount) * 100.;
	if(dRate >= pLimit->dRate)
	{
		pItem->Clear();
		return TRUE;
	}

	return FALSE;
}

void CProcDataNGAlarm::AddAlarm(BOOL bNG, NGAlarmItem *pItem, NGaAlarmLimitItem *pLimit)
{
	if(pLimit->bUse == FALSE)
		return;
	//////////////////////////////////////////////////////////////////////////
	// 수량 카운트

	if(bNG)
	{
		// 불량 카운트 증가
		pItem->nTotalNGCount++;

		// 연속불량이면 연속 카운트도 증가
		if(pItem->bContinue)
		{
			pItem->nContinueNGCount++;
		}
		else
		{
			pItem->bContinue = TRUE;
			pItem->nContinueNGCount = 1;
		}
	}
	else if(pItem->bContinue)
	{
		// 불량이 아니면 연속 카운트 초기화
		pItem->bContinue = FALSE;
		pItem->nContinueNGCount = 0;
	}

	// 전체 수량 증가
	// 최근 100개 까지의 현황만 관리하기 위해 리스트를 활용한다
	pItem->m_NGCount.AddTail(bNG);
	if(pItem->m_NGCount.GetCount() >= 100)
	{
		// 100개가 넘어가면 가장 오래된 거 삭제
		// 삭제할게 불량이면 카운트도 차감
		if( pItem->m_NGCount.RemoveHead() && (pItem->nTotalNGCount > 0) )
		{
			pItem->nTotalNGCount--;
		}
	}
}

#ifdef NOT_USED
BOOL CProcDataNGAlarm::CheckTolMaxOverAlarm(JIG_ID jig, JIG_CH ch)
{
	// return TRUE면 ALARM
	NGAlarmItem *pItem = &m_TolMaxOver[jig][ch];
	NGaAlarmLimitItem *pLimit = &theConfigBank.m_NGAlarmLimit.m_TolMaxOverLimit;

	// Alarm으로 확인되면 해당 채널은 자동 Reset [3/16/2017 OSC]
	return AlarmCheck(pItem, pLimit);
}

void CProcDataNGAlarm::AddTolMaxOver(JIG_ID jig, JIG_CH ch, BOOL bNG)
{
	NGAlarmItem *pItem = &m_TolMaxOver[jig][ch];
	NGaAlarmLimitItem *pLimit = &theConfigBank.m_NGAlarmLimit.m_TolMaxOverLimit;

	AddAlarm(bNG, pItem, pLimit);
}

void CProcDataNGAlarm::ResetTolMaxOver(JIG_ID jig, JIG_CH ch)
{
	m_TolMaxOver[jig][ch].Clear();
}
#endif