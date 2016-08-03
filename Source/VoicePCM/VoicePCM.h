#ifndef __VoicePCM_H__
#define __VoicePCM_H__

#define TOKENDIR "/root/mp3/voice/token"
#define READYDIR "/root/mp3/voice/ready.mp3"
#define TASKOKDIR "/root/mp3/voice/taskok.mp3"
#define QUITDIR "/root/mp3/voice/quit.mp3"
#define VOICEDIR "/root/mp3/voice/voice.mp3"
#define RESPONSEDIR "/root/mp3/voice/response.mp3"
#define STANDDIR "/root/mp3/voice/stand.pcm"
#define TMPSTANDDIR "/root/mp3/voice/standtmp.pcm"

#define MAXLENGTH 5    						// 最大存储秒数
//#define CODESECOND MAXLENGTH    			// 口令辨识存储秒数
#define CODESECOND 5    			// 口令辨识存储秒数
#define CMDSECOND 3    			            // 语音命令存储秒数
#define RATE 8000   						// 采样频率
#define SIZE 16     						// 量化位数
#define CHANNELS 1  						// 声道数目
#define SECONDSIZE RATE*SIZE*CHANNELS/8
void IOCtrlSet( int fd, int device, int *val, char *msg );
int VoicePCMPlay( int fdsp, char *name );
signed char *VoicePCMRecord( int fdsp, int second );

#define PLAYMUSIC 1
#define PLAYVOICE 2
void VoicePCMCmd( char *cmd, int type );

#define MP3ROOT "/root/mp3/"
#define MP3DICTDIR "/root/mp3/dict"
void PlayMp3Rand( char *type, char *autho );
void VoiceMp3FileLoad();
void VoiceMp3FileFree();

char *GetToken();
int Text2Voice( char *voice );
char *Voice2Text( char *audiodata, int buflen );

void VoiceCodeStandInit();
int MonitorVoiceCode( int fdsp );

#endif /*__VoicePCM_H__*/
