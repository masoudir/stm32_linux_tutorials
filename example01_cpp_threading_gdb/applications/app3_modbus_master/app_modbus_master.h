
#ifndef _MODBUS_MASTER_APP_H
#define _MODBUS_MASTER_APP_H

#include "main.h"


/*
 * Modbus Master App
 */

class AppType3 {
private:
    MahdaLogger *log_ptr_;
    char *app_name_;
    MahdaMainSharedData *shared_data_;




private:
    void app_init();

    void app_exit();

    void modbus_init();
    void modbus_delete();


public:
    MahdaSafeTimer *timer1;
    MahdaSafeTimer *timer2;

    static void check_function1();
    static void check_function2();

    void app_loop(std::atomic<bool> &running);

    AppType3(const char *app_name,
             MahdaLogger *log_ptr,
             MahdaMainSharedData *shared_data);

    ~AppType3();

};
#endif //_MODBUS_MASTER_APP_H
