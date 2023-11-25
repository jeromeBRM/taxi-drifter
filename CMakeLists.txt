cmake_minimum_required(VERSION 3.8)

set(CMAKE_CXX_COMPILER "C:/mingw32/bin/g++.exe")
set(CMAKE_C_COMPILER "C:/mingw32/bin/gcc.exe")

project(TaxiDrifter)

add_subdirectory(libs/reactphysics3d)
add_subdirectory(libs/sfml)

add_executable(TaxiDrifter src/Main.cpp)

target_link_libraries(TaxiDrifter PRIVATE reactphysics3d)
target_link_libraries(TaxiDrifter PRIVATE sfml)