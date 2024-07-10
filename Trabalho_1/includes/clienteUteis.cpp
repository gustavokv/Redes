#include "clienteUteis.h"

/* Recebe o hostname do portal para converter para IP */
void hostname_para_ip(char *hostname, char *ip){
    struct hostent *he;
    struct in_addr **lista_addr;

    he = gethostbyname(hostname);

    lista_addr = (struct in_addr**)he->h_addr_list;

    strcpy(ip, inet_ntoa(*lista_addr[0]));
}

/* Separa a string do comando */
string separa_string(string& string_entrada){  
    string delimitadores = "[,]";
    string resultado; /* Armazena o resultado após a separação */
    int ini_pos = 0; 
    int fim_pos = 0; 

    /* Executa o laço enquanto fim_pos não é igual a string::npos */
    while ((fim_pos = string_entrada.find_first_of(delimitadores, ini_pos)) != string::npos) { 
        if (fim_pos != ini_pos) { /* Verificando se a substring não é vazia */
            resultado += string_entrada.substr(ini_pos, fim_pos - ini_pos); 
        } 
  
        ini_pos = fim_pos + 1; /* Atualiza o ini_pos para a posição após o delimitador */
    } 
  
    /* Extrai a substring de ini_pos para o final da string e concatena no resultado */
    if (ini_pos != string_entrada.length()) { 
        resultado += string_entrada.substr(ini_pos); 
    } 

    return resultado; 
} 

/* Mostra todos os arquivos disponíveis para enviar ao portal */
void le_diretorio_funcao_L(){
    DIR* dirp = opendir("./arquivos_fonte/");
    unsigned int quantDir=0;

    struct dirent *dp;

    while ((dp = readdir(dirp)) != NULL) {
        if(strcmp(dp->d_name, ".") != 0 && strcmp(dp->d_name, "..") != 0)
            cout << "[" << dp->d_name << "]" << endl;

        quantDir++;
    }

    if(quantDir == 0)
        cout << "L 0" << endl;

    closedir(dirp);
}