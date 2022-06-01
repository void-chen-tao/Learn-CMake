#include <iostream>

extern void lib1(void);

int main()
{
    #ifdef LIB1_WITH_EXE
    std::cout << "LIB1_WITH_EXE Open\n";
    #else
    std::cout << "LIB1_WITH_EXE Close\n";
    #endif  /* LIB1_WITH_EXE */
}