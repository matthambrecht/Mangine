#include <iostream>
#include <string>
#include <cmath>
#include <unordered_map>
#include <vector>

namespace BM25 {
    // Free params (unoptimized)
    const float k1 = 1.7; // k1 ∈ [1.2, 2.0]
    const float b = 0.75;

    // score(D, Q)
    // double score() {}

    // f(qi, D)
    // int num_occurrences() {}


    // bool get_docs_in() {}

    /* 
        IDF(qi) = ln(
                    ((N - n(qi) + 0.5) /
                    (n(qi) + 0.5)) + 1
                    )
    */
    // double idf(const char query_term, const int num_docs, const int num_docs_in) {}

    std::vector<std::string> vectorize_text(const std::string& document) {
        std::stringstream ss(document);
        std::string curr;
        std::vector<std::string> vectorized;
        
        while (getline(ss, curr, ' ')) {
            vectorized.push_back(curr);
        }

        return vectorized;
    }

    // std::unordered_map<std::string, int> get_docs_in() {

    // }
};