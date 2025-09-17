# brainCloud C++ Library

Thanks for downloading the brainCloud C++ client library! Here are a few notes to get you started. Further information about the brainCloud API, including example Tutorials can be found here:

http://getbraincloud.com/apidocs/

If you haven't signed up or you want to log into the brainCloud portal, you can do that here:

https://portal.braincloudservers.com/

## Build in Visual Studio

Prerequisites:

- git or downloaded zip archive of brainCloud
- Visual Studio 17 2022. But everything after 2013 should work.
- CMake https://cmake.org/
- VCPKG https://vcpkg.io/en/ (If you plan on using OpenSSL on Windows)

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
   You may specify a Visual Studio version with the generator command:
   ```bash
   cmake -G"Visual Studio 16 2019" ..
   cmake -G"Visual Studio 17 2022" ..
   ```    
   Or if you are building for **UWP**, do the following instead:
   ```bash
    cmake -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION="10.0" -DCMAKE_GENERATOR_PLATFORM=x64 -DBUILD_TESTS=ON ..
   ```
5. From file explorer, `"explorer ."`, open `brainCloud.sln` in Visual Studio and build the **brainCloud** project with desired configuration. Release/Debug/static or shared libraries, etc..

6. You may build brainCloud.lib at the shell commandline:
  
   ```bash
   cmake ..
   cmake --build . --target brainCloud
   ```
   
   
## OpenSSL support 


   You may also specify the flag `BC_USE_OPENSSL` to use OpenSSL as a TLS backend instead of mbedtls, if you are building with this flag on Windows, this requires that you use the VCPKG toolchain with CMake. 
   To make things easier, we've provided a CMakePresets.json file so you would just need to use a preset to build with VCPKG.
   **IMPORTANT**: You must have an environment variable with the label `VCPKG` set up that has the path of your VCPKG installation.
   
   #For Windows:
   
   Generate the solution like this:
   ```
	cmake --preset=windows-default -DBC_USE_OPENSSL=ON ..
   ```
   
   #OpenSSL for Linux or Mac:
   
   You do not need VCPKG for this and can simply configure the project like this:
   ```
	cmake -DBC_USE_OPENSSL=ON ..
   ```
   
   
## Build for Linux or Mac

Prerequisis:

- git or downloaded zip archive of brainCloud
- A C++ compiler
- Ninja optional for faster build
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
Or,
   ```bash
   cmake --build . --config Release
   ```

## Other IDE/Build System Support

Use cmake --help to see available generators. Then, use command eg.:
   ```bash
   cmake -G"CodeBlocks" ..
   cmake -G Ninja ..
   ```
   
## CLion

1. Open the folder braincloud-cpp in CLion.
2. Set CMake options in Clion > Settings... For Windows, set  
```
-DSSL_ALLOW_SELFSIGNED=ON -DCMAKE_GENERATOR_PLATFORM=x64
```
3. Select Build > Build 'brainCloud'
Or, add brainCloud directly to your cmake project (see below).

## Build for Android

Do the following step by adding brainCloud directly to your NDK Application's CMake project.

## Add brainCloud to your application's cmake project.

Find the directory where the `CMakeLists.txt` resides. For example, if this is an Android project do these steps inside your `YourAppDir/app/src/main/cpp/`.

1. Clone the repository to your submodules, in `./thirdparty/` path for example.
   ```bash
   mkdir thirdparty
   cd thirdparty
   git submodule add https://github.com/getbraincloud/braincloud-cpp.git
   ```
2. Update submodules so required libraries are present when building.
   ```bash
   cd braincloud-cpp
   git submodule update --init --recursive
   ```
3. Add the subirectory to your `CMakeLists.txt`, example:
   ```cmake
   add_subdirectory(./thirdparty/braincloud-cpp)
   target_link_libraries(${PROJECT_NAME} brainCloud) # Note the capital 'C'
   ```
   Or:
   ```cmake
	add_subdirectory(/path/to/braincloud-cpp ./builddir/braincloud-cpp)
	list(APPEND includes PUBLIC /path/to/braincloud-cpp/include/)
	list(APPEND includes PUBLIC /path/to/braincloud-cpp/libs/jsoncpp-1.0.0)
    list(APPEND libs brainCloud)
    target_include_directories(${PROJECT_NAME} ${includes})
    target_link_libraries(${PROJECT_NAME} ${libs})
    ```

4. **For Android apps only**, don't forget to add `INTERNET` permission to your `AndroidManifest.xml` or else brainCloud won't work.
   ```xml
   <uses-permission android:name="android.permission.INTERNET" />
   ```

## Cocoapod Installation

The library is distributed through `CocoaPods`. In order to install the library, simply add the following to your `Podfile`.

```ruby
pod 'BrainCloudCpp'
```

Or,

```ruby
pod 'BrainCloudCpp', '4.xx.y'
```


If you're a newcomer to the `CocoaPods` world, you can find documentation on how to set up your Xcode project here:

https://guides.cocoapods.org/

After adding to your Podfile, run the command:

```bash
pod update
```

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

## Building and Running Tests

Refer to Jenkinsfile for platform builds and unit test pipeline. See also tests/README.md.

To run unit tests locally, you can use the autobuild script runtests.sh or runtest.bat. The process is:

1. Run cmake with BUILD_TESTS set to ON in debug configuration.
2. Run cmake to build target bctests in debug configuration.
3. Create/copy file ids.txt to the folder where tests will run. Input your server url, app ids and secret keys.
4. Execute with desired test filter.

## Attributions

The brainCloud C++ windows library uses [libwebsockets v3.0.1](https://github.com/warmcat/libwebsockets/releases/tag/v3.0.1).
Linux and Mac uses [libwebsockets v4.3-stable](https://github.com/warmcat/libwebsockets/commit/58af7b44).
Also depends on [MBedTLS v2.8](https://github.com/ARMmbed/mbedtls.git).
And redistributes public domain library [JsonCpp](https://github.com/getbraincloud/braincloud-jsoncpp.git).

### Cocoapod Depenencies

The brainCloud library ships with SSKeychain and requires SocketRocket and JsonCpp cocoapods. 

### Important Submodule Note

These are included as submodules so can be retrieved using 

   ```bash
   git submodule update --init --recursive
   ```
However should not be updated or pulled to another version since the initialized version is known to build and run stable. 

JsonCpp has recently been made a submodule (as of brainCloud 4.15) so if pulling or changing branches the existing folder lib/jsoncpp-1.0.0/ should be deleted or there will be git errors.