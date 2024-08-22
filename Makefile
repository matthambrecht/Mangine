CC=g++
CFLAGS=-Wall
DEBUG_TOOL=gdb
LEAK_TOOL=valgrind
LEAK_FLAGS=--leak_check=full
CLIENT_OUT=searchman
SERIVCE_OUT=searchman_service

# Building
all: client service

all_c: client service clean

client: client.o
	$(CC) $(CFLAGS) client.o -o $(CLIENT_OUT)

service: service.o
	$(CC) $(CFLAGS) service.o -o $(SERIVCE_OUT)


# Objects
client.o: client.cpp
	$(CC) $(CFLAGS) -o client.cpp client

service.o: service.cpp
	$(CC) $(CFLAGS) service.cpp -o service


# Utils
clean:
	rm *.o
	rm *.vgcore


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