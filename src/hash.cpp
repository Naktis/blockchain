#include "../headers/hash.hpp"

std::string hash(std::string input) {
    int256 h;

    // create a base
    for (int i = 0; i < 16; i++) {
        h.bits[i] = 4096 + (i + 1) * pow(2, input.length() % 10) * 9719;
    }

    // read every symbol from input and modify the hash
    for (int i = 0; i < input.length(); i++) {
        h.bits[0] = h.bits[0] * 7213 + 1327 * pow(input[i] % 10, 3) + input[i];
        for (int j = 1; j < 16; j++) {
            h.bits[j] = h.bits[j] * h.bits[j - 1] + input[i];
        }
    }

    // append all hash values to one string
    std::ostringstream hashStream;
    for (int j = 0; j < 16; j++) {
        char fill = '0' + h.bits[j] % 10; // get the last digit as char
        hashStream << std::setfill(fill) << std::setw(4) << std::hex << h.bits[j];
    }

    return hashStream.str();
}