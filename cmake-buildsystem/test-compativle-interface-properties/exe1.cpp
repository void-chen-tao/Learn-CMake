#include <iostream>

extern void lib1(void);

void main(void)
{
    std::cout << __FILE__ << ": " << __FUNCTION__ << std::endl;
    std::cout << std::endl;
    lib1();
}