#ifndef MP3_INCLUDED
#define MP3_INCLUDED

#define MP3ROOT "/root/mp3/"

char *GetMp3Name();
void Mp3Init();
void Mp3Free();
int Mp3Play( char *name );

#endif
