#include <iostream>

#include "../include/mailbox.h"

int main(int argc, char * argv[]) {
    const char * key = "Mangine";
    struct office * of = init_office(key);
    int box_id = get_vacant(of);
    M mail = {
        {},
        box_id,
        argc,
        {}
    };

    if (!box_id) {
        std::cerr << "Mangine service isn't running!" << std::endl;
        return 1;
    }

    add_mailbox(of, box_id);

    for (int i = 0; i < argc && i < 128; ++i) { // Copy args to ipc msg
        if (strlen(argv[i]) >= 48) {
            std::cout << "\"" <<
            argv[i] << "\"? You aren't gonna find a word that long bud..." 
            << std::endl;
            remove_mailbox(of, box_id);
            return 1;
        }

        strncpy(mail.argv[i], argv[i], 48 - 1);
    }

    send_mail(of, 0, &mail);

    M * resp = await_mail(of, box_id);
    std::cout << resp->msg_buf << std::endl;
    
    remove_mailbox(of, box_id);

    return 0;
}