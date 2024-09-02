#include "Indexer.h"

void Indexer::index_all() {
    _log.normal(CLASS_NAME, "Beginning indexing all manpages...");

    // Get all commands and manpages
    try {
        chunker();
    } catch (const std::exception& e) {
        std::string error_msg = std::string("Hit an exception while indexing: ") + e.what();
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Completed indexing all manpages.");
}

void Indexer::chunker() {
    Man man = Man(_log, _config);
    std::vector<Chunk> chunks;
    std::vector<std::string> chunk_strs;
    std::vector<pgvector::Vector> embeddings;

    _log.normal(CLASS_NAME, "Chunking all manpages...");

    // Get and chunk all of the command manpage contents
    for (const auto& command : man.getAllCommands()) {
        std::vector<Chunk> command_chunks = man.getCommandChunks(
            command,
            man.getCommandMan(command)
        );

        // Add chunks and contents to respective vectors
        chunks.reserve(chunks.size() + command_chunks.size());
        chunk_strs.reserve(chunk_strs.size() + command_chunks.size());
        std::copy(
            command_chunks.begin(),
            command_chunks.end(),
            std::back_inserter(chunks));
        std::transform(
            command_chunks.begin(),
            command_chunks.end(),
            std::back_inserter(chunk_strs),
            [](const Chunk& chunk) { return chunk.getVal(); });
    }

    _log.normal(CLASS_NAME, "Completed chunking, recieved " + std::to_string(chunks.size()) + " chunks.");
    batch_embed(chunks, chunk_strs);
}

void Indexer::batch_embed(std::vector<Chunk>& chunks, std::vector<std::string> chunk_strs) {
    _log.normal(CLASS_NAME, "Batch embedding all chunks, this could take a while...");
    
    std::vector<pgvector::Vector> embeddings = _request->getEmbeddingBatch(chunk_strs);

    for (int idx = 0; idx < (int)chunks.size(); idx++) {
        chunks.at(idx).setEmbedding(embeddings.at(idx));
    }

    if (chunks.size() != embeddings.size()) {
        std::string error_msg = "Not all chunks were embedded due to some internal error.";
        _log.error(CLASS_NAME, error_msg);
        throw std::runtime_error(error_msg);
    }

    _log.normal(CLASS_NAME, "Completed batch embeddings.");
};