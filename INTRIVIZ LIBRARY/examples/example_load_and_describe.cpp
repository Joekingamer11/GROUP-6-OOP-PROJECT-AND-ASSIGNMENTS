#include <cppviz/cppviz.hpp>
#include <iostream>
#include <string>

int main() {
    try {
        std::string data_path = std::string(CPPVIZ_DATA_DIR) + "/input/tips_sample.csv";
        cppviz::Dataset tips = cppviz::load_csv(data_path);
        cppviz::Dataset clean = cppviz::drop_missing(tips);
        
        auto summaries = cppviz::describe(clean);
        std::cout << cppviz::to_string(summaries) << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error running example: " << e.what() << std::endl;
        return 1;
    }
    return 0;
}