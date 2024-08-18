#pragma once

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <cstring>

class EnderecoHandler{
    struct sockaddr_in addr;

    void setarZero();
public:
    EnderecoHandler();
    EnderecoHandler(char* ip, int porta);
    EnderecoHandler(in_addr_t tipo, int porta);
    void bindarComSocket(int socket);
    struct sockaddr_in* getAddrAddr();
    struct sockaddr_in getAddr();

};