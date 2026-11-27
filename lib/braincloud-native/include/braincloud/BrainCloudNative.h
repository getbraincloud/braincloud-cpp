#pragma once

#include <cstddef>
#include <cstdint>

#if defined(_WIN32) && defined(BCNATIVE_SHARED)
#if defined(BCNATIVE_BUILDING)
#define BCNATIVE_API __declspec(dllexport)
#else
#define BCNATIVE_API __declspec(dllimport)
#endif
#elif !defined(_WIN32) && (defined(BCNATIVE_SHARED) || defined(BCNATIVE_BUILDING))
#define BCNATIVE_API __attribute__((visibility("default")))
#else
#define BCNATIVE_API
#endif

namespace braincloud {

/**
 * The state of the stored app configuration.
 */
enum class ConfigState {
    /** No configuration is stored. */
    None,
    /** Credentials are stored in plain text. */
    Legacy,
    /** A configuration is stored and can be read. */
    Ready,
    /** A configuration is stored but cannot be read. */
    Invalid,
};

/**
 * Reads an app configuration and signs requests with it.
 *
 * Strings are UTF-8 and owned by this object; no standard library types cross this
 * interface, so it links into any engine regardless of the C++ runtime it uses.
 */
class BCNATIVE_API NativeConfig {
public:
    /** The number of characters in a signature, not counting the terminating null. */
    static const size_t kSignatureLength = 32;

    NativeConfig();
    ~NativeConfig();

    NativeConfig(const NativeConfig&) = delete;
    NativeConfig& operator=(const NativeConfig&) = delete;

    /**
     * Reads the configuration from a file.
     *
     * @param path The file to read
     * @param tuning The build's tuning value, written with the configuration
     * @param tuningLength The length of the tuning value in bytes
     * @return True if the configuration was read
     */
    bool Resolve(const char* path, const uint8_t* tuning, size_t tuningLength);

    /**
     * Reads the configuration from the contents of a file.
     *
     * @param contents The file's contents
     * @param length The length of the contents in bytes
     * @param tuning The build's tuning value, written with the configuration
     * @param tuningLength The length of the tuning value in bytes
     * @return True if the configuration was read
     */
    bool ResolveContents(const char* contents, size_t length, const uint8_t* tuning, size_t tuningLength);

    /**
     * Returns true if Resolve or ResolveContents succeeded.
     */
    bool IsResolved() const;

    /**
     * Returns the app id, or an empty string if the configuration has not been read.
     */
    const char* GetAppId() const;

    /**
     * Returns the number of child apps in the configuration.
     */
    size_t GetChildAppCount() const;

    /**
     * Returns the id of a child app.
     *
     * @param index The child app's index, less than GetChildAppCount()
     * @return The child app's id, or an empty string if the index is out of range
     */
    const char* GetChildAppId(size_t index) const;

    /**
     * Returns the server url, or an empty string if none is stored.
     */
    const char* GetServerUrl() const;

    /**
     * Returns the app version, or an empty string if none is stored.
     */
    const char* GetAppVersion() const;

    /**
     * Returns true if the configuration can sign for an app.
     *
     * @param appId The app id or a child app id
     */
    bool HasApp(const char* appId) const;

    /**
     * Signs a request payload for the app.
     *
     * @param payload The request payload
     * @param length The length of the payload in bytes
     * @param signatureOut Receives kSignatureLength hex characters and a terminating null
     * @return True if the payload was signed
     */
    bool Sign(const uint8_t* payload, size_t length, char* signatureOut) const;

    /**
     * Signs a request payload for the app or one of its child apps.
     *
     * @param appId The app id or a child app id
     * @param payload The request payload
     * @param length The length of the payload in bytes
     * @param signatureOut Receives kSignatureLength hex characters and a terminating null
     * @return True if the payload was signed, false if the app is not in the configuration
     */
    bool Sign(const char* appId, const uint8_t* payload, size_t length, char* signatureOut) const;

    /**
     * Returns the state of the stored configuration.
     *
     * @param configPath The configuration file, or null to skip it
     * @param legacyIniPath The plain text credentials file, or null to skip it
     */
    static ConfigState DetectState(const char* configPath, const char* legacyIniPath);

    /**
     * Returns this library's version.
     */
    static const char* Version();

private:
    struct Impl;
    Impl* _impl;
};

}
