#include "Indexer.h"


bool Indexer::index(const std::string& command) {
    bool status = true;
    std::vector<Chunk> chunks;
    std::vector<std::string> chunk_strs;
    Man man;

    try {
        chunks = man.getCommandChunks(command, man.getCommandMan(command));
    
        if (!chunks.size()) { // Command likely wasn't retrieveable
            _log.error(
                "The command '" + command + "' doesn't exist or wasn't retrievable");
            return false;
        }

        for (const Chunk& chunk : chunks) {
            _database->insertChunk(chunk);
        }

    } catch (const std::exception& e) {
        _log.error(
            "Something went wrong when attempting to chunk the command '" + command + "' -> " +
            std::string(e.what()));

        return false;
    }

    return true;
}