set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_package(
	Qt5
	REQUIRED
	COMPONENTS
	Core
	Concurrent
	Qml
	Gui
	Quick
	QuickControls2
)

# not possible, see: https://stackoverflow.com/questions/75624351/unknown-cmake-command-qt-standard-project-setup-on-raspberry-pi
# qt_standard_project_setup()
find_package(PahoMqttCpp REQUIRED)

find_library(GPIOD_LIBRARY NAMES libgpiodcxx.so)

if(NOT GPIOD_LIBRARY)
	message(FATAL_ERROR "gpiod library not found. Install apt install libgpiod-dev")
endif()

find_library(LIBLINPHONE_LIBRARY NAMES liblinphone.so)

find_library(GPIOD_LIBRARY NAMES libgpiodcxx.so)

if(NOT GPIOD_LIBRARY)
	message(FATAL_ERROR "gpiod library not found. Install apt install libgpiod-dev")
endif()

find_package(BelleSIP 4.2.0 REQUIRED)
find_package(Mediastreamer2 4.2.0 REQUIRED)

# find_package(Ortp 1.0.0 REQUIRED)
# find_package(BCToolbox 1.0.0 REQUIRED)
# find_package(Belr 4.2.0 REQUIRED)
if(NOT LIBLINPHONE_LIBRARY)
	message(FATAL_ERROR "liblinphone library not found. Install apt install liblinphone-dev")
endif()

find_library(LIBLINPHONE_CXX_LIBRARY NAMES liblinphone++.so)

if(NOT LIBLINPHONE_CXX_LIBRARY)
	message(FATAL_ERROR "liblinphone library not found. Install apt install liblinphone-dev")
endif()

add_definitions(-DDIEKLINGEL_CORE_VERSION="${CMAKE_PROJECT_VERSION}")

add_executable(
	dieklingel-core
	gpio/Input.cpp
	gpio/Output.cpp
	video/SplitterCamera.cpp
	video/SplitterSource.cpp
	video/SplitterSink.cpp
	App.cpp
	Capturer.cpp
	main.cpp
	CoreConfig.cpp
	InactivityDetector.cpp
	Mqtt.cpp
)

target_link_libraries(
	dieklingel-core
	PRIVATE
	${GPIOD_LIBRARY}
	${LIBLINPHONE_LIBRARY}
	${LIBLINPHONE_CXX_LIBRARY}
	mediastreamer
	ortp
	paho-mqtt3as
	paho-mqttpp3
	Qt5::Core
	Qt5::Concurrent
	Qt5::Qml
	Qt5::Gui
	Qt5::Quick
	Qt5::QuickControls2
)

install(
	TARGETS
	dieklingel-core
)
