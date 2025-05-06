#pragma once

#include <cstddef>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <random>
#include <string>
#include <unordered_map>


inline size_t text_to_tape(const std::string& text_filename, const std::string& tape_filename) {
    std::remove(tape_filename.c_str());

    std::ifstream in(text_filename);
    if (!in.is_open()) {
        throw std::runtime_error("Failed to open input text file: " + text_filename);
    }

    std::ofstream out(tape_filename, std::ios::out | std::ios::binary);
    if (!out.is_open()) throw std::runtime_error("Failed to open tape file: " + tape_filename);


    out.rdbuf() -> pubsetbuf(nullptr, 0); // Disable buffering

    int32_t v;

    size_t count = 0;
    while (in >> v) {
        out.write(reinterpret_cast<const char*>(&v), sizeof(int32_t));
        if (!out.good()) throw std::runtime_error("Failed while writing value to tape file.");
        count++;
    }

    in.close();
    out.close();
    return count;
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


inline void generate_random_numbers_file(
    const std::string& filename,
    size_t n,
    int32_t min = 1,
    int32_t max = 99999999
) {
    std::ofstream output_file(filename);
    if (!output_file) throw std::runtime_error("Failed to open file: " + filename);

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int32_t> dist(min, max);

    for (size_t i = 0; i < n; ++i) {
        output_file << dist(gen);
        if (i != n - 1) output_file << "\n";
    }
}


inline size_t generate_zero_file(
    const std::string& filename,
    size_t n
) {
    std::remove(filename.c_str());

    std::ofstream out(filename, std::ios::out | std::ios::binary);
    if (!out.is_open()) {
        throw std::runtime_error("Failed to open file: " + filename);
    }

    out.rdbuf()->pubsetbuf(nullptr, 0); // Disable buffering

    const int32_t zero = 0;
    size_t count = 0;

    for (size_t i = 0; i < n; ++i) {
        out.write(reinterpret_cast<const char*>(&zero), sizeof(int32_t));
        if (!out.good()) {
            throw std::runtime_error("Failed while writing to file: " + filename);
        }
        count++;
    }

    out.close();
    return count;
}


inline bool parse_config(const std::string& path, std::unordered_map<std::string, size_t>& config) {
    std::ifstream cfg(path);
    
    if (!cfg.is_open()) {
        std::cerr << "Failed to open config file: " << path << "\n";
        return false;
    }
    
    std::string line;
    while (std::getline(cfg, line)) {
        if (line.empty() || line[0] == '#') continue;
        size_t eq = line.find('=');
    
        if (eq == std::string::npos) {
            std::cerr << "Invalid config line (no '='): " << line << "\n";
            return false;
        }
    
        std::string key = line.substr(0, eq);
        std::string val = line.substr(eq + 1);
    
        try {
            size_t num = std::stoul(val);
            config[key] = num;
        } catch (...) {
            std::cerr << "Invalid number for key " << key << ": " << val << "\n";
            return false;
        }
    }
    
    return true;
}

