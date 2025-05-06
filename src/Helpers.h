#pragma once

#include <fstream>
#include <string>


inline void text_to_tape(const std::string& text_filename, const std::string& tape_filename) {
    std::remove(tape_filename.c_str());

    // Open input text file
    std::ifstream in(text_filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input text file: " + text_filename);
    }

    // Open tape file in binary mode
    std::ofstream out(tape_filename, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open tape file: " + tape_filename);
    }

    out.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering

    // Read integers and write them as binary
    int32_t v;
    while (in >> v) {
        out.write(reinterpret_cast<const char*>(&v), sizeof(int32_t));
        if (!out.good()) {
            throw std::runtime_error("Failed while writing value to tape file.");
        }
    }

    in.close();
    out.close();
}
