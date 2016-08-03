#include "MainWindow.h"
#include "../Tools/Serial.h"
#include "../AutoFile/StateMachine.h"

int sernum = 2;

void
Init232 () {
	OpenComport ( sernum, 9600 );

	fflush ( stdout );
}

void ReadTemp () {
	unsigned char rbuf[30];
	unsigned char sbuf[] = {0x11, 0x04, 0x00, 0x00, 0x00, 0x02, 0x73, 0x5B};
//	for ( int i=0; i<8; i++ ) {
//		SendByte ( sernum, sbuf[i] );
//	}

	int sernum = 1;
	OpenComport ( sernum, 9600 );
	usleep ( 1000000 );
	printf ( "send len:%d\n", SendBuf(sernum, sbuf, 8) );

	usleep ( 1000000 );

	int result;
	if ( ( result = PollComport(sernum, rbuf, 28) ) > 0 ) {
		printf ( "recived:%d\n", result );
	} else {
		printf ( "none\n" );
	}
	fflush ( stdout );
	CloseComport ( sernum );
}



void Read232 () {
	unsigned char buf[30];
	int result;
	if ( ( result = PollComport(sernum, buf, 29) ) > 0 ) {
		printf ( "==========\n" );
		fflush ( stdout );
		if ( buf[0] == 1 ) {
//			pSut->CI_LiveLightCmd = true;
//		} else if ( buf[0] == 2 ) {
//			pSut->CI_DispOpenCmd = true;
//		} else if ( buf[0] == 3 ) {
//			pSut->CI_DispCloseCmd = true;
		}
	}
}

void 
Close232 () {
	CloseComport ( sernum );
	printf ( "com end\n" );
	fflush ( stdout );
}

