if (MSVC)
    cmake_minimum_required (VERSION 3.16.0)
else()
    cmake_minimum_required (VERSION 3.2.0)
endif()

project (brainCloudUnitTest)

# xcode generator properties
set(CMAKE_XCODE_GENERATE_SCHEME ON)
# visual studio generator properties
set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT bctests)
    
if (UNIX AND NOT APPLE)
    set(LINUX 1)
    add_definitions(-DBC_TEST_LINUX)
endif()

# Multi core compilation in VS
if (MSVC)
    SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}" "/MP /FS") # Issue with VS2019. Things don't build if generated from cmake without the /FS flag
endif()

################################
# Braincloud
################################

set (BC_DIR "${PROJECT_SOURCE_DIR}/..")
include_directories("${BC_DIR}/lib/jsoncpp-1.0.0")
include_directories("src")

file(GLOB_RECURSE INCS "src/*.h")
file(GLOB_RECURSE SOURCES "src/*.cpp")
file(GLOB_RECURSE BC_INCS "${BC_DIR}/include/*.h")

source_group(braincloud FILES ${BC_INCS})
source_group(headers FILES ${INCS})
source_group(src FILES ${SOURCES})

add_definitions(-D_CRT_SECURE_NO_WARNINGS)

add_executable(bctests ${BC_INCS} ${WRAPPER_INCS} ${INCS} ${SOURCES})

if (APPLE)
	find_library(GAMEKIT_LIB GameKit)
	find_library(COREFOUNDATION_LIB CoreFoundation)
	find_library(FOUNDATION_LIB Foundation)
	find_library(SECURITY_LIB Security)
	target_link_libraries(bctests PUBLIC ${GAMEKIT_LIB} ${COREFOUNDATION_LIB} ${FOUNDATION_LIB} ${SECURITY_LIB})
else()
	set(ZLIB_DLL "${BC_DIR}/lib/zlib/zlib1.dll")
	
	add_custom_command(TARGET bctests POST_BUILD 
		COMMAND ${CMAKE_COMMAND} -E copy_if_different 
		${ZLIB_DLL} $<TARGET_FILE_DIR:bctests> 
		COMMENT "Copying zlib1.dll to ${CMAKE_BINARY_DIR}/$<CONFIG>")
endif()

################################
# GTest
################################

if (MSVC)
if (MSVC_VERSION GREATER_EQUAL 1800)
	# C++11 has tupple since at least Visual Studio 2013. and tr1 is deprecated in VS2017
	add_definitions(-DGTEST_HAS_TR1_TUPLE=0)
endif()
endif()
add_subdirectory(gtest-1.7.0)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
target_link_libraries(bctests PUBLIC gtest gtest_main)

################################
# BrainCloudLib
################################

	message(STATUS "BUILDING WITH BRAINCLOUD LIBRARY")
target_link_libraries(bctests PUBLIC -ldl brainCloud)
