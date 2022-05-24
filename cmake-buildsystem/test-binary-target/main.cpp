#include <iostream>

extern void Showlib1(void);
extern void Showlib2(void);

int main()
{
    std::cout << "I'm main function\n";
    std::cout << "-------------------" << std::endl;
    Showlib1();
    std::cout << "-------------------" << std::endl;
    Showlib2();
}