#include <iostream>

extern void DisplayINFO(void);

int main(void)
{
    std::cout << "=== head.h ===\n";
    #include "head.h"
    std::cout << "=== temp.h ===\n";
    #include "temp.h"
    std::cout << "=== temp.cpp ===\n";
    DisplayINFO();

    return 0;
}