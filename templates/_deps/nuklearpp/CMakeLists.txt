add_library(nuklear
    nuklear_sfml_gl3.cpp
    demos.cpp
    nuklear_impl.c
)
target_compile_options(nuklear PRIVATE -fPIE)	