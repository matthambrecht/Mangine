#include "Database.h"

const std::string CLASS_NAME = "Database";

const void check_conn(Log & logger, sqlite3 * connection) {
    if (!connection) {
        std::string error_msg = "Database not connected";
        logger.error(error_msg);
        throw std::runtime_error(error_msg);
    }
}


const void check_query(Log & logger, int ret_val, char * sql_error_msg) {
    if (ret_val != SQLITE_OK) {
        std::string error_msg = "Error running db query: " + std::string(sql_error_msg);
        logger.error(error_msg);
        throw std::runtime_error(error_msg);
    }
}


const std::string uuid_gen() {
    /* This is a slightly less random version of a UUIDv4 gen 
    but because collisions don't matter so long as the commands
    are different we don't care. If they do collide, please buy a
    lottery ticket.
    */
    std::random_device rd;
    std::mt19937_64 gen(rd());
    std::uniform_int_distribution<> dis(0, 15);
    std::uniform_int_distribution<> dis2(8, 11);
    int sets[] = {8, 4, 3, 4, 12};
    std::string seps[] = {"-", "-4", "-", "-"};
    std::stringstream ss; ss << std::hex;
    int i, j;

    for (j = 0; j < sizeof(sets) / sizeof(sets[0]); j++) {
        for (i = 0; i < sets[j]; i++) {
            ss << dis(gen);
        }
        
        if (j < sizeof(seps) / sizeof(seps[0])) {
            ss << seps[j];
        }
    }

    return ss.str();
}


Database::Database() : _log(Log("Database")), _config(Config()) {
    const std::string db_name = _config._config["database"]["name"].get<std::string>();
    int ret_val = sqlite3_open(db_name.c_str(), &_conn);

    // Attempt to connect to database
    if (ret_val != SQLITE_OK) {
        std::string error_msg = "Error connecting to db: " + std::string(sqlite3_errmsg(_conn));
        _log.error(error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal("Conected to database '" + std::string(db_name) + "'");
}


Database::~Database() {
    if (_conn) {
        sqlite3_close(_conn);
    }
}


void Database::init() { // Create all tables for the index
    std::string db_query = "CREATE TABLE IF NOT EXISTS documents ("
                           "     id text PRIMARY KEY NOT NULL,"
                           "     command text NOT NULL,"
                           "     document_str text NOT NULL)";
    char * sql_error_msg = NULL;

    check_conn(_log, _conn);
    check_query(
        _log,
        sqlite3_exec(
            _conn,
            db_query.c_str(),
        NULL,
        NULL,
        &sql_error_msg),
        sql_error_msg
    );

    _log.normal("Recieved init() query");
}


void Database::reset() { // Drop all tables to reset the index
    std::string db_query = "DROP TABLE IF EXISTS documents;";
    char * sql_error_msg = NULL;
    int ret_val = sqlite3_exec(_conn, db_query.c_str(), NULL, NULL, &sql_error_msg);

    check_conn(_log, _conn);
    check_query(
        _log,
        sqlite3_exec(
            _conn,
            db_query.c_str(),
        NULL,
        NULL,
        &sql_error_msg),
        sql_error_msg
    );

    _log.normal("Recieved reset() query");
} 


void Database::insertDocument(const Document& document) {
    const std::string db_query = "INSERT INTO documents (id, command, document_str) VALUES ('"
        + uuid_gen() + "', '" + document.getVal() + "', '" + document.getCommand() + "');";
    char * sql_error_msg = NULL;
    
    check_conn(_log, _conn);
    check_query(
        _log,
        sqlite3_exec(
            _conn,
            db_query.c_str(),
        NULL,
        NULL,
        &sql_error_msg),
        sql_error_msg
    );
}


void Database::insertDocuments(const std::vector<Document>& documents) {
    std::stringstream db_query;
    char * sql_error_msg = NULL;

    if (documents.empty()) {
        _log.warning("Attempted to bulk index empty vector of documents.");
        return;
    }

    check_conn(_log, _conn);
    db_query << "INSERT INTO documents (id, command, document_str) VALUES";

    for (const Document& document : documents) {
       db_query << " ('" << uuid_gen() << "', '" << document.getVal() << "', '" << document.getCommand() << "'),";
    }

    db_query.seekp(-1, db_query.cur);
    db_query << ";";

    check_query(
        _log,
        sqlite3_exec(
            _conn,
            db_query.str().c_str(),
        NULL,
        NULL,
        &sql_error_msg),
        sql_error_msg
    );
}