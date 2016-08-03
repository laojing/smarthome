#include <pthread.h>

#include "home.h"

int main() {

	int endSignal = 0;
    pthread_t threadVoicePCM;                               // PCM声卡控制线程

	pthread_create ( &threadVoicePCM, NULL, VoicePCMThread, (void *)&endSignal );

	getchar();
	endSignal = 1;

	pthread_join ( threadVoicePCM, NULL );

	return 0;
}

