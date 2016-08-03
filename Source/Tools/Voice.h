#ifndef VOICE_INCLUDED
#define VOICE_INCLUDED

#define PLAYMUSIC 1
#define PLAYVOICE 2

#define TOKENDIR "/root/mp3/voice/token"
#define READYDIR "/root/mp3/voice/ready.mp3"
#define TASKOKDIR "/root/mp3/voice/taskok.mp3"
#define QUITDIR "/root/mp3/voice/quit.mp3"
#define VOICEDIR "/root/mp3/voice/voice.mp3"
#define RESPONSEDIR "/root/mp3/voice/response.mp3"
#define STANDDIR "/root/mp3/voice/stand.pcm"

#define LENGTH 5    /* 存储秒数 */
#define RATE 8000   /* 采样频率 */
#define SIZE 16     /* 量化位数 */
#define CHANNELS 1  /* 声道数目 */
#define BUFSIZE LENGTH*RATE*SIZE*CHANNELS/8
#define VOICECMDSIZE 3*RATE*SIZE*CHANNELS/8

void IoSet( int fd, int device, int *val, char *msg );
 
int GetVoiceInput();
int CompareStand( signed char *buffer );

int VoiceGet( char *voice );
void VoiceCmd( char *cmd, int type );
void *VoicecmdThread();
char *VoiceSet( char *buf, int buflen );

extern char token[];
void VoiceInitToken();
int UpdateAuthenToken();

void VoiceModeHandle();

#endif
