.PHONY: deps\:build deps\:run deps\:all build run clean

### Install dependencies to build the executable
deps\:build:
	apt-get install -y \
	    build-essential \
    	cmake \
    	git \
		libgl1-mesa-dev \
    	libgpiod-dev \
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
		clang-format

### Install dependencies to build and run the executable
deps\:all: deps\:build deps\:run

### Build the executable file
build:
	cmake -B build
	cmake --build build

### Build and run the executable
run: build
	build/src/app/dieklingel-core

### Install the executable into a bin location
install: build
	make -C build install

### Cleanup any existing build files
clean:
	rm -rf build

### Check the format of all cpp, hpp, cc and cxx files
format\:check:
	find src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' | xargs clang-format --dry-run -Werror -i -style=Google

### Format all cpp, hpp, cc and cxx source files
format\:format:
	find src -regex '.*\.\(cpp\|hpp\|cc\|cxx\)' | xargs clang-format -i -style=Google
