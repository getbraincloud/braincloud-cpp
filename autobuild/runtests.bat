::@echo off
::execute from cmake directory
rmdir /S /Q build
mkdir build
pushd build
copy /Y ..\autobuild\ids.txt .
cmake -DSSL_ALLOW_SELFSIGNED=ON -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --target bctests --config Debug
tests\Debug\bctests.exe --test_output=all --gtest_output=xml:tests\results.xml --gtest_filter=*TestBC%1*
popd
