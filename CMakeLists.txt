cmake_minimum_required(VERSION 3.9)
project(rayltime)
set(ROOT "${CMAKE_CURRENT_LIST_DIR}")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sdl2_ttf)

# Use main.cpp, src/, include/
include_directories("include/")
set(SRC_DIR "src/")
file(GLOB_RECURSE SRCFILES "${SRC_DIR}/*.cpp")
list(APPEND SRCFILES main.cpp)

# Use SDL2 library
set(SDL2_DIR "sdl2/")
find_package(SDL2 REQUIRED)
include_directories(${SDL2_INCLUDE_DIRS})

# Use SDL2 ttf library for UI
set(SDL2_TTF_PATH ${CMAKE_CURRENT_SOURCE_DIR}/sdl2_ttf CACHE BOOL "" FORCE)
find_package(SDL2_ttf REQUIRED)

# Use OpenMP
# NOTE: Some compilers have dysfunctional find_package() functionality when it comes
# to OpenMP. You may need to link in OpenMP in a different manner.
find_package(OpenMP REQUIRED)

# Create executable
add_executable(${PROJECT_NAME} ${SRCFILES})
target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_TTF_INCLUDE_DIRS} SYSTEM PUBLIC ${ROOT}/eigen/) # use Eigen library
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} ${SDL2_TTF_LIBRARIES} OpenMP::OpenMP_CXX)
