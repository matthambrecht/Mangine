#include <gtest/gtest.h>
#include <signal.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <pgvector/pqxx.hpp>
#include <unordered_set>

#include "utils/Log.cpp"
#include "utils/Config.cpp"
#include "indexer/Man.cpp"
#include "service/Request.cpp"
#include "database/Database.cpp"
#include "indexer/Indexer.cpp"

// Log Tests
TEST (LogTest, All) { // Check if logging messages workes properly
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
TEST (RequestTest, ConstructorTest) { // Ensure We Can Make Request Objects
    Log log;
    Config config;

    ASSERT_NO_THROW(Request(log, config));
}

TEST (RequestTest, NormalCase) { // Ensure valid vector is returned
    Log log;
    Config config;
    Request endpoint = Request(log, config);
    int vector_len = config._config["endpoint"]["vector_len"].get<int>();

    ASSERT_EQ(
        endpoint.getEmbedding("Test").dimensions(),
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

TEST (RequestTest, BatchEmbedTest) { // Test batch embeddings
    Log log;
    Config config; 
    Request endpoint = Request(log, config);
    int vector_len = config._config["endpoint"]["vector_len"].get<int>();
    std::vector<std::string> query_batch = {
        "test 1",
        "test 2",
        "test 3"
    };

    std::vector<pgvector::Vector> embedding_batch = endpoint.getEmbeddingBatch(query_batch);

    ASSERT_EQ(query_batch.size(), embedding_batch.size());

    for (std::vector<pgvector::Vector>::iterator it = embedding_batch.begin(); it != embedding_batch.end(); ++it) {
        ASSERT_EQ((int)(*it).dimensions(), vector_len);
    }
}

// Manpage Tests
TEST (ManTest, ConstructorTest) { // Ensure we can make Manpage objects
    Log log;
    Config config;
    
    ASSERT_NO_THROW(Man(log, config));
}


TEST (ManTest, GetAllTest) { // Ensure we can get all of the system commands
    Log log;
    Config config;
    Man man(log, config);

    std::vector<std::string> command_list = man.getAllCommands();
    ASSERT_GT(command_list.size(), 1);
}


TEST (ManTest, GetManTest) { // Ensure we can get all of the manpage contents
    Log log;
    Config config;
    Man man(log, config);

    std::vector<std::string> command_list = man.getAllCommands();

    for (std::vector<std::string>::iterator it = command_list.begin(); it != command_list.begin() + 10; ++it) {
        ASSERT_GT(man.getCommandMan(*it).size(), 0);
    }
}

// Chunking Tests
TEST (ChunkTest, GetChunkTest) { // Check if chunking is working properly
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
TEST (DatabaseTest, ConnectDatabaseTest) { // Make sure we can connect to the database
    Log log;
    Config config;
    
    ASSERT_NO_THROW(Database(log, config));
}

TEST (DatabaseTest, InitResetTest) { // Check to see if we can make and remove tables
    Log log;
    Config config;
    Database database = Database(log, config);

    ASSERT_NO_THROW(database.init());
    ASSERT_NO_THROW(database.reset());
}

TEST (DatabaseTest, InsertCommandsTest) { // Make sure that we can insert and retrieve commands
    Log log;
    Config config;
    Database database = Database(log, config);
    std::vector<std::string> test_commands = {
        "test1",
        "test2",
        "test3"
    };
    std::unordered_set<std::string> test_output;

    database.init();

    // Insert test commands into database
    for (std::vector<std::string>::iterator it = test_commands.begin(); it != test_commands.end(); ++it) {
        ASSERT_NO_THROW(database.insertCommand(*it));
    }

    test_output = database.getAllCommands();
    ASSERT_EQ(test_output.size(), test_commands.size());

    // Check if all commands are in set
    for (std::vector<std::string>::iterator it = test_commands.begin(); it != test_commands.end(); ++it) {
        ASSERT_EQ(*test_output.find(*it), *it);
    }

    database.reset();
}

TEST (DatabaseTest, InsertChunksTest) {
    Log log;
    Config config;
    Request endpoint = Request(log, config);

    Database database = Database(log, config);
    pgvector::Vector test_embedding = endpoint.getEmbedding("Hello world");
    std::vector<Chunk> test_commands = {
        Chunk("test1", "Hello world", test_embedding),
        Chunk("test2", "Hello world", test_embedding),
        Chunk("test3", "Hello world", test_embedding)
    };

    database.init();

    // Insert test commands into database
    for (std::vector<Chunk>::iterator it = test_commands.begin(); it != test_commands.end(); ++it) {
        ASSERT_NO_THROW(database.insertChunk(*it));
    }

    database.reset();
}


// Indexer Tests
TEST (IndexerTest, ConstructorTest) { // Ensure that indexer object can be created
    Log log = Log();
    Config config = Config();
    Database * database = new Database(log, config);
    Request * embedder = new Request(log, config);

    database->init();

    ASSERT_NO_THROW(Indexer(log, config, database, embedder));

    database->reset();

    delete database;
    delete embedder;
}

TEST (IndexerTest, IndexAllTest) { // Attempt to index all commands
    Log log = Log();
    Config config = Config();
    Database * database = new Database(log, config);
    Request * embedder = new Request(log, config);

    database->init();

    Indexer * indexer = new Indexer(log, config, database, embedder);
    ASSERT_NO_THROW(indexer->indexAll()); // Normal case
    ASSERT_NO_THROW(indexer->indexAll()); // Edge case (should get 0)

    database->reset();

    delete database;
    delete embedder;
    delete indexer;
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