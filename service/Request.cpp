#include "Request.h"

Request::Request() : _vector_size(30), _log(Log()), _config(Config()) {
    _endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed"
        );

    _vector_size = _config._config["endpoint"]["vector_len"].get<int>();
}

Request::Request(const Log& log, const Config& config) : _vector_size(30), _log(log), _config(config) {
    _endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed"
        );

    _vector_size = _config._config["endpoint"]["vector_len"].get<int>();
}

Request::Request(const std::string& endpoint, const int vector_size, const Log& log, const Config& config)
    : _endpoint(new std::string(endpoint)), _vector_size(vector_size), _log(log), _config(config) {}

Request::~Request() {
    if (_endpoint != nullptr) {
        delete _endpoint;
        _endpoint = nullptr;
    }
}

std::vector<double> Request::getEmbedding(const std::string& query) {
    std::vector<double> embedding_vector;

    _log.normal(CLASS_NAME, "Sending query to be embedded...");

    nlohmann::json payload = {
        {"query", query},
    };

    // Curl response for endpoint
    cpr::Response response = cpr::Post(
        cpr::Url{*_endpoint},
        cpr::Header{{"Content-Type", "application/json"}},
        cpr::Body(payload.dump())
    );

    if (response.status_code != 200) {
        const std::string error_msg = "Failed to make request to Embedding endpoint. Response code: "
         + std::to_string(response.status_code);

        _log.error(CLASS_NAME, error_msg);
        throw std::domain_error(error_msg);
    } else { // Handle embeddings returned
        nlohmann::json response_json = nlohmann::json::parse(response.text);

        if (response_json.contains("embedding")&& response_json["embedding"].is_array()) {
            embedding_vector = response_json["embedding"].get<std::vector<double>>();
        } else {
            const std::string error_msg = "Embedding vector doesn't exist, or isn't of the expected type.";
            _log.error(CLASS_NAME, error_msg);
            throw std::invalid_argument(error_msg);
        }
    }

    _log.normal(CLASS_NAME, "Query successfully embedded.");
    return embedding_vector;
}