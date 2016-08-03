#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "../home.h"

char *ips[] = {
	"192.168.1.10",
	"192.168.1.11",
	"192.168.1.12"
};

int ConAndCmdTest( int channel, int open ) {

	int shand;
	struct sockaddr_in addr;
	int board = channel / BOARDCHANS;
	char cmd[2];
	cmd[0] = open ? 'L' : 'D';
	cmd[1] = (channel % 8) + '1';

	if ( (shand = socket ( AF_INET, SOCK_STREAM, 0 )) == -1 ) {
		printf ( "Creat Socket %d Failed!\n", board+1 );
		fflush ( stdout );
		close ( shand );
		return 1;
	}

	memset ( (char*)&(addr), '\0', sizeof(struct sockaddr_in) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr(ips[board]);
	bzero ( &(addr.sin_zero), 8 );

	if ( connect ( shand, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) == -1 ) {
		printf ( "Connect %s Failed!\n", ips[board] );
		fflush ( stdout );
		close ( shand );
		return 1;
	}

	if ( send ( shand, cmd, 2, 0 ) == -1 ) {
		printf ( "Send Command Failed!\n" );
		fflush ( stdout );
		close ( shand );
		return 1;
	}

	close ( shand );
	return 0;
}

int ConAndCmdChannel( int channel, int open ) {
	if( ConAndCmdTest( channel, open) ) {
		int board = channel / BOARDCHANS;
		if( board !=  BOARD1POWER / BOARDCHANS ) {
			ConAndCmdChannel( BOARD1POWER+board, OPEN );
			sleep( 3 );
			ConAndCmdChannel( BOARD1POWER+board, CLOSE );
		}
		return 1;
	} else {
		return 0;
	}
}

unsigned char RelayState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "relay" );
		if ( end ) end += 5;
	}
	if ( strlen(end) > 2 ) {
		if ( end[1] == 'f' ) { 
			return 0;
		}
		else return 1;
	} else {
		return 0;
	}
}

unsigned char InputState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "I" );
		if ( end ) end += 1;
	}
	if ( strlen(end) >= 1 ) {
		if ( end[0] == 'L' ) return 0;
		else return 1;
	} else {
		return 0;
	}
}

int ConAndReadTest( int board, int *inputs, int *relays ) {

	int shand;
	struct sockaddr_in addr;
	int reclen = 0;
	char buf[512];

	if ( (shand = socket ( AF_INET, SOCK_STREAM, 0 )) == -1 ) {
		printf ( "Creat Socket %d Failed!\n", board+1 );
		fflush ( stdout );
		close ( shand );
		return 1;
	}

	memset ( (char*)&(addr), '\0', sizeof(struct sockaddr_in) );
	addr.sin_family = AF_INET;
	addr.sin_port = htons(1234);
	addr.sin_addr.s_addr = inet_addr(ips[board]);
	bzero ( &(addr.sin_zero), 8 );

	if ( connect ( shand, (struct sockaddr*)&addr, sizeof(struct sockaddr_in) ) == -1 ) {
		printf ( "Connect %s Failed!\n", ips[board] );
		fflush ( stdout );
		close ( shand );
		return 1;
	}

	if ( send ( shand, "dump", 4, 0 ) == -1 ) {
		printf ( "Board %d Send dump Command Failed!\n", board+1 );
		fflush ( stdout );
		close ( shand );
		return 1;
	}
	if ( reclen = recv ( shand, buf, sizeof(buf), 0 ) ) {
		if ( reclen < 100 ) {
			printf ( "Board %d Recv len:%d Error!\n", board+1, reclen );
			fflush ( stdout );
			close ( shand );
			return 1;
		}
		for ( int i=0; i<8; i++ ) {
			inputs[i] = InputState ( buf, i+1 );
			relays[i] = RelayState ( buf, i+1 );
		}
	}

	close ( shand );
	return 0;
}

int ConAndReadBoard( int board, int *inputs, int *relays ) {
	if( ConAndReadTest( board, inputs, relays) ) {
		if( board !=  BOARD1POWER / BOARDCHANS ) {
			ConAndCmdChannel( BOARD1POWER+board, OPEN );
			sleep( 5 );
			ConAndCmdChannel( BOARD1POWER+board, CLOSE );
			sleep( 20 );
		}
		return 1;
	} else {
		return 0;
	}
}
