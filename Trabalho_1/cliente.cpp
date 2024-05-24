#include <iostream>
#include <unistd.h>
#include <string>
#include <cstring>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "includes/clienteUteis.h"

using namespace std;

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

    string cmd;
    
    int recebidos, enviados;
    char msg[256];
    char resposta[256];

    do{
        getline(cin, cmd);

        if(cmd.substr(0, cmd.find(' ')) == "S"){ /* Comando para enviar os códigos fonte */

        }
        else if(cmd.substr(0, cmd.find(' ')) == "L"){ /* Lista os códigos fonte para serem enviados */

        }

        enviados = send(meu_socket, msg, strlen(msg), 0);
        
        //Após enviar a mensagem espera-se a resposta do servidor

        recebidos = recv(meu_socket, resposta, strlen(resposta), 0);
        resposta[recebidos] = '\0';

        printf("Servidor: %s\n", resposta);
    }while(1);

    close(meu_socket);

    return 0;
}
