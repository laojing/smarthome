//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//
// 项目名    ：通用逻辑控制设计器
//
// 文件描述  ：界面定义头文件
//
// 修改记录  ：
//
// 创建日期  ：2012年08月30日
//
// 作者      ：沈阳工业大学 老井 laojingsut@gmail.com
//
//++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/time.h>
#include "../home.h"

#define PROGRAMNAME "laojing"                             //语言文件名称
#define LANGUAGEDIR "Languages"                             //语言文件位置

#define TOILLIGHT	0
#define BATHLIGHT	1
#define BOOKLIGHT	2
#define COATLIGHT	3
#define MAINLIGHT	4
#define SECDLIGHT	5
#define LIVELIGHT	6
#define SOFALIGHT	7

#define BARSLIGHT	8
#define DOORLIGHT	9
#define WALLLIGHT	10
#define HALLLIGHT	11
#define ATMOLIGHT	12
#define RESTLIGHT	13
#define PASSLIGHT	14
#define KICHLIGHT	15

#define SUNSLIGHT	16
#define HUMANPOWER	17
#define BOARD1POWER	18
#define BOARD2POWER	19

#define BOARDCHANS	8
#define OPEN		1
#define CLOSE		0

int ConAndCmdChannel( int channel, int open );
int ConAndReadBoard( int board, int *inputs, int *relays );













int InitSocket();
void ReadSocket();
void WriteSocket();
void CloseSocket();

void Init232 ();
void Read232 ();
void ReadTemp ();
void Close232 ();

void *MainLoop ();
int GetLightValue ( int index );
int GetHumanValue ( int index );
int GetStateValue ( int index );

#endif /* __MAINWINDOW_H__ */
