#include "Man.h"

Man::Man() {
    _config = Config();
}

// Gets the list of all executable commands with manpages (Sec. 1)
std::vector<std::string> Man::getAllCommands() {
    int buffer_size = 256;
    char stdout_buffer[buffer_size];
    std::vector<std::string> command_vector;
    std::string command = "apropos -s 1 ''";
    std::string command_result = "";
    FILE * pipe = popen(command.c_str(), "r");

    // Check for pipe health
    if (!pipe) {
        throw std::runtime_error("Failed to open stdout pipe()");
    }

    // Run the command and get stdout
    try {
        while(fgets(stdout_buffer, sizeof stdout_buffer, pipe) != NULL) {
            command_result += stdout_buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    // Parse command lines out of stdout
    if (!command_result.empty()) {
        std::stringstream line_stream(command_result);
        std::string line_delimited;
        
        // Parse each line
        while (std::getline(line_stream, line_delimited, '\n')) {
            std::stringstream command_stream(line_delimited);
            std::string command_delimited;

            // Parse each command from the line
            std::getline(command_stream, command_delimited, ' ');
            command_vector.push_back(command_delimited); 
        }
    } else {
        throw std::ios_base::failure("Failed to recieve `man` command output");
    }

    pclose(pipe);

    return command_vector;
}

// Gets manpage output for a command
std::string Man::getCommandMan(std::string command) {
    int buffer_size = 256;
    char stdout_buffer[buffer_size];
    std::vector<std::string> command_vector;
    std::string man_command = "man " + command + " | cat 2>/dev/null";
    std::string command_result = "";
    FILE * pipe = popen(man_command.c_str(), "r");

    // Check for pipe health
    if (!pipe) {
        throw std::runtime_error("Failed to open stdout pipe()");
    }

    // Run the command and get stdout
    try {
        while(fgets(stdout_buffer, sizeof stdout_buffer, pipe) != NULL) {
            command_result += stdout_buffer;
        }
    } catch (...) {
        pclose(pipe);
        throw;
    }

    pclose(pipe);

    return command_result;
}

std::vector<Chunk> Man::getCommandChunks(std::string command, std::string man_contents) {
    std::vector<Chunk> man_chunks;
    int chunk_size = _config._config["embedding"]["chunk_size"].get<int>();

    for (size_t i = 0; i < man_contents.size(); i += chunk_size) {
        std::string chunk_content = man_contents.substr(i, chunk_size);
        std::vector<double> embedding = Request().getEmbedding(chunk_content);

        man_chunks.push_back(Chunk(command, chunk_content, embedding));
    }

    return man_chunks;
}