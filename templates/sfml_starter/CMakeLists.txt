cmake_minimum_required(VERSION 3.10)

#Set up project
project(<PNAME>  VERSION 1.0)

include(${CMAKE_BINARY_DIR}/../conanbuildinfo.cmake)
conan_basic_setup()


#Set executable
add_executable(${PROJECT_NAME}     
    src/main.cpp

	src/Animation.cpp
	src/Profiler.cpp
	src/Screen.cpp
	src/Game/ScreenMainMenu.cpp
	src/Game/ScreenGame.cpp

	src/imgui_sfml/imgui-SFML.cpp
)

#Set C++17
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)
set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)

#Set flags
if(MSVC)
  	target_compile_options(${PROJECT_NAME} PRIVATE 
    	/W4 /WX)
else()
  	target_compile_options(${PROJECT_NAME} PRIVATE 
		-Wall -Wextra -pedantic)
endif()

#Set module path
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake_modules" ${CMAKE_MODULE_PATH})

#Find libraries
find_package(Threads)

target_include_directories(${PROJECT_NAME} PRIVATE deps)



#Finally
target_link_libraries(${PROJECT_NAME} 
    Threads::Threads 
    ${CONAN_LIBS}
)

