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
#include "../pipeline/TextProcessor.h"
#include "../indexer/Man.h"
#include "../indexer/Document.h"
#include "../indexer/Indexer.h"
#include "../database/Database.h"
#include "../database/Corpora.h"
#include "../database/Search.h"

// ========================== Begin Log Tests =================================
TEST (LogTest, All) { // Check if logging messages workes properly
    Log log("Test");

    ASSERT_NO_THROW(log.error("Error"));
    ASSERT_NO_THROW(log.warning("Warning"));
    ASSERT_NO_THROW(log.normal("Normal"));
}
// ========================== End Log Tests ===================================


// ========================== Begin Config Tests ==============================
TEST (ConfigTest, NormalCase) { // Confirm config works properly
    Config config("../tests/test_config.json");

    ASSERT_NO_THROW(config._config["test"]["test1"]);
    ASSERT_EQ(config._config["test"]["test1"], 1337);
    ASSERT_TRUE(config._config["test"]["test2"].is_null());
}
// ========================== End Config Tests ================================


// ========================== Begin Pipeline Tests ============================
TEST (PipelineTest, TextProcessor) { // Confirm that the pipeline cleans up the string as expected
    std::string test_string = "\ttest that. &*))██\nhello world!  ";
    std::string expected_out = "test that hello world";
    std::string empty_string = "";

    TextProcessor::run(test_string);

    ASSERT_EQ(test_string, expected_out);
    ASSERT_NO_THROW(TextProcessor::run(empty_string));
}

TEST (PipelineTest, DocumentVectorization) {};

TEST (PipelineTest, LemmatizeTest) {}

TEST (PipelineTest, SynonymTest) {}

TEST (PipelineTest, BM25ScoreTest) {
    Corpora corpora;
    Man man;
    std::string query = "How do I download a file from a website?";
    TextProcessor::run(query);
    std::vector<std::string> commands = man.getAllCommands();
    std::vector<std::string> chopped(commands.begin(), commands.begin() + 20);
    chopped.push_back("curl");

    for (const std::string& command : chopped) {
        corpora.addDocument(command, man.getCommandMan(command).getVal());
    }

    BM25::score(query, corpora, chopped, 15);
}
// ========================== End Pipeline Tests ==============================


// ========================== Begin Manpage Tests =============================
TEST (ManpageTest, TestGetAllCommands) {
    Man man;

    ASSERT_NO_THROW(man.getAllCommands());
    ASSERT_TRUE(man.getAllCommands().size());
}

TEST (ManpageTest, TestRetrieval) { // Confirm that the pipeline is able to get us a command vector
    Man man;

    ASSERT_NO_THROW({
        std::vector<std::string> tmp = man.getAllCommands();
        Document tmp_manpage = man.getCommandMan(tmp[12]);
        
        if (tmp_manpage.getVal().empty()) {
            throw std::runtime_error("Nothing came back from command retrieval");
        }
    });
}
// ========================== End Manpage Tests ===============================


// ========================== Begin Database Tests ============================
TEST (DatabaseTest, DatabaseConnection) {
    ASSERT_NO_THROW(Database database);
}

TEST (DatabaseTest, InitDatabase) {
    // Valid DB Setup
    Database valid_database;
    ASSERT_NO_THROW(valid_database.init());
}

TEST (DatabaseTest, ResetDatabase) {
    ASSERT_NO_THROW({
        Database database;
        database.reset();
        database.init();
        database.reset();
    });
}

TEST (DatabaseTest, DocumentInsert) {
    ASSERT_NO_THROW({
        Database database;
        database.init();
        database.insertDocument(Document("touch", "Create file"));
    });
}

TEST (DatabaseTest, GetSingleDocument) {
    Database database("../tests/test.db");
    database.getDocument("touch");
    database.getDocument("notacommand");
}

TEST (DatabaseTest, GetAllDocuments) {

}

TEST (DatabaseTest, CorporaInsert) {
    Corpora corpora;
    const std::string test_str1 = "hello how are you hello";
    const std::string test_str2 = "hey hi hello";
    
    ASSERT_EQ((int)corpora.avgdl(), 0);
    ASSERT_EQ(corpora.f("hello", "test1"), 0);
    ASSERT_EQ(corpora.D_mag("test1"), 0);
    ASSERT_EQ(corpora.n("hello"), 0);

    corpora.addDocument("test1", test_str1);

    ASSERT_EQ((int)corpora.avgdl(), 5);
    ASSERT_EQ(corpora.f("hello", "test1"), 2);
    ASSERT_EQ(corpora.D_mag("test1"), 5);
    ASSERT_EQ(corpora.n("hello"), 1);
    ASSERT_EQ(corpora.n("hi"), 0);

    corpora.addDocument("test2", test_str2);

    ASSERT_EQ((int)corpora.avgdl(), 4);
    ASSERT_EQ(corpora.f("hello", "test2"), 1);
    ASSERT_EQ(corpora.D_mag("test2"), 3);
    ASSERT_EQ(corpora.n("hello"), 2);
    ASSERT_EQ(corpora.n("hi"), 1);
}

TEST (DatabaseTest, CorporaReadFromDb) {
    Database * db = new Database("../tests/test.db");



    delete db;
}
// ========================== End Database Tests ==============================


// ========================== Begin Indexer Tests ============================
TEST (IndexerTest, TestSingleCommandInsert) {
    GTEST_SKIP();
    Database * database = new Database();
    Indexer indexer(database);

    database->reset();
    database->init();

    ASSERT_TRUE(indexer.index("touch"));
    ASSERT_FALSE(indexer.index("nonexistentcommand"));

    database->reset();

    ASSERT_FALSE(indexer.index("touch"));

    delete database;
}

TEST (IndexerTest, IndexMultiple) {
    GTEST_SKIP();
    Database * database = new Database();
    Man man;
    Indexer indexer(database);
    std::vector<std::string> commands = man.getAllCommands();

    database->reset();
    database->init();

    for (int i(0); i < 20; i++) {
        indexer.index(commands[i]);
    }

    database->reset();

    delete database;
}

TEST (IndexerTest, IndexAll) {
    GTEST_SKIP();
    Database * database = new Database();
    Man man;
    Indexer indexer(database);

    database->reset();
    database->init();

    ASSERT_NO_THROW(indexer.indexAll());

    delete database;
}
// ========================== End Indexer Tests ==============================

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    int retval = RUN_ALL_TESTS();
    return retval;
}