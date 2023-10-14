
#ifndef _MODBUS_SLAVE_APP_H
#define _MODBUS_SLAVE_APP_H

#include "main.h"

/*
 * Modbus Slave App
 */

class AppType2 {
public:
    MahdaMainSharedData *shared_data_;
private:
    MahdaLogger *log_ptr_;
    char *app_name_;





private:
    void app_init();

    void app_exit();

public:

    void app_loop(std::atomic<bool> &running);

    AppType2(const char *app_name,
             MahdaLogger *log_ptr,
             MahdaMainSharedData *shared_data);

    ~AppType2();

};
#endif //_MODBUS_SLAVE_APP_H
