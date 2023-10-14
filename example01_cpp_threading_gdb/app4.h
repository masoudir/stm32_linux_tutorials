
#ifndef _APP4_H
#define _APP4_H

#include "main.h"

class AppType4 {
private:
    char *app_name_;

private:
    void app_init();

    void app_exit();

public:
    void app_loop(std::atomic<bool> &running);

    AppType4(const char *app_name);

    ~AppType4();

};


#endif //_MAHDA_USER_APP1_H
