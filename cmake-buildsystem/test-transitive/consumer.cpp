#include <iostream>

extern void Archive(void);
extern void Extras(void);
extern void Serialization(void);


int main (void)
{
    Archive();
    std::cout << "-----------\n";
    Extras();
    std::cout << "-----------\n";
    Serialization();
    std::cout << "-----------\n";
    #ifdef USING_ARCHIVE_LIB
    std::cout << "USING_ARCHIVE_LIB\n";
    #endif
    #ifdef USING_SERIALIZATION_LIB
    std::cout << "USING_SERIALIZATION_LIB\n";
    #endif
    
    return 0;
}