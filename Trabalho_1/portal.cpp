#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>

#include <sys/socket.h>
#include <arpa/inet.h>

using namespace std;

void *manipulador_conexao(void *);

int main(int argc, char *argv[]) {
    int meu_socket, novo_socket, addr_len, *novo_sock;
    struct sockaddr_in meu_addr, clienteAddr;
    char *resposta;

    memset((void *)&meu_addr, 0, sizeof(struct sockaddr_in));
    meu_addr.sin_family = AF_INET;
    meu_addr.sin_addr.s_addr = INADDR_ANY;
    meu_addr.sin_port = htons(47006);

    if ((meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) == -1) {
        perror("[Portal] Falha ao criar o socket");
        return 1;
    }
    if (bind(meu_socket, (const struct sockaddr *) &meu_addr, sizeof(meu_addr)) == -1){
        perror("[Portal] Falha ao executar o bind");
        close(meu_socket);
        return 1;
    }
    if (listen(meu_socket, 1) == -1){
        perror("[Portal] Falha no listening");
        close(meu_socket);
        return 1;
    }

	cout << "Aguardando por chegada de conexoes...." << endl;

    addr_len = sizeof(struct sockaddr_in);
    while ((novo_socket = accept(meu_socket, (struct sockaddr *)&clienteAddr, (socklen_t *)&addr_len)) != -1){
        cout << "Conexao aceita!" << endl;

        //Respondendo ao cliente
		resposta = "Ae mano, Acabei de receber a parada da conexao. E agora e nois na fita, ficaremos conectados\n";
		write(novo_socket, resposta, strlen(resposta));

		pthread_t sniffer_thread;
		novo_sock = malloc(1);
		*novo_sock = novo_socket;

		if( pthread_create( &sniffer_thread, NULL, manipulador_conexao, (void*) novo_sock) < 0){
			perror("Nao foi possivel criar a thread!!");
			return 1;
		}

		//Nessa parte a thread é colocada no conjunto de conexoes ativas.
		//A conexao não é desfeita antes da thread.
		//pthread_join(sniffer_thread, NULL)
	    cout << "Identificador de conexao atribuido!" << endl;
    }

    if (novo_socket == -1)
        perror("[Portal] Falha na conexao do novo socket");

    return 0;
}

void *manipulador_conexao(void *meu_socket){
	//Obter o descritor (identificador) do socket
	int sock = *(int*)meu_socket;
	int tamanho_dado_lido;

	char *mensagem, mensagem_cliente[2000];

	//enviar mensagem para o cliente
	mensagem = "Ai brou, e da hora esse nosso manipulador de conexoes\n";
	write(sock, mensagem, strlen(mensagem));

	mensagem = "Como sou um servidor, vamos nos comunicar. Digita ai mano!\n";
	write(sock, mensagem, strlen(mensagem));

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, mensagem_cliente, 2000, 0)) > 0)
		//envia a mesma mensagem de volta para o cliente.
		write(sock, mensagem_cliente, strlen(mensagem_cliente));

	if(tamanho_dado_lido == 0){
	    cout << "Cliente Desconectado!" << endl;
		fflush(stdout);
	}
	else if(tamanho_dado_lido == -1)
		perror("Funcao receive falhou!");

	//liberar o ponteiro, memória
	free(meu_socket);

	return 0;
}