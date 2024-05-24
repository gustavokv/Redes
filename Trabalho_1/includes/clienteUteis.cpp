#include "clienteUteis.h"

#include <netdb.h>
#include <arpa/inet.h>

#include <cstring>

/* Recebe o hostname do portal para converter para IP */
void hostname_para_ip(char *hostname, char *ip){
    struct hostent *he;
    struct in_addr **lista_addr;

    he = gethostbyname(hostname);

    lista_addr = (struct in_addr**)he->h_addr_list;

    strcpy(ip, inet_ntoa(*lista_addr[0]));
}