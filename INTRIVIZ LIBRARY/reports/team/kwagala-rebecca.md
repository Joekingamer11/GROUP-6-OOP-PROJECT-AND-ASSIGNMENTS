# week1

# files added

### include\ccpviz\scale.hpp
This header defines the public contract for `Scale` and `LinearScale`

### src\scale.cpp
This file implements input validation using `InvalidArgument` from `cppviz/error.hpp`, linear mapping, range inversion (including handling reversed pixel ranges), and initial tick helpers

### tests\test_scale.cpp
This test executable relies on Member 10's `mini_test.hpp` framework to verify mapping accuracy, roundtrips, reversed ranges, polymorphism, and invalid inputs
