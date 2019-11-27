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

#define MULTICAST_ADDR "225.0.0.37"
#define PORT_SCKT_CLIENT 9734
#define PORT_SCKT_SERVER 9734 //mesma porta pois está no mesmo host

#define NUM_TRAINS 4

using namespace BlackLib;

pthread_mutex_t work_mutex_1, work_mutex_2, work_mutex_3, work_mutex_4, work_mutex_5;

BlackGPIO led_0_verde(GPIO_66, output); //verde 1
BlackGPIO led_1_verde(GPIO_67, output); //verde 2
BlackGPIO led_2_verde(GPIO_69, output); // verde 3

BlackGPIO led_3_amarelo(GPIO_45, output); // amarelo 1
BlackGPIO led_0_amarelo(GPIO_68, output); // amarelo 2
BlackGPIO led_1_amarelo(GPIO_44, output); // amarelo 3
BlackGPIO led_2_amarelo(GPIO_26, output); //amarelo 4

BlackGPIO led_2_azul(GPIO_20, output); //azul 1
BlackGPIO led_0_azul(GPIO_47, output); // azul 2
BlackGPIO led_1_azul(GPIO_27, output); // azul 3

BlackGPIO led_1_vermelho(GPIO_46, output); // vermelho 1
BlackGPIO led_2_vermelho(GPIO_65, output); // vermelho 2
BlackGPIO led_3_vermelho(GPIO_61, output); // vermelho 3
BlackGPIO led_0_vermelho(GPIO_60, output); //vermelho 4

BlackGPIO leds[14] = {led_0_verde,
					  led_1_verde,
					  led_2_verde,
					  led_0_amarelo,
					  led_1_amarelo,
					  led_2_amarelo,
					  led_3_amarelo,
					  led_0_azul,
					  led_1_azul,
					  led_2_azul,
					  led_0_vermelho,
					  led_1_vermelho,
					  led_2_vermelho,
					  led_3_vermelho};

void *thread_function_1(void *arg);
void *thread_function_2(void *arg);
void *thread_function_3(void *arg);
void *thread_function_4(void *arg);
void *thread_function_5(void *arg); // Para setar o sleep

float tempo[NUM_TRAINS];

void L(int trem, int apagar_1, int apagar_2, int apagar_3)
{
	leds[trem].setValue(high);
	leds[apagar_1].setValue(low);
	leds[apagar_2].setValue(low);
	leds[apagar_3].setValue(low);
	if (apagar_3 != -1)
		leds[apagar_3].setValue(low);
}

