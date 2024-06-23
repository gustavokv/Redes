#include <iostream>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/EnderecoHandler.h"

using namespace std;

void *compila_arquivos_fonte(void *meu_socket);

int main(){
    EnderecoHandler meu_addr(INADDR_ANY, 18900), portalAddr;
    int meu_socket, novo_socket, *novo_sock, addr_len;

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    meu_addr.bindarComSocket(meu_socket);

    listen(meu_socket, 1);

    addr_len = sizeof(struct sockaddr_in);

    while ((novo_socket = accept(meu_socket, (struct sockaddr *)portalAddr.getAddrAddr(), (socklen_t *)&addr_len)) != -1){
		pthread_t sniffer_thread;
		novo_sock = (int*)malloc(1);
		*novo_sock = novo_socket;

		pthread_create(&sniffer_thread, NULL, compila_arquivos_fonte, (void*) novo_sock);
    }

    return 0;
}

void *compila_arquivos_fonte(void *meu_socket){
    int sock = *(int*)meu_socket;
	int tamanho_dado_lido;
	char arq_fonte[5000];

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, 5000, 0)) > 0){
		
		memset(arq_fonte, 0, 5000);
	}

	pthread_exit(0);
	free(meu_socket);
}