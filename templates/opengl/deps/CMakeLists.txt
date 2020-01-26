#Glad
set(GLAD_PATH ${CMAKE_CURRENT_SOURCE_DIR}/glad)
add_library(glad ${GLAD_PATH}/glad.c)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})