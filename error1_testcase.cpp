//Test cases error 1

//case statements if not reg, add, sub, mul, comp, mux 2x1, shr, shl then default error
#include <iostream>
#include <fstream>
#include <stdexcept>

int main() {

    std::ifstream inputFile("error1.txt");
    std::ofstream outputFile("output.txt");

    if(!inputFile.is_open()) {
        std::cerr << "Unable to open input file." << std::endl;
        return 1;
    }

    if(!outputFile.is_open()) {
        std::cerr << "Unable to open output file." << std::endl;
        return 1;
    }

while (inputFile >> i) {
    switch(i) {
        case reg:

            break;

        case add:

            break;

        case sub:

            break;

        case mul:

            break;

        case comp:

            break;

        case mux:

            break;

        case shr:

            break;

        case shl:

            break;

        default:
            outputFile << "ERROR" << std::endl;
            break;
    }
}
    inputFile.close();
    outputFile.close();

    return 0;
}