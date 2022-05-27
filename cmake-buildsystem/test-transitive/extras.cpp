#include <iostream>


void Extras(void)
{
    using std::cout;

    cout << "extras.cpp\n";
    #ifdef USING_ARCHIVE_LIB
    cout << "extras.cpp USING_ARCHIVE_LIB\n";
    #endif
    #ifdef USING_SERIALIZATION_LIB
    cout << "extras.cpp USING_SERIALIZATION_LIB\n";
    #endif
    cout << "extras.cpp\n";
}