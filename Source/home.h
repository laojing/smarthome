#ifndef __HOME_H__
#define __HOME_H__

#include <stdio.h>

#define null NULL


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


void *VoicePCMThread( void *ptrEndSignal );
void GiveTime();


























//====================================================================
// 数据传输结构体
//====================================================================
struct msg_st
{
    long int msg_type;                  // 消息类型
    int type;                           // 命令 SETLIGTH : 设置给定
                                        //      GETLIGHT : 查询给定
    int item;                           // 端口号
    int value;                          // 设定值或返回状态
};

#define MSGCGITOLOOP         1234
#define MSGLOOPTOCGI         4321

#define SETLIGHT             1
#define GETLIGHT             2
#define GETHUMAN             3
#define GETSTATE             4
#define CARTOPEN             5
#define CARTCLOSE            6

#define TOTALLIGHT           10
#define TOTALHUMAN           8
#define TOTALSTATE           17

#define FIRSTIP              "192.168.1.10"
#define SECONDIP             "192.168.1.11"
#define THIRDIP              "192.168.1.12"

#endif /*__HOME_H__*/
