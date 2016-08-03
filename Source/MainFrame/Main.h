#ifndef MAIN_H_
#define MAIN_H_

#include "Filter.h"

#define COLD_BOOT        0
#define WARM_BOOT        1

#ifndef max
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif
#ifndef min
#define min(a,b) ((a) < (b) ? (a) : (b))
#endif

#define BIT_00 0x01
#define BIT_01 0x02
#define BIT_02 0x04
#define BIT_03 0x08
#define BIT_04 0x10
#define BIT_05 0x20
#define BIT_06 0x40
#define BIT_07 0x80
#define BIT_08 0x100
#define BIT_09 0x200
#define BIT_10 0x400
#define BIT_11 0x800
#define BIT_12 0x1000
#define BIT_13 0x2000
#define BIT_14 0x4000
#define BIT_15 0x8000
#define BIT_16 0x10000
#define BIT_17 0x20000
#define BIT_18 0x40000
#define BIT_19 0x80000
#define BIT_20 0x100000
#define BIT_21 0x200000
#define BIT_22 0x400000
#define BIT_23 0x800000
#define BIT_24 0x1000000
#define BIT_25 0x2000000
#define BIT_26 0x4000000
#define BIT_27 0x8000000
#define BIT_28 0x10000000
#define BIT_29 0x20000000
#define BIT_30 0x40000000
#define BIT_31 0x80000000

#define A_NONE          0
#define A_LOG           1
#define A_WARNING       2
#define A_FORBIDSTART   3
#define A_NORMALSTOP    4
#define A_GRIDERROR     5
#define A_FASTSTOP      6
#define A_BATSTOP       7
#define A_MANUALERROR   8
#define A_TURBPROTECT   9
#define A_EMERGENCY     10

#define A_RESET_ALL     0
#define A_RESET_MANUAL  1
#define A_RESET_AUTO    2

#define A_CAT_INFO      0
#define A_CAT_REQSTOP   1
#define A_CAT_NORMAL    2
#define A_CAT_FAULT     3

void SetAlarmState ( U8 i, U1 b );
U1 GetAlarmState ( U8 i );
U8 GetAlarmConseq ( U8 i );
U8 GetAlarmResetType ( U8 i );
U8 GetAlarmCategory ( U8 i );

void ResetAlarmCycles ( U8 i );
U1 IsAlarmCycles ( U8 i ); 
void DecAlarmCycles ( U8 i );

void InitAlarmConseq ();
void RaiseAutoAlarms ();
void InitAlarms();
void UpdateAlarms();
void ResetManualAlarms (); 

void InitStateMachines ();
void UpdateStateMachines ();
void UpdateInCalibrations ();
void UpdateOutCalibrations ();
void InitDerivedChannels();
void UpdateDerivedChannels();

void InitController ();
void StepController ();

U16 StartStepTimer();
REAL GetStepTimerDuration ( U16 start );

void HumanDoorOn();
void HumanSwitchOff();
void HalfTime();

#endif
