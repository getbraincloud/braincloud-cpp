project(brainCloud)

if (CMAKE_SYSTEM_NAME STREQUAL WindowsStore)
    set(UWP 1)
endif()

if (CMAKE_SYSTEM_NAME STREQUAL Android)
    set(ANDROID 1)
    set(BC_USE_OPENSSL 1)
endif()

if (CMAKE_GENERATOR STREQUAL Xcode)
    set(XCODE 1)
endif()

if (UWP)
    # We require 3.4.0 for UWP generation
    cmake_minimum_required(VERSION 3.4.0)
else()
    cmake_minimum_required(VERSION 3.0.0)
endif()

if(COMMAND cmake_policy)
    cmake_policy(SET CMP0022 NEW)
    cmake_policy(SET CMP0079 NEW)
    cmake_policy(SET CMP0003 NEW)
endif()


# Setup modules path
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH}
        "${CMAKE_CURRENT_SOURCE_DIR}/cmake/")

# Usefull flags
if (UNIX AND NOT APPLE AND NOT ANDROID)
    set(LINUX 1)
endif()

#=============================================================================
# CMake options
#=============================================================================
option(BUILD_TESTS "brainCloud Unit Tests" OFF)
option(USE_CURL_WIN "Force use libCurl on Win32" OFF)

if(DEFINED SSL_ALLOW_SELFSIGNED)
    set(SSL_ALLOW_SELFSIGNED 1)
else()
    set(SSL_ALLOW_SELFSIGNED 0)
endif()

#=============================================================================
# Definitions
#=============================================================================
add_definitions(-DUSE_IMMEDIATE_RETRY)
add_definitions(-DGAMECLIENT_DEBUGLEVEL=1)
if (UWP)
    add_definitions(-DBC_UWP)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS) # sprintf instead of sprintf_s
    if (BUILD_TESTS)
        # add_definitions(-Dgtest_force_shared_crt=ON)
        option(
                gtest_force_shared_crt
                "Use shared (DLL) run-time lib even when Google Test is built as static lib."
                ON)
    endif()
endif()

if (SSL_ALLOW_SELFSIGNED)
    message("brainCloud Skipping certificate verification in RTT")
    add_definitions(-DBC_SSL_ALLOW_SELFSIGNED)
    else()
    message("brainCloud Building with certificates for RTT")
endif()

# lets use libwebsockets by default
if (NOT UWP)
    set(USE_LIBWEBSOCKETS 1)
else()
    add_definitions(-DLIBWEBSOCKETS_OFF)
endif()

if(BC_USE_OPENSSL)
    message("brainCloud Building with OpenSSL TLS")
    add_definitions(-DBC_MBEDTLS_OFF)
else()
    message("brainCloud Building with MbedTLS TLS")
endif()

#=============================================================================
# Compiler flags
#=============================================================================
# We want to use C++11 flag on the compiler
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)

#=============================================================================
# Unit Tests
#=============================================================================
if (DEFINED(${BUILD_TESTS}) OR ${BUILD_TESTS})
    message("brainCloud Building with BUILD_TESTS ON")
    add_subdirectory("${CMAKE_CURRENT_SOURCE_DIR}/tests")
endif()

#=============================================================================
# Braincloud client
#=============================================================================
set (BC_DIR "${PROJECT_SOURCE_DIR}")
set (OS_SPECIFIC_SRCS "")
set (OS_SPECIFIC_INCS "")
set (ZLIB_INCLUDE_DIR "${BC_DIR}/include/zlib")

file(GLOB_RECURSE JSONCPP_INCS_FILES "${BC_DIR}/lib/jsoncpp-1.0.0/*.h")
file(GLOB_RECURSE JSONCPP_SRCS_FILES "${BC_DIR}/lib/jsoncpp-1.0.0/*.cpp")

# HTTP related files
if (APPLE)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/apple/nsFileUploader.h"
            "include/braincloud/internal/apple/nsPinger.h"
            "include/braincloud/internal/apple/nsURLLoader.h")
    list(APPEND OS_SPECIFIC_SRCS
            "src/apple/nsFileUploader.mm"
            "src/apple/nsPinger.mm"
            "src/apple/nsURLLoader.mm")
