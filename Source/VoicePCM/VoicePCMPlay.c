#include <stdio.h>
#include <mad.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/ioctl.h>
#include <sys/soundcard.h>

#include "VoicePCM.h"

struct MadBuffer {
  unsigned char const *start;
  unsigned long length;
  int fdsp;
  int init;
};

static 
int PCMDecode(
		unsigned char const *start, 
		unsigned long length,
		int fdsp );

int 
VoicePCMPlay( int fdsp, char *name ) {
	struct stat stat;
	int fd;
	void *fdm;

    ioctl( fdsp, SOUND_PCM_RESET, NULL ); 

	fd = open( name, O_RDONLY );

	if( fstat(fd, &stat) == -1 || stat.st_size == 0 ) return 4;

	fdm = mmap( NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0 );
	if( fdm == MAP_FAILED ) return 4;

	PCMDecode( fdm, stat.st_size, fdsp );
	sleep( 1 );

	if( munmap(fdm, stat.st_size) == -1 ) return 4;

	close( fd );
	return 0;
}

static
enum mad_flow PCMInput(
		void *data,
		struct mad_stream *stream ) {

  struct MadBuffer *buffer = data;

  if( !buffer->length ) return MAD_FLOW_STOP;

  mad_stream_buffer( stream, buffer->start, buffer->length );
  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

static inline
signed int PCMScale( mad_fixed_t sample ) {
	sample += (1L << (MAD_F_FRACBITS - 16));

	if (sample >= MAD_F_ONE) sample = MAD_F_ONE - 1;
	else if (sample < -MAD_F_ONE) sample = -MAD_F_ONE;

	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static
enum mad_flow PCMOutput(
		void *data,
		struct mad_header const *header,
		struct mad_pcm *pcm ) {

  	unsigned char output[4], *OutputPtr;
	int arg;
	struct MadBuffer *buffer = data;
	unsigned int nsamples  = pcm->length;
	mad_fixed_t const *left_ch, *right_ch;

  	left_ch   = pcm->samples[0];
  	right_ch  = pcm->samples[1];

	if( buffer->init == 0 ) {
		arg = AFMT_S16_LE;
		IOCtrlSet( buffer->fdsp, SNDCTL_DSP_SETFMT, &arg, "Set Format" );
		arg = pcm->channels;
		IOCtrlSet( buffer->fdsp, SNDCTL_DSP_CHANNELS, &arg, "Set Channels" );
		arg = pcm->samplerate;
		IOCtrlSet( buffer->fdsp, SNDCTL_DSP_SPEED, &arg, "Set Speed" );
		buffer->init = 1;
	}

  while (nsamples--) {
    signed int sample;
  	OutputPtr = output;

    sample = PCMScale(*left_ch++);
	*(OutputPtr++) = sample >> 0;
	*(OutputPtr++) = sample >> 8;

    if( pcm->channels == 2 ) {
      	sample = PCMScale(*right_ch++);
		*(OutputPtr++) = sample >> 0;
		*(OutputPtr++) = sample >> 8;
		OutputPtr = output;
		write( buffer->fdsp, OutputPtr, 4 );
    }else {
		OutputPtr = output;
		write( buffer->fdsp, OutputPtr, 2 );
	}
  }

  return MAD_FLOW_CONTINUE;
}

static
enum mad_flow PCMError(
		void *data,
		struct mad_stream *stream,
		struct mad_frame *frame ) {
	struct MadBuffer *buffer = data;
	fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
		stream->error, mad_stream_errorstr(stream),
		stream->this_frame - buffer->start);
	return MAD_FLOW_CONTINUE;
}

static 
int PCMDecode(
		unsigned char const *start, 
		unsigned long length,
		int fdsp ) {

	struct MadBuffer buffer;
	struct mad_decoder decoder;
	int result;
	buffer.start  = start;
	buffer.length = length;
	buffer.fdsp = fdsp;
	buffer.init = 0;
	mad_decoder_init( &decoder, &buffer,
		   PCMInput, 0 /* header */, 0 /* filter */, PCMOutput,
		   PCMError, 0 /* message */ );
	result = mad_decoder_run( &decoder, MAD_DECODER_MODE_SYNC );
	mad_decoder_finish( &decoder );

	return result;
}

void IOCtrlSet( int fd, int device, int *val, char *msg ) {
	int tmp = *val;
	if( ioctl( fd, device, val ) == -1 ) {
		printf( "Error %s\n", msg );
	}
	if( tmp != *val ) {
		printf( "Error %s\n", msg );
	}
}
