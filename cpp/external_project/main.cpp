#include <core/core/set_manager.h>
#include <iostream>

int main()
{
    SetManager manager{};
    std::cout << "Hello World! : " << manager.getTrue() << std::endl;
    return 0;
}
