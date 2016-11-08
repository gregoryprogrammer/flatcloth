rm -rf build_debug/
mkdir build_debug
cd build_debug
cmake -DCMAKE_BUILD_TYPE=Debug ..
make
