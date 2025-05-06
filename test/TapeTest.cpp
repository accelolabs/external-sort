#include <gtest/gtest.h>
#include <fstream>
#include <cstdio>
#include <iostream>
#include "src/FileTape.h"

class TapeTestSuite : public ::testing::Test {
protected:
    std::string tape_filename = "test_tape.bin";
    size_t tape_size = 12;

    void SetUp() override {
        std::ofstream out(tape_filename, std::ios::binary);

        for (int32_t i = 0; i < static_cast<int32_t>(tape_size); ++i) {
            out.write(reinterpret_cast<const char*>(&i), sizeof(int32_t));
        }

        out.close();
    }

    void TearDown() override {
        std::remove(tape_filename.c_str());
    }
};


TEST_F(TapeTestSuite, ReadTest) {
    FileTape tape(tape_filename, tape_size);

    for (size_t i = 0; i < tape_size; ++i) {
        tape.rewind();
        for (size_t j = 0; j < i; ++j) {
            ++tape;
        }
        int32_t value = tape.read();
        EXPECT_EQ(value, static_cast<int32_t>(i));
    }
}


TEST_F(TapeTestSuite, WriteTest) {
    FileTape tape(tape_filename, tape_size);

    for (size_t i = 0; i < tape_size; ++i) {
        tape.rewind();
        for (size_t j = 0; j < i; ++j) {
            ++tape;
        }
        int32_t new_value = static_cast<int32_t>(i * 10);
        tape.write(new_value);
    }

    for (size_t i = 0; i < tape_size; ++i) {
        tape.rewind();
        for (size_t j = 0; j < i; ++j) {
            ++tape;
        }
        int32_t value = tape.read();
        EXPECT_EQ(value, static_cast<int32_t>(i * 10));
    }
}
