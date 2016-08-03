#include "Main.h"
#include "../home.h"
#include "../AutoFile/StateMachine.h"
#include <time.h>
#include <stdio.h>

U16 m_uiCycleTimeMs;
U16 m_uiStepTimerMs;

void 
InitControllerTime()
{
    m_uiStepTimerMs = 0;
    m_uiCycleTimeMs = (U16) ((pSut->P_CycleTime * 1000) + 0.5);
}

int voiceTime = 30;
int halfSpan = 1;
void 
IncrementStepTimer()
{
    m_uiStepTimerMs += m_uiCycleTimeMs;
	pSut->S_DayTime = (time( NULL ) - 57600) % 86400;

	if( pSut->S_DayTime > 0 * 3600
			&& pSut->S_DayTime < 22 * 3600
			&& pSut->S_DayTime % (voiceTime*60) < 60
			&& halfSpan ) {
		GiveTime();
		halfSpan = 0;
	}
	if( pSut->S_DayTime % (voiceTime*60) >100 ){
		halfSpan = 1;
	}
//	printf( "DayTime%d, halfSpan:%d\n", pSut->S_DayTime, halfSpan );
//	fflush( stdout );
}

U16 
StartStepTimer()
{
    return m_uiStepTimerMs;
}

REAL 
GetStepTimerDuration ( U16 start )
{
    return (REAL)(m_uiStepTimerMs - start) / 1000;
}

void 
InitController ()
{
	InitChannels( COLD_BOOT );
	InitControllerTime();
	InitDerivedChannels();
	InitAlarms();
	InitStateMachines();
}

void 
StepController ()
{
	IncrementStepTimer ();
	UpdateDerivedChannels();
	UpdateAlarms();
}
