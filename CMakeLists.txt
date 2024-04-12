cmake_minimum_required(VERSION 3.16)

project(core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RASPBERRYPI_LIBCAMERA_SUPPORT "Append the libcamera-v4l2 compability layer as LD_PRELOAD to the service file." OFF)
option(GENERATE_SAMPLE_CONFIG "Install a sample core.ini configuration file in /etc/dieklingel-core/core.ini." ON)

add_subdirectory(lib)
add_subdirectory(src/app)
add_subdirectory(service/app)
