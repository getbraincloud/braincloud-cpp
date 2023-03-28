#!/bin/bash

# Generate makefiles
mkdir build
cd build

cmake -GXcode -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..

cmake --build . --target bctests --config Debug

# write ids.txt file to tests folder
cd tests/Debug
echo "serverUrl=${BC_BRAINCLOUD_SERVER_URL}" >ids.txt
echo "appId=${BC_CLIENTUNIT_APP_ID}" >>ids.txt
echo "secret=${BC_CLIENTUNIT_APP_SECRET}" >>ids.txt
echo "version=1.0.0" >>ids.txt
echo "childAppId=${BC_CLIENTUNIT_CHILD_APP_ID}" >>ids.txt
echo "childSecret=${BC_CLIENTUNIT_SERVER_SECRET}" >>ids.txt
echo "parentLevelName=Master" >>ids.txt
echo "peerName=peerapp" >>ids.txt
echo "redirectAppId=25176" >>ids.txt

# Run tests
./bctests --test_output=all --gtest_output=xml:results.xml --gtest_filter=*TestBCAuth*
