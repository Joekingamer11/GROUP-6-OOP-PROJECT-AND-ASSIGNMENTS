#include "mini_test.hpp"

#include <cmath>
#include "cppviz/axes.hpp" 
#include "cppviz/canvas.hpp" 
#include "cppviz/dataset.hpp" 
#include "cppviz/error.hpp"
#include "cppviz/swarmplot.hpp" 
#include "cppviz/theme.hpp" 


int main() {
   using namespace cppviz; {

   //layot. single observation on category center line
std::vector<double> y_pts = {100.0};
        auto layout = swarm_layout(y_pts, 150.0, 100.0, 4);
        CHECK_EQ(layout.size(), 1u);
        CHECK_EQ(layout.x, 150);
        CHECK_EQ(layout.y, 100);
}
// No dots in category are closer than diameter
{
  const int radius = 4; 
  const int min_dist_sq = (2 * radius) * (2 * radius); 
  std::vector<double> y_pts = {50.0, 50.0, 51.0, 52.0, 50.5, 60.0, 70.0};
        auto layout = swarm_layout(y_pts, 200.0, 120.0, radius);
        CHECK_EQ(layout.size(), y_pts.size());
        
       for (std::size_t i = 0; i &lt; layout.size(); ++i) {
         for (std::size_t j = i + 1; j &lt; layout.size(); ++j) { 
            int dx = layout[i].x - layout[j].x; 
            int dy = layout[i].y - layout[j].y;
            int dist_sq = dx * dx + dy * dy;
            
            // Points with sufficient band width must not collide 
            CHECK(dist_sq &gt;= min_dist_sq); 
        }
     }
 } 
 // Identical values must spread symmettically around center_x
 {
    std::vector<double> y_pts = {100.0, 100.0, 100.0};
        auto layout = swarm_layout(y_pts, 0.0, 100.0, 4);
        CHECK_EQ(layout.size(), 3u);
        // Center point at 0
        CHECK_EQ(layout.x, 0);
        // Left and right points spread symmetrically (+d and -d)
        CHECK_EQ(layout[11].x, -layout[12].x);
    }

// 200 points terminate quickly
{
    std::vector<double> many_pts;
        many_pts.reserve(200);
        for (int i = 0; i < 200; ++i) {
            many_pts.push_back(100.0 + (i % 20));
        }
        auto layout = swarm_layout(many_pts, 150.0, 150.0, 3);
        CHECK_EQ(layout.size(), 200u);
}

// End to end swarmplot rendering on Axes

{
    Dataset df; 
    df.add_text_column("day", {"Thur", "Fri", "Thur", "Fri", "Sat"}); 
    df.add_numeric_column("bill", {16.99, 10.34, 21.01, 23.68, 24.59});

    Canvas canvas(400, 300, Color{255, 255, 255}); 
    Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white()); 
    
    SwarmplotOptions opts; 
    opts.marker_radius = 4; 
    opts.color_by_category = true; 
    
    swarmplot(ax, df, "day", "bill", opts);

    //to verify that canvas ws drawn on
    const auto&amp; pixels = canvas.data(); 
    bool has_non_white = false; 
    for (std::size_t i = 0; i &lt; pixels.size(); i += 3) { 
        if (pixels[i] != 255 || pixels[i + 1] != 255 || pixels[i + 2] != 255) {
             has_non_white = true; 
             break;
             
            }
             } 
    
    CHECK(has_non_white);
}
// Error Handling
 {
Dataset df; 
df.add_text_column("day", {"Thur", "Fri"}); 
df.add_numeric_column("bill", {15.0, 25.0}); 
Canvas canvas(400, 300); 
Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());


//Missing column throws DataError
CHECK_THROWS(swarmplot(ax, df, "nonexistent", "bill"), DataError);
CHECK_THROWS(swarmplot(ax, df, "day", "nonexistent"), DataError);

// Type mismatch throws DataError 
CHECK_THROWS(swarmplot(ax, df, "bill", "day"), DataError);

// Empty dataset throws DataError 
Dataset empty_df; 
CHECK_THROWS(swarmplot(ax, empty\_df, "day", "bill"), DataError);

// Invalid layout inputs throw InvalidArgument 
CHECK_THROWS(swarm_layout({10.0}, 100.0, 0), InvalidArgument); 
CHECK_THROWS(swarm_layout({10.0}, -10.0, 4), InvalidArgument); 

}
return 0;

}


