#include <stdio.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include "VoicePCM.h"

signed char RecordBuf[MAXLENGTH*SECONDSIZE];

signed char* 
VoicePCMRecord( int fdsp, int second ) {

	int arg;
    ioctl( fdsp, SOUND_PCM_RESET, NULL ); 

	arg = SIZE;
	IOCtrlSet( fdsp, SNDCTL_DSP_SETFMT, &arg, "Set Format" );
	arg = CHANNELS;
	IOCtrlSet( fdsp, SNDCTL_DSP_CHANNELS, &arg, "Set Channels" );
	arg = RATE;
	IOCtrlSet( fdsp, SNDCTL_DSP_SPEED, &arg, "Set Speed" );

	int status = read( fdsp, RecordBuf, second*SECONDSIZE );
	if( status != second*SECONDSIZE ) return NULL;

	return RecordBuf;
}
