CC=g++
CFLAGS=-Wall
LIBS=-lspacy
DEBUG_TOOL=gdb
LEAK_TOOL=valgrind
LEAK_FLAGS=--leak_check=full
CLIENT_OUT=searchman
SERIVCE_OUT=searchman_service

# Building
all: client service

all_c: client service clean

client: client.o
	$(CC) $(CFLAGS) searchman_client.o -o $(CLIENT_OUT)

service: service.o
	$(CC) $(CFLAGS) searchman_service.o -o $(SERIVCE_OUT)


# Objects
client.o: searchman_client.cpp
	$(CC) $(CFLAGS) $(LIBS) -c searchman_client.cpp

service.o: searchman_service.cpp
	$(CC) $(CFLAGS) $(LIBS) -c searchman_service.cpp


# Utils
clean:
	rm -f *.o
	rm -f *.vgcore

clean_all:	clean
	rm -f $(SERIVCE_OUT)
	rm -f $(CLIENT_OUT)


# Debugging
client_debug:
	$(DEBUG_TOOL) ./$(CLIENT_OUT)

service_debug:
	$(DEBUG_TOOL) ./$(SERIVCE_OUT)


# Memory leak analysis
service_leak:
	$(LEAK_TOOL) $(LEAK_FLAGS) ./$(SERVICE_OUT)

client_leak:
	$(LEAK_TOOL) $(LEAK_FLAGS) ./$(CLIENT_OUT)