#include "Main.h"
#include "../AutoFile/StateMachine.h"
#include "../AutoFile/StateMachine.h"

U8 m_eActiveResponse;
U8 m_AutoAlarmCycles;
U8 m_AlarmCycles[ALARMSNUMBER];

void 
ResetAlarms ( U8 ResetType ) 
{
    for (int i=0; i<ALARMSNUMBER; i++) {
        if ( ResetType == A_RESET_ALL || ResetType == GetAlarmResetType ( i ) ) {
            SetAlarmState ( i, false );
        }
    }
}

void 
ResetAlarmCycles ( U8 i ) 
{
    m_AlarmCycles[i] = m_AutoAlarmCycles;
}

U1 
IsAlarmCycles ( U8 i ) 
{
    return m_AlarmCycles[i] > 0;
}

void 
DecAlarmCycles ( U8 i ) 
{
    m_AlarmCycles[i]--;
}

void 
InitAlarms ( ) 
{
    memset(m_AlarmCycles, 0, ALARMSNUMBER);
    m_AutoAlarmCycles = (U8)( 1.0 / pSut->P_CycleTime );
	InitAlarmConseq ();
    ResetAlarms ( A_RESET_ALL );
}

void UpdateAlarmCommands() {
	/*
    if(pSut->UC_ResetAlarms) {
        ResetAlarms(ALARMRESET_MANUAL);
        pSut->UC_ResetAlarms = false;
    }
    if(pSut->UC_Start) {
		pSut->UC_RunState = false;
		pSut->UC_Start = false;
	}
    if(pSut->UC_Stop) {
    	pSut->UC_RunState = true;
    	pSut->UC_Stop = false;
    }
	*/
}

void 
UpdateAlarms() 
{
    RaiseAutoAlarms();
    UpdateAlarmCommands();

	pSut->S_ActAlaLev = A_NONE;
	pSut->S_ActAlarm = 0;
    for ( int i=0; i<ALARMSNUMBER; i++) {
        if ( GetAlarmState(i) ) {
            if ( GetAlarmConseq(i) > pSut->S_ActAlaLev ) {
                pSut->S_ActAlaLev = GetAlarmConseq(i);
                pSut->S_ActAlarm = i + 1;
            }
        }
    }
//	if ( pSut->D_ResetEdge ) {
//		ResetManualAlarms (); 
//	}
}
