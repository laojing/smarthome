#ifndef CHANNEL_DEF_H 			
#define CHANNEL_DEF_H 

#include "../MainFrame/Main.h"

#define BUILD_ID  149868

#define ALARMSNUMBER 0

typedef struct _Channels Channels; 			
struct _Channels {
    REAL   P_CycleTime;
    U8     S_OperState;
    U8     S_ActAlaLev;
    S16    S_ActAlarm;
    U1     CI_SunsLightCmd;
    U1     CI_SunsHuman;
    U1     CI_SunsSwitch;
    U1     CI_SunsLightState;
    U1     CO_SunsLight;
    U8     S_SunsState;
    U8     S_SunsRS;
    REAL   S_SunsSC;
    U1     S_SunsUpdate;
    U1     D_SunsPulseOn;
    U1     D_SunsPulseOff;
    U1     CI_BookLightState;
    U1     CO_BookLight;
    U8     S_DengState;
    U8     S_DengRS;
    REAL   S_DengSC;
    U1     S_BookUpdate;
    U1     CI_HumanDoor;
    U1     CI_HumanLive;
    U1     CI_HumanPass;
    U1     CI_HumanBook;
    U1     CI_HumanMain;
    U1     CI_HumanSecd;
    U1     CI_HumanPowerState;
    U1     CO_HumanPower;
    U1     P_ActiveHuman;
    U1     P_DeactiveHuman;
    U16    P_PowerOnTime;
    U16    P_PowerOffTime;
    U16    P_PowerTimeSpan;
    U8     S_HumaState;
    U8     S_HumaRS;
    REAL   S_HumaSC;
    U16    S_DayTime;
    U1     S_HumanPower;
    U1     S_HumanUpdate;
    U1     D_HumanDoorOn;
    U1     D_HumanDoorOff;
    U1     D_HumanLiveOn;
    U1     D_HumanLiveOff;
    U1     D_HumanPassOn;
    U1     D_HumanPassOff;
    U1     D_HumanBookOn;
    U1     D_HumanBookOff;
    U1     D_HumanMainOn;
    U1     D_HumanMainOff;
    U1     D_HumanSecdOn;
    U1     D_HumanSecdOff;
    U16    D_HumanOffTime;
    U16    D_HumanOnTime;
    U8     S_RespState;
    U8     S_RespRS;
    REAL   S_RespSC;
};
extern Channels *pSut;
void InitChannels ( U8 InitType );


#endif
