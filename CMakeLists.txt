cmake_minimum_required(VERSION 3.16)

project(dieklingel-core VERSION 0.1.3 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RASPBERRYPI_LIBCAMERA_SUPPORT "Append the libcamera-v4l2 compability layer as LD_PRELOAD to the service file." OFF)
option(GENERATE_SAMPLE_CONFIG "Install a sample core.ini configuration file in /etc/dieklingel-core/core.ini." ON)

add_subdirectory(src/app)
add_subdirectory(service/app)

set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS "ON")
set(CPACK_DEBIAN_FILE_NAME "DEB-DEFAULT")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kai Mayer <kai.mayer@dieklingel.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "dieklingel-core: The private, integrateable and customizeable doorunit!")
include(CPack)
