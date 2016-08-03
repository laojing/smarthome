#include <stdlib.h> 			
#include <math.h> 			
#include "ChannelDefs.h" 			
#include "StateFilter.h"

tPosEdge D_SunsPulseOnStruct;
tNegEdge D_SunsPulseOffStruct;
tPosEdge D_HumanDoorOnStruct;
tNegEdge D_HumanDoorOffStruct;
tPosEdge D_HumanLiveOnStruct;
tNegEdge D_HumanLiveOffStruct;
tPosEdge D_HumanPassOnStruct;
tNegEdge D_HumanPassOffStruct;
tPosEdge D_HumanBookOnStruct;
tNegEdge D_HumanBookOffStruct;
tPosEdge D_HumanMainOnStruct;
tNegEdge D_HumanMainOffStruct;
tPosEdge D_HumanSecdOnStruct;
tNegEdge D_HumanSecdOffStruct;
tTrueDuration D_HumanOffTimeStruct;
tTrueDuration D_HumanOnTimeStruct;

void
InitDerivedChannels(){
    InitPosEdge(&D_SunsPulseOnStruct, false);
    InitNegEdge(&D_SunsPulseOffStruct, false);
    InitPosEdge(&D_HumanDoorOnStruct, false);
    InitNegEdge(&D_HumanDoorOffStruct, false);
    InitPosEdge(&D_HumanLiveOnStruct, false);
    InitNegEdge(&D_HumanLiveOffStruct, false);
    InitPosEdge(&D_HumanPassOnStruct, false);
    InitNegEdge(&D_HumanPassOffStruct, false);
    InitPosEdge(&D_HumanBookOnStruct, false);
    InitNegEdge(&D_HumanBookOffStruct, false);
    InitPosEdge(&D_HumanMainOnStruct, false);
    InitNegEdge(&D_HumanMainOffStruct, false);
    InitPosEdge(&D_HumanSecdOnStruct, false);
    InitNegEdge(&D_HumanSecdOffStruct, false);
    InitTrueDuration(&D_HumanOffTimeStruct, pSut->P_CycleTime, ( ! pSut->CI_HumanSecd )
             && ( ! pSut->CI_HumanDoor )
             && ( ! pSut->CI_HumanLive )
             && ( ! pSut->CI_HumanPass )
             && ( ! pSut->CI_HumanBook )
             && ( ! pSut->CI_HumanMain ));
    InitTrueDuration(&D_HumanOnTimeStruct, pSut->P_CycleTime, ( pSut->CI_HumanSecd )
             || ( pSut->CI_HumanDoor )
             || ( pSut->CI_HumanLive )
             || ( pSut->CI_HumanPass )
             || ( pSut->CI_HumanBook )
             || ( pSut->CI_HumanMain ));
}

void
UpdateDerivedChannels(){
    pSut->D_SunsPulseOn = PosEdge(&D_SunsPulseOnStruct, pSut->CI_SunsSwitch );
    pSut->D_SunsPulseOff = NegEdge(&D_SunsPulseOffStruct, pSut->CI_SunsSwitch );
    pSut->D_HumanDoorOn = PosEdge(&D_HumanDoorOnStruct, pSut->CI_HumanDoor );
    pSut->D_HumanDoorOff = NegEdge(&D_HumanDoorOffStruct, pSut->CI_HumanDoor );
    pSut->D_HumanLiveOn = PosEdge(&D_HumanLiveOnStruct, pSut->CI_HumanLive );
    pSut->D_HumanLiveOff = NegEdge(&D_HumanLiveOffStruct, pSut->CI_HumanLive );
    pSut->D_HumanPassOn = PosEdge(&D_HumanPassOnStruct, pSut->CI_HumanPass );
    pSut->D_HumanPassOff = NegEdge(&D_HumanPassOffStruct, pSut->CI_HumanPass );
    pSut->D_HumanBookOn = PosEdge(&D_HumanBookOnStruct, pSut->CI_HumanBook );
    pSut->D_HumanBookOff = NegEdge(&D_HumanBookOffStruct, pSut->CI_HumanBook );
    pSut->D_HumanMainOn = PosEdge(&D_HumanMainOnStruct, pSut->CI_HumanMain );
    pSut->D_HumanMainOff = NegEdge(&D_HumanMainOffStruct, pSut->CI_HumanMain );
    pSut->D_HumanSecdOn = PosEdge(&D_HumanSecdOnStruct, pSut->CI_HumanSecd );
    pSut->D_HumanSecdOff = NegEdge(&D_HumanSecdOffStruct, pSut->CI_HumanSecd );
    pSut->D_HumanOffTime = TrueDuration(&D_HumanOffTimeStruct, ( ! pSut->CI_HumanSecd )
             && ( ! pSut->CI_HumanDoor )
             && ( ! pSut->CI_HumanLive )
             && ( ! pSut->CI_HumanPass )
             && ( ! pSut->CI_HumanBook )
             && ( ! pSut->CI_HumanMain ));
    pSut->D_HumanOnTime = TrueDuration(&D_HumanOnTimeStruct, ( pSut->CI_HumanSecd )
             || ( pSut->CI_HumanDoor )
             || ( pSut->CI_HumanLive )
             || ( pSut->CI_HumanPass )
             || ( pSut->CI_HumanBook )
             || ( pSut->CI_HumanMain ));
}
