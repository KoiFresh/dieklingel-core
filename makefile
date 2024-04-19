.PHONY: deps\:build deps\:run deps\:all build run clean

### Install dependencies to build the executable
deps\:build:
	apt-get install -y \
	    build-essential \
		clang-tidy \
    	cmake \
    	git \
		libgl1-mesa-dev \
		libgmock-dev \
    	libgpiod-dev \
		libgtest-dev \
    	liblinphone-dev \
    	libpaho-mqtt-dev \
    	libpaho-mqttpp-dev \
    	libssl-dev \
    	qtbase5-dev \
    	qtdeclarative5-dev \
    	qtquickcontrols2-5-dev

### Install dependencies to run the executable
deps\:run:
	apt-get install -y \
		qml-module-qtmultimedia \
		qml-module-qtquick-controls \
		qml-module-qtquick-controls2 \
		qml-module-qtquick2 \
		qtwayland5

### Install dependencies to develop the application
deps\:dev:
	apt-get install -y \
		clang-format \
		file

### Install dependencies to build and run the executable
deps\:all: deps\:build deps\:run deps\:dev

### Build the executable file
build:
	cmake -B build -DRASPBERRYPI_LIBCAMERA_SUPPORT=${RASPBERRYPI_LIBCAMERA_SUPPORT}
	cmake --build build

### Build and run the executable
run: build
	build/src/app/dieklingel-core

### Build and run tests
test: build
	build/src/app/dieklingel-core_test

### Install the executable into a bin location
install: build
	make -C build install

### Cleanup any existing build files
clean:
	rm -rf build

### Build a debian package
package: build
	make -C build package

### Check the format of all cpp, hpp, cc and cxx files
format\:check:
	find src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' | xargs clang-format --dry-run -Werror -i -style=file

### Format all cpp, hpp, cc and cxx source files
format\:format:
	find src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' | xargs clang-format -i -style=file
