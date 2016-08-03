#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "MainWindow.h"
#include "../AutoFile/StateMachine.h"
#include "../MainFrame/Main.h"

#define STARTNUM 1
#define SWITCHNUM 3
#define CHANNELS 16

char *ips[] = {
	"192.168.1.10",
	"192.168.1.11",
	"192.168.1.12"
};


/*
char buf[512];
unsigned char sa[3][16] = {0};
char *chs[SWITCHNUM][CHANNELS] = {
	{ 
		"次卧感应", "主卧感应",
		"书房感应", "走廊感应",
		"入门感应", "客厅感应",
		"7", "过渡开关",
		"9", "10",
		"11", "12",
		"13", "14",
		"15", "沙发灯"
	},
	{
		"沙发开关", "门前开关",
		"吧台开关", "门厅开关",
		"餐厅开关", "入门开关",
		"次卫开关", "厨房开关",
		"吧台灯", "入门灯",
		"门前灯", "门厅灯",
		"过渡灯", "餐厅灯",
		"走廊灯", "厨房灯"
	},
	{
		"书房开关", "走廊开关",
		"主卫开关", "衣帽开关",
		"主卧开关", "次卧开关",
		"客厅开关", "阳台开关",
		"次卫灯", "主卫灯",
		"书房灯", "衣帽灯",
		"主卧灯", "次卧灯",
		"客厅灯", "阳台灯"
	}
};

typedef struct _Socket Socket;
struct _Socket {
	int shand;
	struct sockaddr_in addr;
};
Socket socks[3];

U1 *vars[SWITCHNUM][CHANNELS];
U1 *relays[SWITCHNUM][CHANNELS];
int
InitSocket () {
	U1 temp = 0;
	vars[0][0] = &(pSut->CI_SecdHuman);
	vars[0][1] = &(pSut->CI_MainHuman);
	vars[0][2] = &(pSut->CI_BookHuman);
	vars[0][3] = &(pSut->CI_PassHuman);
	vars[0][4] = &(pSut->CI_DoorHuman);
	vars[0][5] = &(pSut->CI_LiveHuman);
	vars[0][6] = &temp;
	vars[0][7] = &(pSut->CI_AtmoSwitch);
	vars[0][8] = &temp;
	vars[0][9] = &temp;
	vars[0][10] = &temp;
	vars[0][11] = &temp;
	vars[0][12] = &temp;
	vars[0][13] = &temp;
	vars[0][14] = &temp;
	vars[0][15] = &(pSut->CI_SofaLightState);
	vars[1][0] = &(pSut->CI_SofaSwitch);
	vars[1][1] = &(pSut->CI_WallSwitch);
	vars[1][2] = &(pSut->CI_BarsSwitch);
	vars[1][3] = &(pSut->CI_HallSwitch);
	vars[1][4] = &(pSut->CI_RestSwitch);
	vars[1][5] = &(pSut->CI_DoorSwitch);
	vars[1][6] = &(pSut->CI_ToilSwitch);
	vars[1][7] = &(pSut->CI_KichSwitch);
	vars[1][8] = &(pSut->CI_BarsLightState);
	vars[1][9] = &(pSut->CI_DoorLightState);
	vars[1][10] = &(pSut->CI_WallLightState);
	vars[1][11] = &(pSut->CI_HallLightState);
	vars[1][12] = &(pSut->CI_AtmoLightState);
	vars[1][13] = &(pSut->CI_RestLightState);
	vars[1][14] = &(pSut->CI_PassLightState);
	vars[1][15] = &(pSut->CI_KichLightState);
	vars[2][0] = &(pSut->CI_BookSwitch);
	vars[2][1] = &(pSut->CI_PassSwitch);
	vars[2][2] = &(pSut->CI_BathSwitch);
	vars[2][3] = &(pSut->CI_CoatSwitch);
	vars[2][4] = &(pSut->CI_MainSwitch);
	vars[2][5] = &(pSut->CI_SecdSwitch);
	vars[2][6] = &(pSut->CI_LiveSwitch);
	vars[2][7] = &(pSut->CI_SunsSwitch);
	vars[2][8] = &(pSut->CI_ToilLightState);
	vars[2][9] = &(pSut->CI_BathLightState);
	vars[2][10] = &(pSut->CI_BookLightState);
	vars[2][11] = &(pSut->CI_CoatLightState);
	vars[2][12] = &(pSut->CI_MainLightState);
	vars[2][13] = &(pSut->CI_SecdLightState);
	vars[2][14] = &(pSut->CI_LiveLightState);
	vars[2][15] = &(pSut->CI_SunsLightState);

	relays[0][0] = &temp;
	relays[0][1] = &temp;
	relays[0][2] = &temp;
	relays[0][3] = &temp;
	relays[0][4] = &temp;
	relays[0][5] = &temp;
	relays[0][6] = &temp;
	relays[0][7] = &(pSut->S_SofaUpdate);
	relays[0][8] = &temp;
	relays[0][9] = &temp;
	relays[0][10] = &temp;
	relays[0][11] = &temp;
	relays[0][12] = &temp;
	relays[0][13] = &temp;
	relays[0][14] = &temp;
	relays[0][15] = &(pSut->CO_SofaLight);

	relays[1][0] = &(pSut->S_BarsUpdate);
	relays[1][1] = &(pSut->S_DoorUpdate);
	relays[1][2] = &(pSut->S_WallUpdate);
	relays[1][3] = &(pSut->S_HallUpdate);
	relays[1][4] = &(pSut->S_AtmoUpdate);
	relays[1][5] = &(pSut->S_RestUpdate);
	relays[1][6] = &(pSut->S_PassUpdate);
	relays[1][7] = &(pSut->S_KichUpdate);
	relays[1][8] = &(pSut->CO_BarsLight);
	relays[1][9] = &(pSut->CO_DoorLight);
	relays[1][10] = &(pSut->CO_WallLight);
	relays[1][11] = &(pSut->CO_HallLight);
	relays[1][12] = &(pSut->CO_AtmoLight);
	relays[1][13] = &(pSut->CO_RestLight);
	relays[1][14] = &(pSut->CO_PassLight);
	relays[1][15] = &(pSut->CO_KichLight);

	relays[2][0] = &(pSut->S_ToilUpdate);
	relays[2][1] = &(pSut->S_BathUpdate);
	relays[2][2] = &(pSut->S_BookUpdate);
	relays[2][3] = &(pSut->S_CoatUpdate);
	relays[2][4] = &(pSut->S_MainUpdate);
	relays[2][5] = &(pSut->S_SecdUpdate);
	relays[2][6] = &(pSut->S_LiveUpdate);
	relays[2][7] = &(pSut->S_SunsUpdate);
	relays[2][8] = &(pSut->CO_ToilLight);
	relays[2][9] = &(pSut->CO_BathLight);
	relays[2][10] = &(pSut->CO_BookLight);
	relays[2][11] = &(pSut->CO_CoatLight);
	relays[2][12] = &(pSut->CO_MainLight);
	relays[2][13] = &(pSut->CO_SecdLight);
	relays[2][14] = &(pSut->CO_LiveLight);
	relays[2][15] = &(pSut->CO_SunsLight);

	for ( int i=STARTNUM; i<SWITCHNUM; i++ ) {
		if ( (socks[i].shand = socket ( AF_INET, SOCK_STREAM, 0 )) == -1 ) {
			printf ( "Creat Socket %d Failed!\n", i+1 );
			fflush ( stdout );
			close ( socks[i].shand );
			return 0;
		}
		memset ( (char*)&(socks[i].addr), '\0', sizeof(struct sockaddr_in) );
		socks[i].addr.sin_family = AF_INET;
		socks[i].addr.sin_port = htons(1234);
		socks[i].addr.sin_addr.s_addr = inet_addr(ips[i]);
		bzero ( &(socks[i].addr.sin_zero), 8 );

		if ( connect ( socks[i].shand, (struct sockaddr*)&socks[i].addr, sizeof(struct sockaddr_in) ) == -1 ) {
			printf ( "Connect %s Failed!\n", ips[i] );
			fflush ( stdout );
			close ( socks[i].shand );
			return 0;
		}
	}
	return 1;
}

BOOL RelayState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "relay" );
		if ( end ) end += 5;
	}
	if ( strlen(end) > 2 ) {
		if ( end[1] == 'f' ) { 
			return false;
		}
		else return true;
	} else {
		return false;
	}
}

BOOL InputState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "I" );
		if ( end ) end += 1;
	}
	if ( strlen(end) >= 1 ) {
		if ( end[0] == 'L' ) return false;
		else return true;
	} else {
		return false;
	}
}

void
CloseSocket () {
	for ( int i=STARTNUM; i<SWITCHNUM; i++ ) {
		close ( socks[i].shand );
	}
}
void
ReadSocket () {
	int reclen = 0;
	for ( int j=STARTNUM; j<SWITCHNUM; j++ ) {
		if ( send ( socks[j].shand, "dump", 4, 0 ) == -1 ) {
			printf ( "Board %d Send Command Failed!\n", j );
			fflush ( stdout );
			close ( socks[j].shand );
			return;
		}
		if ( reclen = recv ( socks[j].shand, buf, sizeof(buf), 0 ) ) {
			if ( reclen < 100 ) continue;
			for ( int i=0; i<8; i++ ) {
				unsigned char ch = InputState ( buf, i+1 );
				*(vars[j][i]) = ch;
//				if ( ch != sa[j][i] ) {
//					sa[j][i] = ch;
//					printf ( "Blcok %d: %03d:Change To:%d\n", j, i+1, ch );
//					fflush ( stdout );
//				}
			}

			for ( int i=0; i<8; i++ ) {
				unsigned char ch = RelayState ( buf, i+1 );
				*(vars[j][i+8]) = ch;
//				if ( ch != sa[j][i+8] ) {
//					sa[j][i+8] = ch;
//					printf ( "Blcok %d: %03d:State:%d\n", j, i+1, ch );
//					fflush ( stdout );
//				}
			}
		}
	}
}

char cmd[2];
void 
WriteSocket() {
	for ( int j=STARTNUM; j<SWITCHNUM; j++ ) {
		for ( int i=0; i<CHANNELS/2; i++ ) {
			if ( *(relays[j][i]) == true ) {
				if ( *(relays[j][i+8]) ) cmd[0] = 'L';
				else cmd[0] = 'D';
				cmd[1] = '0' + (i+1);
				if ( send ( socks[j].shand, cmd, 2, 0 ) == -1 ) {}
				*(relays[j][i]) = 0;
			}
		}
	}
}
*/




















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

BOOL RelayState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "relay" );
		if ( end ) end += 5;
	}
	if ( strlen(end) > 2 ) {
		if ( end[1] == 'f' ) { 
			return false;
		}
		else return true;
	} else {
		return false;
	}
}

BOOL InputState ( char *buf, int index ) {
	char *end = buf;
	while ( index-- ) {
		end = strstr ( end, "I" );
		if ( end ) end += 1;
	}
	if ( strlen(end) >= 1 ) {
		if ( end[0] == 'L' ) return false;
		else return true;
	} else {
		return false;
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
