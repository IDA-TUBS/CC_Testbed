rm -rf build
mkdir build
cd build
cmake .. -DLOG=ON -DFILE=ON -DCONSOLE=ON
cmake --build . -j6