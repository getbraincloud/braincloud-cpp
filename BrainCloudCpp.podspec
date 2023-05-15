#
#  BrainCloudSDK.podspec
#  BrainCloudSDK C++
#
#  Copyright (c) 2016 BitHeads Inc. All rights reserved.
#

Pod::Spec.new do |s|

  # ―――  Spec Metadata  ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.name     = "BrainCloudCpp"

  s.version  = "4.14.1"

  s.summary  = "The C++ client library for brainCloud"
  s.homepage = "http://getbraincloud.com/"

  s.ios.deployment_target  = "11.0"
  s.osx.deployment_target  = "10.13"
  s.tvos.deployment_target = "11.0"
  s.watchos.deployment_target = "4.0"

  # ―――  Spec License  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.license                = {:type => "Apache License, Version 2.0", :file => "LICENSE"}

  # ――― Author Metadata  ――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.authors                = {"brainCloud Team" => "support@getbraincloud.com"}
  s.social_media_url       = "https://twitter.com/braincloudbaas"

  # ――― Source Location ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.source                 = {:git => "https://github.com/getbraincloud/braincloud-cpp.git", :tag => s.version}

  # ――― Source Code ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.header_mappings_dir     = "include"
  s.preserve_paths          = "include/*", "include/**/*"
  s.public_header_files     = "include/braincloud/*.h", "include/braincloud/internal/*.h", "include/braincloud/internal/apple/*.h"
  s.source_files            = "src/*.{c,cpp}", "src/apple/*.{c,cpp,mm}", "include/braincloud/*.h", "include/braincloud/internal/*.h", "include/braincloud/internal/apple/*.h"
  s.exclude_files           = "src/DefaultSaveDataHelper.cpp", "src/DefaultGUID.cpp" , "src/DefaultFileUploader.cpp", "src/DefaultWebSocket.cpp", "src/DefaultPinger.cpp"

  # hack for use_frameworks!
  s.xcconfig = {
    'USER_HEADER_SEARCH_PATHS' => '"${PODS_ROOT}/BrainCloudCpp/include"'
  }

  # ――― Project Linking ―――――――――――――――――――――――――――――――――――――――――――――――――――――――――― #

  s.libraries               = 'c++', 'z'
  s.osx.framework           = 'LDAP'
  s.dependency                'Lockbox', '~> 3.0.6'
  s.dependency                'BrainCloudJsonCpp'
  s.ios.dependency            'SocketRocket', '~> 0.6'
  s.osx.dependency            'SocketRocket', '~> 0.6'
  #s.watchos.dependency       'SocketRocket', '~> 0.6'
  s.tvos.dependency           'SocketRocket', '~> 0.6'
end
