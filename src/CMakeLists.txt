file(GLOB SOURCES "*.cpp")

add_library(chess_srcs STATIC ${SOURCES})

target_include_directories(chess_srcs PRIVATE ../include)

# Optional: Expose SOURCES to the parent scope
set(SOURCES ${SOURCES} PARENT_SCOPE)