#include <cppviz/cppviz.hpp>
#include <filesystem>

int main() {
    auto data = cppviz::parse_csv("group,score\nControl,5\nControl,6\nControl,7\nTreatment,6\nTreatment,8\nTreatment,9\n");
    cppviz::Theme theme = cppviz::theme_whitegrid();
    theme.background = cppviz::Color{248, 249, 250};
    theme.grid_color = cppviz::Color{222, 226, 230};
    theme.axis_color = cppviz::Color{33, 37, 41};
    theme.text_color = cppviz::Color{33, 37, 41};
    cppviz::Figure fig(980, 620, theme);
    cppviz::Margins margins; margins.left = 70; margins.right = 30; margins.top = 40; margins.bottom = 70; fig.set_margins(margins);
    cppviz::PointplotOptions opts; opts.color = cppviz::Color{30, 144, 255}; opts.marker_radius = 8; opts.line_thickness = 3; opts.cap_size = 5; cppviz::pointplot(fig.axes(), data, "group", "score", opts);
    cppviz::draw_title(fig.axes(), "Mean Score by Group"); cppviz::draw_xlabel(fig.axes(), "Group"); cppviz::draw_ylabel(fig.axes(), "Score");
    auto output = std::filesystem::path(INTRIVIZ_DATA_DIR) / "output"; std::filesystem::create_directories(output); fig.save((output / "example_pointplot.bmp").string());
    return 0;
}
