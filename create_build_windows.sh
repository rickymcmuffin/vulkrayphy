mkdir -p build_windows

cd build_windows

cmake -G "Unix Makefiles" \
      -DCMAKE_SYSTEM_NAME=Windows \
      -D CMAKE_C_COMPILER=x86_64-w64-mingw32-gcc \
      -D CMAKE_CXX_COMPILER=x86_64-w64-mingw32-g++ ..
