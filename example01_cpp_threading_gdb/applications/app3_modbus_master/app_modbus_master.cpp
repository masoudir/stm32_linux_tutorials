#include "app_modbus_master.h"




void AppType3::modbus_delete() {
    delete shared_data_->modbus_master;
}

void AppType3::modbus_init() {

    MahdaModbusConfig* cfg = &shared_data_->modbus_master_config;
    shared_data_->modbus_master = new MahdaModbusMaster(&cfg->modbus_cfg, &cfg->modbus_connection);


    {
        std::cout << "connecting..." << std::endl;
        while (!shared_data_->modbus_master->connect()) {
            std::this_thread::sleep_for(std::chrono::seconds(1));
        }

        std::cout << "connected!" << std::endl;
    }

}


AppType3::AppType3(const char *app_name, MahdaLogger *log_ptr, MahdaMainSharedData *shared_data) :
        log_ptr_(log_ptr),
        app_name_(const_cast<char *>(app_name)),
        shared_data_(shared_data) {
    log_ptr_->log_info(app_name_, "Application started executing init function.");
    app_init();
    log_ptr_->log_info(app_name_, "Application finished executing init function.");
}

AppType3::~AppType3() {
    log_ptr_->log_info(app_name_, "Application started executing exit function.");
    app_exit();
    log_ptr_->log_info(app_name_, "Application finished executing exit function.");
}

void AppType3::app_init() {
    modbus_init();
}

void AppType3::app_loop(std::atomic<bool> &running) {
    while (running) {
        log_ptr_->log_info(app_name_, "This is loop function.");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void AppType3::app_exit() {
    delete shared_data_->modbus_master;
    std::cout << "disconnected!" << std::endl;
}


