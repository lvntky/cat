cmake_minimum_required(VERSION 3.22)

project(cat VERSION 1.0.0 DESCRIPTION "CAT: C Advanced Threads Library")

# Declare the library target.
add_library(${PROJECT_NAME} SHARED
    ./src/cat.c
    ./include/cat.h
    ./src/thread_pool.c
    ./include/thread_pool.h
)

# Configure the directories to search for header files.
target_include_directories(${PROJECT_NAME} PRIVATE source)
# Set the version property.
set_target_properties(${PROJECT_NAME} PROPERTIES VERSION ${PROJECT_VERSION})
# Set the shared object version property to the project's major version.
set_target_properties(${PROJECT_NAME} PROPERTIES SOVERSION ${PROJECT_VERSION_MAJOR})
# Set the public header property to the one with the actual API.
set_target_properties(${PROJECT_NAME} PROPERTIES PUBLIC_HEADER source/convert.h)

# Set compiler flags
#target_compile_options(${PROJECT_NAME} PRIVATE
#    -Wall
#    -Wextra
#    -Werror
#    -pedantic
# )
