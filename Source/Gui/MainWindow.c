//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 项目名    ：智能家居
//
// 文件描述  ：主控程序入口，建立两个进程，一个是状态机切换进程，一个
//             是与网页cgi程序通讯进程。
//
// 修改记录  ：
//
// 创建日期  ：2014年11月25日
//
// 项目作者  ：沈阳工业大学 老井 laojingsut@gmail.com
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#include <stdio.h>
#include <time.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>
#include <string.h>
#include <pthread.h>

#include "../Socket/Com.h"
#include "../Tools/Mp3.h"
#include "../Tools/Voice.h"

#include "MainWindow.h"

int endSignal = 0;

int 
jjmain ( int argc, char *argv[] ) {

 	VoiceInitToken();
	Mp3Init();

	pthread_t threadVoice;
	pthread_t threadMain;
	pthread_t threadCom;

//	pthread_create ( &threadMain, NULL, MainLoop, NULL );
//	pthread_create ( &threadCom, NULL, ComThread, NULL );

	pthread_create ( &threadVoice, NULL, VoicecmdThread, NULL );
//	GetVoiceInput();

//	char *voice = malloc( strlen( "任务完成" ) + 1 );
//	strcpy( voice, "任务完成" );
//	VoiceGet( voice );
//
//	Mp3Play( VOICEDIR );


	getchar();
	endSignal = 1;
	pthread_join ( threadVoice, NULL );
//	pthread_join ( threadMain, NULL );
//	pthread_join ( threadCom, NULL );

	Mp3Free();

	return 0;
}
//	Mp3Play( RESPONSEDIR );
//	Mp3Play( RESPONSEDIR );
//	char *voice = malloc( strlen( "宝塔镇河妖" ) + 1 );
//	strcpy( voice, "宝塔镇河妖" );
//	VoiceGet( voice );

//	Mp3Play( VOICEDIR );

//	int sfd = open( "/dev/dsp", O_WRONLY );
//	close( sfd );

//	VoiceCmd( "/root/mp3/voice/voice.mp3", PLAYMUSIC );


