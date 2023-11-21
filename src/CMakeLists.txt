file(GLOB SOURCES "*.cpp")

add_library(chess_srcs STATIC ${SOURCES})

target_include_directories(chess_srcs PRIVATE
    ${CMAKE_SOURCE_DIR}/crow
    ${CMAKE_SOURCE_DIR}/include
)
# Optional: Expose SOURCES to the parent scope
set(SOURCES ${SOURCES} PARENT_SCOPE)