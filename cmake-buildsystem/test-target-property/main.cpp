#include <iostream>

extern void Show(void);

int main(void)
{
    Show();

    std::cout << "Main:\n";
    #ifdef USE_FILE1
    std::cout << "USE_FILE1\n";
    #endif
    #ifdef USE_FILE2
    std::cout << "USE_FILE2\n";
    #endif
    
    return 0;
}