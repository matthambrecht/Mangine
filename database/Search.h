#ifndef BM25_H
#define BM25_H

#include <iostream>
#include <cmath>
#include <string>
#include <queue>

#include <Corpora.h>

namespace BM25 {
    // Free params (unoptimized)
    const float k1 = 1.4; // k1 ∈ [1.2, 2.0]
    const float b = 0.75;

    void score(
        const std::string& query,
        const Corpora& corpora,
        const std::vector<std::string> all_commands,
        unsigned int n = 15) {
        
        std::priority_queue<int, std::vector<int>, std::greater<int>> n_best;

        for (const std::string& D : all_commands) {
            std::istringstream Q(query);
            std::string qi;
            double curr_doc_score = 0.0;

            while (getline(Q, qi, ' ')) {
                curr_doc_score += (
                    (corpora.f(qi, D) * (k1 + 1))
                    /
                    (corpora.f(qi, D) + k1 * (1 - b + b * corpora.D_mag(D) / corpora.avgdl()))
                ) * log(
                    (corpora.N() - corpora.n(qi) + 0.5)
                    /
                    (corpora.n(qi) + 0.5)
                    + 1
                );
            }
        }
    }
};

#endif