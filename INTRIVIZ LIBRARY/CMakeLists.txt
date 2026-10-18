cmake_minimum_required(VERSION 3.16)
project(intriviz VERSION 1.0.0 LANGUAGES CXX)

# ---- Language settings -------------------------------------------------
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  set(CMAKE_BUILD_TYPE Release)
endif()

option(INTRIVIZ_BUILD_EXAMPLES "Build the programs in examples/" ON)
option(INTRIVIZ_BUILD_TESTS "Build the tests in tests/" ON)

# ---- 1. The library: every .cpp file in src/ ----------------------------
file(GLOB INTRIVIZ_SOURCES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp)
if(NOT INTRIVIZ_SOURCES)
  message(FATAL_ERROR "No .cpp files found in src/. Add at least src/version.cpp.")
endif()

add_library(intriviz STATIC ${INTRIVIZ_SOURCES})
target_include_directories(intriviz PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

if(MSVC)
  target_compile_options(intriviz PRIVATE /W4)
else()
  target_compile_options(intriviz PRIVATE -Wall -Wextra -Wpedantic)
endif()

# ---- 2. Examples: examples/foo.cpp becomes a program called foo ---------
if(INTRIVIZ_BUILD_EXAMPLES)
  file(GLOB INTRIVIZ_EXAMPLES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/examples/*.cpp)
  foreach(example_src ${INTRIVIZ_EXAMPLES})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    target_link_libraries(${example_name} PRIVATE intriviz)
    target_compile_definitions(${example_name} PRIVATE INTRIVIZ_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
  endforeach()
endif()

# ---- 3. Tests: tests/test_foo.cpp becomes a test called test_foo --------
if(INTRIVIZ_BUILD_TESTS)
  enable_testing()
  file(GLOB INTRIVIZ_TESTS CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_*.cpp)
  foreach(test_src ${INTRIVIZ_TESTS})
    get_filename_component(test_name ${test_src} NAME_WE)
    add_executable(${test_name} ${test_src})
    target_link_libraries(${test_name} PRIVATE intriviz)
    target_compile_definitions(${test_name} PRIVATE INTRIVIZ_DATA_DIR="${CMAKE_CURRENT_SOURCE_DIR}/data")
    add_test(NAME ${test_name} COMMAND ${test_name})
  endforeach()
endif()

# ---- 4. Install: library file and public headers ------------------------
install(TARGETS intriviz
  ARCHIVE DESTINATION lib
  LIBRARY DESTINATION lib
  RUNTIME DESTINATION bin)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/include/ DESTINATION include)
