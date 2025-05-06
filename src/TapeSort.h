#pragma once


#include <algorithm>
#include <csignal>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <stdexcept>
#include <vector>
#include "FileTape.h"


class TapeSort {

public:

    static void sort_tapes(
        FileTape& in,
        FileTape& out,
        size_t memory_bytes,
        std::vector<FileTape>& tapes
    ) {

        size_t k = tapes.size();
        size_t n_m = memory_bytes / sizeof(int32_t);
        size_t r = in.size() % n_m;
        size_t m = (n_m - r) / k;
        size_t kmr = k * m + r;

        if (m == 0) throw std::runtime_error("Low memory error.");
        if (in.size() / n_m != k) throw std::invalid_argument("Invalid k or size of array.");
        if (in.size() != out.size()) throw std::invalid_argument("Invalid sizes of input and output.");

        for (FileTape& tape : tapes) {
            if (tape.size() != n_m) throw std::invalid_argument("Invalid sizes of temporary tapes.");
        }

        int32_t* buffer = new int32_t[n_m];

        // Sort data into temporary tapes
        for (FileTape& tape : tapes) {
            fill_buffer(in, buffer, n_m);
            std::sort(buffer, buffer + n_m);

            for (size_t i = 0; i < n_m; ++i) {
                tape.write(buffer[i]);
                if (tape.index() < tape.size() - 1) ++tape;
            }

            tape.rewind();
        }

        // Read from temporary tapes into k blocks of size m
        size_t offset = 0;

        for (FileTape& tape : tapes) {
            offset += fill_buffer(tape, buffer + offset, m);
        }

        // Unwind remainder into buffer
        offset += fill_buffer(in, buffer + offset, r);

        // Sort and flush first k, fill from start
        for (size_t i = 0; i < n_m; ++i) {
            std::sort(buffer, buffer + kmr);

            for (size_t i = 0; i < k; ++i) {
                out.write(buffer[i]);
                if (out.index() < out.size() - 1) ++out;
            }

            offset = 0;
            for (FileTape& tape : tapes) {
                // Rely on the fact that all tapes are n_m length
                offset += fill_buffer(tape, buffer + offset, 1);
            }
        }

        // Flush all else when temporary tapes are empty
        for (size_t i = 0; i < kmr; ++i) {
            out.write(buffer[i]);
            if (out.index() < out.size() - 1) ++out;
        }
    
        delete[] buffer; // CAN and WILL leak
    };


private:

    static size_t fill_buffer(FileTape& in, int32_t* buffer, size_t cap) {
        size_t count = 0;

        while (count < cap) {
            buffer[count++] = in.read();
            if (in.index() < in.size() - 1) ++in;
        }

        return count;
    }

    static void print_buffer(int32_t* buffer, size_t size) {
        for (size_t i = 0; i < size; ++i) {
            std::cout << buffer[i];
            
            // Add separator (except after last element)
            if (i < size - 1) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;  // Newline at the end
    }

};
