#ifndef __VoiceCMD_H__
#define __VoiceCMD_H__

void VoiceModeCommand( int fdsp, int *voicemode );

char *ReadLine( FILE *fd );
void VoiceTemplateInit();
void VoiceTemplateFree();
void VoiceTemplateDelete( char *voice );
void VoiceTemplateRecord( int fdsp, char *voice );
int VoiceWordIdent( int fdsp, int mode, int *voicemode );
char *DynamicWarping( double *mfcc, int frame_num, char *mode[], int modelen );

#define IDENTLIMIT 20000

#define FS 8000
#define FRAME_LEN 160
#define FRAME_MOV 80
#define NDCTS 36
#define NOISE_SUM_LIMIT 9
#define NOISE_VALID_NUM 10

#define VALID_MAX 3

double NoiseZero( double *signal, int len );
void NoiseLimit( double *signal, int len, double *noise_sum_max, double *noise_zero_max );
int EndPoint( double *signal, int len, double noise_sum_max, double noise_zero_max,  int *valid_start, int *valid_end );
double *MFCC( double *signal, int len, int *frame_num );
void SaveVoice( double *signal, int len, char *name );


#endif /*__VoiceCMD_H__*/
