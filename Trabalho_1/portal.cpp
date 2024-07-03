#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>

#include "includes/EnderecoHandler.h"

using namespace std;

static string formaEscalonamento;

void *recebe_arquivos_fonte(void *);

int main(int argc, char *argv[]) {
    int socket_portal_cliente, novo_socket, addr_len, *novo_sock;
	EnderecoHandler meu_addr(INADDR_ANY, 47006), clienteAddr;
	formaEscalonamento = argv[1];
	const int liberar = 1;
	
	socket_portal_cliente = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(socket_portal_cliente, SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

	meu_addr.bindarComSocket(socket_portal_cliente);
   
    listen(socket_portal_cliente, 2);

    addr_len = sizeof(struct sockaddr_in);

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
	char arq_fonte[5000], resposta[1000];
	int socket_portal_servidor[3], recebidos[3] = {0, 0, 0}; 
	const int liberar = 1;
	// EnderecoHandler addrServidores((char*)"172.27.1.209", 18900);
	EnderecoHandler addrServidores[3] = {EnderecoHandler((char*)"172.27.1.209", 18900), 
										EnderecoHandler((char*)"172.27.1.209", 18901), 
										EnderecoHandler((char*)"172.27.1.209", 18902)};

	for(unsigned int i = 0;i < 3; i++)
		socket_portal_servidor[i] = socket(AF_INET, SOCK_STREAM, 0);

	for(unsigned int i = 0;i < 3; i++)
		setsockopt(socket_portal_servidor[i], SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

	for(unsigned int i = 0;i < 3; i++)
		addrServidores[i].bindarComSocket(socket_portal_servidor[i]);

	for(unsigned int i = 0;i < 3; i++){
		if(connect(socket_portal_servidor[i], (struct sockaddr*)addrServidores[i].getAddrAddr(), sizeof(addrServidores[i].getAddr())) == -1){
			cout << "Erro em se conectar ao servidor." << endl;
			return 0;
		}
		cout << "Conectado" << endl;
	}

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, 5000, 0)) > 0){
		if(formaEscalonamento == "rr"){
			if(recebidos[0] == 0){
				send(socket_portal_servidor[0], arq_fonte, strlen(arq_fonte), 0);
				resposta[recebidos[0]] = '\0';
			}
			else if(recebidos[1] == 0){
				send(socket_portal_servidor[1], arq_fonte, strlen(arq_fonte), 0);
				resposta[recebidos[1]] = '\0';
			}
			else if(recebidos[2] == 0){
				send(socket_portal_servidor[2], arq_fonte, strlen(arq_fonte), 0);
				resposta[recebidos[2]] = '\0';
			}

			if(recebidos[0] = recv(socket_portal_servidor[0], resposta, 1000, 0))
				recebidos[0] = 0;
			else if(recebidos[1] = recv(socket_portal_servidor[1], resposta, 1000, 0))
				recebidos[1] = 0;
			else if(recebidos[2] = recv(socket_portal_servidor[2], resposta, 1000, 0))
				recebidos[2] = 0;
		}
		else if(formaEscalonamento == "altr"){

		}

		send(sock, resposta, 1000, 0);
		
		memset(arq_fonte, 0, 5000);
	}

	pthread_exit(0);
	free(meu_socket);

	return 0;
}