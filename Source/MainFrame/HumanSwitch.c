#include "Main.h"
#include "../home.h"
#include "../AutoFile/ChannelDefs.h"
#include "../VoicePCM/VoicePCM.h"

void HumanDoorOn(){

	if( pSut->S_DayTime > 6 * 3600
			&& pSut->S_DayTime < 22 * 3600 ) {
		if( pSut->S_DayTime > 18 * 3600 ) {
			ConAndCmdChannel( DOORLIGHT, 1 );
			ConAndCmdChannel( WALLLIGHT, 1 );
			ConAndCmdChannel( HALLLIGHT, 1 );
			ConAndCmdChannel( ATMOLIGHT, 1 );
		}
		PlayMp3Rand( "music", NULL );
	}
}

void HumanSwitchOff(){
}

