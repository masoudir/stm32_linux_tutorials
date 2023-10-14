/******************************************************************************

Welcome to GDB Online.
GDB online is an online compiler and debugger tool for C, C++, Python, Java, PHP, Ruby, Perl,
C#, OCaml, VB, Swift, Pascal, Fortran, Haskell, Objective-C, Assembly, HTML, CSS, JS, SQLite, Prolog.
Code, Compile, Run and Debug online from anywhere in world.

*******************************************************************************/
#include "main.h"

std::atomic<bool> app4_running {true};

int main()
{
    std::cout << "Hello World" << std::endl;

    AppType4 app4("app4");
    std::thread th_app4(&AppType4::app_loop, &app4, std::ref(app4_running));
    th_app4.detach();

    std::this_thread::sleep_for(std::chrono::seconds(4));
    app4_running = false;

    // while(1);

    return 0;
}