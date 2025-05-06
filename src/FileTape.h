#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include "../lib/ITape.h"
#include "FileTapeConfig.h"


class FileTape : ITape<int32_t> {

private:

    mutable std::fstream file;
    std::string filename;
    size_t tape_size;
    size_t current_index = 0;

    size_t read_delay = 0;
    size_t write_delay = 0;
    size_t rewind_delay = 0;
    size_t shift_delay = 0;

public:

    explicit FileTape(const FileTapeConfig& config);

    ~FileTape();


    FileTape(const FileTape&) = delete;
    FileTape& operator=(const FileTape&) = delete;

    FileTape(FileTape&&) = default;
    FileTape& operator=(FileTape&&) = default;


    int32_t operator--() override;
    int32_t operator++() override;
    size_t index() override;
    size_t size() override;
    void rewind() override;

    int32_t read() override;
    void write(const int32_t& value) override;

private:

    void seek_to_index();

};
