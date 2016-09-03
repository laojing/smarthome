#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/soundcard.h>

#include "VoicePCM.h"
#include "../VoiceCMD/VoiceCMD.h"
#include "../VoiceCMD/VoiceTemplate.h"

struct dnode{
	char *cmd;
	int type;
	struct dnode *next;
	struct dnode *prev;
};

static struct dnode *head = NULL;
static struct dnode *tail = NULL;

void *VoicePCMThread( void *ptrEndSignal ) {
 	int fdsp;
	if( (fdsp = open( "/dev/dsp", O_RDWR )) < 0 ) {
		printf( "open dsp error!d\n" ); fflush(stdout);
	  	return NULL;
	}
	VoiceMp3FileLoad();
//	VoiceCodeStandInit();
	VoiceTemplateInit();
//	VoiceTemplateRecord( fdsp, "tuixia" );

	SaveRawWord( fdsp, "xiaoqu" );
	
	int VoiceMode = 0;

//	while( !(*(int*)ptrEndSignal) ) {
	while( VoiceMode ) {

		// 语音控制模式
		if( VoiceMode > 0 ) {
			if( VoiceMode++ > 10 ) {
				VoiceMode = 0;
				VoicePCMPlay( fdsp, QUITDIR );
			} else {
				VoicePCMPlay( fdsp, READYDIR );
				VoiceWordIdent( fdsp, 1, &VoiceMode );
			}
		// 正常模式，有播放任务就做，没有的话，就辨识输入口令
		} else {
			// 任务队列有任务，弹出一个处理
			if( head != NULL ) {
				struct dnode *temp = head;
				head = head->next;
				if( head == NULL ) tail = NULL;
				if( temp->type == PLAYMUSIC ) {
					VoicePCMPlay( fdsp, temp->cmd );
				} else {
					Text2Voice( temp->cmd );
					VoicePCMPlay( fdsp, VOICEDIR );
				}
				free( temp->cmd );
				free( temp );
			// 空闲状态，辨识输入口令
			} else {
				if( VoiceWordIdent( fdsp, 0, NULL ) ) {
					VoicePCMPlay( fdsp, RESPONSEDIR );
					VoiceMode = 1;
				}
//				PlayMp3Rand();
//				if( MonitorVoiceCode( fdsp ) ) {
//					VoicePCMPlay( fdsp, RESPONSEDIR );
//					VoiceMode = 1;
//				}
			}
		}
	}

	VoiceMp3FileFree();
  	VoiceTemplateFree();
	close( fdsp );
}

void 
VoicePCMCmd( char *cmd, int type ) {
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
