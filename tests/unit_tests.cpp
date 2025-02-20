#include <gtest/gtest.h>
#include <signal.h>
#include <sys/types.h>
#include <iostream>
#include <string>
#include <vector>
#include <unistd.h>
#include <fcntl.h>
#include <unordered_set>

#include "../utils/Log.h"
#include "../utils/Config.h"
#include "../pipeline/Pipeline.h"


// ========================== Begin Log Tests =============================
TEST (LogTest, All) { // Check if logging messages workes properly
    Log log;

    ASSERT_NO_THROW(log.error("Test", "Error"));
    ASSERT_NO_THROW(log.warning("Test", "Warning"));
    ASSERT_NO_THROW(log.normal("Test", "Normal"));
}
// ========================== End Log Tests ===============================


// ========================== Begin Config Tests ==========================
TEST (ConfigTest, NormalCase) { // Confirm config works properly
    Config config;

    ASSERT_NO_THROW(config._config["service"]["command_buffer_size"]);
    ASSERT_NO_THROW(config._config["endpoint"]["host"]);
    ASSERT_NO_THROW(config._config["endpoint"]["port"]);
    ASSERT_NO_THROW(config._config["endpoint"]["vector_len"]);
    ASSERT_TRUE(config._config["service"]["ports"].is_null());
}
// ========================== End Config Tests ============================


// ========================== Begin Pipeline Tests ==========================
TEST (PipelineTest, CleanseTest) { // Confirm that the pipeline cleans up the string as expected
    Pipeline pipeline;
    std::string test_string = "\ttest that. &*))██\nhello world!  ";
    std::string expected_out = "test that hello world";

    pipeline.run(test_string);

    ASSERT_EQ(test_string, expected_out);
}
// ========================== End Pipeline Tests ==========================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int retval = RUN_ALL_TESTS();
    return retval;
}