int main(int argc, char *argv[])
{

	int res;
	pthread_t a_thread, b_thread, c_thread, d_thread, e_thread;
	void *thread_result;

	// Iniciar Mutex
	res = pthread_mutex_init(&work_mutex_1, NULL);
	if (res != 0)
	{
		perror("Iniciação do Mutex falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_mutex_init(&work_mutex_2, NULL);
	if (res != 0)
	{
		perror("Iniciação do Mutex falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_mutex_init(&work_mutex_3, NULL);
	if (res != 0)
	{
		perror("Iniciação do Mutex falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_mutex_init(&work_mutex_4, NULL);
	if (res != 0)
	{
		perror("Iniciação do Mutex falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_mutex_init(&work_mutex_5, NULL);
	if (res != 0)
	{
		perror("Iniciação do Mutex falhou");
		exit(EXIT_FAILURE);
	}
	// Criar thread
	res = pthread_create(&a_thread, NULL, thread_function_1, NULL);
	if (res != 0)
	{
		perror("Criação da Thread falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&b_thread, NULL, thread_function_2, NULL);
	if (res != 0)
	{
		perror("Criação da Thread falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&c_thread, NULL, thread_function_3, NULL);
	if (res != 0)
	{
		perror("Criação da Thread falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&d_thread, NULL, thread_function_4, NULL);
	if (res != 0)
	{
		perror("Criação da Thread falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_create(&e_thread, NULL, thread_function_5, NULL);
	if (res != 0)
	{
		perror("Criação da Thread falhou");
		exit(EXIT_FAILURE);
	}

	// Juntando thread main() com a_thread

	res = pthread_join(a_thread, &thread_result);
	if (res != 0)
	{
		perror("Junção da Thread falhou");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(b_thread, &thread_result);
	if (res != 0)
	{
		perror("Junção da Thread falhou");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(c_thread, &thread_result);
	if (res != 0)
	{
		perror("Junção da Thread falhou");
		exit(EXIT_FAILURE);
	}
	res = pthread_join(d_thread, &thread_result);
	if (res != 0)
	{
		perror("Junção da Thread falhou");
		exit(EXIT_FAILURE);
	}

	res = pthread_join(e_thread, &thread_result);
	if (res != 0)
	{
		perror("Junção da Thread falhou");
		exit(EXIT_FAILURE);
	}

	pthread_mutex_destroy(&work_mutex_1); // destruição do multex
	pthread_mutex_destroy(&work_mutex_2); // destruição do multex
	pthread_mutex_destroy(&work_mutex_3); // destruição do multex
	pthread_mutex_destroy(&work_mutex_4); // destruição do multex
	pthread_mutex_destroy(&work_mutex_5); // destruição do multex

	exit(EXIT_SUCCESS);
	return 0;
}

void *thread_function_1(void *arg)
{
	while (1)
	{
		L(0, 1, 2, -1);
		sleep(tempo[0]);

		pthread_mutex_lock(&work_mutex_2);
		pthread_mutex_lock(&work_mutex_1);

		L(1, 0, 2, -1);
		sleep(tempo[0]);
		L(2, 0, 1, -1);
		sleep(tempo[0]);

		pthread_mutex_unlock(&work_mutex_1);
		pthread_mutex_unlock(&work_mutex_2);
	}
	pthread_exit(0);
}

void *thread_function_2(void *arg)
{
	while (1)
	{
		L(3, 4, 5, 6);
		sleep(tempo[1]);

		pthread_mutex_lock(&work_mutex_1);
		pthread_mutex_lock(&work_mutex_4);
		pthread_mutex_lock(&work_mutex_3);

		L(4, 3, 5, 6);
		sleep(tempo[1]);
		L(5, 4, 3, 6);
		sleep(tempo[1]);
		L(6, 3, 4, 5);
		sleep(tempo[1]);

		pthread_mutex_unlock(&work_mutex_3);
		pthread_mutex_unlock(&work_mutex_4);
		pthread_mutex_unlock(&work_mutex_1);
	}
	pthread_exit(0);
}

void *thread_function_3(void *arg)
{
	while (1)
	{

		L(7, 8, 9, -1);
		sleep(tempo[2]);

		pthread_mutex_lock(&work_mutex_3);
		pthread_mutex_lock(&work_mutex_5);
		L(8, 7, 9, -1);
		sleep(tempo[2]);
		L(9, 7, 8, -1);
		sleep(tempo[2]);
		pthread_mutex_unlock(&work_mutex_5);
		pthread_mutex_unlock(&work_mutex_3);
	}
	pthread_exit(0);
}

void *thread_function_4(void *arg)
{
	while (1)
	{
		L(10, 11, 12, 13);
		sleep(tempo[3]);

		pthread_mutex_lock(&work_mutex_5);
		pthread_mutex_lock(&work_mutex_4);
		pthread_mutex_lock(&work_mutex_2);
		L(11, 10, 12, 13);
		sleep(tempo[3]);
		L(12, 11, 10, 13);
		sleep(tempo[3]);
		L(13, 11, 10, 12);
		sleep(tempo[3]);
		pthread_mutex_unlock(&work_mutex_2);
		pthread_mutex_unlock(&work_mutex_4);
		pthread_mutex_unlock(&work_mutex_5);
	}
	pthread_exit(0);
}

void *thread_function_5(void *arg)
{
	float entradas[NUM_TRAINS];

	// SOCKET
	int server_sockfd;
	size_t server_len;
	socklen_t client_len;
	struct sockaddr_in server_address;
	struct sockaddr_in client_address;

	struct ip_mreq mreq;

	if ((server_sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
	{
		printf("An error has occurred on opening socket");
		exit(1);
	}

	server_address.sin_family = AF_INET;
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);
	server_address.sin_port = htons(PORT_SCKT_SERVER);

	server_len = sizeof(server_address);

	if (bind(server_sockfd, (struct sockaddr *)&server_address, server_len) < 0)
	{
		perror("An error has occurend on binding");
		exit(1);
	}

	mreq.imr_multiaddr.s_addr = inet_addr(MULTICAST_ADDR);
	mreq.imr_interface.s_addr = htonl(INADDR_ANY);
	if (setsockopt(server_sockfd, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq)) < 0)
	{
		perror("setsockopt");
		exit(1);
	}

	while (true)
	{
		client_len = sizeof(client_address);

		client_len = sizeof(client_address);
		if (recvfrom(server_sockfd, &entradas, sizeof(entradas), 0, (struct sockaddr *)&client_address, &client_len) < 0)
		{
			perror("error on RECVFROM()");
			exit(1);
		}

		printf("Servidor recebe: (Trem 1, Trem 2, Trem 3, Trem 4) = (%f, %f, %f, %f)\n", entradas[0], entradas[1], entradas[2], entradas[3]);

		for (int i = 0; i < NUM_TRAINS; i++)
		{
			tempo[i] = (int) entradas[i];
		}

		sleep(1);
	}
}
