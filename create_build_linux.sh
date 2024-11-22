mkdir -p build_linux

cd build_linux

cmake -G "Unix Makefiles" \
      -D CMAKE_C_COMPILER=gcc \
      -D CMAKE_CXX_COMPILER=g++ ..
