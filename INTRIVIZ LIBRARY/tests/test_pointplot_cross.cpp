#include "mini_test.hpp"

#include "cppviz/axes.hpp"
#include "cppviz/canvas.hpp"
#include "cppviz/dataset.hpp"
#include "cppviz/error.hpp"
#include "cppviz/pointplot.hpp"
#include "cppviz/theme.hpp"

int main() {
        using namespace cppviz;
    //Pointplot rendering and pixel color check
    {
        Dataset df;
        df.add_text_column("group", {"A", "A", "A", "B", "B", "B"});
        df.add_numeric_column("score", {10.0, 12.0, 14.0, 20.0, 22.0, 24.0});

        Canvas canvas(400, 300, Color{255, 255, 255});
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts; 
        opts.show_ci = true; 
        opts.connect_points = true;

        pointplot(ax, df, "group", "score", opts);

        //To verify that drawings occured on canvas
        const auto&amp; pixels = canvas.data(); 
        bool has_non_white = false; 
        for (std::size\_t i = 0; i &lt; pixels.size(); i += 3) { 
            if (pixels[i] != 255 || pixels[i + 1] != 255 || pixels[i + 2] != 255) {
                 has_non_white = true; 
                 break; 
                } 
            }
            CHECH(has_non_white);
    }

    //To verify mean pixel alignment
    {
        //Group A mean = 10.0
        //Group B mean = 30.0
        Dataset df;
        df.add_text_column("group", {"A", "A", "B", "B"}); 
        df.add_numeric_column("value", {10.0, 10.0, 30.0, 30.0});

        Canvas canvas(400, 300, Color{255, 255, 255}); 
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts; 
        
        // Disabling CI to inspect mean markers directly 
        opts.show_ci = false; 
        pointplot(ax, df, "group", "value", opts);

        //mean of group A (0) at y = 10.0 must be lower than mean of Group B (1) at y = 30.0
        PointI pt_A = ax.to_pixel(0.0, 10.0); 
        PointI pt_B = ax.to_pixel(1.0, 30.0);

        CHECK(pt_A.y > pt_B.y);
    }

    //Option Flags

    {
        Dataset df;
        df.add_text_column("cat", {"C1", "C1", "C2", "C2"}); 
        df.add_numeric_column("val", {5.0, 15.0, 25.0, 35.0});

        Canvas canvas(400, 300, Color{255, 255, 255}); 
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        PointplotOptions opts; 
        opts.show_ci = false; 
        opts.connect_points = false;

        CHECH_NOTHROW(pointplot(ax, df, "cat", "val", opts));
    }
//Checking for invalid inputs and error handling  

     {
        Dataset df;
        df.add_text_column("cat", {"A", "B"}); 
        df.add_numeric_column("val", {10.0, 20.0}); 
        
        Canvas canvas(400, 300); 
        Axes ax(canvas, Rect{50, 30, 320, 220}, theme_white());

        //Checking for non existing columns; throw DataError
        CHECK_THROWS(pointplot(ax, df, "missing", "val"), DataError); 
        CHECK_THROWS(pointplot(ax, df, "cat", "missing"), DataError);

        //Column type swap; throw DataError
        CHECK_THROWS(pointplot(ax, df, "val", "cat"), DataError); 
        
        // Empty dataset throws DataError 
        Dataset empty_df; 
        CHECK_THROWS(pointplot(ax, empty\_df, "cat", "val"), DataError); 
        
        // Invalid confidence level (&lt;= 0 or &gt;= 1) throws InvalidArgument 
        PointplotOptions bad_opts; 
        bad_opts.ci_level = 1.5; 
        CHECK_THROWS(pointplot(ax, df, "cat", "val", bad\_opts), InvalidArgument); 
    } 
    
    return finish();
}
