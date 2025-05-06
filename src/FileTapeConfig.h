#pragma once


#include <cstddef>
#include <string>


struct FileTapeConfig {
    std::string filename;
    size_t tape_size;

    size_t read_delay = 0;
    size_t write_delay = 0;
    size_t shift_delay = 0;
};
