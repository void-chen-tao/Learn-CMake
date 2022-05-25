#include <iostream>


extern void Display(void);
extern void Display1(void);
extern void Display2(void);


int main()
{
    std::cout << "main: \n";
    std::cout << "========= Display() ==========\n";
    Display();
    std::cout << "========= Display1() ==========\n";
    Display1();
    std::cout << "========= Display2() ===========\n";
    Display2();

    return 0;
}