#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cassert>
#include <climits>
#include <queue>

#define assertm(exp, msg) assert(((void)msg, exp))

void task(std::istream& input_stream = std::cin,
            std::ostream& output_stream = std::cout) {
    int i = 0;
}                

int main() {
    std::ifstream input_file;
    std::ofstream output_file;
    input_file.open("input.txt");
    output_file.open("output.txt");
    task(input_file, output_file);
    input_file.close();
    output_file.close();

    return 0;
}