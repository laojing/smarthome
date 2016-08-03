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
#define NOISE_SUM_LIMIT 9
#define NOISE_VALID_NUM 10

#define VALID_MAX 3

float NoiseZero( float *signal, int len );
void NoiseLimit( float *signal, int len, float *noise_sum_max, float *noise_zero_max );
int EndPoint( float *signal, int len, float noise_sum_max, float noise_zero_max,  int *valid_start, int *valid_end );
double *MFCC( float *signal, int len, int *frame_num );


#endif /*__VoiceCMD_H__*/
