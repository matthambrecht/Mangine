#ifndef REQUEST
#define REQUEST

#include <iostream>
#include <string>
#include <vector>
#include <cpr/cpr.h>
#include "../utils/json.hpp"

class Request {
public:
    Request();
    Request(std::string endpoint, int vector_size);
    ~Request();
    std::vector<double> getEmbedding(std::string);
private:
    std::string * _endpoint;
    int _vector_size;
};
#endif