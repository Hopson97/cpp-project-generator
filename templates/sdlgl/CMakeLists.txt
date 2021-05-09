cmake_minimum_required(VERSION 3.10)

#Set up project
project(
    nukclear_sdl_gl_test 
    VERSION 1.0
)

#Set executable
add_executable(${PROJECT_NAME}     
    src/main.c
    src/Maths.c
    src/Graphics/GLDebug.c
    src/Graphics/Shader.c
)

#Set C11
target_compile_features(${PROJECT_NAME} PUBLIC c_std_11)

#Set flags
if(MSVC)
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4 /WX)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)		
endif()

find_package(SDL2 REQUIRED)

add_subdirectory(deps/glad)
add_subdirectory(deps/cglm)
add_subdirectory(deps/nuklear)
add_subdirectory(deps/stb)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    deps
    ${SDL2_INCLUDE_DIRS}
)

#Finally
target_link_libraries(${PROJECT_NAME} 
    cglm 
    glad 
    nuklear
    m
    stb
    ${CMAKE_DL_LIBS}
    ${SDL2_LIBRARIES}
)
