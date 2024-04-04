set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

find_library(GPIOD_LIBRARY NAMES libgpiodcxx.so)

if(NOT GPIOD_LIBRARY)
	message(FATAL_ERROR "gpiod library not found. Install apt install libgpiod-dev")
endif()

find_package(Linphone 4.2.0 REQUIRED)
find_package(LinphoneCxx REQUIRED)
find_package(Mediastreamer2 4.2.0 REQUIRED)
find_package(PahoMqttCpp REQUIRED)
find_package(
	Qt6
	REQUIRED
	COMPONENTS
	Core
	Concurrent
	Qml
	Gui
	Quick
	QuickControls2
)

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
	linphone
	linphone++
	mediastreamer
	ortp
	paho-mqtt3as
	paho-mqttpp3
	Qt6::Core
	Qt6::Concurrent
	Qt6::Qml
	Qt6::Gui
	Qt6::Quick
	Qt6::QuickControls2
)

install(
	TARGETS
	dieklingel-core
	DESTINATION
	${CMAKE_INSTALL_PREFIX}/bin
)
