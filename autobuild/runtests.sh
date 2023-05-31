				#!/bin/bash
				# Generate makefiles
				mkdir -p build
				pushd build
				rm -rf *	
				cmake -GNinja -DBUILD_TESTS=ON -DCMAKE_BUILD_TYPE=Debug ..

				# Build
				cmake --build . --target bctests --config Debug -j 8

				cp ../ids.txt .

				# Run tests
				./tests/bctests --test_output=all --gtest_output=xml:tests/results.xml --gtest_filter=*TestBC${1}*
				
				popd