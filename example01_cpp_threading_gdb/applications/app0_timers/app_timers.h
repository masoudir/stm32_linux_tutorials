
#ifndef APP0_TIMERS_H
#define APP0_TIMERS_H

#include "main.h"

void check_function1();
void check_function2();

/*
 * Modbus Master App
 */

class AppType0 {
private:
    MahdaLogger *log_ptr_;
    char *app_name_;



private:
    void app_init();

    void app_exit();


public:
    MahdaSafeTimer *timer1;
    MahdaSafeTimer *timer2;



    AppType0(const char *app_name,
             MahdaLogger *log_ptr,
             MahdaMainSharedData *shared_data);

    ~AppType0();

};
#endif //_MODBUS_MASTER_APP_H
