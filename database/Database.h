/* 
This database just serves to act as storage if the program
crashes, for efficiency sake we only ever want the Corpora touching
this.
*/

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
    std::optional<Document> getDocument(const std::string& command);
    std::vector<Document> getAllDocuments();
private:
    Log _log;
    Config _config;
    sqlite3 * _conn;
};

#endif