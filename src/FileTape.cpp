#include <csignal>
#include <cstddef>
#include <cstdint>
#include <fstream>
#include <string>
#include "FileTape.h"



FileTape::FileTape(const std::string& filename, const size_t& tape_size) : tape_size(tape_size) {
    file.open(filename, std::ios::in | std::ios::out | std::ios::binary);
    file.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering

    if (!file.is_open()) throw std::runtime_error("Tape file does not exist.");
}


FileTape::~FileTape() {
    if (file.is_open()) file.close();
}


int32_t FileTape::operator--() {
    if (current_index == 0) throw std::out_of_range("Decrement out of range.");
    --current_index;
    
    seek_to_index();
    
    return read();
}


int32_t FileTape::operator++() {
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
    current_index = 0;
    seek_to_index();
}


int32_t FileTape::read() {
    int32_t value;

    file.read(reinterpret_cast<char*>(&value), sizeof(int32_t));

    if (!file) throw std::runtime_error("Failed to read from tape.");

    return value;
}


void FileTape::write(const int32_t& value) {
    file.write(reinterpret_cast<const char*>(&value), sizeof(int32_t));

    if (!file.good()) throw std::runtime_error("Failed to write to tape.");
}


void FileTape::seek_to_index() {
    std::streampos pos = static_cast<std::streampos>(current_index) * sizeof(int32_t);
    file.seekg(pos, std::ios::beg);
    file.seekp(pos, std::ios::beg);
}
