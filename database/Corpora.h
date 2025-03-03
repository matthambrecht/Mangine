/*
This is our custom in memory storage option that allows us to
quickly discern existence of data in documents.
*/

#ifndef CORPORA
#define CORPORA

#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <utility>

#include "../utils/Config.h"
#include "../utils/Log.h"
#include "../database/Database.h"

class Corpora {
public:
    Corpora() : 
        _log(Log("Corpora")),
        _config(Config()),
        _doc_count(0),
        _total_doc_length(0),
        _database(new Database()) {};
    Corpora(const std::string& db_name) : 
        _log(Log("Corpora")),
        _config(Config()),
        _doc_count(0),
        _total_doc_length(0),
        _database(new Database(db_name)) {
            if (!_database) {
                return;
            }
        
            addDocuments(_database->getAllDocuments());
        };
    ~Corpora() {
        if (_database) {
            delete _database;
        }
    };
    bool addDocument(const Document& doc);
    void addDocuments(const std::vector<Document>& documents);
    void clear();
    unsigned int n(const std::string& keyword) const;
    unsigned int f(const std::string& keyword, const std::string& command) const;
    unsigned int D_mag(const std::string& command) const;
    unsigned int N() const {return _doc_count;}
    double avgdl() const;
private:
    Log _log;
    Config _config;
    Database * _database;
    unsigned int _total_doc_length; // for avgdl
    unsigned int _doc_count; // N

    /*
    f(qi, D)

    Hashmap (
        Key: Command Name
        Value: Hashmap(
                    Key: Vocab Word
                    Value: Count
               ),
    )
    */
    std::unordered_map<
        std::string,
            std::unordered_map<std::string, unsigned int
            >> _inner_count;

    /*
    n(qi)

    Hashmap (
        Key: Keyword
        Value: # of Documents Containing Keyword
    )
    */
   std::unordered_map<std::string, unsigned int> _overall_count;

   /*
   |D|

   Hashmap (
        Key: Command
        Value: Document Length
   )
   */
   std::unordered_map<std::string, unsigned int> _doc_sizes;
};

#endif