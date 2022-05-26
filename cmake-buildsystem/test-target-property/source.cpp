#include <iostream>
#include "file.h"


void Show1(void)
{
    std::cout << "1111\n";
}

void Show2(void)
{
    std::cout << "2222\n";
}

void Show(void)
{
    #ifdef USE_FILE1
    Show1();
    #endif

    #ifdef USE_FILE2
    Show2();
    #endif
}