#include <intriviz/Intriviz.hpp>
#include <iostream>
#include <string>

int main() {
    using namespace intriviz;

    // Use CMake path macro to portably locate sample dataset
    std::string data_path = std::string(INTRIVIZ_DATA_DIR) + "/input/scores_sample.csv";

    try {
        // 1. Load sample CSV into Dataset
        Dataset data = load_csv(data_path);

        // 2. Generate point plot using quick interface
        Figure fig = quick_pointplot(data, "group", "score", "Student Test Scores by Group (Mean & 95% CI)");

        // 3. Save plot to BMP image file
        std::string out_path = "data/output/pointplot_scores.bmp";
        fig.save(out_path);

        std::cout << "Successfully generated point plot example: " << out_path << std::endl;
    } catch (const VizError& e) {
        std::cerr << "Error running pointplot example: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
