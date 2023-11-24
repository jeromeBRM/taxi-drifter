# Minimum cmake version required 
cmake_minimum_required(VERSION 3.8) 
 
# Import the ReactPhysics3D library

set( CMAKE_CXX_COMPILER "C:/mingw32/bin/g++.exe" )
set( CMAKE_C_COMPILER "C:/mingw32/bin/gcc.exe" )
 
# Project 
project(Drifter)

# Create the executable 
add_executable(Drifter src/Main.cpp)