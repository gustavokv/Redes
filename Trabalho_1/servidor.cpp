#include <iostream>
#include <unistd.h>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/EnderecoHandler.h"

using namespace std;

void *compila_arquivos_fonte(void *meu_socket);
void coloca_em_arquivo(string arq_fonte, string *nome_fonte);
void le_arquivo_insere_array(string *resultado);

int main(){
    EnderecoHandler meu_addr(INADDR_ANY, 18900), portalAddr;
    int meu_socket, novo_socket, *novo_sock, addr_len;

    meu_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    meu_addr.bindarComSocket(meu_socket);

    listen(meu_socket, 3);

    addr_len = sizeof(struct sockaddr_in);

    while ((novo_socket = accept(meu_socket, (struct sockaddr *)portalAddr.getAddrAddr(), (socklen_t *)&addr_len)) != -1){
		pthread_t sniffer_thread;
		novo_sock = (int*)malloc(1);
		*novo_sock = novo_socket;

		pthread_create(&sniffer_thread, NULL, compila_arquivos_fonte, (void*) novo_sock);
    }

    return 0;
}

void coloca_em_arquivo(string arq_fonte, string *nome_fonte){
	*nome_fonte = arq_fonte.substr(0, arq_fonte.find(' '));
	ofstream arq(*nome_fonte);
	string::size_type i = arq_fonte.find(*nome_fonte);

	if (i != string::npos)
		arq_fonte.erase(i, (*nome_fonte).length());

	arq << arq_fonte;

	arq.close();
}

void le_arquivo_insere_array(string *resultado){
	FILE *fp_in = popen("./a.out", "r");
	char c;

	while(fread(&c, sizeof(char), 1, fp_in))
		*resultado += c;

	fclose(fp_in);
}

void *compila_arquivos_fonte(void *meu_socket){
    int sock = *(int*)meu_socket, enviados;
	int tamanho_dado_lido;
	char arq_fonte[5000], comandoComp[100];
	string nome_fonte, resultado;

	//receber mensagem do cliente
	while((tamanho_dado_lido = recv(sock, arq_fonte, 5000, 0)) > 0){
		string str_fonte(arq_fonte);

		coloca_em_arquivo(str_fonte, &nome_fonte);

		strcpy(comandoComp, "g++ ");
		strcat(comandoComp, nome_fonte.c_str());

		system(comandoComp); //Compila o arquivo
		
		le_arquivo_insere_array(&resultado);

		send(sock, resultado.c_str(), resultado.length(), 0);
		
		remove(nome_fonte.c_str());
		remove("./a.out");
		resultado.clear();
		memset(arq_fonte, 0, 5000);
	}

	pthread_exit(0);
	free(meu_socket);
}