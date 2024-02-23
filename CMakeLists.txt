cmake_minimum_required(VERSION 3.16)

project(dieklingel-core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(RASPBERRYPI_LIBCAMERA_SUPPORT "Append the libcamera-v4l2 compability layer as LD_PRELOAD to the service file." OFF)
option(GENERATE_SAMPLE_CONFIG "Install a sample core.ini configuration file in /etc/dieklingel-core/core.ini." ON)

add_subdirectory(src/app)
add_subdirectory(service/app)

set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Kai Mayer <kai.mayer@dieklingel.com>")
set(CPACK_DEBIAN_PACKAGE_DESCRIPTION "dieKlingle Core")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_DEPENDS "qml-module-qtquick-controls2 (>= 2.12), qml-module-qtquick2 (>= 2.12), qml-module-qtmultimedia (>= 2.12)")
set(CPACK_GENERATOR "DEB")
include(CPack)