elseif (MSVC AND NOT USE_CURL_WIN)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/win/XMLHTTPRequestCallback.h"
            "include/braincloud/internal/win/XMLHTTPRequestFileUploader.h"
            "include/braincloud/internal/win/XMLHTTPRequestLoader.h"
            "include/braincloud/internal/win/XMLHTTPRequestPinger.h")
    list(APPEND OS_SPECIFIC_SRCS
            "src/win/XMLHTTPRequestCallback.cpp"
            "src/win/XMLHTTPRequestFileUploader.cpp"
            "src/win/XMLHTTPRequestLoader.cpp"
            "src/win/XMLHTTPRequestPinger.cpp")
else()
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/nix/cURLFileUploader.h"
            "include/braincloud/internal/nix/cURLLoader.h"
            "include/braincloud/internal/nix/cURLPinger.h")
    list(APPEND OS_SPECIFIC_SRCS
            "src/nix/cURLFileUploader.cpp"
            "src/nix/cURLLoader.cpp"
            "src/nix/cURLPinger.cpp")
endif()

# Sockets
if (USE_LIBWEBSOCKETS)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/DefaultWebSocket.h")
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultWebSocket.cpp")
endif()
if (WIN32)
    list(APPEND OS_SPECIFIC_SRCS "src/win/Win32TCPSocket.cpp")
    list(APPEND OS_SPECIFIC_SRCS "src/win/Win32RelayTCPSocket.cpp")
    list(APPEND OS_SPECIFIC_SRCS "src/win/Win32RelayUDPSocket.cpp")
    if (UWP)
        list(APPEND OS_SPECIFIC_INCS
                "include/braincloud/internal/win/UWPWebSocket.h")
        list(APPEND OS_SPECIFIC_SRCS "src/win/UWPWebSocket.cpp")
    endif()
else()
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultTCPSocket.cpp")
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultRelayTCPSocket.cpp")
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultRelayUDPSocket.cpp")
endif()

# Guid
if (APPLE)
    list(APPEND OS_SPECIFIC_SRCS "src/apple/GUID.mm")
elseif (WIN32)
    list(APPEND OS_SPECIFIC_SRCS "src/win/WindowsGUID.cpp")
elseif (LINUX)
    list(APPEND OS_SPECIFIC_SRCS "src/nix/LinuxGUID.cpp")
else()
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultGUID.cpp")
endif()

# Save data helper
if (APPLE)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/apple/AppleSaveDataHelper.h")
    list(APPEND OS_SPECIFIC_SRCS "src/apple/AppleSaveDataHelper.mm")
elseif (ANDROID)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/android/AndroidSaveDataHelper.h")
    list(APPEND OS_SPECIFIC_SRCS "src/android/AndroidSaveDataHelper.cpp")
elseif (WIN32)
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/win/WindowsSaveDataHelper.h")
    list(APPEND OS_SPECIFIC_SRCS "src/win/WindowsSaveDataHelper.cpp")
else()
    list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/DefaultSaveDataHelper.h")
    list(APPEND OS_SPECIFIC_SRCS "src/DefaultSaveDataHelper.cpp")
endif()

# Device
if (APPLE)
    list(APPEND OS_SPECIFIC_SRCS "src/apple/AppleDevice.cpp")
elseif (WIN32)
    list(APPEND OS_SPECIFIC_SRCS "src/win/WindowsDevice.cpp")
elseif (LINUX)
    list(APPEND OS_SPECIFIC_SRCS "src/nix/LinuxDevice.cpp")
elseif (ANDROID)
    list(APPEND OS_SPECIFIC_SRCS "src/android/AndroidDevice.cpp")
endif()

# other platform specific srcs/includes
if (APPLE)
    list(APPEND OS_SPECIFIC_SRCS
            "lib/apple/sskeychain/SSKeychain/SSKeychain.m"
            "lib/apple/sskeychain/SSKeychain/SSKeychainQuery.m")
    list(APPEND OS_SPECIFIC_INCS
            "lib/apple/sskeychain/SSKeychain/SSKeychain.h"
            "lib/apple/sskeychain/SSKeychain/SSKeychainQuery.h")
    # for memory management in obj-c files
    set_source_files_properties(
            "lib/apple/sskeychain/SSKeychain/SSKeychain.m"
            "lib/apple/sskeychain/SSKeychain/SSKeychainQuery.m"
            PROPERTIES COMPILE_FLAGS "-fobjc-arc")
    list(APPEND includes PUBLIC "lib/apple/sskeychain")
