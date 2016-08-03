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

struct dnode{
	char *cmd;
	int type;
	struct dnode *next;
	struct dnode *prev;
};

static struct dnode *head = NULL;
static struct dnode *tail = NULL;

signed char buf[BUFSIZE];

int SureVoiceInput() {
	char *result = VoiceSet( buf, BUFSIZE );
	if( strstr( result, "长江" ) ) return 1;
	else return 0;
}

int GetVoiceInput() {
	int i;
	float sum = 0;
	int result = 0;
	int fd; 
	int arg; 
	int status;

	fd = open("/dev/dsp", O_RDWR);
	if (fd < 0) {
		perror("open of /dev/dsp failed");
		exit(1);
	}

	/* 设置采样时的量化位数 */
	arg = SIZE;
	status = ioctl(fd, SOUND_PCM_WRITE_BITS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_BITS ioctl failed");
	if (arg != SIZE)
		perror("unable to set sample size");

	/* 设置采样时的声道数目 */
	arg = CHANNELS;
	status = ioctl(fd, SOUND_PCM_WRITE_CHANNELS, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_CHANNELS ioctl failed");
	if (arg != CHANNELS)
		perror("unable to set number of channels");

	/* 设置采样时的采样频率 */
	arg = RATE;
	status = ioctl(fd, SOUND_PCM_WRITE_RATE, &arg);
	if (status == -1)
		perror("SOUND_PCM_WRITE_WRITE ioctl failed");

	status = read(fd, buf, sizeof(buf)); /* 录音 */
	if (status != sizeof(buf))
		perror("read wrong number of bytes");
	else {
		if( CompareStand( buf ) )
			result = SureVoiceInput();
//		int fd = open( STANDDIR, O_WRONLY|O_CREAT, 0777 );
//		write( fd, buf, status );
//		close( fd );
//		result = CompareStand( buf );

	}
	close(fd);
	return result;
}


extern int endSignal;
int voicemode = 0;

void
*VoicecmdThread() {
	while( !endSignal ) {
		if( voicemode > 0 ) {
			VoiceModeHandle();
			if( voicemode > 0 ) Mp3Play( READYDIR );
		} else {
			if( head != NULL ) {
				struct dnode *temp = head;
				head = head->next;
				if( head == NULL ) tail = NULL;
				if( temp->type == PLAYMUSIC ) {
					Mp3Play( temp->cmd );
				} else {
					VoiceGet( temp->cmd );
	//				Mp3Play( VOICEDIR );
				}
				free( temp );
			} else {
				if( GetVoiceInput( ) ) {
					Mp3Play( RESPONSEDIR );
					Mp3Play( READYDIR );
					voicemode = 1;
				}
			}
			sleep( 1 );
		}
	}
}

void 
VoiceCmd( char *cmd, int type ) {
	if( type == PLAYMUSIC ) {
		char *str = "";
		if( strstr( cmd, "music" ) ) {
			str = "请您听个小曲。";
		} else if( strstr( cmd, "yueyunpeng" ) ) {
			str = "请您听段相声，岳云鹏孙越。";
		} else {
			str = "请您听段相声，郭德纲于谦。";
		}
		char *temp = malloc( strlen(str) + 1 );
		strcpy( temp, str );
		VoiceCmd( temp, PLAYVOICE );
	}

	struct dnode *temp = malloc( sizeof(struct dnode) );
	temp->cmd = cmd;
	temp->type = type;
	temp->prev = tail;
	temp->next = NULL;
	if( tail == NULL ) {
		head = temp;
	} else {
		tail->next = temp;
	}
	tail = temp;
}

