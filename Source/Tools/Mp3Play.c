#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/soundcard.h>
#include <mad.h> 

#include "Mp3.h"

static int decodestart = 0;
static int fdsp;
static int decode(unsigned char const *, unsigned long);

int 
Mp3Play( char *name ) {
	struct stat stat;
	int fd;
	void *fdm;

	if( (fdsp = open( "/dev/dsp", O_WRONLY )) < 0 ) {
		printf( "open dsp error:%d\n", errno ); fflush(stdout);
	  	return 4;
	}
	ioctl( fdsp, SOUND_PCM_RESET, NULL );

	fd = open( name, O_RDONLY );

	if( fstat(fd, &stat) == -1 || stat.st_size == 0 ) {
	 	close( fdsp );
	 	return 4;
	}

	fdm = mmap( NULL, stat.st_size, PROT_READ, MAP_SHARED, fd, 0 );
	if( fdm == MAP_FAILED ) {
	 	close( fdsp );
	 	return 4;
	}

	decodestart = 0;
	decode( fdm, stat.st_size );

	if( munmap(fdm, stat.st_size) == -1 ) {
	 	close( fdsp );
	 	return 4;
	}

	close( fd );
	close( fdsp );

	return 0;
}

void IoSet( int fd, int device, int *val, char *msg ) {
	int tmp = *val;
	if( ioctl( fd, device, val ) == -1 ) {
		printf( "Error %s\n", msg );
	  	exit (-1);
	}
	if( tmp != *val ) {
		printf( "Error %s\n", msg );
	  	exit (-1);
	}
}

/*
int 
Mp3Play( char *name ) {

	if( Mp3Decode( name ) == 0 ) {

		int fdsp = open("/dev/dsp", O_RDWR );
//		int fdsp = open("/dev/dsp", O_WRONLY );

		IoSet( fdsp, SNDCTL_DSP_SETFMT, &mixfmt, "Set Format" );
		IoSet( fdsp, SNDCTL_DSP_CHANNELS, &mixchannel, "Set Channels" );
		IoSet( fdsp, SNDCTL_DSP_SPEED, &mixspeed, "Set Speed" );

		int version = 0;
		ioctl( fdsp, OSS_GETVERSION, &version );
		printf( "version is:%x\n", version );


  		int wsize =  write( fdsp, mixbuffer, mixindex );

		close( fdsp );

		free( mixbuffer );
		mixbuffer = NULL;
	}

  	return 0;
}
*/


struct buffer {
  unsigned char const *start;
  unsigned long length;
};

static
enum mad_flow input(void *data,
		    struct mad_stream *stream)
{
  struct buffer *buffer = data;

  if (!buffer->length)
    return MAD_FLOW_STOP;

  mad_stream_buffer(stream, buffer->start, buffer->length);

  buffer->length = 0;

  return MAD_FLOW_CONTINUE;
}

/*
 * The following utility routine performs simple rounding, clipping, and
 * scaling of MAD's high-resolution samples down to 16 bits. It does not
 * perform any dithering or noise shaping, which would be recommended to
 * obtain any exceptional audio quality. It is therefore not recommended to
 * use this routine if high-quality output is desired.
 */

static inline
signed int scale(mad_fixed_t sample)
{
  /* round */
  sample += (1L << (MAD_F_FRACBITS - 16));

  /* clip */
  if (sample >= MAD_F_ONE)
    sample = MAD_F_ONE - 1;
  else if (sample < -MAD_F_ONE)
    sample = -MAD_F_ONE;

  /* quantize */
  return sample >> (MAD_F_FRACBITS + 1 - 16);
}

/*
 * This is the output callback function. It is called after each frame of
 * MPEG audio data has been completely decoded. The purpose of this callback
 * is to output (or play) the decoded PCM audio.
 */


static
enum mad_flow output(void *data,
		     struct mad_header const *header,
		     struct mad_pcm *pcm)
{
  unsigned int nchannels, nsamples;
  unsigned char output[12], *OutputPtr;
  int fmt, speed;
  int arg; /* 用于ioctl调用的参数 */
  mad_fixed_t const *left_ch, *right_ch;

  /* pcm->samplerate contains the sampling frequency */

  nchannels = pcm->channels;
  nsamples  = pcm->length;
  left_ch   = pcm->samples[0];
  right_ch  = pcm->samples[1];
  speed     = pcm->samplerate;


  fmt = AFMT_S16_LE;

  if( decodestart == 0 ) {
	IoSet( fdsp, SNDCTL_DSP_SETFMT, &fmt, "Set Format" );
	IoSet( fdsp, SNDCTL_DSP_CHANNELS, &nchannels, "Set Channels" );
	IoSet( fdsp, SNDCTL_DSP_SPEED, &speed, "Set Speed" );
	decodestart = 1;
  }

  while (nsamples--) {
    signed int sample;
  	OutputPtr = output;

    sample = scale(*left_ch++);
	*(OutputPtr++) = sample >> 0;
	*(OutputPtr++) = sample >> 8;

    if (nchannels == 2) {
      	sample = scale(*right_ch++);
		*(OutputPtr++) = sample >> 0;
		*(OutputPtr++) = sample >> 8;
		OutputPtr = output;
		write( fdsp, OutputPtr, 4 );
    }else {
		OutputPtr = output;
		write( fdsp, OutputPtr, 2 );
	}
  }

  return MAD_FLOW_CONTINUE;
}

static
enum mad_flow error(void *data,
		    struct mad_stream *stream,
		    struct mad_frame *frame)
{
  struct buffer *buffer = data;
  fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
	  stream->error, mad_stream_errorstr(stream),
	  stream->this_frame - buffer->start);
  return MAD_FLOW_CONTINUE;
}

static
int decode(unsigned char const *start, unsigned long length)
{
  struct buffer buffer;
  struct mad_decoder decoder;
  int result;
  buffer.start  = start;
  buffer.length = length;
  mad_decoder_init(&decoder, &buffer,
		   input, 0 /* header */, 0 /* filter */, output,
		   error, 0 /* message */);
  result = mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);
  mad_decoder_finish(&decoder);

  return result;
}
