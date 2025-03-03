#include "Corpora.h"

bool Corpora::addDocument(
    const Document& doc) {
    int doc_len(0);
    std::stringstream ss(doc.getVal());
    std::string curr;
    std::unordered_set<std::string> seen_words;
    const std::string command = doc.getCommand();

    // User must use update
    if (_doc_sizes.find(command) != _doc_sizes.end() ||
        _inner_count.find(command) != _inner_count.end()) {
            _log.warning(
                "Attempted to add '" + command +
                "' to Corpora, but that command already exists, use updateDocument()");
            return false;
        }

    // Time to go word by word
    while (getline(ss, curr, ' ')) {
        if (doc_len == 0 && !curr.empty()) { // first iteration
            _inner_count[command][curr] = 1;
        } else {
            _inner_count[command][curr] += 1;
        }

        if (_overall_count.find(curr) == _overall_count.end()) {
            _overall_count[curr] = 1;
        } else if (seen_words.find(curr) == seen_words.end()) {
            _overall_count[curr] += 1;
        }

        seen_words.insert(curr);
        doc_len++;
    }

    _doc_count++;
    _total_doc_length += doc_len;
    _doc_sizes[command] = doc_len;

    try {
        _database->insertDocument(doc);
    } catch (...) {
        return false;
    }

    return true;
}


void Corpora::addDocuments(
    const std::vector<Document>& documents 
) {
    for (const Document& doc : documents) {
        addDocument(doc);
    }
}


void Corpora::clear() {
    if (_database) {
        _database->reset();
        _database->init();
    }

    _total_doc_length = 0;
    _doc_count = 0;
    _inner_count.clear();
    _overall_count.clear();
    _doc_sizes.clear();
}

// These are all of the accessors for calculating BM25
unsigned int Corpora::n(const std::string& keyword) const {
    if (_overall_count.find(keyword) == _overall_count.end()) {
        return 0;
    }
    
    return _overall_count.find(keyword)->second;
}


unsigned int Corpora::f(
    const std::string& keyword,
    const std::string& command) const {
    if (_inner_count.find(command) == _inner_count.end()) {
        return 0;
    }

    auto it = _inner_count
    .find(command)->second
    .find(keyword);

    if (it == 
        _inner_count
        .find(command)->second
        .end()) {
        return 0;
    }
    
    return it->second;
}


unsigned int Corpora::D_mag(const std::string& command) const {
    if (_doc_sizes.find(command) == _doc_sizes.end()) {
        return 0;
    }
    
    return _doc_sizes.find(command)->second;
}


double Corpora::avgdl() const {
    if (!_doc_count) {
        return 0;
    }

    return _total_doc_length / _doc_count;
}