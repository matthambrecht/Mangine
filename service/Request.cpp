#include "Request.h"

Request::Request() : _vector_size(30), _log(Log()), _config(Config()) {
    _normal_endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed"
        );

    _batch_endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed_batch"
        );

    _vector_size = _config._config["endpoint"]["vector_len"].get<int>();
}

Request::Request(const Log& log, const Config& config) : _vector_size(30), _log(log), _config(config) {
    _normal_endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed"
        );

    _batch_endpoint = new std::string(
        "http://"
        + _config._config["endpoint"]["host"].get<std::string>()
        + ":"
        + _config._config["endpoint"]["port"].get<std::string>()
        + "/embed_batch"
        );

    _vector_size = _config._config["endpoint"]["vector_len"].get<int>();
}

Request::~Request() {
    if (_normal_endpoint != nullptr) {
        delete _normal_endpoint;
        _normal_endpoint = nullptr;
    }

    if (_batch_endpoint != nullptr) {
        delete _batch_endpoint;
        _batch_endpoint = nullptr;
    }
}

 pgvector::Vector Request::getEmbedding(const std::string& query) { // Handle embedding of single string
    std::vector<float> embedding_vector;

    nlohmann::json payload = {
        {"query", query},
    };

    // Curl response for endpoint
    _log.normal(CLASS_NAME, "Sending a single embedding request.");
    cpr::Response response = cpr::Post(
        cpr::Url{*_normal_endpoint},
        cpr::Header{
            {"Content-Type", "application/json"}
            },
        cpr::Body(
            payload.dump(
                -1,
                ' ',
                false,
                nlohmann::json::error_handler_t::replace)));

    // Validate response is in proper format
    if (response.status_code != 200) {
        const std::string error_msg = "Failed to make request to Embedding endpoint. Response code: "
         + std::to_string(response.status_code);

        _log.error(CLASS_NAME, error_msg);
        throw std::domain_error(error_msg);
    } else { // Handle embeddings returned
        nlohmann::json response_json = nlohmann::json::parse(response.text);
        _log.normal(CLASS_NAME, "Recieved a single embedding response.");

        if (response_json.contains("embedding")&& response_json["embedding"].is_array()) {
            embedding_vector = response_json["embedding"].get<std::vector<float>>();
        } else {
            const std::string error_msg = "Embedding vector doesn't exist, or isn't of the expected type.";
            _log.error(CLASS_NAME, error_msg);
            throw std::invalid_argument(error_msg);
        }
    }

    _log.normal(CLASS_NAME, "Query successfully embedded.");
    return pgvector::Vector(embedding_vector);
}


std::vector<pgvector::Vector> Request::getEmbeddingBatch(const std::vector<std::string>& query_batch) { // Handle embedding of multiple strings
    std::vector<pgvector::Vector> embedding_batch;
    nlohmann::json payload = {
        {"query_batch", query_batch},
    };

     // Curl response for endpoint
     _log.normal(CLASS_NAME, "Sending batch embedding request with " + std::to_string(query_batch.size()) + " queries.");
    cpr::Response response = cpr::Post(
        cpr::Url{*_batch_endpoint},
        cpr::Header{
            {"Content-Type", "application/json"}
            },
        cpr::Body(
            payload.dump(
                -1,
                ' ',
                false,
                nlohmann::json::error_handler_t::replace)));

    // Validate response is in proper format
    if (response.status_code != 200) {
        const std::string error_msg = "Failed to make request to Batch Embedding endpoint. Response code: "
         + std::to_string(response.status_code);

        _log.error(CLASS_NAME, error_msg);
        throw std::domain_error(error_msg);
    } else {
        nlohmann::json response_json = nlohmann::json::parse(response.text);
        _log.normal(CLASS_NAME, "Recieved a batch embedding response.");

        if (response_json.contains("embedding_batch")&& response_json["embedding_batch"].is_array()) {
            for (const auto& embedding : response_json["embedding_batch"]) { // Parse individual embeddings
                if (embedding.is_array()) {
                    std::vector<float> embedding_vector = embedding.get<std::vector<float>>();
                    embedding_batch.emplace_back(pgvector::Vector(embedding_vector));
                } else {
                    const std::string error_msg = "Embedding batch output doesn't exist, or isn't of the expected type.";
                    _log.error(CLASS_NAME, error_msg);
                    throw std::invalid_argument(error_msg);   
                }
            }
        } else {
            const std::string error_msg = "Embedding batch output doesn't exist, or isn't of the expected type.";
            _log.error(CLASS_NAME, error_msg);
            throw std::invalid_argument(error_msg);        
        }
    }

    return embedding_batch;
}