#include <intriviz/Intriviz.hpp>

#include <filesystem>
#include <iostream>
#include <memory>
#include <string>
#include <vector>

int main() {
    try {
        std::cout << "=== INTRIVIZ WEEK 3 INTEGRATION SHOWCASE ===" << std::endl;

        std::string raw_csv =
            "day,total_bill,tip\n"
            "Thu,16.99,1.01\n"
            "Thu,10.34,1.66\n"
            "Thu,NA,3.50\n"
            "Fri,21.01,3.00\n"
            "Fri,12.69,1.50\n"
            "Fri,13.81,2.00\n"
            "Sat,20.65,3.35\n"
            "Sat,17.92,4.08\n"
            "Sat,20.29,2.75\n"
            "Sat,15.77,2.23\n";

        intriviz::Dataset raw_ds = intriviz::parse_csv(raw_csv);
        intriviz::Dataset ds = intriviz::drop_missing(raw_ds);

        auto summary = intriviz::describe(ds);
        std::cout << "\nDataset Summary:\n" << intriviz::to_string(summary) << std::endl;

        const auto& tips = ds.numeric("tip");
        const auto& totals = ds.numeric("total_bill");

        double mean_tip = intriviz::mean(tips);
        double std_tip = intriviz::stddev(tips);
        intriviz::Interval ci = intriviz::bootstrap_ci(tips, 0.95, 1000, 42);

        std::cout << "Tip Mean: $" << mean_tip << " (95% CI: ["
                  << ci.low << ", " << ci.high << "])\n"
                  << "Tip Std Dev: $" << std_tip << std::endl;

        intriviz::Theme theme = intriviz::theme_whitegrid();
        intriviz::Figure fig(800, 500, theme);
        intriviz::Axes& ax = fig.axes();

        std::vector<std::string> categories = {"Thu", "Fri", "Sat"};
        auto x_range = ax.x_pixel_range();
        auto y_range = ax.y_pixel_range();

        auto x_scale = std::make_shared<intriviz::CategoricalScale>(categories, x_range.first, x_range.second);
        double y_max = intriviz::max_value(totals) + 5.0;
        auto y_scale = std::make_shared<intriviz::LinearScale>(0.0, y_max, y_range.first, y_range.second);

        ax.set_x_scale(x_scale);
        ax.set_y_scale(y_scale);
        ax.draw_frame();

        intriviz::PointplotOptions point_opts;
        point_opts.color = intriviz::Color{31, 119, 180};
        point_opts.show_ci = true;
        point_opts.ci_level = 0.95;
        point_opts.connect_points = true;
        point_opts.marker_radius = 5;
        pointplot(ax, ds, "day", "total_bill", point_opts);

        intriviz::SwarmplotOptions swarm_opts;
        swarm_opts.marker_radius = 4;
        swarm_opts.color_by_category = true;
        swarm_opts.color = intriviz::Color{255, 127, 14};
        swarmplot(ax, ds, "day", "total_bill", swarm_opts);

        intriviz::draw_title(ax, "Total Bill Distribution by Day");
        intriviz::draw_xlabel(ax, "Day of Week");
        intriviz::draw_ylabel(ax, "Total Bill ($)");

        std::vector<std::string> legend_names = {"Category Mean (95% CI)", "Individual Observations"};
        std::vector<intriviz::Color> legend_colors = {intriviz::Color{31, 119, 180}, intriviz::Color{255, 127, 14}};
        intriviz::draw_legend(ax, legend_names, legend_colors, "Legend");

        std::filesystem::path output_dir = "data/output";
        std::filesystem::create_directories(output_dir);
        std::filesystem::path output_path = output_dir / "week3_showcase.bmp";

        fig.save(output_path.string());
        std::cout << "Successfully generated image: " << output_path << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Error running showcase: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}