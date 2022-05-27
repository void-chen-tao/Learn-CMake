#include <iostream>


void Serialization(void)
{
    using std::cout;

    cout << "serialization.cpp\n";
    #ifdef USING_SERIALIZATION_LIB
    cout << "USING_SERIALIZATION_LIB\n";
    #endif
    cout << "serialization.cpp\n";
}