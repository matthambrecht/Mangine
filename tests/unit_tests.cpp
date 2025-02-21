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
#include "../indexer/Man.h"
#include "../indexer/Chunk.h"


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

    ASSERT_NO_THROW(config._config["entry_params"]["max_chunks"]);
    ASSERT_FALSE(config._config["entry_params"]["chunk_size"].is_null());
    ASSERT_TRUE(config._config["service"]["ports"].is_null());
}
// ========================== End Config Tests ============================


// ========================== Begin Pipeline Tests ==========================
TEST (PipelineTest, CleanseTest) { // Confirm that the pipeline cleans up the string as expected
    Pipeline pipeline;
    std::string test_string = "\ttest that. &*))██\nhello world!  ";
    std::string expected_out = "test that hello world";
    std::string empty_string = "";

    pipeline.run(test_string);

    ASSERT_EQ(test_string, expected_out);
    ASSERT_NO_THROW(pipeline.run(empty_string));
}
// ========================== End Pipeline Tests ==========================


// ========================== Begin Manpage Tests ==========================
TEST (ManpageTest, TestGetAllCommands) {
    Man man;

    ASSERT_NO_THROW(man.getAllCommands());
    ASSERT_TRUE(man.getAllCommands().size());
}

TEST (ManpageTest, TestChunking) {
    Man man;

    ASSERT_NO_THROW(man.getCommandChunks("touch", man.getCommandMan("touch")));
    ASSERT_TRUE(man.getCommandChunks("touch", man.getCommandMan("touch")).size());
}

TEST (ManpageTest, TestRetrieval) { // Confirm that the pipeline is able to get us a command vector
    Man man;

    ASSERT_NO_THROW({
        std::vector<std::string> tmp = man.getAllCommands();
        std::string tmp_manpage = man.getCommandMan(tmp[20]);
        std::vector<Chunk> tmp_chunks = man.getCommandChunks(tmp[20], tmp_manpage);
        std::cout << tmp_manpage << std::endl;
        if (!tmp_chunks.size()) {
            throw std::runtime_error("Nothing came back from command retrieval");
        }
    });
}
// ========================== End Manpage Tests ==========================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int retval = RUN_ALL_TESTS();
    return retval;
}