elseif (WIN32)
    if (UWP)
        list(APPEND OS_SPECIFIC_INCS
                "include/braincloud/internal/win/MD5.h")
        list(APPEND OS_SPECIFIC_SRCS "src/win/MD5.cpp")
    endif()
elseif (ANDROID)
            list(APPEND OS_SPECIFIC_INCS
            "include/braincloud/internal/android/com_bitheads_braincloud_AndroidBridge.h"
            "include/braincloud/internal/android/AndroidGlobals.h")
            list(APPEND OS_SPECIFIC_SRCS
                    "src/android/com_bitheads_braincloud_AndroidBridge.cpp")
endif()

if (NOT UWP)
    list(APPEND OS_SPECIFIC_SRCS "src/md5x.c") # UWP doesn't like c files
endif()

list(APPEND includes PUBLIC "include")
list(APPEND includes PUBLIC "lib/jsoncpp-1.0.0")

add_library(brainCloud STATIC
        # public headers
        include/braincloud/AuthenticationIds.h
        include/braincloud/AuthenticationType.h
        include/braincloud/BrainCloudAppStore.h
        include/braincloud/BrainCloudAsyncMatch.h
        include/braincloud/BrainCloudAuthentication.h
        include/braincloud/BrainCloudBlockchain.h
        include/braincloud/BrainCloudChat.h
        include/braincloud/BrainCloudClient.h
        include/braincloud/BrainCloudCustomEntity.h
        include/braincloud/BrainCloudDataStream.h
        include/braincloud/BrainCloudEntity.h
        include/braincloud/BrainCloudEvent.h
        include/braincloud/BrainCloudFile.h
        include/braincloud/BrainCloudFriend.h
        include/braincloud/BrainCloudGamification.h
        include/braincloud/BrainCloudGlobalApp.h
        include/braincloud/BrainCloudGlobalEntity.h
        include/braincloud/BrainCloudGlobalFile.h
        include/braincloud/BrainCloudGlobalStatistics.h
        include/braincloud/BrainCloudGroup.h
        include/braincloud/BrainCloudGroupFile.h
        include/braincloud/BrainCloudIdentity.h
        include/braincloud/BrainCloudItemCatalog.h
        include/braincloud/BrainCloudLobby.h
        include/braincloud/BrainCloudMail.h
        include/braincloud/BrainCloudMatchmaking.h
        include/braincloud/BrainCloudMessaging.h
        include/braincloud/BrainCloudOneWayMatch.h
        include/braincloud/BrainCloudPlaybackStream.h
        include/braincloud/BrainCloudPlayerState.h
        include/braincloud/BrainCloudPlayerStatistics.h
        include/braincloud/BrainCloudPlayerStatisticsEvent.h
        include/braincloud/BrainCloudPresence.h
        include/braincloud/BrainCloudProfanity.h
        include/braincloud/BrainCloudPushNotification.h
        include/braincloud/BrainCloudRTT.h
        include/braincloud/BrainCloudRedemptionCode.h
        include/braincloud/BrainCloudRelay.h
        include/braincloud/BrainCloudS3Handling.h
        include/braincloud/BrainCloudScript.h
        include/braincloud/BrainCloudSocialLeaderboard.h
        include/braincloud/BrainCloudTime.h
        include/braincloud/BrainCloudTournament.h
        include/braincloud/BrainCloudTypes.h
        include/braincloud/BrainCloudUserItems.h
        include/braincloud/BrainCloudVirtualCurrency.h
        include/braincloud/BrainCloudWrapper.h
        include/braincloud/FriendPlatform.h
        include/braincloud/IEventCallback.h
        include/braincloud/IFileUploadCallback.h
        include/braincloud/IGlobalErrorCallback.h
        include/braincloud/INetworkErrorCallback.h
        include/braincloud/IRewardCallback.h
        include/braincloud/IServerCallback.h
        include/braincloud/OperationParam.h
        include/braincloud/Platform.h
        include/braincloud/ServerCall.h
        include/braincloud/ServiceName.h
        include/braincloud/ServiceOperation.h
        include/braincloud/http_codes.h
        include/braincloud/reason_codes.h

        # private headers
        include/braincloud/internal/DataUtilities.h
        include/braincloud/internal/DefaultBrainCloudComms.h
        include/braincloud/internal/DefaultPinger.h
        include/braincloud/internal/DefaultRelayTCPSocket.h
        include/braincloud/internal/DefaultRelayUDPSocket.h
        include/braincloud/internal/DefaultTCPSocket.h
        include/braincloud/internal/DefaultWebSocket.h
        include/braincloud/internal/Device.h
        include/braincloud/internal/GUID.h
        include/braincloud/internal/IBrainCloudComms.h
        include/braincloud/internal/IFileUploader.h
        include/braincloud/internal/IPinger.h
        include/braincloud/internal/IRelaySocket.h
        include/braincloud/internal/IRelayTCPSocket.h
        include/braincloud/internal/IRelayUDPSocket.h
        include/braincloud/internal/ITCPSocket.h
        include/braincloud/internal/JsonUtil.h
        include/braincloud/internal/Mutex.h
        include/braincloud/internal/RTTComms.h
        include/braincloud/internal/RelayComms.h
        include/braincloud/internal/SaveDataHelper.h
        include/braincloud/internal/StringUtil.h
        include/braincloud/internal/TimeUtil.h
        include/braincloud/internal/TimerCallback.h
        include/braincloud/internal/URLLoader.h
        include/braincloud/internal/URLLoaderClient.h
        include/braincloud/internal/URLRequest.h
        include/braincloud/internal/URLRequestHeader.h
        include/braincloud/internal/URLRequestMethod.h
        include/braincloud/internal/URLResponse.h
        include/braincloud/internal/md5x.h
        include/braincloud/internal/timegm.h

        # source files
        src/AuthenticationType.cpp
        src/BrainCloudAppStore.cpp
        src/BrainCloudAsyncMatch.cpp
        src/BrainCloudAuthentication.cpp
        src/BrainCloudBlockchain.cpp
        src/BrainCloudChat.cpp
        src/BrainCloudClient.cpp
        src/BrainCloudCustomEntity.cpp
        src/BrainCloudDataStream.cpp
        src/BrainCloudEntity.cpp
        src/BrainCloudEvent.cpp
        src/BrainCloudFile.cpp
        src/BrainCloudFriend.cpp
        src/BrainCloudGamification.cpp
        src/BrainCloudGlobalApp.cpp
        src/BrainCloudGlobalEntity.cpp
        src/BrainCloudGlobalFile.cpp
        src/BrainCloudGlobalStatistics.cpp
        src/BrainCloudGroup.cpp
        src/BrainCloudGroupFile.cpp
        src/BrainCloudIdentity.cpp
        src/BrainCloudItemCatalog.cpp
        src/BrainCloudLobby.cpp
        src/BrainCloudMail.cpp
        src/BrainCloudMatchmaking.cpp
        src/BrainCloudMessaging.cpp
        src/BrainCloudOneWayMatch.cpp
        src/BrainCloudPlaybackStream.cpp
        src/BrainCloudPlayerState.cpp
        src/BrainCloudPlayerStatistics.cpp
        src/BrainCloudPlayerStatisticsEvent.cpp
        src/BrainCloudPresence.cpp
        src/BrainCloudProfanity.cpp
        src/BrainCloudPushNotification.cpp
        src/BrainCloudRTT.cpp
        src/BrainCloudRedemptionCode.cpp
        src/BrainCloudRelay.cpp
        src/BrainCloudS3Handling.cpp
        src/BrainCloudScript.cpp
        src/BrainCloudSocialLeaderboard.cpp
        src/BrainCloudTime.cpp
        src/BrainCloudTournament.cpp
        src/BrainCloudUserItems.cpp
        src/BrainCloudVirtualCurrency.cpp
        src/BrainCloudWrapper.cpp
        src/DefaultBrainCloudComms.cpp
        src/DataUtilities.cpp
        src/FriendPlatform.cpp
        src/GUID.cpp
        src/IBrainCloudComms.cpp
        src/JsonUtil.cpp
        src/OperationParam.cpp
        src/Platform.cpp
        src/RTTComms.cpp
        src/RelayComms.cpp
        src/SaveDataHelper.cpp
        src/ServerCall.cpp
        src/ServiceName.cpp
        src/ServiceOperation.cpp
        src/StringUtil.cpp
        src/TimeUtil.cpp
        src/TimerCallback.cpp
        src/URLLoader.cpp
        src/URLLoaderClient.cpp
        src/URLRequest.cpp
        src/URLRequestHeader.cpp
        src/URLRequestMethod.cpp
        src/URLResponse.cpp

        ${OS_SPECIFIC_INCS}
        ${OS_SPECIFIC_SRCS}
        ${JSONCPP_INCS_FILES}
        ${JSONCPP_SRCS_FILES}
        )

