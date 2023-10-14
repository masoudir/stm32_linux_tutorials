#include "mahda_user_app1.h"



AppType1::AppType1(const char *app_name, MahdaLogger *log_ptr, MahdaMainSharedData *shared_data) :
        log_ptr_(log_ptr),
        app_name_(const_cast<char *>(app_name)),
        shared_data_(shared_data) {
    log_ptr_->log_info(app_name_, "Application started executing init function.");
    app_init();
    log_ptr_->log_info(app_name_, "Application finished executing init function.");
}

AppType1::~AppType1() {
    log_ptr_->log_info(app_name_, "Application started executing exit function.");
    app_exit();
    log_ptr_->log_info(app_name_, "Application finished executing exit function.");
}

void AppType1::app_init() {

}

void AppType1::app_loop(std::atomic<bool> &running) {
    while(running) {
        log_ptr_->log_info(app_name_, "This is loop function.");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void AppType1::app_exit() {

}
