#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <fstream>
#include <vector>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/clienteUteis.h"
#include "includes/EnderecoHandler.h"

#define MAX_BUF_SIZE 50000

using namespace std;

int main(int argc, char *argv[]){
    if(argc != 3)
        return -1;

    int meu_socket_cliente = socket(AF_INET, SOCK_STREAM, 0);
    const int liberar = 1;

    /* Este comando faz com que não ocorra bug ao se reconectar aos programas */
    setsockopt(meu_socket_cliente, SOL_SOCKET, SO_REUSEADDR, &liberar, sizeof(int));

    char *id_portal = argv[1], ip_portal[100]; /* IP/nome máquina */
    int porta_portal = atoi(argv[2]);

    /* Caso seja o nome da máquina do portal digitado, ele converte para o IP. */
    if(!isdigit(id_portal[0]))
        hostname_para_ip(id_portal, ip_portal);
    else
        strcpy(ip_portal, id_portal);

    EnderecoHandler addr_portal(ip_portal, porta_portal);

    addr_portal.bindarComSocket(meu_socket_cliente);

    connect(meu_socket_cliente, (struct sockaddr*)addr_portal.getAddrAddr(), sizeof(addr_portal.getAddr()));

    string cmd;
    char resposta[1000];
    int recebidos;

    do{
        getline(cin, cmd); /* Recebe no cliente os comando S ou L */

        cmd = separa_string(cmd);
        
        if(cmd.substr(0, cmd.find(' ')) == "S"){ /* Comando para enviar os códigos fonte */
            DIR* dirp = opendir("./arquivos_fonte/"); /* arquivos_fonte é o diretório que está os arquivos fonte */
            struct dirent *dp;

            istringstream tokenizador {cmd}; /* Separa o comando em tokens */
            string token;

            char dir[100], char_dir;
            ifstream arq;
            string arq_fonte;

            /* Separa os arquivos para serem enviados em tokens, abre eles, lê o conteúdo e envia ao portal */
            while(tokenizador >> token){
                if(token != "S"){
                    while ((dp = readdir(dirp)) != NULL) {
                        if(dp->d_name == token){
                            strcpy(dir, "./arquivos_fonte/");
                            strcat(dir, dp->d_name);

                            arq.open(dir);

                            arq_fonte += dp->d_name; /* Coloca no arquivo a ser enviado o nome dele */
                            arq_fonte += " ";
                            while(arq.get(char_dir))
                                arq_fonte += char_dir; /* Para caracter a caracter o conteúdo do arquivo fonte */  
                            
                            send(meu_socket_cliente, arq_fonte.c_str(), arq_fonte.length(), 0);

                            recebidos = recv(meu_socket_cliente, resposta, MAX_BUF_SIZE, 0);
                            resposta[recebidos] = '\0';

                            cout << resposta << endl;
                            
                            arq_fonte.clear();
                            arq.close();
                        }            
                    }
                    closedir(dirp);
                    dirp = opendir("./arquivos_fonte/");
                }
            }

            closedir(dirp);
        }
        else if(cmd.substr(0, cmd.find(' ')) == "L") /* Lista os códigos fonte para serem enviados */
            le_diretorio_funcao_L();

    }while(1);
    
    close(meu_socket_cliente);

    return 0;
}