# Set files to be "consume windows runtime exceptions" to build them with CX
if (UWP)
    set_source_files_properties(
            "src/win/WindowsSaveDataHelper.cpp"
            "src/win/UWPWebSocket.cpp"
            PROPERTIES COMPILE_FLAGS "/ZW")
endif()

#=============================================================================
# Libraries
#=============================================================================
if (APPLE)
    find_library(BC_GAMEKIT_LIB GameKit)
    find_library(BC_COREFOUNDATION_LIB CoreFoundation)
    find_library(BC_FOUNDATION_LIB Foundation)
    find_library(BC_SYSCONFIG_LIB SystemConfiguration)
    find_library(BC_SECURITY_LIB Security)
	find_library(BC_Z_LIB z)
				
    list(APPEND libs
            ${BC_GAMEKIT_LIB}
            ${BC_COREFOUNDATION_LIB}
            ${BC_FOUNDATION_LIB}
            ${BC_SYSCONFIG_LIB}
            ${BC_SECURITY_LIB}
			${BC_Z_LIB})
elseif (LINUX)
    find_package(CURL REQUIRED)
    list(APPEND includes PUBLIC ${CURL_INCLUDE_DIRS})

    find_package(UUID REQUIRED)
    list(APPEND includes PUBLIC ${UUID_INCLUDE_DIRS})
	
	find_package(ZLIB REQUIRED)  # Use the system zlib
    include_directories(${ZLIB_INCLUDE_DIR})  # Ensure the headers are found

    list(APPEND libs
            ${CURL_LIBRARIES}
            ${UUID_LIBRARIES}
			${ZLIB_LIBRARIES})
			
