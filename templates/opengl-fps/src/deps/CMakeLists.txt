#Setup glad
add_library(GladOpenGL STATIC glad.c)
target_include_directories(GladOpenGL PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})