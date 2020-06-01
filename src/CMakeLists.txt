
cmake_minimum_required(VERSION 2.8)
project(pgm)

add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLAD)

# Add G++ Warning on Unix
if(UNIX)
add_definitions(-g -O2 -std=c++11 -Wall -Wextra)
    set(CMAKE_CXX_COMPILER g++)
    find_package(glfw3 REQUIRED) #Expect glfw3 to be installed on your system
endif()

# In Window set directory to precompiled version of glfw3
if(WIN32)
    set(CMAKE_BUILD_TYPE Debug) # Switch to Release for faster execution
    set(GLFW_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/lib_windows/glfw3_win/include")
    include_directories(${GLFW_INCLUDE_DIRS})
    set(GLFW_LIBRARIES "${CMAKE_CURRENT_SOURCE_DIR}/lib_windows/glfw3_win/lib/glfw3.lib")
endif()


include_directories(".")
include_directories("third_party/eigen/")
file(
    GLOB_RECURSE
    source_files
    vcl/*.[ch]pp
    main/*.[ch]pp
    third_party/*.[ch]pp
    third_party/*.[ch]
    scenes/*.[ch]pp
    scenes/*.glsl
    )
add_executable(pgm ${source_files})


if(UNIX)
target_link_libraries(pgm glfw dl -static-libstdc++)
endif()

if(WIN32)
    source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${source_files}) # Allow to explore source directories as a tree
    target_link_libraries(pgm ${GLFW_LIBRARIES})
endif()