elseif (ANDROID)
    list(APPEND includes PUBLIC "lib/android/include/")

    add_library(libz STATIC IMPORTED)
    set_target_properties(libz PROPERTIES IMPORTED_LOCATION ${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libz.a)
    add_library(curl STATIC IMPORTED)
    set_target_properties(curl PROPERTIES IMPORTED_LOCATION ${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libcurl.a)
    add_library(ssl STATIC IMPORTED)
    set_target_properties(ssl PROPERTIES IMPORTED_LOCATION ${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libssl.a)
    add_library(crypto STATIC IMPORTED)
    set_target_properties(crypto PROPERTIES IMPORTED_LOCATION ${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libcrypto.a)
    add_library(uv_a STATIC IMPORTED)
    set_target_properties(uv_a PROPERTIES IMPORTED_LOCATION ${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libuv_a.a)

    list(APPEND libs
            curl
            ssl
            crypto
            uv_a
            libz
            )

elseif (WIN32)

	find_library(BC_ZLIB zlib
			"${BC_DIR}/lib/zlib")
	
	include_directories(${ZLIB_INCLUDE_DIR})  # Ensure the headers are found
	
	if(NOT BC_ZLIB)
		message(FATAL_ERROR "ZLIB NOT FOUND IN ${BC_DIR}/lib/zlib")
	else()
		message(STATUS "FOUND ZLIB: ${BC_ZLIB}")
	endif()
	
	list(APPEND libs 
				${BC_ZLIB})
	
    if (USE_CURL_WIN)
        # pthread
        include_directories(
                "${BC_DIR}/lib/win32/pthread-w32-2-8-0/Pre-built.2/include")
        add_definitions(-DWIN32 -D_LIB -DPTW32_STATIC_LIB)
        find_library(BC_PTHREADW32_LIB pthreadVCE2
                "${BC_DIR}/lib/win32/pthread-w32-2-8-0/Pre-built.2/lib")

        # curl
        add_definitions(-DCURL_STATICLIB)
        include_directories("${BC_DIR}/lib/win32/curl-7.44.0/include")
        find_library(BC_CURL_LIB_RELEASE libcurl_a.lib
                "${BC_DIR}/lib/win32/curl-7.44.0/lib")
        find_library(BC_CURL_LIB_DEBUG libcurl_a_debug.lib
                "${BC_DIR}/lib/win32/curl-7.44.0/lib")
		

        list(APPEND libs general
                ${Socket_LIBRARIES}
                ${BC_PTHREADW32_LIB}
                optimized ${BC_CURL_LIB_RELEASE}
                debug ${BC_CURL_LIB_DEBUG})
    else()
        message(status "Using msxml6 socket library.")
        find_package(Socket)
        list(APPEND libs msxml6 ${Socket_LIBRARIES})
    endif()
endif()

# android uses openssl instead of tls (pre-built libwebsockets.a is linked above)
if (USE_LIBWEBSOCKETS)
    set(LWS_WITH_SSL ON CACHE BOOL "Enable SSL abstraction" FORCE)
    # TLS
    option(ENABLE_TESTING "" OFF)
    option(ENABLE_PROGRAMS "" OFF)

    add_compile_definitions(CACERTS_FILE_PATH="${CMAKE_SOURCE_DIR}/resources/cacerts.pem")
    message("CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

	if (BC_USE_OPENSSL)

        if(ANDROID)
    		set(LWS_OPENSSL_LIBRARIES "${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libssl.a;${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libcrypto.a")
	    	set(LWS_OPENSSL_INCLUDE_DIRS "${BC_DIR}/lib/android/include/")
            set(OPENSSL_LIBRARIES "${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libssl.a;${BC_DIR}/lib/android/libs/${ANDROID_ABI}/libcrypto.a")
            set(OPENSSL_INCLUDE_DIRS "${BC_DIR}/lib/android/include/")
            set(OPENSSL_FOUND 1)
            option(LWS_SSL_CLIENT_USE_OS_CA_CERTS "SSL support should make use of the OS-installed CA root certs" OFF)
        else()
            # Point CMake to vcpkg for OpenSSL only
            find_package(OpenSSL REQUIRED)

            # Include dirs and libraries from vcpkg
            set(LWS_OPENSSL_LIBRARIES $<BUILD_INTERFACE:${OPENSSL_LIBRARIES}>)
            set(LWS_OPENSSL_INCLUDE_DIRS $<BUILD_INTERFACE:${OPENSSL_INCLUDE_DIR}>)
            
            include_directories($<BUILD_INTERFACE:${LWS_OPENSSL_INCLUDE_DIRS}>)
            
            # Force LWS to use OpenSSL
            set(LWS_WITH_SSL ON CACHE BOOL "Use OpenSSL" FORCE)
            set(LWS_WITH_MBEDTLS OFF CACHE BOOL "Disable MbedTLS" FORCE)
        endif()

        option(LWS_WITH_SSL "Include SSL support (defaults to OpenSSL)" ON)
        option(LWS_WITH_MBEDTLS "Disable MbedTLS" OFF)

	else()
		# TLS
		option(ENABLE_TESTING "" OFF)
		option(ENABLE_PROGRAMS "" OFF)

        # mbedtls-2.28 (SHA1 is 1963d67e)
        set(BC_MBEDTLS_DIR lib/mbedtls)

		add_subdirectory("${BC_MBEDTLS_DIR}")

		list(APPEND libs PUBLIC mbedtls mbedx509 mbedcrypto)
		list(APPEND includes PUBLIC ${BC_MBEDTLS_DIR}/include/)
				
		set(LWS_WITH_MBEDTLS ON CACHE BOOL "Use MbedTLS" FORCE)
		
		#set(LWS_MBEDTLS_INCLUDE_DIRS "${CMAKE_CURRENT_SOURCE_DIR}/${BC_MBEDTLS_DIR}/include" CACHE PATH "MbedTLS include dir" FORCE)
		
		set(LWS_MBEDTLS_INCLUDE_DIRS "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/${BC_MBEDTLS_DIR}/include>")
        if(WIN32)
            #set(LWS_MBEDTLS_LIBRARIES "${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/${CMAKE_BUILD_TYPE}/mbedtls.lib;${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/${CMAKE_BUILD_TYPE}/mbedcrypto.lib;${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/${CMAKE_BUILD_TYPE}/mbedx509.lib")
            set(LWS_MBEDTLS_LIBRARIES
                    "${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/$<CONFIG>/mbedtls.lib"
                    "${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/$<CONFIG>/mbedcrypto.lib"
                    "${CMAKE_CURRENT_BINARY_DIR}/${BC_MBEDTLS_DIR}/library/$<CONFIG>/mbedx509.lib"
                    CACHE STRING "MbedTLS libraries" FORCE
                )
        elseif(XCODE)
            set(LWS_MBEDTLS_LIBRARIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/libmbedtls.a;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/libmbedcrypto.a;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/$CONFIGURATION$EFFECTIVE_PLATFORM_NAME/libmbedx509.a")
        else()
            set(LWS_MBEDTLS_LIBRARIES "$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/libmbedtls.a;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/libmbedcrypto.a;$<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>/${BC_MBEDTLS_DIR}/library/libmbedx509.a")
        endif()
		
		#include_directories(${LWS_MBEDTLS_INCLUDE_DIRS})
		
		message("LWS Configured with MBEDTLS")
	endif()

    # LibWebSockets
	set(BC_LWS_DIR lib/libwebsockets)
	set(BC_LWS_INC ${BC_LWS_DIR}/include)

    option(LWS_WITHOUT_SERVER
            "Don't build the server part of the library" ON)
    option(LWS_WITHOUT_TESTAPPS
            "Don't build the libwebsocket-test-apps" ON)
    option(LWS_WITHOUT_TEST_SERVER
            "Don't build the test server" ON)
    option(LWS_WITHOUT_TEST_SERVER_EXTPOLL
            "Don't build the test server version that uses external poll" ON)
    option(LWS_WITHOUT_TEST_PING
            "Don't build the ping test application" ON)
    option(LWS_WITHOUT_TEST_CLIENT
            "Don't build the client test application" ON)
    option(LWS_WITH_SHARED
            "Build the shared version of the library" OFF)
    option(LWS_WITHOUT_EXTENSIONS "" ON)
    option(LWS_WITH_TLS "" ON)

    add_subdirectory("${BC_LWS_DIR}")

    # turn off 'treat warnings as errors' for libwebsockets in msvc solution (errors on 'size_t' as 'int')
    if (MSVC)
       target_compile_options(websockets PRIVATE /WX-)
       if (NOT BC_USE_OPENSSL)
           target_compile_options(mbedtls PRIVATE /WX-)
           target_compile_options(mbedcrypto PRIVATE /WX-)
       endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang" AND NOT ANDROID)
       target_compile_options(websockets PRIVATE -Wno-unused-but-set-variable -Wno-typedef-redefinition)
       if (NOT BC_USE_OPENSSL)
          target_compile_options(mbedtls PRIVATE -Wno-unused-but-set-variable)
          target_compile_options(mbedcrypto PRIVATE -Wno-unused-but-set-variable)
       endif()
    elseif(CMAKE_CXX_COMPILER_ID MATCHES "Clang")
        target_compile_options(websockets PRIVATE -Wno-typedef-redefinition)
    endif()


    list(APPEND libs websockets)
    list(APPEND includes
            PUBLIC
            ${BC_LWS_INC}
            )

endif()

# Setup includes and linked libraries
target_include_directories(brainCloud PUBLIC "include" ${includes})
target_link_libraries(brainCloud PUBLIC ${libs})


# Compile flags
if (WIN32 AND NOT UWP)
    target_compile_options(brainCloud PRIVATE
            /MP     # Multi-core compilation
            )
elseif (UWP)
    target_compile_options(brainCloud PRIVATE
            /MP     # Multi-core compilation
            /wd4996 # DEPRECATED throws error without this
            )
endif()
