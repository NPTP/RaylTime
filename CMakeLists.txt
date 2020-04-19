cmake_minimum_required(VERSION 3.9)
project(rayltime)
set(ROOT "${CMAKE_CURRENT_LIST_DIR}")

# Use main.cpp, src/, include/
include_directories("include/")
set(SRC_DIR "src/")
file(GLOB_RECURSE SRCFILES "${SRC_DIR}/*.cpp")
list(APPEND SRCFILES main.cpp)

# Use SDL2 library
set(SDL2_DIR "sdl2/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Use OpenMP
# NOTE: Some compilers have dysfunctional find_package() functionality when it comes
# to OpenMP. You may need to link in OpenMP in a different manner.
find_package(OpenMP REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} SYSTEM PUBLIC ${ROOT}/eigen/) # use Eigen library
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} OpenMP::OpenMP_CXX)
