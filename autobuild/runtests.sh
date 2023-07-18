#!/bin/bash
# execute from cmake directory

./autobuild/buildtests.sh

pushd build

cp ../autobuild/ids.txt .

# Run tests
./tests/bctests --test_output=all --gtest_output=xml:tests/results.xml --gtest_filter=*TestBC${1}*

popd