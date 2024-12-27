CC=g++
SHELL=/bin/bash
CFLAGS=-g -Wall -I/usr/local/include -I/usr/local/lib -I./
LIBS=-L/usr/local/lib -lcpr -lpqxx -lpq -ldl -lssl -lcurl -lcrypto
TEST_LIBS=$(LIBS) -lgtest -lgtest_main -pthread
DEBUG_TOOL=gdb
LEAK_TOOL=valgrind
LEAK_FLAGS=--leak_check=full
CLIENT_OUT=searchman_client
SERVICE_OUT=searchman_service
TEST_OUT=rungt
INSTALL_SCRIPT=scripts/install.sh
UNINSTALL_SCRIPT=scripts/uninstall.sh

$(source .env)

# Build targets
all: client service test

client: searchman_client.o
	$(CC) $(CFLAGS) searchman_client.o $(LIBS) -o $(CLIENT_OUT)

service: searchman_service.o
	$(CC) $(CFLAGS) searchman_service.o $(LIBS) -o $(SERVICE_OUT)

test: test.o
	$(CC) $(CFLAGS) test.o $(TEST_LIBS) -o $(TEST_OUT)

# Object files
searchman_client.o: processes/searchman_client.cpp
	$(CC) $(CFLAGS) -c processes/searchman_client.cpp

searchman_service.o: processes/searchman_service.cpp
	$(CC) $(CFLAGS) -c processes/searchman_service.cpp

test.o: test/test.cpp
	$(CC) $(CFLAGS) -c test/test.cpp

# Utils
clean:
	rm -f *.o
	rm -f *.vgcore

clean_all:	clean
	rm -f $(SERVICE_OUT)
	rm -f $(CLIENT_OUT)
	rm -f $(TEST_OUT)


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

# Utilities
install:
	$(SHELL) ./$(INSTALL_SCRIPT)

uninstall:
	$(SHELL) ./$(UNINSTALL_SCRIPT)
	