#include "app4.h"



AppType4::AppType4(const char *app_name) :
        app_name_(const_cast<char *>(app_name)) {
    std::cout << "Application started executing init function." << std::endl;
    app_init();
}

AppType4::~AppType4() {
    std::cout << "Application started executing exit function." << std::endl;
    app_exit();
}

void AppType4::app_init() {

}

void AppType4::app_loop(std::atomic<bool> &running) {
    while(running) {
        std::cout << "This is loop function." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}

void AppType4::app_exit() {

}
