/* Esta biblioteca irá conter algumas funções úteis para que o código
   do cliente esteja mais limpo.*/
#include <iostream>
#include <cstring>
#include <dirent.h>

#include <netdb.h>
#include <arpa/inet.h>

using namespace std;

void hostname_para_ip(char *hostname, char *ip);
string separa_string(string& string_entrada);
void le_diretorio_funcao_L();