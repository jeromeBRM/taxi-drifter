cmake_minimum_required(VERSION 3.8)

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY build)
set(CMAKE_CXX_COMPILER "C:/mingw32/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/mingw32/bin/gcc.exe")

project(TaxiDrifter)

set(SFML_STATIC_LIBRARIES TRUE)
set(BUILD_SHARED_LIBS FALSE)

add_subdirectory(libs/reactphysics3d)
add_subdirectory(libs/sfml)
add_subdirectory(libs/glew-2.1.0/build/cmake)

add_executable(TaxiDrifter src/Main.cpp)

target_link_options(TaxiDrifter PRIVATE -static-libgcc -static-libstdc++ -static -Bstatic -lstdc++ -lpthread)
target_link_libraries(TaxiDrifter sfml-graphics sfml-audio reactphysics3d glew)
target_include_directories(TaxiDrifter PRIVATE libs/glew-2.1.0/include)