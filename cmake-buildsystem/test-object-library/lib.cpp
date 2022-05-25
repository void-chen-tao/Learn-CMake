#include <iostream>

extern void Display1(void);
extern void Display2(void);


void Display(void)
{
    using std::cout;
    using std::endl;

    cout << "I am library, I include Display1() and Display2()" <<endl;
    cout << "I am in lib, but ";
    Display1();
    cout << "I am in lib, but ";
    Display2();
}