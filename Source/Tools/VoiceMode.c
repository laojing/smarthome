#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <sys/msg.h>
#include <sys/soundcard.h>
#include <arpa/inet.h>

#include "Voice.h"
#include "Mp3.h"
#include "../Gui/MainWindow.h"

extern signed char buf[BUFSIZE];
extern int voicemode;

void VoiceModeHandle() {
	int arg; 

	int fd = open("/dev/dsp", O_RDWR);
	if (fd < 0) { perror("open of /dev/dsp failed"); exit(1); }

	arg = SIZE;
	IoSet( fd, SOUND_PCM_WRITE_BITS, &arg, "Set Format" );
	arg = CHANNELS;
	IoSet( fd, SOUND_PCM_WRITE_CHANNELS, &arg, "Set Channels" );
	arg = RATE;
	IoSet( fd, SOUND_PCM_WRITE_RATE, &arg, "Set Speed" );

	char *result = NULL;
	if( read(fd, buf, VOICECMDSIZE) == VOICECMDSIZE ) {
		result = VoiceSet( buf, VOICECMDSIZE );
	}
	close(fd);


	if( result == NULL ) return;

	if( voicemode++ > 10 ) {
		Mp3Play( QUITDIR );
		voicemode = 0;
	}
	if( strstr( result, "下" ) ) { 
		Mp3Play( QUITDIR );
		voicemode = 0; 
	}
	if( strstr( result, "滚" ) ) { 
		Mp3Play( QUITDIR );
		voicemode = 0; 
	}

	if( strstr( result, "打开" ) ) { 
		if( strstr( result, "客厅" ) ) { 
			ConAndCmdChannel( DOORLIGHT, 1 );
			Mp3Play( TASKOKDIR );
		}
	}

	if( strstr( result, "关闭" ) ) { 
		if( strstr( result, "客厅" ) ) { 
			ConAndCmdChannel( DOORLIGHT, 0 );
			Mp3Play( TASKOKDIR );
		}
	}


}
