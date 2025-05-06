#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include "../lib/ITape.h"


class FileTape : ITape<int32_t> {

private:

    mutable std::fstream file;
    size_t size;
    size_t current_index = 0;

public:

    explicit FileTape(const std::string& filename, const size_t& tape_size);

    ~FileTape();


    FileTape(const FileTape&) = delete;
    FileTape& operator=(const FileTape&) = delete;

    FileTape(FileTape&&) = delete;
    FileTape& operator=(FileTape&&) = delete;


    int32_t operator--() override;
    int32_t operator++() override;

    int32_t read() override;
    void write(const int32_t& value) override;

private:

    void seek_to_index();

};
