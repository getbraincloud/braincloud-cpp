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

  s.summary  = "The C++ client library for brainCloud"
  s.homepage = "http://getbraincloud.com/"

  s.ios.deployment_target      = "12.0"
  s.osx.deployment_target      = "10.13"
  s.tvos.deployment_target     = "12.0"
  s.visionos.deployment_target = "1.0"
  s.watchos.deployment_target  = "4.0"

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
  s.source_files            = "src/*.{c,cpp}", "src/apple/*.{c,cpp,mm,m}", "include/braincloud/*.h", "include/braincloud/internal/*.h", "include/braincloud/internal/apple/*.h", "src/apple/SAMKeychain/*.{h,m}"
  s.exclude_files           = "src/DefaultSaveDataHelper.cpp", "src/DefaultGUID.cpp" , "src/DefaultFileUploader.cpp", "src/DefaultWebSocket.cpp", "src/DefaultPinger.cpp", "src/DefaultRelayWSSocket.cpp", "include/braincloud/internal/DefaultRelayWSSocket.h"
  s.xcconfig = {
      'USER_HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/BrainCloudCpp/include" "${PODS_ROOT}/BrainCloudCpp/src/apple"'
  }

  # ――― Project Linking ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.libraries               = 'c++', 'z'
  s.osx.framework           = 'LDAP'
  s.dependency                'BrainCloudJsonCpp', '~>6.0.0'
end
