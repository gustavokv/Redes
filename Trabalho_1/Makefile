# Makefile for compiling cliente.cpp, servidor.cpp, and portal.cpp

# Compiler
CXX = g++
# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Source files
CLIENTE_SRC = cliente.cpp includes/clienteUteis.cpp includes/EnderecoHandler.cpp
SERVIDOR_SRC = servidor.cpp includes/EnderecoHandler.cpp
PORTAL_SRC = portal.cpp includes/EnderecoHandler.cpp

# Object files
CLIENTE_OBJ = $(CLIENTE_SRC:.cpp=.o)
SERVIDOR_OBJ = $(SERVIDOR_SRC:.cpp=.o)
PORTAL_OBJ = $(PORTAL_SRC:.cpp=.o)

# Targets
all: cliente servidor portal

cliente: $(CLIENTE_OBJ)
	$(CXX) $(CXXFLAGS) -o cliente $(CLIENTE_OBJ)

servidor: $(SERVIDOR_OBJ)
	$(CXX) $(CXXFLAGS) -o servidor $(SERVIDOR_OBJ)

portal: $(PORTAL_OBJ)
	$(CXX) $(CXXFLAGS) -o portal $(PORTAL_OBJ)

# Compile source files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
	rm -f cliente servidor portal $(CLIENTE_OBJ) $(SERVIDOR_OBJ) $(PORTAL_OBJ)

