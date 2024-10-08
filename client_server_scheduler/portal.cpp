#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <mutex>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "includes/EnderecoHandler.h"

#define MAX_BUF_SIZE 50000
#define IP_SERVIDOR_1 (char*)"172.26.4.220"
#define IP_SERVIDOR_2 (char*)"172.26.4.220"
#define IP_SERVIDOR_3 (char*)"172.26.4.220"

using namespace std;

static string formaEscalonamento;
int i=0;
mutex m;

void *recebe_arquivos_fonte(void *);

int main(int argc, char *argv[]) {
    int socket_portal_cliente, novo_socket, addr_len, *novo_sock;
	EnderecoHandler meu_addr(INADDR_ANY, 47006), clienteAddr;
	formaEscalonamento = argv[1];
	const int liberar = 1;
	
	srand(time(NULL)); /* Usado para sortear valores aleatórios seguindo o relógio do computador */
	
	socket_portal_cliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(socket_portal_cliente, SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

	meu_addr.bindarComSocket(socket_portal_cliente);
   
    listen(socket_portal_cliente, 2);

    addr_len = sizeof(struct sockaddr_in);

	/* Aceita a conexão do cliente e cria uma thread para receber os arquivos e enviar ao servidor */
    while ((novo_socket = accept(socket_portal_cliente, (struct sockaddr *)clienteAddr.getAddrAddr(), (socklen_t *)&addr_len)) != -1){
		pthread_t sniffer_thread;
		novo_sock = (int*)malloc(1);
		*novo_sock = novo_socket;

		pthread_create(&sniffer_thread, NULL, recebe_arquivos_fonte, (void*) novo_sock);
    }

    return 0;
}

void *recebe_arquivos_fonte(void *meu_socket){
	int sock = *(int*)meu_socket;
	int tamanho_dado_lido;
	int socket_portal_servidor[3], recebidos = 0; 
	const int liberar = 1;
	char resposta[MAX_BUF_SIZE], arq_fonte[MAX_BUF_SIZE];
	/* Endereço dos 3 servidores */
	EnderecoHandler addrServidores[3] = {EnderecoHandler(IP_SERVIDOR_1, 18900), 
										EnderecoHandler(IP_SERVIDOR_2, 18900), 
										EnderecoHandler(IP_SERVIDOR_3, 18900)};

	/* Cria os sockets para cada servidor */
	for(unsigned int i = 0;i < 3; i++)
		socket_portal_servidor[i] = socket(AF_INET, SOCK_STREAM, 0);

	for(unsigned int i = 0;i < 3; i++)
		setsockopt(socket_portal_servidor[i], SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

	/* Binda cada endereço com seu respectivo socket */
	for(unsigned int i = 0;i < 3; i++)
		addrServidores[i].bindarComSocket(socket_portal_servidor[i]);

	/* Conecta e se mantém conectado os três servidores */
	for(unsigned int i = 0;i < 3; i++)
		connect(socket_portal_servidor[i], (struct sockaddr*)addrServidores[i].getAddrAddr(), sizeof(addrServidores[i].getAddr()));
	
	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, MAX_BUF_SIZE, 0)) > 0){
		if(formaEscalonamento == "rr"){
			m.lock();

			send(socket_portal_servidor[i], arq_fonte, strlen(arq_fonte), 0);
			recebidos = recv(socket_portal_servidor[i], resposta, MAX_BUF_SIZE, 0);
			resposta[recebidos] = '\0';

			send(sock, resposta, strlen(resposta), 0);

			i++;
			if(i == 3)
				i=0;
				
			m.unlock();
		}
		else if(formaEscalonamento == "altr"){
			m.lock();
			
			i = rand() % 3;
			
			send(socket_portal_servidor[i], arq_fonte, strlen(arq_fonte), 0);
			recebidos = recv(socket_portal_servidor[i], resposta, MAX_BUF_SIZE, 0);
			resposta[recebidos] = '\0';

			send(sock, resposta, strlen(resposta), 0);
			
			m.unlock();
		}
		
		memset(arq_fonte, 0, MAX_BUF_SIZE);
		memset(resposta, 0, MAX_BUF_SIZE);
	}

	pthread_exit(0);
	free(meu_socket);

	return 0;
}
