/**
#include <iostream>

int main(int argc, char* argv[]) {
    // argc: The number of arguments (including the program name itself)
    // argv: An array of C-style strings containing the arguments
    
    std::cout << "Program name: " << argv[0] << std::endl;
    
    if (argc > 1) {
        std::cout << "First argument: " << argv[1] << std::endl;
    }
    
    return 0;
}
**/