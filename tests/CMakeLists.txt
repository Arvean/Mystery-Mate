# Find the Google Test package
find_package(GTest REQUIRED)

# Glob all the test files
file(GLOB TEST_SOURCES "*.cpp")

# Create a test executable
add_executable(ChessProjectTests ${TEST_SOURCES})

# Link Google Test and your chess_srcs library to the test executable
target_link_libraries(ChessProjectTests PRIVATE GTest::GTest GTest::Main chess_srcs)

# If needed, include directories
target_include_directories(ChessProjectTests PRIVATE ../include)

# Add the tests
gtest_discover_tests(ChessProjectTests)