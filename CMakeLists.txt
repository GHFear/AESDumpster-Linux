cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(AESDumpster VERSION 1.2.5)
include_directories(includes)
add_executable(
AESDumpster
includes/AESDumpster/AESDumpster.cpp
includes/KeyTools/KeyDumpster.h
includes/KeyTools/KeyDumpster.cpp
includes/OtherTools/OtherTools.h
includes/OtherTools/OtherTools.cpp
)
