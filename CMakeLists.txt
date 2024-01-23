cmake_minimum_required(VERSION 3.16)

project(core VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(
	Qt5
	REQUIRED
	COMPONENTS 
	Core
	Qml
	Gui
	Quick
	QuickControls2
)
# not possible, see: https://stackoverflow.com/questions/75624351/unknown-cmake-command-qt-standard-project-setup-on-raspberry-pi
# qt_standard_project_setup()

add_executable(
	core
    src/main.cpp
	src/listeners/CoreCallback.cpp
)

target_link_libraries(
	core
	PRIVATE
	linphone++
	Qt5::Core
	Qt5::Qml
	Qt5::Gui
	Qt5::Quick
	Qt5::QuickControls2
)
