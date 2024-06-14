#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>
#include <dirent.h>
#include <sstream>
#include <fstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/clienteUteis.h"

using namespace std;

void le_diretorio_funcao_L();

int main(int argc, char *argv[]){
    if(argc != 3)
        return -1;

    int meu_socket = socket(AF_INET, SOCK_STREAM, 0);

    char *id_portal = argv[1], ip_portal[100]; /* IP/nome máquina */
    int porta_portal = atoi(argv[2]);

    /* Caso seja o nome da máquina do portal digitado, ele converte para o IP. */
    if(!isdigit(id_portal[0]))
        hostname_para_ip(id_portal, ip_portal);
    else
        strcpy(ip_portal, id_portal);

    struct sockaddr_in addr_portal;

    addr_portal.sin_family = AF_INET;
    addr_portal.sin_port = htons(porta_portal);
    addr_portal.sin_addr.s_addr = inet_addr(ip_portal);

    memset(&addr_portal.sin_zero, 0, sizeof(addr_portal.sin_zero));

    bind(meu_socket, (sockaddr*)&addr_portal, sizeof(sockaddr));

    if(connect(meu_socket, (struct sockaddr*)&addr_portal, sizeof(addr_portal)) == -1){
        cout << "Erro em se conectar ao servidor." << endl;
        return 1;
    }

    cout << "Conectado." << endl;

    string cmd, resposta;

    do{
        getline(cin, cmd);
        
        if(cmd.substr(0, cmd.find(' ')) == "S"){ /* Comando para enviar os códigos fonte */
            DIR* dirp = opendir("./arquivos_fonte/");
            struct dirent *dp;

            istringstream tokenizer {cmd};
            string token;

            char dir[100], cDir;
            ifstream arq;
            string arqFonte;

            /* Separa os arquivos para serem enviados em tokens, abre eles, lê o conteúdo e envia ao portal */
            while(tokenizer >> token){
                if(token != "S"){
                    while ((dp = readdir(dirp)) != NULL) {
                        if(dp->d_name == token){
                            strcpy(dir, "./arquivos_fonte/");
                            strcat(dir, dp->d_name);

                            arq.open(dir);

                            while(arq.get(cDir))
                                arqFonte += cDir;                              

                            send(meu_socket, arqFonte.c_str(), arqFonte.length(), 0);

                            arqFonte.clear();
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

        recv(meu_socket, &(resposta[0]), resposta.length(), 0);
        resposta += '\0';

        cout << resposta << endl;
    }while(1);

    close(meu_socket);

    return 0;
}

/* Mostra todos os arquivos disponíveis para enviar ao portal */
void le_diretorio_funcao_L(){
    DIR* dirp = opendir("./arquivos_fonte/");

    struct dirent *dp;

    while ((dp = readdir(dirp)) != NULL) {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            cout << dp->d_name << endl;
    }

    closedir(dirp);
}
