#include <cppviz/cppviz.hpp>
#include <iostream>
#include <string>

int main() {
    using namespace cppviz;

    std::string data_path = std::string(CPPVIZ_DATA_DIR) + "/input/scores_sample.csv";

    try {
        Dataset data = load_csv(data_path);

        // 1. Instantiate Figure and retrieve Axes reference
        Theme theme = theme_whitegrid();
        Figure fig(640, 480, theme);
        Axes& ax = fig.axes();

        // 2. Render point plot on Axes
        PointplotOptions opts;
        opts.color = Color{76, 114, 176}; // Seaborn #4C72B0
        opts.show_ci = true;
        pointplot(ax, data, "group", "score", opts);

        // 3. Set plot titles and axis labels
        draw_title(ax, "Annotated Student Performance Analysis");
        draw_xlabel(ax, "Student Group");
        draw_ylabel(ax, "Exam Score");

        // 4. Add target benchmark line at score = 85.0
        annotate_hline(ax, 85.0, Color{219, 68, 85}, 2);
        annotate_text(ax, 0.1, 85.5, "Target Benchmark (85.0)");

        // 5. Add arrow pointing to Group C peak
        annotate_arrow(ax, 1.5, 96.0, 2.0, 92.5, "Highest Group Mean");

        // 6. Export rendered canvas
        std::string out_path = "data/output/annotations_example.bmp";
        fig.save(out_path);

        std::cout << "Successfully generated annotations example: " << out_path << std::endl;
    } catch (const VizError& e) {
        std::cerr << "Error running annotations example: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}