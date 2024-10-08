#include <iostream>
#include <unistd.h>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/EnderecoHandler.h"

#define MAX_BUF_SIZE 50000
#define MAX_COMMAND_SIZE 200

using namespace std;

char arq_fonte[MAX_BUF_SIZE], comandoComp[MAX_COMMAND_SIZE];
string nome_fonte, resultado;
ifstream erro_arq; /* Arquivo para captar erros e warnings da compilação */

void *compila_arquivos_fonte(void *meu_socket);
void coloca_em_arquivo(string arq_fonte, string *nome_fonte);
void le_resultado_insere_array(unsigned int tipo_arq, string *resultado);

int main(){
    EnderecoHandler meu_addr(INADDR_ANY, 18900), portalAddr;
    int meu_socket_servidor, novo_socket, *novo_sock, addr_len;
	const int liberar = 1;

    meu_socket_servidor = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	setsockopt(meu_socket_servidor, SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

    meu_addr.bindarComSocket(meu_socket_servidor);

    listen(meu_socket_servidor, 3);

    addr_len = sizeof(struct sockaddr_in);

	/* Aceita as conexões do portal e cria uma thread para receber, compilar e reenviar o resultado */
    while ((novo_socket = accept(meu_socket_servidor, (struct sockaddr *)portalAddr.getAddrAddr(), (socklen_t *)&addr_len)) != -1){
		pthread_t sniffer_thread;
		novo_sock = (int*)malloc(1);
		*novo_sock = novo_socket;

		pthread_create(&sniffer_thread, NULL, compila_arquivos_fonte, (void*) novo_sock);
    }

	close(meu_socket_servidor);

    return 0;
}

/* Aqui o arquivo recebido é lido e colocado em um arquivo para que ocorra a compilação */
void coloca_em_arquivo(string arq_fonte, string *nome_fonte){
	*nome_fonte = arq_fonte.substr(0, arq_fonte.find(' '));
	ofstream arq(*nome_fonte); /* O nome do arquivo é o primeiro token do arquivo */
	string::size_type i = arq_fonte.find(*nome_fonte);

	if (i != string::npos) /* Tira o nome do arquivo do arquivo */
		arq_fonte.erase(i, (*nome_fonte).length());

	arq << arq_fonte;

	arq.close();
}

/* Abre o arquivo resultante ./a.out e coloca na string resultado */
void le_resultado_insere_array(unsigned int tipo_arq, string *resultado){
	FILE *fp_in;
	char c;

	if(tipo_arq == 1)
		fp_in = popen("./a.out", "r");
	else
		fp_in = fopen("erro.txt", "r");

	while(fread(&c, sizeof(char), 1, fp_in))
		*resultado += c;

	fclose(fp_in);
}

void *compila_arquivos_fonte(void *meu_socket){
    int sock = *(int*)meu_socket, enviados;
	int tamanho_dado_lido, ini_arq_erro, fim_arq_erro;

	/* Recebe arquivo do portal */
	while((tamanho_dado_lido = recv(sock, arq_fonte, MAX_BUF_SIZE, 0)) > 0){
		string str_fonte(arq_fonte);
		
		coloca_em_arquivo(str_fonte, &nome_fonte);

		strcpy(comandoComp, "g++ ");
		strcat(comandoComp, nome_fonte.c_str());
		strcat(comandoComp, " 2>> erro.txt");

		system(comandoComp); //Compila o arquivo

		erro_arq.open("erro.txt");
		ini_arq_erro = erro_arq.tellg();
		erro_arq.seekg(0, ios::end);
		fim_arq_erro = erro_arq.tellg();

		if(fim_arq_erro - ini_arq_erro == 0){
			le_resultado_insere_array(1, &resultado);
			remove("./a.out");
		}
		else
			le_resultado_insere_array(0, &resultado);

		send(sock, resultado.c_str(), resultado.length(), 0);

		erro_arq.close();
		remove("erro.txt");		
		remove(nome_fonte.c_str());
		resultado.clear();
		memset(arq_fonte, 0, MAX_BUF_SIZE);
	}

	pthread_exit(0);
	free(meu_socket);
}
