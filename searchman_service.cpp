#include <iostream>
#include <string>
#include "service/Request.cpp"

int main() {
    std::string test_input;
    std::string endpoint = "http://127.0.0.1:30998/embed";
    std::cout << ">>> ";
    std::cin >> test_input;
    Request newRequest(endpoint, 300);
    std::vector<double> embeddings = newRequest.getEmbedding(test_input);

    for (
        std::vector<double>::iterator it = embeddings.begin(); it != embeddings.end();) {
        std::cout << *it++ << std::endl;
    }

    return 0;
}