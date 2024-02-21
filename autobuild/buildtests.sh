#!/bin/bash
# execute from cmake directory

#!/bin/bash
# execute from cmake directory

# Generate makefiles
mkdir -p build
pushd build
if [ "$1" != "-noclean" ]; then
  rm -rf *
fi
cmake -GNinja -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build
cmake --build . --target bctests --config Debug -j 8

popd