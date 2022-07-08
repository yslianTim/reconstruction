
if(NOT UNIX)
    message(FATAL_ERROR "Must be installed on UNIX distribution..")
endif()
message(STATUS "------------------------")
message(STATUS "------- MyProject ------")
message(STATUS "------------------------")

# Specify CMAKE version
cmake_minimum_required(VERSION 3.17 FATAL_ERROR)

# Set the project name and version
project(MyProject VERSION 1.0)

# Specify C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Locate Qt package
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core)

# Locate ROOT package
find_package(ROOT 6.24 CONFIG REQUIRED)
list(APPEND ROOT_LIBS ROOT::Core ROOT::Gpad ROOT::Hist)

# Locate Eigen package
find_package(Eigen3 3.3 REQUIRED NO_MODULE)
list(APPEND EIGEN_LIBS Eigen3::Eigen)

# Locate HDF5 package
find_package(HDF5 COMPONENTS CXX HL REQUIRED)
list(APPEND HDF5_LIBS ${HDF5_HL_LIBRARIES} ${HDF5_CXX_LIBRARIES} ${HDF5_LIBRARIES})

# Control where the static and shared libraries are built so that on windows
# we don't need to tinker with the path to run the executable
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_BINARY_DIR}")

# 
option(BUILD_SHARED_LIBS "Build using shared libraries" ON)


# Configure a header file to pass some of the CMake settings to the source code
configure_file(config.h.in config.hpp)



# Get GoogleTest
include(FetchContent)
FetchContent_Declare(googletest
  GIT_REPOSITORY https://github.com/google/googletest
  GIT_TAG release-1.11.0)
FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BUILD_DIR})
endif()

include_directories(src)
add_subdirectory(src)
add_subdirectory(test)