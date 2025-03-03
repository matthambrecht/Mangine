#include "Indexer.h"


bool Indexer::index(const std::string& command) {
    try {
        Document document = Man().getCommandMan(command);
    
        if (document.getVal().empty()) { // Command likely wasn't retrieveable
            _log.error(
                "The command '" + command + "' doesn't exist or wasn't retrievable");
            return false;
        }

        return _corpora.addDocument(document);
    } catch (const std::exception& e) {
        _log.error(
            "Something went wrong when attempting to document the command '" + command + "' -> " +
            std::string(e.what()));

        return false;
    }
}


// We keep this void because it's to be expected that removed programs may still show up
void Indexer::indexAll() {
    Man man;
    std::vector<Document> documents;
    std::vector<std::string> commands = man.getAllCommands();

    try {
        for (const std::string& command : commands) {
            Document document = man.getCommandMan(command);

            if (document.getVal().empty()) { // Command likely wasn't retrieveable
                _log.error(
                    "The command '" + command + "' doesn't exist or wasn't retrievable");
            }

            documents.push_back(document);
        }

        _corpora.addDocuments(documents);
    } catch (const std::exception& e) {
        _log.error(
            "Something went wrong when attempting to document all commands -> " + std::string(e.what()));
    }
}