cmake_minimum_required(VERSION 3.1...3.15)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
    cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
else()
    cmake_policy(VERSION 3.15)
endif()

#Set up project
project(<PNAME> VERSION 1.0
				LANGUAGES CXX)
				
#Set up the C++ flags
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_LIBRARIES -lstdc++fs)

if(MSVC)
    add_compile_options(/W4 /WX)
else()
    add_compile_options(-Wall -Wextra -pedantic -Werror)
endif()


set(SOURCES
    src/main.cpp
    src/input/keyboard.cpp
)

add_executable(${PROJECT_NAME} ${SOURCES})

find_package(Threads)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
