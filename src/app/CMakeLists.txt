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

add_executable(
	dieklingel-core
    main.cpp
	App.cpp
	Mqtt.cpp
)

target_link_libraries(
	dieklingel-core
	PRIVATE
	linphone++
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
