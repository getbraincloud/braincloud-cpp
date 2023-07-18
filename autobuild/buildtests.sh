#!/bin/bash
# execute from cmake directory

#!/bin/bash
# execute from cmake directory

# Generate makefiles
mkdir -p build
pushd build
rm -rf *	
cmake -GNinja -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..

# Build
cmake --build . --target bctests --config Debug -j 8

popd