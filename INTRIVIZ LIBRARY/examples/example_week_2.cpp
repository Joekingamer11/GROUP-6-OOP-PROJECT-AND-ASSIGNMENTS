#include <intriviz/Intriviz.hpp>
#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

int main() {
    try {
        std::cout << "==================================================\n";
        std::cout << "       CPPCIZ WEEK 2 INTEGRATED SHOWCASE         \n";
        std::cout << "==================================================\n\n";

        // ---------------------------------------------------------------------
        // 1. DATA INGESTION & WEEK 2 CLEANING / PREPROCESSING
        // ---------------------------------------------------------------------
        // Week 2 CSV Loader treats "NA", "NaN", "null", and "" as missing
        std::string csv_raw =
            "day,total_bill,tip\n"
            "Thu,16.99,1.01\n"
            "Thu,10.34,1.66\n"
            "Thu,21.01,3.50\n"
            "Fri,13.81,2.00\n"
            "Fri,NA,3.00\n"        // Missing numeric value token
            "Fri,28.97,5.87\n"
            "Sat,20.65,3.35\n"
            "Sat,17.92,4.08\n"
            "Sat,38.01,3.00\n"
            "Sun,48.17,5.00\n"
            "Sun,25.00,null\n"      // Missing numeric value token
            "Sun,32.40,4.20\n";

        intriviz::CsvOptions options;
        options.missing_tokens = {"NA", "NaN", "null", ""};
        intriviz::Dataset raw_data = intriviz::parse_csv(csv_raw, options);

        std::cout << "[1] Ingested raw dataset with " << raw_data.row_count() << " rows.\n";

        // Week 2 Data Preprocessing (M10): Drop incomplete rows
        intriviz::Dataset clean_data = intriviz::drop_missing(raw_data);
        std::cout << "    Dropped missing values -> Clean dataset has " 
                  << clean_data.row_count() << " complete rows.\n\n";

        // ---------------------------------------------------------------------
        // 2. WEEK 2 DESCRIPTIVE SUMMARIES & CORRELATION
        // ---------------------------------------------------------------------
        std::cout << "[2] Summary Statistics (describe table):\n";
        auto summaries = intriviz::describe(clean_data);
        std::cout << intriviz::to_string(summaries) << "\n";

        // Week 2 Stats (M5): Pearson Correlation Matrix
        auto corr_mat = intriviz::correlation_matrix(clean_data, {"total_bill", "tip"});
        std::cout << "    Pearson Correlation (total_bill vs tip): " 
                  << corr_mat[0][1] << "\n\n";

        // ---------------------------------------------------------------------
        // 3. WEEK 2 GROUP BY & BOOTSTRAP CONFIDENCE INTERVALS
        // ---------------------------------------------------------------------
        std::cout << "[3] Grouping by Category ('day') & 95% Bootstrap CIs:\n";
        // Week 2 GroupBy (M3): Preserves first-appearance order (Thu, Fri, Sat, Sun)
        auto groups = intriviz::group_by(clean_data, "day", "total_bill");

        std::vector<std::string> cat_names;
        std::vector<double> group_means;
        std::vector<intriviz::Interval> group_cis;

        for (const auto& g : groups) {
            double m = intriviz::mean(g.values);
            // Week 2 Stats (M5): Resampling bootstrap confidence interval
            intriviz::Interval ci = intriviz::bootstrap_ci(g.values, 0.95, 1000, 42);

            cat_names.push_back(g.key);
            group_means.push_back(m);
            group_cis.push_back(ci);

            std::cout << "    Category [" << g.key << "]: Mean = $" << m 
                      << " | 95% CI = [" << ci.low << ", " << ci.high << "]\n";
        }
        std::cout << "\n";

        // ---------------------------------------------------------------------
        // 4. WEEK 2 SCALING, DRAWING & BITMAP TEXT RENDERING
        // ---------------------------------------------------------------------
        std::cout << "[4] Rendering Point Plot with CIs onto Canvas...\n";

        constexpr int width = 600, height = 400;
        intriviz::Canvas canvas(width, height, intriviz::Color{248, 249, 250}); // Off-white background

        // Week 2 Categorical Scale (M1): Domain [-0.5, N-0.5] mapped to pixel range
        intriviz::CategoricalScale x_scale(cat_names, 80.0, 520.0);

        // Week 2 Ticks & Range Padding (M6)
        auto [y_min, y_max] = intriviz::padded_range({10.0, 50.0}, 0.1);
        intriviz::LinearScale y_scale(y_min, y_max, 320.0, 60.0); // Y reversed for canvas pixels

        // Week 2 Colormap & Luminance Contrast (M9)
        intriviz::Colormap viridis = intriviz::make_colormap("viridis");
        intriviz::Color plot_blue = intriviz::color_from_hex("#4C72B0");
        intriviz::Color dark_gray = intriviz::Color{40, 40, 40};

        // Week 2 Drawing (M7): Outer plot area border & axis lines
        intriviz::fill_rect(canvas, 70, 50, 460, 280, intriviz::Color{255, 255, 255}); // Plot area white fill
        intriviz::draw_rect(canvas, 70, 50, 460, 280, intriviz::Color{200, 200, 200}); // Frame border
        intriviz::draw_line(canvas, 70, 330, 530, 330, dark_gray, 2);               // X-Axis line
        intriviz::draw_line(canvas, 70, 50, 70, 330, dark_gray, 2);                // Y-Axis line

        // Week 2 Bitmap Font (M8): Main Title & Axis Labels
        intriviz::draw_text(canvas, 180, 18, "AVERAGE BILL BY DAY (WITH 95% CI)", dark_gray, 2);
        intriviz::draw_text(canvas, 270, 365, "DAY OF WEEK", dark_gray, 1);
        intriviz::draw_text_vertical(canvas, 25, 220, "TOTAL BILL ($)", dark_gray, 1); // Rotated text

        // Draw Y Ticks & Labels (Week 2 Ticks M6 & Font M8)
        std::vector<double> y_ticks = intriviz::nice_ticks(y_min, y_max, 5);
        for (double tick_val : y_ticks) {
            int y_px = static_cast<int>(y_scale.map(tick_val));
            if (y_px >= 50 && y_px <= 330) {
                // Tick line & grid
                intriviz::draw_line(canvas, 65, y_px, 70, y_px, dark_gray, 1);
                intriviz::draw_line(canvas, 70, y_px, 530, y_px, intriviz::Color{230, 230, 230}, 1);
                
                // Tick label text
                std::string label = intriviz::format_tick(tick_val);
                int tw = intriviz::text_width(label, 1);
                intriviz::draw_text(canvas, 60 - tw, y_px - 3, label, dark_gray, 1);
            }
        }

        // Draw Group Means, Connecting Lines & CI Bars (Week 2 Shapes M7)
        for (std::size_t i = 0; i < cat_names.size(); ++i) {
            int x_px = static_cast<int>(x_scale.map(static_cast<double>(i)));
            int y_mean_px = static_cast<int>(y_scale.map(group_means[i]));
            int y_low_px  = static_cast<int>(y_scale.map(group_cis[i].low));
            int y_high_px = static_cast<int>(y_scale.map(group_cis[i].high));

            // 1. Draw 95% Confidence Interval Bar & End Caps
            intriviz::draw_line(canvas, x_px, y_high_px, x_px, y_low_px, plot_blue, 2);
            intriviz::draw_line(canvas, x_px - 8, y_high_px, x_px + 8, y_high_px, plot_blue, 2); // Top cap
            intriviz::draw_line(canvas, x_px - 8, y_low_px,  x_px + 8, y_low_px,  plot_blue, 2); // Bottom cap

            // 2. Draw Group Mean Point (Filled Circle)
            intriviz::fill_circle(canvas, x_px, y_mean_px, 6, viridis.at(0.2 + 0.2 * i));
            intriviz::draw_circle(canvas, x_px, y_mean_px, 6, dark_gray);

            // 3. Draw Connecting Line to previous point
            if (i > 0) {
                int prev_x = static_cast<int>(x_scale.map(static_cast<double>(i - 1)));
                int prev_y = static_cast<int>(y_scale.map(group_means[i - 1]));
                intriviz::draw_line(canvas, prev_x, prev_y, x_px, y_mean_px, plot_blue, 2);
            }

            // 4. Draw X-Axis Categorical Tick Label
            std::string day_label = x_scale.tick_label(static_cast<double>(i));
            int tw = intriviz::text_width(day_label, 1);
            intriviz::draw_text(canvas, x_px - tw / 2, 340, day_label, dark_gray, 1);
        }

        // ---------------------------------------------------------------------
        // 5. IMAGE EXPORT
        // ---------------------------------------------------------------------
        const std::filesystem::path out_path =
            std::filesystem::path(INTRIVIZ_DATA_DIR) / "output" / "week2_showcase.bmp";
        intriviz::save_image(canvas, out_path.string());
        std::cout << "[5] Successfully saved rendered image to '" << out_path.string() << "'!\n";
        std::cout << "==================================================\n";

    } catch (const std::exception& e) {
        std::cerr << "Error running Week 2 showcase: " << e.what() << "\n";
        return 1;
    }

    return 0;
}