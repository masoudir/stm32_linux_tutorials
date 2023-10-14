
#ifndef _MAHDA_USER_APP1_H
#define _MAHDA_USER_APP1_H

#include "main.h"

class AppType1 {
private:
    MahdaLogger *log_ptr_;
    char *app_name_;
    MahdaMainSharedData *shared_data_;

private:
    void app_init();

    void app_exit();

public:
    void app_loop(std::atomic<bool> &running);

    AppType1(const char *app_name,
             MahdaLogger *log_ptr,
             MahdaMainSharedData *shared_data);

    ~AppType1();

};


#endif //_MAHDA_USER_APP1_H
