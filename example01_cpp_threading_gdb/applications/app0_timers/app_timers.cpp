#include "app_timers.h"

MahdaMainSharedData *shared_data_;


//uint8_t dest1 = 0;
//bool gg1 = false;
//uint8_t dest2 = 0;
//bool gg2 = false;

void check_function2() {
//    if(master->modbus_read_bits_by_address(0x01, 1, &dest2)) {
//        std::cout << "bit1=" << (int) dest2 << std::endl;
//    }
//    gg2 = !gg2;
//    master->modbus_write_bit_by_address(0x01, gg2);

    if (shared_data_->modbus_master != nullptr) {
        if (shared_data_->modbus_master->modbus_write_float_registers_by_address(0x160, 1.05)) {
            std::cout << "write ok" << std::endl;
        }
    }

}

void check_function1() {
//    if (master->modbus_read_bits_by_address(0x00, 1, &dest1)) {
//        std::cout << "bit0=" << (int) dest1 << std::endl;
//    }
//    gg1 = !gg1;
//    master->modbus_write_bit_by_address(0x00, gg1);

//    master->test_illegal_address();

    if (shared_data_->modbus_master != nullptr) {
        auto out = shared_data_->modbus_master->modbus_read_float_registers_by_address(0x160);
        std::cout << "read float=" << out << std::endl;
    }

}

AppType0::AppType0(const char *app_name, MahdaLogger *log_ptr, MahdaMainSharedData *shared_data) :
        log_ptr_(log_ptr),
        app_name_(const_cast<char *>(app_name)) {
    shared_data_ = shared_data;
    log_ptr_->log_info(app_name_, "Application started executing init function.");
    app_init();
    log_ptr_->log_info(app_name_, "Application finished executing init function.");
}

AppType0::~AppType0() {
    log_ptr_->log_info(app_name_, "Application started executing exit function.");
    app_exit();
    log_ptr_->log_info(app_name_, "Application finished executing exit function.");
}

void AppType0::app_init() {
    timer1 = new MahdaSafeTimer(10, 1000, check_function1);
    timer2 = new MahdaSafeTimer(100, 2000, check_function2);
    timer1->start();
    timer2->start();
}

void AppType0::app_exit() {
    delete timer1;
    delete timer2;
    std::cout << "disconnected!" << std::endl;
}



