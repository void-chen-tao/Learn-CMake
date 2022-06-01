#include <iostream>


void sharedlib(void)
{
    #ifdef LIB1_WITH_SHARED_LIB
    std::cout << "LIB1_WITH_SHARED_LIB Open\n";
    #else
    std::cout << "LIB1_WITH_SHARED_LIB Close\n";
    #endif
}