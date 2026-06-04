#
#  BrainCloudSDK.podspec
#  BrainCloudSDK C++
#
#  Copyright (c) 2026 BitHeads Inc. All rights reserved.
#
  
Pod::Spec.new do |s|

  # ―――  Spec Metadata  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.name     = "BrainCloudCpp"

  s.version  = "6.0.0"

  s.summary  = "The C++ client library for brainCloud."
  s.homepage = "http://getbraincloud.com/"

  s.ios.deployment_target      = "15.0"
  s.osx.deployment_target      = "13.0"
  s.tvos.deployment_target     = "15.0"
  s.visionos.deployment_target = "1.0"
  s.watchos.deployment_target  = "8.0"

  # ―――  Spec License  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.license                = {:type => "Apache License, Version 2.0", :file => "LICENSE"}

  # ――― Author Metadata  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.authors                = {"brainCloud Team" => "support@getbraincloud.com"}

  # ――― Source Location ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.source                 = {:git => "https://github.com/getbraincloud/braincloud-cpp.git", :tag => s.version}

  # ――― Source Code ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.header_mappings_dir     = "include"
  s.preserve_paths          = "include/*", "include/**/*"
  s.public_header_files     = "include/braincloud/*.h", "include/braincloud/internal/*.h", "include/braincloud/internal/apple/*.h"
  s.source_files            = "src/*.{c,cpp}", "src/apple/*.{c,cpp,mm}", "include/braincloud/*.h", "include/braincloud/internal/*.h", "include/braincloud/internal/apple/*.h"
  s.exclude_files           = "src/DefaultSaveDataHelper.cpp", "src/DefaultGUID.cpp" , "src/DefaultFileUploader.cpp", "src/DefaultWebSocket.cpp", "src/DefaultPinger.cpp", "src/DefaultRelayWSSocket.cpp", "include/braincloud/internal/DefaultRelayWSSocket.h"

  # for use_frameworks!
  # to use development pod: change below to your full source code path
  # to use cocoapod release: change to be relative to ${PODS_ROOT}
  s.xcconfig = {
        # eg. '"${PODS_ROOT}/BrainCloudCpp/include"', '"/local/path/to/braincloud-cpp/include"'
        'USER_HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/BrainCloudCpp/include"'
  }

  # ――― Project Linking ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.libraries               = 'c++', 'z'
  s.osx.framework           = 'LDAP'
  s.dependency                'BrainCloudJsonCpp', '~>6.0.0'
  s.ios.dependency            'SocketRocket', '~> 0.5'
  s.osx.dependency            'SocketRocket', '~> 0.5'
  s.tvos.dependency           'SocketRocket', '~> 0.5'
  s.visionos.dependency       'SocketRocket', '~> 0.5'
  #s.watchos.dependency        'SocketRocket', '~> 0.5'
end
