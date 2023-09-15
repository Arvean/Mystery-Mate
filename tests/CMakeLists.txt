# Find the Google Test package
find_package(GTest 1.10 REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(GMOCK REQUIRED gmock)

# Glob all the test files
file(GLOB TEST_SOURCES "*.cpp")

link_directories(/usr/local/lib)

# Create a test executable
add_executable(ChessProjectTests ${TEST_SOURCES})

# Link Google Test and your chess_srcs library to the test executable
target_link_libraries(ChessProjectTests PRIVATE GTest::GTest GTest::Main ${GMOCK_LIBRARIES} chess_srcs)

# If needed, include directories
target_include_directories(ChessProjectTests PRIVATE ../include ${GMOCK_INCLUDE_DIRS})


# Add the tests
gtest_discover_tests(ChessProjectTests)