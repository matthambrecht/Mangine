#include <gtest/gtest.h>
#include <signal.h>
#include <sys/types.h>
#include <iostream>
#include <string>

#include "utils/Config.cpp"
#include "service/Request.cpp"

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
    Request endpoint = Request();
    Config config;
    int vector_len = config._config["endpoint"]["vector_len"].get<int>();

    ASSERT_EQ(
        endpoint.getEmbedding("Test").size(),
        vector_len
        );
}

TEST (RequestTest, BadEndpoint) { // Improper endpoint in config
    Request endpoint = Request();
    
    ASSERT_THROW(
        endpoint.getEmbedding(""),
        std::invalid_argument
        );
}

TEST (RequestTest, BadInput) { // Empty input
    Request endpoint = Request("http://127.0.0.1:30998/embed", 96);
    
    ASSERT_THROW(
        endpoint.getEmbedding(""),
        std::invalid_argument
        );
}

int main(int argc, char **argv) {
    pid_t child = fork();

    if (child == 0) {
        execl("/bin/bash", "sh", "embedding/entrypoint.sh", nullptr);
    } else if (child > 0) {
        testing::InitGoogleTest(&argc, argv);
        int retval = RUN_ALL_TESTS();
        kill(child, SIGKILL);
        return retval;
    } else {
        std::cout << "Failed to initiate child process..." << std::endl;
        return -1;
    }
}