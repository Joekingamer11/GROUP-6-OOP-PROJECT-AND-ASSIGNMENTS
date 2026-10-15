#include "mini_test.hpp"
#include "cppviz/scale.hpp"
#include "cppviz/error.hpp"

int main(){
    using namespace cppviz;

    {
        //basic linear scale mapping
        LinearScale scale(0.0, 10.0, 0.0, 100.0);
        CHECK_NEAR(scale.map(0.0), 0.0, 1e-9);
        CHECK_NEAR(scale.map(5.0), 50.0, 1e-9);
        CHECK_NEAR(scale.map(10.0), 100.0, 1e-9);

        //invert round trip(pixel to data)
        CHECK_NEAR(scale.invert(0.0), 0.0, 1e-9);
        CHECK_NEAR(scale.invert(50.0), 5.0, 1e-9); 
        CHECK_NEAR(scale.invert(100.0), 10.0, 1e-9);

            //reversed pixel range
        LinearScale reversed_scale(10.0, 0.0, 100.0, 0.0);
        CHECK_NEAR(reversed_scale.map(0.0), 100.0, 1e-9);
        CHECK_NEAR(reversed_scale.map(5.0), 50.0, 1e-9);
        CHECK_NEAR(reversed_scale.map(10.0), 0.0, 1e-9);

            //Domain accessors and tick values
        CHECK_NEAR(scale.domain_min(), 0.0, 1e-9);
        CHECK_NEAR(scale.domain_max(), 10.0, 1e-9);
        auto ticks = scale.tick_values();
        CHECK_EQ(ticks.size(), 2u);
        CHECK_NEAR(ticks[0], 0.0, 1e-9);
        CHECK_NEAR(ticks[1], 10.0, 1e-9);

            //polymorphic call via const Scale reference
        const Scale& scale_ref = scale;
        CHECK_NEAR(scale_ref.map(5.0), 50.0, 1e-9);

            //Invalid domain inputs
        CHECK_THROWS(LinearScale(5.0, 5.0, 0.0, 100.0), InvalidArgument);
        return finish();

    } 

    
}