#EXAMPLE Linux Commandline
```
# Generate makefiles
mkdir build
cd build
cmake -DSSL_ALLOW_SELFSIGNED=ON -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Release ..
# Build
cmake --build . --target bctests --config Release
# write ids.txt file to tests folder
# Run tests
./bctests --test_output=all --gtest_output=xml:results.xml --gtest_filter=*TestBCRelayComms*
```

#EXAMPLE xcode generator
```
mkdir build
cd build
cmake -G Xcode -DBUILD_TESTS=ON ..
# open this generated project file in xcode and build
cd tests/Debug
# place the ids.txt file here
cp ../../../../../../ids.txt .
```

#EXAMPLE Windows
```
REM Generate the project
mkdir build
cd build
cmake -DSSL_ALLOW_SELFSIGNED=ON -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_TESTS=ON ..
REM Open in visual studio or build at commandline
cmake --build . --target bctests --config Debug
cd build
cd tests
cd Debug
REM Create ids.txt
bctests.exe --test_output=all --gtest_output=xml:results.xml --gtest_filter=*TestBC%TEST_NAME%*
 ```

#Watch For:
```
ERROR: Failed to load ids.txt file! this means that you either don’t have the file in the right spot OR you don’t have the path correct in source code
```
