#include "EnderecoHandler.h"

EnderecoHandler::EnderecoHandler(){}

EnderecoHandler::EnderecoHandler(char *ip, int porta){
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(ip);
    addr.sin_port = htons(porta);

    setarZero();
}

EnderecoHandler::EnderecoHandler(in_addr_t tipo, int porta){
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = tipo;
    addr.sin_port = htons(porta);

    setarZero();
}

void EnderecoHandler::setarZero(){
    memset(&addr.sin_zero, 0, sizeof(addr.sin_zero));
}   

void EnderecoHandler::bindarComSocket(int socket){
    bind(socket, (sockaddr*)&addr, sizeof(sockaddr));
}

struct sockaddr_in* EnderecoHandler::getAddrAddr(){
    return &addr;
}

struct sockaddr_in EnderecoHandler::getAddr(){
    return addr;
}
