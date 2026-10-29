#include "intriviz/Intriviz.hpp"
#include <iostream>
#include <string>
#include <vector>

#ifndef INTRIVIZ_DATA_DIR
#define INTRIVIZ_DATA_DIR "data"
#endif

int main() {
    using namespace intriviz;

    std::cout << "Running example_themes: Rendering plots across all 4 theme presets...\n";

    // 1. Construct a sample dataset
    Dataset ds;
    ds.add_text_column("day", {"Thu", "Fri", "Sat", "Sun", "Thu", "Fri", "Sat", "Sun"});
    ds.add_numeric_column("total_bill", {16.99, 10.34, 21.01, 23.68, 24.59, 25.29, 8.77, 26.88});

    struct ThemeConfig {
        std::string name;
        Theme theme;
        std::string filename;
    };

    std::vector<ThemeConfig> configs = {
        {"theme_white", theme_white(), "theme_white.bmp"},
        {"theme_whitegrid", theme_whitegrid(), "theme_whitegrid.bmp"},
        {"theme_darkgrid", theme_darkgrid(), "theme_darkgrid.bmp"},
        {"theme_dark", theme_dark(), "theme_dark.bmp"}
    };

    std::string out_dir = std::string(INTRIVIZ_DATA_DIR) + "/output/";

    for (const auto& cfg : configs) {
        // Create Figure with specified theme
        Figure fig(640, 480, cfg.theme);
        
        // Render plot and annotations
        swarmplot(fig.axes(), ds, "day", "total_bill");
        draw_title(fig.axes(), "Bill by Day (" + cfg.name + ")");
        draw_xlabel(fig.axes(), "Day of Week");
        draw_ylabel(fig.axes(), "Total Bill ($)");

        std::string out_path = out_dir + cfg.filename;
        fig.save(out_path);
        std::cout << "Saved: " << out_path << "\n";
    }

    std::cout << "Theme showcase completed successfully!\n";
    return 0;
}