#include <gtest/gtest.h>
#include <signal.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>

#include "utils/Log.cpp"
#include "utils/Config.cpp"
#include "indexer/Man.cpp"
#include "service/Request.cpp"
#include "database/Database.cpp"

// Log Tests
TEST (LogTest, All) {
    Log log;

    ASSERT_NO_THROW(log.error("Test", "Error"));
    ASSERT_NO_THROW(log.warning("Test", "Warning"));
    ASSERT_NO_THROW(log.normal("Test", "Normal"));
}

// Config Tests
TEST (ConfigTest, NormalCase) { // Confirm config works properly
    Config config;

    ASSERT_NO_THROW(config._config["service"]["command_buffer_size"]);
    ASSERT_NO_THROW(config._config["endpoint"]["host"]);
    ASSERT_NO_THROW(config._config["endpoint"]["port"]);
    ASSERT_NO_THROW(config._config["endpoint"]["vector_len"]);
    ASSERT_TRUE(config._config["service"]["ports"].is_null());
}

// Request Tests
TEST (RequestTest, NormalCase) { // Ensure valid vector is returned
    Log log;
    Config config;
    Request endpoint = Request(log, config);

    int vector_len = config._config["endpoint"]["vector_len"].get<int>();

    ASSERT_EQ(
        endpoint.getEmbedding("Test").size(),
        vector_len
        );
}

TEST (RequestTest, BadEndpoint) { // Improper endpoint in config
    Log log;
    Config config; 
    Request endpoint = Request(log, config);
    
    ASSERT_THROW(
        endpoint.getEmbedding(""),
        std::invalid_argument
        );
}

TEST (RequestTest, BadInput) { // Empty input
    Log log;
    Config config;
    Request endpoint = Request("http://127.0.0.1:30998/embed", 96, log, config);
    
    ASSERT_THROW(
        endpoint.getEmbedding(""),
        std::invalid_argument
        );
}

// Manpage Tests
TEST (ManTest, GetAllTest) {
    Log log;
    Config config;
    Man man(log, config);

    std::vector<std::string> command_list = man.getAllCommands();
    ASSERT_GT(command_list.size(), 1);
}

TEST (ManTest, GetManTest) {
    Log log;
    Config config;
    Man man(log, config);

    std::vector<std::string> command_list = man.getAllCommands();

    for (std::vector<std::string>::iterator it = command_list.begin(); it != command_list.begin() + 10; ++it) {
        ASSERT_GT(man.getCommandMan(*it).size(), 0);
    }
}

// Chunking Tests
TEST (ChunkTest, GetChunkTest) {
    Log log;
    Config config;
    Man man(log, config);

    std::string test_command = "tail";
    std::vector<Chunk> chunk_list;
    int chunk_size = config._config["embedding"]["chunk_size"].get<int>(); 
    std::string man_str = man.getCommandMan(test_command);

    chunk_list = man.getCommandChunks(test_command, man_str);
    
    ASSERT_EQ(chunk_list.size(), (int)(man_str.size() / chunk_size) + 1);
}

// Database Tests
TEST (DatabaseTest, ConnectDatabaseTest) {
    Log log;
    Config config;
    Database * database = new Database(log, config);

    delete database;
}

TEST (DatabaseTest, InitResetTest) {
    Log log;
    Config config;
    Database * database = new Database(log, config);

    ASSERT_NO_THROW(database->init());
    ASSERT_NO_THROW(database->reset());

    delete database;
}

int main(int argc, char **argv) {
    pid_t child = fork();

    if (child == 0) {
        int devNull = open("/dev/null", O_WRONLY);

        dup2(devNull, STDOUT_FILENO);
        dup2(devNull, STDERR_FILENO);
        close(devNull);

        execl("/bin/bash", "sh", "embedding/entrypoint.sh", nullptr);

        return 0;
    } else if (child > 0) {
        std::cout << "Starting embedding server...\n";
        sleep(5);
        testing::InitGoogleTest(&argc, argv);
        int retval = RUN_ALL_TESTS();
        kill(child, SIGKILL);

        return retval;
    } else {
        std::cout << "Failed to initiate child process...\n";
        return -1;
    }
}