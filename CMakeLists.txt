cmake_minimum_required(VERSION 3.8)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_CXX_COMPILER "C:/mingw32/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/mingw32/bin/gcc.exe")

project(TaxiDrifter)

set(BUILD_SHARED_LIBS FALSE)

#SFML
set(SFML_STATIC_LIBRARIES TRUE)

#GLEW
set(BUILD_UTILS FALSE)

#GLFW
set(GLFW_BUILD_EXAMPLES FALSE)
set(GLFW_BUILD_TESTS FALSE)
set(GLFW_BUILD_DOCS FALSE)

add_subdirectory(libs/reactphysics3d)
add_subdirectory(libs/sfml)
add_subdirectory(libs/glew-2.1.0/build/cmake)
add_subdirectory(libs/glfw-3.3.8)
add_subdirectory(libs/glm)

add_executable(TaxiDrifter src/Main.cpp)

target_link_options(TaxiDrifter PRIVATE -static-libgcc -static-libstdc++ -static -Bstatic -lstdc++ -lpthread)
target_link_libraries(TaxiDrifter sfml-graphics sfml-audio reactphysics3d glew_s glfw glm)
target_include_directories(TaxiDrifter PRIVATE libs/glew-2.1.0/include)