#include "Database.h"

Database::Database() : _log(Log()), _config(Config()) {
    std::string db_vars[3] = {"db_name", "db_user", "db_pass"};

    for (int i = 0; i < (int)(sizeof(db_vars) / sizeof(std::string)); i++) { // Validate the config vars exist
        std::string tmp = _config._config["database"][db_vars[i]].get<std::string>();

        if (tmp == "null") {
            const std::string error_msg = "Error: " + db_vars[i] + " is undefined in 'config.json'";
            _log.error(CLASS_NAME, error_msg);
            throw std::runtime_error(error_msg);
        }

        db_vars[i] = tmp;
    }

    std::string conn_str = "user=" + db_vars[1] + " host=127.0.0.1 password=" + db_vars[2] + " dbname=" + db_vars[0];

    // Attempt to connect to database
    try {
        _conn = new pqxx::connection(conn_str.c_str());
    } catch (const std::exception& e) {
        std::string error_msg = "Error connecting to db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Conected to database " + db_vars[1] + "@127.0.0.1");
}

Database::Database(const Log& log, const Config& config) : _log(log), _config(config) {
    std::string db_vars[3] = {"db_name", "db_user", "db_pass"};

    for (int i = 0; i < (int)(sizeof(db_vars) / sizeof(std::string)); i++) { // Validate the config vars exist
        std::string tmp = _config._config["database"][db_vars[i]].get<std::string>();

        if (tmp == "null") {
            const std::string error_msg = "Error: " + db_vars[i] + " is undefined in 'config.json'";
            _log.error(CLASS_NAME, error_msg);
            throw std::runtime_error(error_msg);
        }

        db_vars[i] = tmp;
    }

    std::string conn_str = "user=" + db_vars[1] + " host=127.0.0.1 password=" + db_vars[2] + " dbname=" + db_vars[0];

    // Attempt to connect to database
    try {
        _conn = new pqxx::connection(conn_str.c_str());
    } catch (const std::exception& e) {
        std::string error_msg = "Error connecting to db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Conected to database " + db_vars[1] + "@127.0.0.1");
}


Database::~Database() {
    if (_conn) {
        _log.normal(CLASS_NAME, "Disconnecting from database...");
        delete _conn;
        _conn = nullptr;
    }
}

void Database::init() { // Create all tables for the index
    try {
        pqxx::work transaction(*_conn);
        int vector_size = _config._config["endpoint"]["vector_len"].get<int>();
        const std::string query = R"(
            CREATE EXTENSION IF NOT EXISTS vector;
            CREATE TABLE IF NOT EXISTS mappings (
                id bigserial PRIMARY KEY,
                command text
            );
            CREATE TABLE IF NOT EXISTS chunks (
                id bigserial PRIMARY KEY,
                command text,
                chunk_str text,
                embedding vector()" + std::to_string(vector_size) + R"()
            );
            )";

        transaction.exec0(query);
        transaction.commit();
    } catch (const std::exception& e) {
        std::string error_msg = "Error initializing db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Recieved init() query");
}


void Database::reset() { // Drop all tables to reset the index
    try {
        pqxx::work transaction(*_conn);
        const std::string query = R"(
            DROP TABLE IF EXISTS mappings;
            DROP TABLE IF EXISTS chunks;
            DROP EXTENSION IF EXISTS vector;
            )";

        transaction.exec0(query);
        transaction.commit();
    } catch (const std::exception& e) {
        std::string error_msg = "Error resetting db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Recieved reset() query");
} 


void Database::insertCommand(const std::string& command) {
    try {
        pqxx::work transaction(*_conn);
        const std::string query = R"(
            INSERT INTO mappings (command) VALUES ($1);
        )";
        transaction.exec_params(query, command);
        transaction.commit();
    } catch (const std::exception& e) {
        std::string error_msg = "Error inserting command into db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }
}

void Database::insertChunk(const Chunk& chunk) {
    try {
        pqxx::work transaction(*_conn);
        const std::string query = R"(
            INSERT INTO chunks (command, chunk_str, embedding) VALUES ($1, $2, $3);
        )";
        transaction.exec_params(query, chunk.getCommand(), chunk.getVal(), chunk.getEmbedding());
        transaction.commit();
    } catch (const std::exception& e) {
        std::string error_msg = "Error inserting chunk into db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }
}

std::unordered_set<std::string> Database::getAllCommands() {
    std::unordered_set<std::string> commands;
    pqxx::work transaction(*_conn);
    const std::string query = R"(
        SELECT command FROM mappings;
    )";

    try {
        pqxx::result result = transaction.exec(query);

        for (int idx = 0; idx < std::size(result); ++idx) {
            commands.insert(result[idx][0].as<std::string>());
        }

        _log.normal(CLASS_NAME, "Retrieved " + std::to_string(commands.size()) + " commands from db.");
    } catch (const std::exception& e) {
        std::string error_msg = "Error getting commands from db: " + std::string(e.what());
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    return commands;
}