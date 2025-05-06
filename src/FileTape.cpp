#include <csignal>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include <thread>
#include "FileTape.h"
#include "FileTapeConfig.h"



FileTape::FileTape(const FileTapeConfig& config) :
    filename(config.filename),
    tape_size(config.tape_size),
    read_delay(config.read_delay),
    write_delay(config.write_delay),
    shift_delay(config.shift_delay)
{
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    file.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering

    if (!file.is_open()) throw std::runtime_error("Tape file" + filename + " does not exist.");
}


FileTape::~FileTape() {
    if (file.is_open()) file.close();
}


int32_t FileTape::operator--() {
    std::this_thread::sleep_for(std::chrono::milliseconds(shift_delay));

    if (current_index == 0) throw std::out_of_range("Decrement out of range.");
    --current_index;
    
    seek_to_index();
    
    return read();
}


int32_t FileTape::operator++() {
    std::this_thread::sleep_for(std::chrono::milliseconds(shift_delay));

    if (current_index >= tape_size) throw std::out_of_range("Increment out of range.");
    ++current_index;
    
    seek_to_index();
    
    return read();
}


size_t FileTape::index() {
    return current_index;
}


size_t FileTape::size() {
    return tape_size;
}


void FileTape::rewind() {
    std::this_thread::sleep_for(std::chrono::milliseconds(current_index * shift_delay));

    current_index = 0;
    seek_to_index();
}


int32_t FileTape::read() {
    std::this_thread::sleep_for(std::chrono::milliseconds(read_delay));

    int32_t value;

    file.read(reinterpret_cast<char*>(&value), sizeof(int32_t));
    seek_to_index();

    if (!file) throw std::runtime_error("Failed to read from tape " + filename + ".");

    return value;
}


void FileTape::write(const int32_t& value) {
    std::this_thread::sleep_for(std::chrono::milliseconds(write_delay));

    file.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));
    seek_to_index();

    if (!file.good()) throw std::runtime_error("Failed to write to tape " + filename + ".");
}


void FileTape::seek_to_index() {
    std::streampos pos = static_cast<std::streampos>(current_index) * sizeof(int32_t);
    file.seekg(pos, std::ios::beg);
    file.seekp(pos, std::ios::beg);
}
