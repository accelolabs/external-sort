#include <cstddef>
#include <cstdint>
#include <filesystem>
#include <iostream>
#include <unordered_map>
#include <vector>

#include "FileTape.h"
#include "Helpers.h"
#include "TapeSort.h"


int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0] << " <input_file> <output_file>\n";
        return 1;
    }

    const std::string input_file = argv[1];
    const std::string output_file = argv[2];
    const std::string config_file = "tape.config";


    std::unordered_map<std::string, size_t> cfg;
    if (!parse_config(config_file, cfg)) return 1;

    // Expected keys
    const std::vector<std::string> requiredKeys = {
        "read_delay",
        "write_delay",
        "shift_delay",
        "memory_limit"
    };
    
    for (const auto& key : requiredKeys) {
        if (cfg.find(key) == cfg.end()) {
            std::cerr << "Missing config parameter: " << key << "\n";
            return 1;
        }
    }

    size_t read_delay = cfg["read_delay"];
    size_t write_delay = cfg["write_delay"];
    size_t shift_delay = cfg["shift_delay"];
    size_t memory_limit = cfg["memory_limit"];

    std::filesystem::path tmp_dir = "tmp";
    if (!std::filesystem::exists(tmp_dir)) std::filesystem::create_directory(tmp_dir);

    // Create input and output tapes
    std::string tmp_input_file = (tmp_dir / "input_file").string() + ".tmp";
    std::string tmp_output_file = (tmp_dir / "output_file").string() + ".tmp";

    size_t element_count = text_to_tape(input_file, tmp_input_file);
    generate_zero_file(tmp_output_file, element_count);

    FileTape input = FileTape({
        tmp_input_file,
        element_count,
        read_delay,
        write_delay,
        shift_delay
    });

    FileTape output = FileTape({
        tmp_output_file,
        element_count,
        read_delay,
        write_delay,
        shift_delay
    });

    size_t n_m = memory_limit / sizeof(int32_t);
    size_t k = element_count / n_m;
    std::vector<FileTape> temp_tapes;

    std::cout << n_m << " " << k << "\n";

    for (size_t i = 0; i < k; ++i) {
        std::string temp_tape_filename = (tmp_dir / std::to_string(i)).string() + ".tmp";
        generate_zero_file(temp_tape_filename, n_m);
        temp_tapes.emplace_back(
            FileTape({
                temp_tape_filename,
                n_m,
                read_delay,
                write_delay,
                shift_delay
            })
        );
    }

    TapeSort::sort_tapes(input, output, memory_limit, temp_tapes);

    tape_to_text(tmp_output_file, output_file);
}
