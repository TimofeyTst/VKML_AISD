#include <iostream>
#include <sstream>
#include <cassert>

//1.4 Инвертируйте значение бита в числе N по его номеру K.
//Формат входных данных.Число N, номер бита K.
//Формат выходных данных.Число с инвертированным битом в десятичном виде.

uint32_t invertBit(const uint32_t& num, const uint32_t& k) {
    // Сдвинем единицу на k символов, останется все другие оставить не тронутыми, а на текущем 
    // в случае 0 получить 1, в случае 1 получить 0
    // xor при сдвиге единицы оставит нетронутыми все биты кроме нужного, а его изменит
    return num ^ (1 << k);
}


void run(std::istream& input, std::ostream& output) {
    uint32_t num, k, result;
    input >> num >> k;

    result = invertBit(num, k);

    output << result;
}

void testInvertBit() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "25 1";
        run(input, output);
        assert(output.str() == "27");
    }
    {
        std::stringstream input, output;
        input << "25 4";
        run(input, output);
        assert(output.str() == "9");
    }
}

int main() {
    run(std::cin, std::cout);
    //testInvertBit();
    return 0;
}