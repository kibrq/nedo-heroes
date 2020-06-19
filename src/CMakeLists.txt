include_directories("${PROJECT_SOURCE_DIR}/${INC}")

file(GLOB SRCS "*.cpp")
set(MODULE_NAME MAIN)
add_library(${MODULE_NAME} OBJECT ${SRCS})
target_compile_options(${MODULE_NAME} PRIVATE -std=c++17 -g)
add_source($<TARGET_OBJECTS:${MODULE_NAME}>)


