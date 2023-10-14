#include "app_modbus_slave.h"

AppType2 *app2_ptr;

void modbus_slave_timer_callback() {
//    if (master->modbus_read_bits_by_address(0x00, 1, &dest1)) {
//        std::cout << "bit0=" << (int) dest1 << std::endl;
//    }
//    gg1 = !gg1;
//    master->modbus_write_bit_by_address(0x00, gg1);

//    master->test_illegal_address();

    if (app2_ptr->shared_data_->modbus_slave != nullptr) {
        auto out = app2_ptr->shared_data_->modbus_slave->update_database_from_memory();
        out = app2_ptr->shared_data_->modbus_slave->update_input_data_memory_from_database();
    }

}

AppType2::AppType2(
        const char *app_name, MahdaLogger
*log_ptr, MahdaMainSharedData *shared_data) :
        log_ptr_(log_ptr),
        app_name_(const_cast<char *>(app_name)),
        shared_data_(shared_data) {
    log_ptr_->log_info(app_name_, "Application started executing init function.");
    MahdaModbusConfig *cfg = &shared_data_->modbus_slave_config;
    shared_data_->modbus_slave_database = new MahdaModbusSlaveData(cfg->modbus_cfg.ut_bits_nb, cfg->modbus_cfg.ut_input_bits_nb,
                                                     cfg->modbus_cfg.ut_input_registres_nb,
                                                     cfg->modbus_cfg.ut_registers_nb_max);
    app_init();
    app2_ptr = this;
    log_ptr_->log_info(app_name_, "Application finished executing init function.");
}

AppType2::~AppType2() {
    log_ptr_->log_info(app_name_, "Application started executing exit function.");
    app_exit();
    log_ptr_->log_info(app_name_, "Application finished executing exit function.");
}

void AppType2::app_init() {
    MahdaModbusConfig *cfg = &shared_data_->modbus_slave_config;
    shared_data_->modbus_slave = new MahdaModbusSlave(&cfg->modbus_cfg, &cfg->modbus_connection, shared_data_->modbus_slave_database);
    shared_data_->modbus_slave_timer = new MahdaSafeTimer(10, 1, modbus_slave_timer_callback);
    shared_data_->modbus_slave_timer->start();
}

void AppType2::app_loop(std::atomic<bool> &running) {
    while (true) {
        log_ptr_->log_info(app_name_, "This is loop function.");
        log_ptr_->log_info(app_name_, "Waiting Modbus slave to be connected.");
        while (!shared_data_->modbus_slave->connect());
        log_ptr_->log_info(app_name_, "Modbus slave is connected and listening ...");
        if (!shared_data_->modbus_slave->listen_for_a_new_request()) {
            shared_data_->modbus_slave_timer->stop();
            app_exit();
            app_init();
        }
    }
}

void AppType2::app_exit() {
//    std::this_thread::sleep_for(std::chrono::seconds(10));
    delete shared_data_->modbus_slave_timer;
    delete shared_data_->modbus_slave;

}




