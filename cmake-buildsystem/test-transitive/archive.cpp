#include <iostream>


void Archive(void)
{
    using std::cout;

    cout << "archive.cpp\n";
    #ifdef USING_ARCHIVE_LIB
    cout << "USING_ARCHIVE_LIB\n"; 
    #endif
    cout << "archive.cpp\n";
}