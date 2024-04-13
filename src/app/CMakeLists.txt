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

add_definitions(-DDIEKLINGEL_CORE_VERSION="${CMAKE_PROJECT_VERSION}")

add_executable(
	dieklingel-core
	audio/Audio.cpp
	camera/Camera.cpp
	gpio/Input.cpp
	gpio/Output.cpp
	mqtt/Mqtt.cpp
	kiosk/InactivityDetector.cpp
	kiosk/Kiosk.cpp
	setup/Setup.cpp
	softphone/Softphone.cpp
	video/SplitterCamera.cpp
	video/SplitterSource.cpp
	video/SplitterSink.cpp
	web/WebServer.cpp
	Capturer.cpp
	main.cpp
	CoreConfig.cpp
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
	DESTINATION
	${CMAKE_INSTALL_PREFIX}/bin
)

# GTest

find_package(GTest REQUIRED)

enable_testing()

add_executable(
  dieklingel-core_test
  setup/Setup.cpp
  setup/SetupTest.cpp
  web/WebServer.cpp
  web/WebServerTest.cpp
)
target_link_libraries(
  dieklingel-core_test
  GTest::gtest_main
  GTest::gmock_main
  ${GPIOD_LIBRARY}
  linphone
  linphone++
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

include(GoogleTest)
gtest_discover_tests(dieklingel-core_test)
