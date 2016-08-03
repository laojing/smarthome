#include "MainWindow.h"
#include "../MainFrame/Main.h"
#include "../AutoFile/StateMachine.h"

void
ReadInputDelay() {
	int inputs[BOARDCHANS];
	int relays[BOARDCHANS];
	if( ConAndReadBoard( 2, inputs, relays ) == 0 ) {
		pSut->CI_SunsSwitch = inputs[0];
		pSut->CI_HumanPass = inputs[1];
		pSut->CI_HumanLive = inputs[2];
		pSut->CI_HumanMain = inputs[3];
		pSut->CI_HumanBook = inputs[4];
		pSut->CI_HumanSecd = inputs[5];
		pSut->CI_HumanDoor = inputs[6];

		pSut->CI_SunsLightState = relays[0];
		pSut->CI_HumanPowerState = relays[1];
	}
}

int UpdateCommand( int light, int open, U1 *update ) {
	if( *update ) {
		ConAndCmdChannel( light, open );
		*update = false;
		return 1;
	}
	return 0;
}

void
WriteOutDelay() {
	UpdateCommand( SUNSLIGHT, pSut->CO_SunsLight, &(pSut->S_SunsUpdate) );
	UpdateCommand( HUMANPOWER, pSut->CO_HumanPower, &(pSut->S_HumanUpdate) );
}

extern int endSignal;
void *MainLoop () {
	InitController ();
	while( !endSignal ) {
		ReadInputDelay();
		StepController();
		if ( StartStepTimer() > 3000 ) { 
			UpdateStateMachines ();
			WriteOutDelay();

//			printf( "Human:%d\n", 
//					pSut->S_HumaState );
//			fflush( stdout );

//			printf( "==:%02d:%02d:%02d\n",
//					pSut->S_DayTime/3600, 
//					(pSut->S_DayTime%3600)/60,
//					pSut->S_DayTime%60 );
//			fflush( stdout );
		}
		sleep( 1 );
	}
	pthread_exit ( NULL );
}
