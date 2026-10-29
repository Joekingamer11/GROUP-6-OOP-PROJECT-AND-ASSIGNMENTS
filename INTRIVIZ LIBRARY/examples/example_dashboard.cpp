#include <cppviz/cppviz.hpp>
#include <iostream>
#include <string>

int main() {
    using namespace cppviz;

    std::cout << "========================================" << std::endl;
    std::cout << "  Cppviz Member 9 - Dashboard Example   " << std::endl;
    std::cout << "========================================" << std::endl;

    std::string data_dir = CPPVIZ_DATA_DIR;
    std::string input_path = data_dir + "/input/tips_sample.csv";

    Dataset tips;
    try {
        tips = load_csv(input_path);
        std::cout << "Successfully loaded dataset: " << input_path << std::endl;
    } catch (const VizError& e) {
        std::cout << "Notice: Could not load " << input_path << " (" << e.what() 
                  << "). Using inline sample data." << std::endl;
        tips.add_text_column("day", {"Thu", "Thu", "Fri", "Fri", "Sat", "Sat", "Sun", "Sun"});
        tips.add_numeric_column("total_bill", {16.99, 10.34, 21.01, 13.81, 20.29, 15.01, 24.59, 25.29});
        tips.add_numeric_column("tip", {1.01, 1.66, 3.50, 3.31, 3.16, 2.00, 3.61, 4.71});
    }

    // 1. Generate Swarm Plot
    Figure fig_swarm = quick_swarmplot(tips, "day", "total_bill", "Total Bill Distribution by Day");
    std::string path_swarm = data_dir + "/output/dashboard_swarm.bmp";
    fig_swarm.save(path_swarm);
    std::cout << "[1/3] Saved Swarm Plot: " << path_swarm << std::endl;

    // 2. Generate Point Plot
    Figure fig_point = quick_pointplot(tips, "day", "total_bill", "Mean Total Bill by Day (95% CI)");
    std::string path_point = data_dir + "/output/dashboard_point.bmp";
    fig_point.save(path_point);
    std::cout << "[2/3] Saved Point Plot: " << path_point << std::endl;

    // 3. Generate Correlation Heatmap
    Figure fig_heatmap = quick_heatmap(tips, {"total_bill", "tip"}, "Bill vs Tip Correlation Matrix");
    std::string path_heatmap = data_dir + "/output/dashboard_heatmap.bmp";
    fig_heatmap.save(path_heatmap);
    std::cout << "[3/3] Saved Heatmap: " << path_heatmap << std::endl;

    std::cout << "Dashboard complete! All 3 images saved to data/output/\n";
    return 0;
}