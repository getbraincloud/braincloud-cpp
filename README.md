# brainCloud C++ Library

Thanks for downloading the brainCloud C++ client library! Here are a few notes to get you started. Further information about the brainCloud API, including example Tutorials can be found here:

http://getbraincloud.com/apidocs/

If you haven't signed up or you want to log into the brainCloud portal, you can do that here:

https://portal.braincloudservers.com/

## Build for Windows

Prerequisis:

- git
- Visual Studio 2017. But everything after 2013 should work.
- CMake https://cmake.org/

Steps (Command lines are done in PowerShell):

1. Clone the repository somewhere on your PC, then cd to it.
   ```bash
   git clone https://github.com/getbraincloud/braincloud-cpp.git
   cd braincloud-cpp
   ```
2. Update submodules so thirdparty libraries are present when building.
   ```bash
   git submodule update --init --recursive
   ```
3. Create an out of source build folder, then cd to it.
   ```bash
   mkdir build
   cd build
   ```
4. Generate the solution using cmake.
   ```bash
   cmake ..
   ```
   Or if you are building for **UWP**, do the following instead:
   ```bash
    cmake -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION="10.0" -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_TESTS=ON ..
   ```
5. From file explorer, `"explorer ."`, double click `brainCloud.sln` and open Visual Studio.
6. Build the **brainCloud** project with desired configuration. Release/Debug/static or shared libraries, etc..

## Build for Linux or Mac

Prerequisis:

- git
- A C++ compiler
- CMake https://cmake.org/
- libcurl-dev with openssl
- pthread

Steps (Command lines are done in PowerShell):

1. Clone the repository somewhere on your PC, then cd to it.
   ```bash
   git clone https://github.com/getbraincloud/braincloud-cpp.git
   cd braincloud-cpp
   ```
2. Update submodules so thirdparty libraries are present when building.
   ```bash
   git submodule update --init --recursive
   ```
3. Create an out of source build folder, then cd to it.
   ```bash
   mkdir build
   cd build
   ```
4. Generate the solution using cmake, and specify if you want Debug or Release.
   ```bash
   cmake -DCMAKE_BUILD_TYPE=Release ..
   ```
   Or if you are generating for XCode, do the following:
   ```bash
   cmake -DCMAKE_GENERATOR=Xcode ..
   ```
5. Build brainCloud
   ```bash
   make
   ```

## Build for Android

Do the following step by adding brainCloud directly to your NDK Application's CMake project.

## Add brainCloud to your application's cmake project.

If this is an Android project, do those steps inside your `YourAppDir/app/src/main/cpp/` directory where the `CMakeLists.txt` resides.

1. Clone the repository to your submodules, in `./thirdparty/` path.
   ```bash
   mkdir thirdparty
   cd thirdparty
   git submodule add https://github.com/getbraincloud/braincloud-cpp.git
   ```
2. Update submodules so thirdparty libraries are present when building.
   ```bash
   cd braincloud-cpp
   git submodule update --init --recursive
   ```
3. Add the following to your `CMakeLists.txt`:
   ```cmake
   add_subdirectory(./thirdparty/braincloud-cpp)
   target_link_libraries(${PROJECT_NAME} brainCloud) # Note the capital 'C'
   ```
4. **For Android apps only**, don't forget to add `INTERNET` permission to your `AndroidManifest.xml` or else brainCloud won't work.
   ```xml
   <uses-permission android:name="android.permission.INTERNET" />
   ```

## Installation

The library is distributed through `CocoaPods`. In order to install the library, simply add the following to your `Podfile`.

```ruby
pod 'BrainCloudCpp'
```

If you're a newcomer to the `CocoaPods` world, you can find documentation on how to set up your Xcode project here:

https://guides.cocoapods.org/

## Troubleshooting

Here are a few common errors that you may see on your first attempt to connect to brainCloud.

- **App id not set**: Verify you've set up the app id and app secret correctly in the `initialize()` method.
- **Platform not enabled**: Verify you've enabled your platform on the portal.

