.ONESHELL:

build: FORCE
	conan install .  --build=missing
	cd build/Release
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	cmake --build .
build-debug: FORCE
	conan install . --build=missing --settings=build_type=Debug
	cd build/Debug
	cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -Dtools.system.package_manager:mode=install
	cmake --build .
run: build-debug
	 ./build/Debug/nand2tetris-image-converter data/cactus.png

clean:
	rm -rf build
test: build FORCE
	cd  ./build/Release && ctest --verbose
test-debug: build FORCE
	cd  ./build/Debug && ctest --output-on-failure

install: build FORCE
	cmake --install ./build/Release --config Release
test-prog: build FORCE
	./build/Release/test_prog
FORCE: ;

emscripten: clean
	conan install . -pr:b default -pr:h emscripten.profile -s build_type=Release -b missing 
	cd build/Release
	emcmake cmake ../..
	# cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_TOOLCHAIN_FILE=/home/babkamen/git/emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DCMAKE_BUILD_TYPE=Debug -G Ninja -Dtools.system.package_manager:mode=install
	# cmake  --build ../..     -DCMAKE_BUILD_TYPE=Release -G Ninja
	# cd build  
	# emcmake cmake ..  
	# make
	# emcmake cmake ../.. -DCMAKE_TOOLCHAIN_FILE=generators/conan_toolchain.cmake -DCMAKE_BUILD_TYPE=Release -G Ninja
	# emcmake cmake --build .