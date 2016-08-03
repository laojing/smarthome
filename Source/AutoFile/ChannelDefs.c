#include "ChannelDefs.h" 			

Channels channel; 			
Channels *pSut = &channel; 			

void
InitChannels ( U8 InitType ) { 			
    if ( InitType == COLD_BOOT ) {
    }
    pSut->P_CycleTime = 1;
    pSut->S_OperState = 0;
    pSut->S_ActAlaLev = 0;
    pSut->S_ActAlarm = 0;
    pSut->CI_SunsLightCmd = false;
    pSut->CI_SunsHuman = false;
    pSut->CI_SunsSwitch = false;
    pSut->CI_SunsLightState = false;
    pSut->CO_SunsLight = false;
    pSut->S_SunsState = 0;
    pSut->S_SunsRS = 0;
    pSut->S_SunsSC = 0;
    pSut->S_SunsUpdate = false;
    pSut->CI_BookLightState = false;
    pSut->CO_BookLight = false;
    pSut->S_DengState = false;
    pSut->S_DengRS = false;
    pSut->S_DengSC = false;
    pSut->S_BookUpdate = false;
    pSut->CI_HumanDoor = false;
    pSut->CI_HumanLive = false;
    pSut->CI_HumanPass = false;
    pSut->CI_HumanBook = false;
    pSut->CI_HumanMain = false;
    pSut->CI_HumanSecd = false;
    pSut->CI_HumanPowerState = false;
    pSut->CO_HumanPower = false;
    pSut->P_ActiveHuman = 0;
    pSut->P_DeactiveHuman = 0;
    pSut->P_PowerOnTime = 64800;
    pSut->P_PowerOffTime = 79200;
    pSut->P_PowerTimeSpan = 300;
    pSut->S_HumaState = 0;
    pSut->S_HumaRS = 0;
    pSut->S_HumaSC = 0;
    pSut->S_DayTime = 0;
    pSut->S_HumanPower = false;
    pSut->S_HumanUpdate = false;
    pSut->S_RespState = 0;
    pSut->S_RespRS = 0;
    pSut->S_RespSC = 0;
}
