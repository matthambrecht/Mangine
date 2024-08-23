CC=g++
CFLAGS=-Wall -I/usr/local/include -I/usr/local/lib
LIBS=-L/usr/local/lib -lcpr
DEBUG_TOOL=gdb
LEAK_TOOL=valgrind
LEAK_FLAGS=--leak_check=full
CLIENT_OUT=searchman_client
SERVICE_OUT=searchman_service

# Build targets
all: client service

client: searchman_client.o
	$(CC) $(CFLAGS) searchman_client.o $(LIBS) -o $(CLIENT_OUT)

service: searchman_service.o
	$(CC) $(CFLAGS) searchman_service.o $(LIBS) -o $(SERVICE_OUT)

# Object files
searchman_client.o: searchman_client.cpp
	$(CC) $(CFLAGS) -c searchman_client.cpp

searchman_service.o: searchman_service.cpp
	$(CC) $(CFLAGS) -c searchman_service.cpp

# Utils
clean:
	rm -f *.o
	rm -f *.vgcore

clean_all:	clean
	rm -f $(SERVICE_OUT)
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