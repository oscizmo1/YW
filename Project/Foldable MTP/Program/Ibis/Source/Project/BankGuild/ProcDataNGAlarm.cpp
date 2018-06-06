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
	// Alarm ���� Ȯ��

	// ���ӹ߻����� Ȯ��
	if(pItem->bContinue)
	{
		if(pItem->nContinueNGCount >= pLimit->nCount)
		{
			pItem->Clear();
			return TRUE;
		}
	}

	// ���� �߻��� Ȯ��
	// �ּ� 10���� �׾Ƴ��� %�� �������...
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
	// ���� ī��Ʈ

	if(bNG)
	{
		// �ҷ� ī��Ʈ ����
		pItem->nTotalNGCount++;

		// ���Ӻҷ��̸� ���� ī��Ʈ�� ����
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
		// �ҷ��� �ƴϸ� ���� ī��Ʈ �ʱ�ȭ
		pItem->bContinue = FALSE;
		pItem->nContinueNGCount = 0;
	}

	// ��ü ���� ����
	// �ֱ� 100�� ������ ��Ȳ�� �����ϱ� ���� ����Ʈ�� Ȱ���Ѵ�
	pItem->m_NGCount.AddTail(bNG);
	if(pItem->m_NGCount.GetCount() >= 100)
	{
		// 100���� �Ѿ�� ���� ������ �� ����
		// �����Ұ� �ҷ��̸� ī��Ʈ�� ����
		if( pItem->m_NGCount.RemoveHead() && (pItem->nTotalNGCount > 0) )
		{
			pItem->nTotalNGCount--;
		}
	}
}

#ifdef NOT_USED
BOOL CProcDataNGAlarm::CheckTolMaxOverAlarm(JIG_ID jig, JIG_CH ch)
{
	// return TRUE�� ALARM
	NGAlarmItem *pItem = &m_TolMaxOver[jig][ch];
	NGaAlarmLimitItem *pLimit = &theConfigBank.m_NGAlarmLimit.m_TolMaxOverLimit;

	// Alarm���� Ȯ�εǸ� �ش� ä���� �ڵ� Reset [3/16/2017 OSC]
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