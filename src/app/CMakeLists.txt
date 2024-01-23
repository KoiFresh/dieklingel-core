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
    main.cpp
	App.cpp
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