If you're still having issues, log into the portal and give us a shout through the help system (bottom right icon with the question mark and chat bubble).

## brainCloud Summary

brainCloud is a ready-made back-end platform for the development of feature-rich games, apps and things. brainCloud provides the features you need – along with comprehensive tools to support your team during development, testing and user support.

brainCloud consists of:

- Cloud Service – an advanced, Software-as-a-Service (SaaS) back-end
- Client Libraries – local client libraries (SDKs)
- Design Portal – a portal that allows you to design and debug your apps
- brainCloud Architecture

![architecture](/Screenshots/bc-architecture.png?raw=true)

## What's the difference between the brainCloud Wrapper and the brainCloud Client?

The wrapper contains quality of life improvement around the brainCloud Client. It may contain device specific code, such as serializing the user's login id on an Android or iOS device.
It is recommended to use the wrapper by default.

![wrapper](/Screenshots/bc-wrapper.png?raw=true)

## How do I initialize brainCloud?

If using the wrapper use the following code.

```c++
_bc = new BrainCloudWrapper(); // optionally pass in a _wrapperName
_bc->initialize("https://api.braincloudservers.com/dispatcherv2", _appId, _secret, _appVersion, _companyName, _appName);
```

Your \_appId, \_secret, is set on the brainCloud dashboard. Under Design | Core App Info > Application IDs

![wrapper](/Screenshots/bc-ids.png?raw=true)

\_wrapperName prefixes saved operations that the wrapper will make. Use a \_wrapperName if you plan on having multiple instances of brainCloud running.

---

#### Newly upgraded?

If your app is already live, you should **NOT** specify the \_wrapperName - otherwise the library will look in the wrong location for your user's stored anonymousID and profileID information. Only add a name if you intend to alter the save data.

---

\_companyName and \_appName are also used in determining the fileName. ex. bitHeads.exampleApp, or mainWrapper.bitHeads.exampleApp

\_appVersion is the current version of our app. Having an \_appVersion less than your minimum app version on brainCloud will prevent the user from accessing the service until they update their app to the lastest version you have provided them.

![wrapper](/Screenshots/bc-minVersions.png?raw=true)

## How do I keep the brainCloud SDK updating?

In your project's update loop, you're going to want to update brainCloud client so it can check for responses.

To do this, you need to call runCallbacks();

```cpp
_bc->runCallbacks();
```

## How do I authenticate a user with brainCloud?

The simplest form of authenticating with brainCloud Wrapper is an Anonymous Authentication.

```cpp
_bc->authenticateAnonymous(_callback);
```

This method will create an account, and continue to use a locally saved anonymous id.

Your \_callback will inherit from IServerCallback and contain the functions needed to react to the brainCloud Server response.

To login with a specfic anonymous id, use the brainCloud client.

```cpp
_bc->getBCClient()->getAuthenticationService()->setAnonymousId(_anonymousId); // re-use an Anon id
_bc->getBCClient()->getAuthenticationService()->setAnonymousId(_bc->getBCClient()->getAuthenticationService()->generateAnonymousId().c_str()); // or use brainCloud to generate one
_bc->getBCClient()->getAuthenticationService()->authenticateAnonymous(_forceCreate, _callback);
```

Setting \_forceCreate to false will ensure the user will only login to an existing account. Setting it to true, will allow the user to register a new account

## How do I attach an email to a user's brainCloud profile?

After having the user create an anonymous with brainCloud, they are probably going to want to attach an email or username, so their account can be accessed via another platform, or when their local data is discarded.
Attaching email authenticate would look like this.

```cpp
_bc->getBCClient()->getIdentityService()->attachEmailIdentity(_email, _password, _callback);
```

There are many authentication types. You can also merge profiles and detach idenities. See the brainCloud documentation for more information:
http://getbraincloud.com/apidocs/apiref/?java#capi-auth

## Attributions

The brainCloud C++ library uses [libwebsockets v3.0.1](https://github.com/warmcat/libwebsockets/releases/tag/v3.0.1).
