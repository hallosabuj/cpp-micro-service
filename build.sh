rm -rf build/
mkdir build
cmake -Bbuild -DCMAKE_PREFIX_PATH=$CMAKE_INSTALL_PATH .
cmake --build build