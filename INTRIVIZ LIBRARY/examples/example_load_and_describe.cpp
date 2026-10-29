#include <intriviz/Intriviz.hpp>
#include <filesystem>
#include <iostream>
#include <string>

int main() {
    try {
        const std::filesystem::path data_path =
            std::filesystem::path(INTRIVIZ_DATA_DIR) / "input" / "tips_sample.csv";
        intriviz::Dataset tips = intriviz::load_csv(data_path.string());
        intriviz::Dataset clean = intriviz::drop_missing(tips);
        
        auto summaries = intriviz::describe(clean);
        std::cout << intriviz::to_string(summaries) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error running example: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}