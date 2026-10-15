#include <cppviz/canvas.hpp>
#include <cppviz/colormap.hpp>
#include <cppviz/csv_loader.hpp>
#include <cppviz/image_export.hpp>
#include <cppviz/scale.hpp>
#include <cppviz/stats.hpp>

#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>

#ifndef INTRIVIZ_DATA_DIR
#define INTRIVIZ_DATA_DIR "data"
#endif

int main() {
    try {
        const std::string csv_data =
            "val\n"
            "10.0\n"
            "25.0\n"
            "50.0\n"
            "75.0\n"
            "90.0\n";
        const cppviz::Dataset dataset = cppviz::parse_csv(csv_data);
        const auto& values = dataset.numeric("val");

        const double min_value = cppviz::min_value(values);
        const double max_value = cppviz::max_value(values);
        std::cout << "Data mean: " << cppviz::mean(values)
                  << " | StdDev: " << cppviz::stddev(values) << '\n';

        constexpr int width = 500;
        constexpr int height = 200;
        constexpr int center_y = height / 2;
        constexpr int marker_radius = 5;

        cppviz::Canvas canvas(width, height, cppviz::Color{245, 245, 245});
        const cppviz::LinearScale scale(min_value, max_value, 50.0, 450.0);
        const cppviz::Colormap colors({
            cppviz::Color{30, 100, 220},
            cppviz::Color{220, 30, 30}
        });

        for (double value : values) {
            const int x_center = static_cast<int>(scale.map(value));
            const double normalized = (value - min_value) / (max_value - min_value);
            const cppviz::Color marker_color = colors.at(normalized);

            for (int dy = -marker_radius; dy <= marker_radius; ++dy) {
                for (int dx = -marker_radius; dx <= marker_radius; ++dx) {
                    canvas.set_pixel(x_center + dx, center_y + dy, marker_color);
                }
            }

            if (canvas.get_pixel(x_center, center_y) != marker_color) {
                throw std::runtime_error("Rendered data point check failed");
            }
        }

        const std::filesystem::path output_path =
            std::filesystem::path(INTRIVIZ_DATA_DIR) /
            "output" / "week1_showcase.bmp";
        std::filesystem::create_directories(output_path.parent_path());
        cppviz::save_image(canvas, output_path.string());

        std::ifstream image(output_path, std::ios::binary);
        char signature[2]{};
        image.read(signature, sizeof(signature));
        const std::uintmax_t row_stride =
            (static_cast<std::uintmax_t>(width) * 3 + 3) & ~std::uintmax_t{3};
        const std::uintmax_t expected_size =
            54 + row_stride * static_cast<std::uintmax_t>(height);
        if (!image || signature[0] != 'B' || signature[1] != 'M' ||
            std::filesystem::file_size(output_path) != expected_size) {
            throw std::runtime_error("Generated BMP verification failed");
        }

        std::cout << "CSV visualization checks passed. Generated: "
                  << output_path.string() << '\n';
    } catch (const std::exception& error) {
        std::cerr << "Example failed: " << error.what() << '\n';
        return 1;
    }

    return 0;
}
