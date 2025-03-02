#ifndef DATABASE
#define DATABASE

#include <sqlite3.h>
#include <unordered_set>
#include <random>
#include <sstream>

#include "../utils/Log.h"
#include "../utils/Config.h"
#include "../indexer/Document.h"

class Database {
public:
    Database();
    Database(const std::string& db_name);
    ~Database();
    void init();
    void reset();
    void insertDocument(const Document& document);
    void insertDocuments(const std::vector<Document>& documents);
    std::string getDocument(const std::string& command);
    std::vector<std::string> getAllDocuments();
private:
    Log _log;
    Config _config;
    sqlite3 * _conn;
};

#endif