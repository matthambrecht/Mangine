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

    _conn = new pqxx::connection(conn_str.c_str());
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

    _conn = new pqxx::connection(conn_str.c_str());
    _log.normal(CLASS_NAME, "Conected to database " + db_vars[1] + "@127.0.0.1");
}


Database::~Database() {
    if (_conn) {
        delete _conn;
    }
}

void Database::init() {
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

    send(query);
}

void Database::reset() {
    const std::string query = R"(
        DROP TABLE IF EXISTS mappings;
        DROP TABLE IF EXISTS chunks;
        DROP EXTENSION IF EXISTS vector;
        )";

    send(query);
} 

void Database::send(const std::string& query) {
    pqxx::work transaction(*_conn);
    transaction.exec0(query);

    _log.normal(CLASS_NAME, "Recieved query");
}