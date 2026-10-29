#include <intriviz/Intriviz.hpp>
#include <iostream>
#include <string>

int main() {
    try {
        // Resolve data path via INTRIVIZ_DATA_DIR CMake macro
        std::string input_path = std::string(INTRIVIZ_DATA_DIR) + "/input/tips_sample.csv";
        std::string output_path = "data/output/example_swarmplot.bmp";

        std::cout << "Loading dataset from: " << input_path << "\n";

        // Load CSV and clean rows containing missing values
        intriviz::Dataset raw_data = intriviz::load_csv(input_path);
        intriviz::Dataset clean_data = intriviz::drop_missing(raw_data);

        std::cout << "Loaded " << raw_data.row_count() << " rows (" 
                  << clean_data.row_count() << " rows after dropping missing data).\n";

        // Generate swarm plot figure via high-level quick API
        intriviz::Figure fig = intriviz::quick_swarmplot(
            clean_data, 
            "day", 
            "total_bill", 
            "Total Bill by Day ($)"
        );

        // Save output BMP image
        fig.save(output_path);
        std::cout << "Swarm plot successfully saved to: " << output_path << "\n";

    } catch (const intriviz::VizError& e) {
        std::cerr << "intriviz Error: " << e.what() << "\n";
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Unexpected Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
