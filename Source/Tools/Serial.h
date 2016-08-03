#ifndef RS232_INCLUDED
#define RS232_INCLUDED

#include <stdio.h>
#include <string.h>

#include <termios.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <limits.h>

#define COMNUMBER		22
#define COMNAMELEN		13

int OpenComport(int, int);
int PollComport(int, unsigned char *, int);
int SendByte(int, unsigned char);
int SendBuf(int, unsigned char *, int);
void CloseComport(int);
int IsCTSEnabled(int);

#endif
