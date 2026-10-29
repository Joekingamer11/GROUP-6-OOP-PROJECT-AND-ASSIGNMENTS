#include <cppviz/cppviz.hpp>
#include <filesystem>

int main() {
    const std::vector<std::vector<double>> matrix = {{1.0, 0.8, 0.3}, {0.8, 1.0, 0.5}, {0.3, 0.5, 1.0}};
    const std::vector<std::string> labels = {"Math", "Science", "History"};
    cppviz::Theme theme = cppviz::theme_whitegrid();
    theme.background = cppviz::Color{248, 249, 250};
    theme.grid_color = cppviz::Color{222, 226, 230};
    theme.axis_color = cppviz::Color{33, 37, 41};
    theme.text_color = cppviz::Color{33, 37, 41};
    cppviz::Figure fig(1080, 760, theme); cppviz::Margins margins; margins.left = 90; margins.right = 120; margins.top = 55; margins.bottom = 80; fig.set_margins(margins);
    cppviz::HeatmapOptions opts; opts.annotate = true; opts.colormap = "magma"; cppviz::heatmap(fig.axes(), matrix, labels, labels, opts);
    cppviz::draw_title(fig.axes(), "Subject Correlation Heatmap");
    auto output = std::filesystem::path(INTRIVIZ_DATA_DIR) / "output"; std::filesystem::create_directories(output); fig.save((output / "example_heatmap.bmp").string());
    return 0;
}
