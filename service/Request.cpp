#include "Request.h"

Request::Request() : _vector_size(30) {
    _endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed"
        );

    _vector_size = _config._config["endpoint"]["vector_len"].get<int>();
}

Request::Request(std::string endpoint, int vector_size)
    : _endpoint(new std::string(endpoint)), _vector_size(vector_size) {}

Request::~Request() {
    if (_endpoint != nullptr) {
        delete _endpoint;
        _endpoint = nullptr;
    }
}

std::vector<double> Request::getEmbedding(std::string query) {
    std::vector<double> embedding_vector;
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
        throw std::domain_error("Failed to make request to Embedding endpoint. Response code: "
         + std::to_string(response.status_code)
         );
    } else { // Handle embeddings returned
        nlohmann::json response_json = nlohmann::json::parse(response.text);

        if (response_json.contains("embedding") && response_json["embedding"].is_array()) {
            embedding_vector = response_json["embedding"].get<std::vector<double>>();
        } else {
            throw std::invalid_argument("Embedding vector doesn't exist, or isn't of the expected type.");
        }
    }

    return embedding_vector;
}