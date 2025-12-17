#include <iostream>
#include <string>

using namespace std;

int main()
{
    cout << "C++ CORE READY\n";

    string input;
    
    if (std::getline(std::cin, input)) {
        std::cout << "RECEIVED: " << input << std::endl;
    }

    return 0;
}