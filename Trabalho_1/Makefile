all:	servidor portal cliente

servidor:	servidor.o includes/EnderecoHandler.o
	g++ servidor.o includes/EnderecoHandler.o -o servidor

portal:	portal.o includes/EnderecoHandler.o
	g++ portal.o includes/EnderecoHandler.o -o portal

cliente:	cliente.o includes/EnderecoHandler.o includes/clienteUteis.o
	g++ cliente.o includes/EnderecoHandler.o includes/clienteUteis.o -o cliente
	
servidor.o:	servidor.cpp
	g++ -c servidor.cpp

portal.o:	portal.cpp
	g++ -c portal.cpp

cliente.o:	cliente.cpp
	g++ -c cliente.cpp

EnderecoHandler.o:	includes/EnderecoHandler.cpp
	g++ -c includes/EnderecoHandler.cpp

clienteUteis.o:	includes/clienteUteis.cpp
	g++ -c includes/clienteUteis.cpp
	
clean:
	rm *.o includes/*.o servidor portal cliente
