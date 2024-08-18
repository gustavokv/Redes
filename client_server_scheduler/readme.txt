Trabalho 1 referente à disciplina de Redes de Computadores, 2024
Prof. Dr. Rubens Barbosa Filho

Autor: Gustavo Kermaunar Volobueff
RGM: 47006
E-mail para contato: volobueffgustavo@gmail.com

Comando para compilar:
	make
	
Para executar, deve-se usar os seguintes comandos:

No cliente, segue o padrão ./cliente <IP portal> <47006>
No portal, segue o padrão ./portal rr (para escalonamento Round-Robin) e ./portal altr (para escalonamento Aleatório)
No servidor, basta ./servidor

No caso, a execução deve ocorrer na seguinte sequência: executar os 3 servidores em 3 computadores diferentes, executar o
portal em outro computador e 2 clientes, no máximo, em outros 2 computadores. Portanto, serão 6 computadores diferentes.

Deve-se alterar as constantes IP no início do código do portal para os IP's das máquinas que estão os servidores.
