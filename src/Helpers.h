#pragma once

#include <cstdint>
#include <fstream>
#include <random>
#include <string>


inline void text_to_tape(const std::string& text_filename, const std::string& tape_filename) {
    std::remove(tape_filename.c_str());

    std::ifstream in(text_filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input text file: " + text_filename);
    }

    std::ofstream out(tape_filename, std::ios::out | std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("Failed to open tape file: " + tape_filename);


    out.rdbuf() -> pubsetbuf(nullptr, 0); // Disable buffering

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


inline void generate_random_numbers_file(
    const std::string& filename,
    size_t n,
    int32_t min = 1,
    int32_t max = 99999999
) {
    std::ofstream outfile(filename);
    if (!outfile) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dist(min, max);

    for (size_t i = 0; i < n; ++i) {
        outfile << dist(gen);
        if (i != n - 1) outfile << "\n";
    }
}


inline void tape_to_text(const std::string& tape_filename, const std::string& text_filename) {
    std::remove(text_filename.c_str());

    std::ifstream in(tape_filename, std::ios::in | std::ios::binary);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input tape file: " + tape_filename);
    }

    std::ofstream out(text_filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open output text file: " + text_filename);
    }

    in.rdbuf() -> pubsetbuf(nullptr, 0); // Disable buffering

    int32_t v;
    bool first = true;
    while (in.read(reinterpret_cast<char*>(&v), sizeof(int32_t))) {
        if (!first) {
            out << "\n";
        }
        out << v;
        first = false;
    }

    if (!in.eof() && in.fail()) {
        throw std::runtime_error("Failed while reading value from tape file.");
    }

    in.close();
    out.close();
}


inline void vector_to_text(const std::vector<int32_t>& vector, const std::string& text_filename) {
    std::ofstream out(text_filename);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open output text file: " + text_filename);
    }

    out.rdbuf() -> pubsetbuf(nullptr, 0); // Disable buffering

    bool first = true;
    for (const int32_t& v : vector) {
        if (!first) {
            out << "\n";
        }
        out << v;
        first = false;
    }

    if (!out.good()) {
        throw std::runtime_error("Failed while writing to text file.");
    }

    out.close();
}


inline std::vector<int32_t> text_to_vector(const std::string& text_filename) {
    std::ifstream in(text_filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input text file: " + text_filename);
    }

    in.rdbuf() -> pubsetbuf(nullptr, 0); // Disable buffering

    std::vector<int32_t> result;
    int v;
    
    while (in >> v) {
        result.push_back(v);

        if (in.fail() && !in.eof()) {
            throw std::runtime_error("Failed while reading value from text file.");
        }
    }

    if (!in.eof() && in.fail()) {
        throw std::runtime_error("Invalid data format in text file.");
    }

    in.close();
    return result;
}
