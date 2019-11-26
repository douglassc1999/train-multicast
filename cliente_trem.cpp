/* Diego Bruno Dantas Diógenes
Douglas Carvalho*/

#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>
#include <semaphore.h>
#include <string>

#include "BlackGPIO/BlackGPIO.h"
#include "ADC/Adc.h"

using namespace BlackLib;

#define MULTICAST_ADDR "225.0.0.37"
#define PORT_SCKT_CLIENT 9734

#define NUM_TRAINS 4

void *entradas(void *arg);

int main(int argc, char *argv[])
{

	int res;
	pthread_t thread1;

	void *thread_result;

	res = pthread_create(&thread1, NULL, entradas, NULL);
	if (res != 0)
	{
		perror("Criação da thread 1 falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_join(thread1, &thread_result);
	if (res != 0)
	{
		perror("Juncao da Thread 1 falhou");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);

	return 0;
}

void *entradas(void *arg)
{
	int tempo[NUM_TRAINS];

	ADC adc_1(AINx::AIN0);
	ADC adc_2(AINx::AIN1);
	ADC adc_3(AINx::AIN2);
	ADC adc_4(AINx::AIN3);

	int sockfd;
	int len;
	struct sockaddr_in address;

	sockfd = socket(AF_INET, SOCK_DGRAM, 0);

	address.sin_family = AF_INET;
	address.sin_addr.s_addr = inet_addr(MULTICAST_ADDR);
	address.sin_port = htons(PORT_SCKT_CLIENT);
	len = sizeof(address);

	while (true)
	{
		tempo[0] = adc_1.getIntValue();
		tempo[1] = adc_2.getIntValue();
		tempo[2] = adc_3.getIntValue();
		tempo[3] = adc_4.getIntValue();

		sendto(sockfd, tempo, sizeof(tempo), 0, (struct sockaddr *)&address, len);

		printf("Cliente envia: (Trem 1, Trem 2, Trem 3, Trem 4) = (%d, %d, %d, %d)\n", tempo[0], tempo[1], tempo[2], tempo[3]);

		sleep(1);
	}

	close(sockfd);
}
