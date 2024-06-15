#include <iostream>
#include <string.h>
#include <string>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void *recebe_arquivos_fonte(void *);

int main(int argc, char *argv[]) {
    int meu_socket, novo_socket, addr_len, *novo_sock;
    struct sockaddr_in meu_addr, clienteAddr;

	string formaEscalonamento = argv[1];

    memset((void *)&meu_addr, 0, sizeof(struct sockaddr_in));

    meu_addr.sin_family = AF_INET;
    meu_addr.sin_addr.s_addr = INADDR_ANY;
    meu_addr.sin_port = htons(47006);

	meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
   
	bind(meu_socket, (const struct sockaddr *) &meu_addr, sizeof(meu_addr));
    listen(meu_socket, 1);

	cout << "Aguardando por chegada de conexoes...." << endl;

    addr_len = sizeof(struct sockaddr_in);
    while ((novo_socket = accept(meu_socket, (struct sockaddr *)&clienteAddr, (socklen_t *)&addr_len)) != -1){
        cout << "Conexao aceita!" << endl;

		pthread_t sniffer_thread;
		novo_sock = (int*)malloc(1);
		*novo_sock = novo_socket;

		pthread_create(&sniffer_thread, NULL, recebe_arquivos_fonte, (void*) novo_sock);

		cout << "Oi\n";
    }

    return 0;
}

void *recebe_arquivos_fonte(void *meu_socket){
	int sock = *(int*)meu_socket;
	int tamanho_dado_lido;
	char arq_fonte[5000];

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, 5000, 0)) > 0){


		cout << arq_fonte << endl;
		memset(arq_fonte, 0, 5000);
	}

	free(meu_socket);

	return 0;
}