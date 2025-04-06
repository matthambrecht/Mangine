#include <iostream>

#include "service.h"
#include "../include/mailbox.h"

void Service::run_loop() {
    API * server = new API();
    const char * key = "Mangine";
    office * of = init_office(key);
    add_mailbox(of, 0);

    while (true) {
        std::cout << "Waiting for requests..." << std::endl;
        M * req = await_mail(of, 0);
        M resp = {
            {},
            0,
            0,
            {}
        };

        std::strcpy(
            resp.msg_buf,
            Parser::getArgs(req->argv, req->argc, server).c_str());
        send_mail(of, req->sender, &resp);
    } 
    

    delete server;
    return;
}

int main() {
    std::cout << "Starting server..." << std::endl;
    Service::run_loop();

    return 0;
}