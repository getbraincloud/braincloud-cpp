#!/bin/bash

# Generate makefiles
mkdir -p build
cd build

cmake -GXcode -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake --build . --target bctests --config Debug

# write ids.txt file to tests folder
cd tests/Debug

cp ../../../autobuild/ids.txt .

# Run tests
./bctests --test_output=all --gtest_output=xml:results.xml --gtest_filter=*TestBCAuth*
