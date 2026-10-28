#
#  BrainCloudSocketRocket.podspec
#
#  Unmodified SocketRocket 0.7.1 with minimums raised to what current Xcode supports
#  (upstream's iOS 11 / macOS 10.13 fail trunk validation). Interim until BrainCloudCpp
#  moves to URLSessionWebSocketTask.
#

Pod::Spec.new do |s|
  s.name               = 'BrainCloudSocketRocket'
  s.version            = '0.7.1'
  s.summary            = 'SocketRocket 0.7.1 repackaged for brainCloud with current-Xcode deployment targets.'
  s.homepage           = 'https://github.com/facebookincubator/SocketRocket'
  s.authors            = { 'Nikita Lutsenko' => 'nlutsenko@me.com', 'Dan Federman' => 'federman@squareup.com', 'Mike Lewis' => 'mikelikespie@gmail.com' }
  s.license            = { :type => 'BSD', :file => 'LICENSE' }
  s.source             = { :git => 'https://github.com/facebookincubator/SocketRocket.git', :tag => s.version.to_s }
  s.requires_arc       = true

  # Keep <SocketRocket/SocketRocket.h> imports working (static libs and use_frameworks!).
  s.module_name        = 'SocketRocket'
  s.header_dir         = 'SocketRocket'

  s.source_files       = 'SocketRocket/**/*.{h,m}'
  s.public_header_files = 'SocketRocket/*.h'

  s.ios.deployment_target      = '15.0'
  s.osx.deployment_target      = '12.0'
  s.tvos.deployment_target     = '15.0'
  s.visionos.deployment_target = '1.0'

  s.ios.frameworks     = 'CFNetwork', 'Security'
  s.osx.frameworks     = 'CoreServices', 'Security'
  s.tvos.frameworks    = 'CFNetwork', 'Security'
  s.libraries          = 'icucore'
end
