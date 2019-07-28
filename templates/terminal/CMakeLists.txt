cmake_minimum_required(VERSION 3.1)

project(<PNAME>)
#Set up the C++ flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


set(SOURCES
	src/main.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
