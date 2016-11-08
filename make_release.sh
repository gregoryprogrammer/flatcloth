rm -rf build_release/
mkdir build_release
cd build_release
cmake -DCMAKE_BUILD_TYPE=Release ..
make
