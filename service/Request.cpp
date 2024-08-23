#include "Request.h"

Request::Request() : _endpoint(nullptr), _vector_size(30) {}

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
        std::cout << "[SearchMan] Request failed. Status code: " << response.status_code << std::endl;
        return {};
    } else { // Handle embeddings returned
        nlohmann::json response_json = nlohmann::json::parse(response.text);

        if (response_json.contains("embedding") && response_json["embedding"].is_array()) {
            embedding_vector = response_json["embedding"].get<std::vector<double>>();
        } else {
            std::cerr << "[SearchMan] Response JSON does not contain 'embedding' or is not in a valid format (array)" << std::endl;
        }
    }

    return embedding_vector;
}