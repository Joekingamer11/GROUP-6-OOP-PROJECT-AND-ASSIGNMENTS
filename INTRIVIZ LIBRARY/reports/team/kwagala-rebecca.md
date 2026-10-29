# week1
Axis Scaling module foundations.

# files added

### include\ccpviz\scale.hpp
This header defines the public contract for `Scale` and `LinearScale`

### src\scale.cpp
This file implements input validation using `InvalidArgument` from `cppviz/error.hpp`, linear mapping, range inversion (including handling reversed pixel ranges), and initial tick helpers

### tests\test_scale.cpp
This test executable relies on Member 10's `mini_test.hpp` framework to verify mapping accuracy, roundtrips, reversed ranges, polymorphism, and invalid inputs


## week 2
Expanding the scaling infrustructure by implementing the nick tick generation algorithm, logarithmic scales, tick formatting and domain padding helpers

# files added

### include/cppviz/ticks.hpp
This public header declares the tick selection and formatting utilities

### src/ticks.cpp
This file implements the 'nice numbers' algorithm (1, 2, 5, 10 scaling) base-10 tock placement, clean float formatting and domain expansion.

### tests/test_ticks.cpp
Unit tests for nice_ticks, log_ticks, format_tick and padded_range using mini_test.hpp

## Week 3
Swarm Plot module which consists of the non-overlapping layout algorithmn, category grouping and scale setup, color palette integration and plot frame rendering.

# Files added

### include/cppviz/swarmplot.hpp
It defines 'SwarmplotOptions', the main swarmplot() rendering entry point and the stand alone swarm_layout() functions

### src/swarmplot.cpp
Implements the five step plot convention: input validation, grouping, categorical and linear scale assignment, plot frame drawing, and dot rendering using the iterative collision-avoidance layout algorithm

### tests/test_swarmplot.cpp
This test suite covers standalone `swarm_layout` constraints (center alignment, no overlapping points, symmetric spreading, and 200-point performance) as well as `swarmplot` end-to-end rendering and invalid inputs using `mini_test.hpp`

## Week 4
Cross module testing, scale/axes integration tests, and documentation hardening.
Writing independent tests for Member 5's `pointplot` module and verifies seamless integration across all scale types (`LinearScale`, `LogScale`, `CategoricalScale`), tick generators, and `Axes`.

# Files added

### tests/test_pointplot_cross.cpp
(independent cross test)
Independently verifiy Member 5's `pointplot` module without relying on M5's internal test setup. This checks mean marker positions, confidence interval bar rendering, options flags (`show_ci`, `connect_points`), and invalid input error handling.

### tests/test_scales_integratio.cpp
(Integration test)

This test verifies seamless integration across `Scale`, `LinearScale`, `LogScale`, `CategoricalScale`, `nice_ticks`, `log_ticks`, and `Axes`.