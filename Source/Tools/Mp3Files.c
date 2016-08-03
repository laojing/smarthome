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

struct node{
	char *name;
	struct node *next;
};

static int nodesize = 0;
static struct node *head = NULL;

char *GetMp3Name(){
	int nodecur = rand() % nodesize;
	struct node *temp = head;
	for( int i=0; i<nodecur; i++ ){
		temp = temp->next;
	}
	return temp->name;
}

void 
Mp3Init() {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	srand( time(NULL) );
	if( ( dp = opendir(MP3ROOT)) == NULL ) {
		printf( "cannot open mp3 dir!\n" );
		return;
	}
	chdir( MP3ROOT );
	while( (entry = readdir(dp)) != NULL ) {
		lstat( entry->d_name, &statbuf );
		if( !S_ISDIR(statbuf.st_mode) ){
//			if( strstr( entry->d_name, "music" ) == NULL ) continue;
			struct node *temp = malloc( sizeof(struct node) );
			temp->name = malloc( strlen(MP3ROOT) + strlen(entry->d_name) + 1 );
			strcpy( temp->name, MP3ROOT );
			strcat( temp->name, entry->d_name );
			if( head == NULL ){
				head = temp;
			} else {
				temp->next = head;
				head = temp;
			}
			nodesize++;
		}
	}
	chdir( ".." );
	closedir( dp );
}

void 
Mp3Free() {
	while( head != NULL ){
		struct node *temp = head;
		head = temp->next;
		free( temp->name );
		free( temp );
	}
}

