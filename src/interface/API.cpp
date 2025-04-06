#include "API.h"

// info: Gets info about the db
std::string API::info() {
    std::ostringstream resp;

    resp << R"(
• ▌ ▄ ·.  ▄▄▄·  ▐ ▄  ▄▄ • ▪   ▐ ▄ ▄▄▄ .
·██ ▐███▪▐█ ▀█ •█▌▐█▐█ ▀ ▪██ •█▌▐█▀▄.▀·
▐█ ▌▐▌▐█·▄█▀▀█ ▐█▐▐▌▄█ ▀█▄▐█·▐█▐▐▌▐▀▀▪▄
██ ██▌▐█▌▐█ ▪▐▌██▐█▌▐█▄▪▐█▐█▌██▐█▌▐█▄▄▌
▀▀  █▪▀▀▀ ▀  ▀ ▀▀ █▪·▀▀▀▀ ▀▀▀▀▀ █▪ ▀▀▀ 
    ** A Command Search Engine **
---------------------------------------)";

    if (!_corpora) {
        resp << "\nServer Not Loaded";
        return resp.str();;
    }

    resp << "\nVersion: Pre-Alpha" <<
            "\nAuthor: Matt Hambrecht (https://github.com/matthambrecht)" <<
            "\nLoaded Commands: " << std::to_string(_corpora->N());
    

    return resp.str();
}