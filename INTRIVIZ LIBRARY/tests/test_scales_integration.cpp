#include "mini_test.hpp"

#include <memory>
#include <vector>

#include "cppviz/axes.hpp" 
#include "cppviz/canvas.hpp" 
#include "cppviz/categorical_scale.hpp" 
#include "cppviz/scale.hpp" 
#include "cppviz/theme.hpp" 
#include "cppviz/ticks.hpp"

int main() {
     using namespace cppviz; 
     Canvas canvas(500, 400); 
     Rect area{50, 40, 400, 300}; // left=50, top=40, width=400, height=300 
     Axes ax(canvas, area, theme_whitegrid());

     //Test LinearScale + Axes to_pixel mapping

     {
        auto [x_left, x_right] = ax.x_pixel_range(); // {50.0, 450.0}
        auto [y_bottom, y_top] = ax.y_pixel_range(); // {340.0, 40.0}

        auto x_scale = std::make_shared<LinearScale>(0.0, 100.0, x_left, x_right);
        auto y_scale = std::make_shared<LinearScale>(0.0, 50.0, y_bottom, y_top);

        ax.set_x_scale(x_scale);
        ax.set_y_scale(y_scale);

        //Bottom left corner of plot area
        PointI p_bl = ax.to_pixel(0.0, 0.0); 
        CHECK_EQ(p_bl.x, 50); 
        CHECK_EQ(p_bl.y, 340);

        // Top-right corner of plot area 
        PointI p_tr = ax.to_pixel(100.0, 50.0); 
        CHECK_EQ(p_tr.x, 450); 
        CHECK_EQ(p_tr.y, 40);

        // Center of plot area 
        PointI p_mid = ax.to_pixel(50.0, 25.0); 
        CHECK_EQ(p_mid.x, 250); 
        CHECK_EQ(p_mid.y, 190);
     }

    //LogScale + Axes to_pixel Mapping

    {
        auto [x_left, x_right] = ax.x_pixel_range(); 
        auto [y_bottom, y_top] = ax.y_pixel_range(); 
        
        auto x_scale = std::make_shared<LogScale>(1.0, 1000.0, x_left, x_right);
        auto y_scale = std::make_shared<LinearScale>(0.0, 100.0, y_bottom, y_top);

        ax.set_x_scale(x_scale);
        ax.set_y_scale(y_scale);

        // LogScale mapping check: 1.0 -> 50px, 10.0 -> 183.33px, 100.0 -> 316.67px, 1000.0 -> 450px
        PointI p1 = ax.to_pixel(1.0, 0.0);
        PointI p10 = ax.to_pixel(10.0, 0.0);
        PointI p1000 = ax.to_pixel(1000.0, 0.0);

        CHECK_EQ(p1.x, 50);
        CHECK_EQ(p1000.x, 450);
        CHECK(p10.x > p1.x);
        CHECK(p10.x < p1000.x);
    
    }

// CategoricalScale + Axes integration

{
    auto [x_left, x_right] = ax.x_pixel_range(); 
    auto [y_bottom, y_top] = ax.y_pixel_range();

     std::vector<std::string> cats = {"Thu", "Fri", "Sat", "Sun"};
        auto x_scale = std::make_shared<CategoricalScale>(cats, x_left, x_right);
        auto y_scale = std::make_shared<LinearScale>(0.0, 100.0, y_bottom, y_top);

        ax.set_x_scale(x_scale);
        ax.set_y_scale(y_scale);

        // categories over 400px = 100px band width. Center 0 at 100px (50 + 50)
        CHECK_NEAR(x_scale->band_width(), 100.0, 1e-9);
        PointI p_cat0 = ax.to_pixel(0.0, 50.0);
        CHECK_EQ(p_cat0.x, 100);

        // Tick label lookup from CategoricalScale
        CHECK_EQ(x_scale->tick_label(0.0), "Thu");
        CHECK_EQ(x_scale->tick_label(3.0), "Sun");
    }

    
    // Frame Drawing with Integrated Scales and Ticks
    
    {
        auto [x_left, x_right] = ax.x_pixel_range();
        auto [y_bottom, y_top] = ax.y_pixel_range();

        ax.set_x_scale(std::make_shared<LinearScale>(0.0, 50.0, x_left, x_right));
        ax.set_y_scale(std::make_shared<LinearScale>(0.0, 100.0, y_bottom, y_top));

        // draw_frame() uses nice_ticks under the hood and executes cleanly
        CHECK_NOTHROW(ax.draw_frame());
    }

    return finish();
}

