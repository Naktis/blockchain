#pragma once

#include <iostream>
#include <sstream>
#include <cmath>
#include <iomanip>

struct int256 {
    uint16_t bits[16];
};

std::string hash(std::string input);