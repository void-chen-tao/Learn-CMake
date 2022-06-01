#include <iostream>


void lib1(void)
{
    std::cout << __FILE__ << ":" << __FUNCTION__ << ":" << __LINE__ << std::endl;
}