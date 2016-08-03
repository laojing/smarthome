#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <dirent.h>
#include <sys/stat.h>

#include "VoicePCM.h"

struct node{
	char *name;
	char *author;
	char *type;
	char *path;
	struct node *next;
};

static int musicsize = 0;
static int baoruisize = 0;
static int degangsize = 0;
static int xiaoyuesize = 0;
static int nodesize = 0;
static struct node *head = NULL;

void PlayMp3Rand( char *type, char *author ) {
	int tmpsize = musicsize;
	if( strcmp( type, "crosstalk" ) == 0 ) {
		if( strcmp( author, "degang" ) == 0 ) {
			tmpsize = degangsize;
		}else if( strcmp( author, "baorui" ) == 0 ) {
			tmpsize = baoruisize;
		}else if( strcmp( author, "xiaoyue" ) == 0 ) {
			tmpsize = xiaoyuesize;
		}
	}

	int curindex = 0;
	int nodecur = rand() % tmpsize;
	struct node *temp = head;
	while( temp != NULL ) {
//		printf( "%03d, type:%s, author:%s\n", nodecur, temp->type, temp->author );
		if( strcmp( type, temp->type ) == 0 ) {
			if( strcmp( type, "music" ) == 0 ) {
				if( curindex++ == nodecur ) break;
			} else {
				if( strcmp( author, "degang" ) == 0 ) {
					if( strcmp( temp->author, "郭德纲" ) == 0 ) {
						if( curindex++ == nodecur ) break;
					}
				}else if( strcmp( author, "baorui" ) == 0 ) {
					if( strcmp( temp->author, "刘宝瑞" ) == 0 ) {
						if( curindex++ == nodecur ) break;
					}
				}else if( strcmp( author, "xiaoyue" ) == 0 ) {
					if( strcmp( temp->author, "岳云鹏" ) == 0 ) {
						if( curindex++ == nodecur ) break;
					}
				}
			}
		}

		temp = temp->next;
	}

	/*
	for( int i=0; i<nodecur; i++ ){
		temp = temp->next;
	}

	while( strcmp(temp->type, type) != 0 ) {
		if( temp->next != NULL ) temp = temp->next;
		else temp = head;
	}
	if( strstr(temp->type, "crosstalk") != NULL ) {
		while( strcmp(temp->author, author) != 0 ) {
			if( temp->next != NULL ) temp = temp->next;
			else temp = head;
		}
	}
	*/


	char *strType = NULL;
	if( strstr(temp->type, "music") != NULL ) {
		strType = "请您听个小曲";
	} else {
		strType = "请您听段相声";
	}
	size_t size = strlen( strType ) 
		+ strlen( temp->author ) 
		+ strlen( temp->name ) 
		+ strlen( temp->type ) 
		+ strlen( "，，。" ) + 1; 
	char *str = malloc( size );
	sprintf( str, "%s，%s，%s。", strType, temp->author, temp->name );

	VoicePCMCmd( str, PLAYVOICE );

	char *path = malloc( strlen( temp->path ) + 1 );
	strcpy( path, temp->path );

	VoicePCMCmd( path, PLAYMUSIC );
}

char*
VoiceDictLoad() {
	int fd, size;
	struct stat statbuf;  
	stat( MP3DICTDIR, &statbuf );  
	size = statbuf.st_size;  

	char *result = malloc( size );

	fd = open( MP3DICTDIR, O_RDONLY );
	size = read(fd, result, size);
	close(fd);

	return result;
}

void 
VoiceMp3FileLoad() {
	DIR *dp;
	struct dirent *entry;
	struct stat statbuf;
	srand( time(NULL) );

	char *dictbuf = VoiceDictLoad();

	if( ( dp = opendir(MP3ROOT)) == NULL ) {
		printf( "cannot open mp3 dir!\n" );
		return;
	}
	chdir( MP3ROOT );
	while( (entry = readdir(dp)) != NULL ) {
		lstat( entry->d_name, &statbuf );
		if( strstr(entry->d_name, "mp3") != NULL ){
//			if( strstr( entry->d_name, "guodegang" ) != NULL ) continue;
			struct node *temp = malloc( sizeof(struct node) );

			char *strname = malloc( strlen( entry->d_name ) + 1 );
			strcpy( strname, entry->d_name );
			char *tmp = strstr( strname, " - " );

			// mp3文件类型
			temp->type = malloc( tmp - strname + 1 );
			strncpy( temp->type, strname, tmp - strname );
			temp->type[tmp - strname] = 0;
			if( strstr( temp->type, "music" ) != NULL ) musicsize++;

			// mp3表演者
			char *tmp2 = tmp + 3;
			tmp = strstr( tmp2, " - " );
			char *tmpauthor = malloc( tmp - tmp2 + 1 );
			strncpy( tmpauthor, tmp2, tmp - tmp2 );
			tmpauthor[tmp-tmp2] = 0;
			char *findtmp = strstr( dictbuf, tmpauthor );
			if( findtmp != NULL ) {
				findtmp += 3 + strlen(tmpauthor);
				char *tmp3 = strstr( findtmp, "\n" );
				temp->author = malloc( tmp3 - findtmp + 1 );
				strncpy( temp->author, findtmp, tmp3 - findtmp );
				temp->author[tmp3 - findtmp] = 0;

				if( strstr( tmpauthor, "yueyunpeng" ) != NULL ) xiaoyuesize++;
				if( strstr( tmpauthor, "guodegang" ) != NULL ) degangsize++;
				if( strstr( tmpauthor, "liubaorui" ) != NULL ) baoruisize++;
			} else {
				temp->author = NULL;
				printf( "file name :%s\n", strname );
				printf( "lost author :%s\n", tmpauthor );
			}

			// mp3名称
			tmp2 = tmp + 3;
			tmp = strstr( tmp2, ".mp3" );
			char *tmpname = malloc( tmp - tmp2 + 1 );
			strncpy( tmpname, tmp2, tmp - tmp2 );
			tmpname[tmp-tmp2] = 0;
			findtmp = strstr( dictbuf, tmpname );
			if( findtmp != NULL ) {
				findtmp += 3 + strlen(tmpname);
				char *tmp3 = strstr( findtmp, "\n" );
				temp->name = malloc( tmp3 - findtmp + 1 );
				strncpy( temp->name, findtmp, tmp3 - findtmp );
				temp->name[tmp3 - findtmp] = 0;
			} else {
				temp->name = NULL;
				printf( "file name :%s\n", strname );
				printf( "lost name :%s\n", tmpname );
			}

			free( tmpauthor );
			free( tmpname );
			free( strname );

			temp->path = malloc( strlen(MP3ROOT) + strlen(entry->d_name) + 1 );
			strcpy( temp->path, MP3ROOT );
			strcat( temp->path, entry->d_name );

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
	free( dictbuf );

	printf( ";;%d\n", musicsize );
	printf( ";;%d\n", baoruisize );
	printf( ";;%d\n", degangsize );
	printf( ";;%d\n", xiaoyuesize );
}

void 
VoiceMp3FileFree() {
	while( head != NULL ){
		struct node *temp = head;
		head = temp->next;
		free( temp->name );
		free( temp->type );
		free( temp->author );
		free( temp->path );
		free( temp );
	}
}

