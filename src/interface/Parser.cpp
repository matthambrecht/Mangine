#include "Parser.h"

std::string Parser::getArgs(char argv[][48], int argc, API * api) {
    argparse::ArgumentParser prog(
        "mangine",
        "Mangine v1.0.0",
        argparse::default_arguments::all,
        false);
    argparse::ArgumentParser infoCmd(
        "info",
        "Mangine v1.0.0",
        argparse::default_arguments::all,
        false);
    argparse::ArgumentParser queryCmd(
        "query",
        "Mangine v1.0.0",
        argparse::default_arguments::all,
        false);
    argparse::ArgumentParser serviceCmd(
        "service",
        "Mangine v1.0.0",
        argparse::default_arguments::all,
        false);
    char *tmp_argv[128];
    std::stringstream os;
    
    for (int i = 0; i < argc; i++) {
        tmp_argv[i] = argv[i];
    }

    // Info cmd
    infoCmd.add_description("Displays information about Mangine service");
    prog.add_subparser(infoCmd);

    // Query args
    queryCmd.add_description("Search Mangine's database for the most relevent commands to your query");
    queryCmd.add_argument("<query>")
        .help("The actual query that will be sent")
        .remaining()
        .required();

    prog.add_subparser(queryCmd);

    // Service args
    serviceCmd.add_description("Commands for interacting with the Mangine service");
    prog.add_subparser(serviceCmd);

    try {
        prog.parse_args(argc, tmp_argv);
    } catch (const std::exception& err) {
        os << err.what() << std::endl;
        os << prog;
        return os.str();
    }

    if (prog.is_subcommand_used("info")) {
        return api->info();
    } else if (prog.is_subcommand_used("service")) {
        return "Nothing here yet";
    } else if (prog.is_subcommand_used(queryCmd)) {
        if (!queryCmd.is_used("<query>")) {
            return "A query must be provided!";
        }

        std::vector<std::string> query_str = queryCmd.get<std::vector<std::string>>("<query>");
        std::string resp = "";
        
        for (const std::string& i : query_str) {
            resp += i + " ";
        }
        
        return resp;
    }

    return "Nothing here yet";
}