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

void *recebe_arquivos_fonte(void *);

int main(int argc, char *argv[]) {
    int meu_socket, novo_socket, addr_len, *novo_sock;
	EnderecoHandler meu_addr(INADDR_ANY, 47006), clienteAddr;
	string formaEscalonamento = argv[1];

	meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	meu_addr.bindarComSocket(meu_socket);
   
    listen(meu_socket, 2);

    addr_len = sizeof(struct sockaddr_in);

    while ((novo_socket = accept(meu_socket, (struct sockaddr *)clienteAddr.getAddrAddr(), (socklen_t *)&addr_len)) != -1){
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
	char arq_fonte[5000];
	int meu_socket_server; 
	EnderecoHandler addrServidores((char*)"172.27.1.209", 18900);
	//EnderecoHandler addrServidores[3] = {EnderecoHandler(ip_maquina_1_aqui, 18900), EnderecoHandler(ip_maquina_2_aqui, 18901), EnderecoHandler(ip_maquina_3_aqui, 18902)};

	meu_socket_server = socket(AF_INET, SOCK_STREAM, 0);
	addrServidores.bindarComSocket(meu_socket_server);

	if(connect(meu_socket_server, (struct sockaddr*)addrServidores.getAddrAddr(), sizeof(addrServidores.getAddr())) == -1){
        cout << "Erro em se conectar ao servidor." << endl;
        return 0;
    }

	cout << "Conectado" << endl;

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, 5000, 0)) > 0){

		send(meu_socket_server, arq_fonte, strlen(arq_fonte), 0);
		cout << arq_fonte << endl;
		memset(arq_fonte, 0, 5000);
	}

	pthread_exit(0);
	free(meu_socket);

	return 0;
}