#include "Database.h"

const std::string CLASS_NAME = "Database";

void check_conn(Log & logger, sqlite3 * connection) {
    if (!connection) {
        std::string error_msg = "Database not connected";
        logger.error(error_msg);
        throw std::runtime_error(error_msg);
    }
}


void check_query(
    Log & logger,
    int ret_val,
    char * sql_error_msg = NULL,
    sqlite3 * db = NULL) {
    std::string error_msg;
    
    if (ret_val != SQLITE_OK) {
        if (sql_error_msg) {
            error_msg = "Error running db query: " + std::string(sql_error_msg);
        } else if (db) {
            error_msg = sqlite3_errmsg(db);
        } else if (ret_val == SQLITE_CONSTRAINT) {
            return;
        } else {
            error_msg = sqlite3_errstr(ret_val);
        }

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
    unsigned long int sets[] = {8, 4, 3, 4, 12};
    std::string seps[] = {"-", "-4", "-", "-"};
    std::stringstream ss; ss << std::hex;
    unsigned long int i, j;

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


Database::Database(const std::string& db_name) : _log(Log("Database")), _config(Config()) {
    int ret_val = sqlite3_open(db_name.c_str(), &_conn);

    // Attempt to connect to database
    if (ret_val != SQLITE_OK) {
        std::string error_msg = "Error connecting to db: " + std::string(sqlite3_errmsg(_conn));
        _log.error(error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal("Conected to database '" + db_name + "'");
}


Database::~Database() {
    if (_conn) {
        sqlite3_close(_conn);
        _log.normal("Disconnected from database.");
    }
}


void Database::init() { // Create all tables for the index
    std::string db_query = "CREATE TABLE IF NOT EXISTS documents ("
                           "     id text PRIMARY KEY NOT NULL,"
                           "     command text UNIQUE NOT NULL,"
                           "     document_str text NOT NULL);";
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
    sqlite3_exec(_conn, db_query.c_str(), NULL, NULL, &sql_error_msg);

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
        + uuid_gen() + "', '" + document.getCommand() + "', '" + document.getVal() + "');";
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
        if (!document.getVal().empty() && !document.getCommand().empty()) {
            db_query << " ('" << uuid_gen() << "', '" << document.getCommand() << "', '" << document.getVal() << "'),";
        }
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
        sql_error_msg,
        _conn
    );
}


std::optional<Document> Database::getDocument(const std::string& command) {
    sqlite3_stmt * stmt;
    std::string db_query = "SELECT command, document_str from documents WHERE command = '" + command + "';";
    int ret_val;
    std::vector<Document> documents;

    check_conn(_log, _conn);
    check_query(
        _log,
        sqlite3_prepare_v2(
            _conn,
            db_query.c_str(),
            -1,
            &stmt,
            0),
        NULL,
        _conn
    );

    if ((ret_val = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char * command_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char * command_desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        if (command_name and command_desc) {
            return
                Document(
                    std::string(command_name),
                    std::string(command_desc));
        }
    }

    return std::nullopt;
}


std::vector<Document> Database::getAllDocuments() {
    sqlite3_stmt * stmt;
    std::string db_query = "SELECT command, document_str from documents;";
    int ret_val;
    std::vector<Document> documents;

    check_conn(_log, _conn);
    check_query(
        _log,
        sqlite3_prepare_v2(
            _conn,
            db_query.c_str(),
            -1,
            &stmt,
            0),
        NULL,
        _conn
    );

    while ((ret_val = sqlite3_step(stmt)) == SQLITE_ROW) {
        const char * command_name = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 0));
        const char * command_desc = reinterpret_cast<const char *>(sqlite3_column_text(stmt, 1));

        if (command_name and command_desc) {
            documents.push_back(
                Document(
                    std::string(command_name),
                    std::string(command_desc)));
        }
    }

    return documents;
}