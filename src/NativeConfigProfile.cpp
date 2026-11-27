#include "braincloud/internal/NativeConfigProfile.h"

#if defined(BC_NATIVE_CONFIG)

#include "braincloud/BrainCloudNative.h"
#include "braincloud/internal/NativeTuning.h"
#include "braincloud/internal/SecureMemory.h"

#include <cstdio>
#include <vector>

#if defined(__ANDROID__)
#include "braincloud/internal/android/AndroidGlobals.h"
#include <iostream>
#elif defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#include <climits>
#elif defined(__linux__)
#include <climits>
#include <unistd.h>
#endif

namespace BrainCloud
{
    static const char* const kConfigFileName = "braincloud.cfg";

    NativeConfigProfile::NativeConfigProfile(const std::shared_ptr<braincloud::NativeConfig>& config, const std::string& appId)
        : _config(config)
        , _appId(appId)
    {
    }

    bool NativeConfigProfile::sign(const uint8_t* payload, size_t length, char* signatureOut) const
    {
        return _config->Sign(_appId.c_str(), payload, length, signatureOut);
    }

    static void fillAppConfig(const std::shared_ptr<braincloud::NativeConfig>& config, NativeAppConfig& out)
    {
        out.serverUrl = config->GetServerUrl();
        out.appVersion = config->GetAppVersion();
        out.appId = config->GetAppId();
        out.profiles.clear();
        out.profiles[out.appId] = std::make_shared<NativeConfigProfile>(config, out.appId);
        for (size_t i = 0; i < config->GetChildAppCount(); ++i)
        {
            const std::string childId = config->GetChildAppId(i);
            out.profiles[childId] = std::make_shared<NativeConfigProfile>(config, childId);
        }
    }

#if defined(__ANDROID__)
    // Reads an APK asset through the app's Context; no libandroid needed.
    static bool readAsset(const char* name, std::vector<char>& out)
    {
        JNIEnv* env = appEnv;
        if (env == NULL)
        {
            std::cout << "ERROR | Set BrainCloud::appEnv and BrainCloud::appContext before initialize() so braincloud.cfg can be read from the app's assets." << std::endl;
            return false;
        }
        jobject context = appContext;
        if (context == NULL)
        {
            jclass jcActivityThread = env->FindClass("android/app/ActivityThread");
            jmethodID jmCurrentApp = env->GetStaticMethodID(jcActivityThread, "currentApplication", "()Landroid/app/Application;");
            context = env->CallStaticObjectMethod(jcActivityThread, jmCurrentApp);
            env->DeleteLocalRef(jcActivityThread);
            if (context == NULL)
                return false;
        }

        jclass jcContext = env->FindClass("android/content/Context");
        jmethodID jmGetAssets = env->GetMethodID(jcContext, "getAssets", "()Landroid/content/res/AssetManager;");
        jobject joAssets = env->CallObjectMethod(context, jmGetAssets);
        jclass jcAssets = env->FindClass("android/content/res/AssetManager");
        jmethodID jmOpen = env->GetMethodID(jcAssets, "open", "(Ljava/lang/String;)Ljava/io/InputStream;");
        jstring jName = env->NewStringUTF(name);
        jobject joStream = env->CallObjectMethod(joAssets, jmOpen, jName);
        bool ok = false;
        if (env->ExceptionCheck())
        {
            // FileNotFoundException: no braincloud.cfg in the assets.
            env->ExceptionClear();
        }
        else if (joStream != NULL)
        {
            jclass jcStream = env->FindClass("java/io/InputStream");
            jmethodID jmRead = env->GetMethodID(jcStream, "read", "([B)I");
            jmethodID jmClose = env->GetMethodID(jcStream, "close", "()V");
            jbyteArray jBuffer = env->NewByteArray(4096);
            ok = true;
            for (;;)
            {
                const jint n = env->CallIntMethod(joStream, jmRead, jBuffer);
                if (env->ExceptionCheck())
                {
                    env->ExceptionClear();
                    ok = false;
                    break;
                }
                if (n < 0)
                    break;
                const size_t at = out.size();
                out.resize(at + static_cast<size_t>(n));
                if (n > 0)
                    env->GetByteArrayRegion(jBuffer, 0, n, reinterpret_cast<jbyte*>(&out[at]));
            }
            env->CallVoidMethod(joStream, jmClose);
            if (env->ExceptionCheck())
                env->ExceptionClear();
            env->DeleteLocalRef(jBuffer);
            env->DeleteLocalRef(jcStream);
            env->DeleteLocalRef(joStream);
        }
        env->DeleteLocalRef(jName);
        env->DeleteLocalRef(jcAssets);
        env->DeleteLocalRef(joAssets);
        env->DeleteLocalRef(jcContext);
        if (context != appContext)
            env->DeleteLocalRef(context);
        return ok && !out.empty();
    }
#endif

#if !defined(__ANDROID__)
    static bool fileExists(const std::string& path)
    {
        FILE* f = fopen(path.c_str(), "rb");
        if (f == NULL)
            return false;
        fclose(f);
        return true;
    }

    static std::string executableDir()
    {
        std::string path;
#if defined(_WIN32)
        char buffer[MAX_PATH];
        DWORD n = GetModuleFileNameA(NULL, buffer, MAX_PATH);
        if (n > 0 && n < MAX_PATH)
            path.assign(buffer, n);
#elif defined(__APPLE__)
        uint32_t size = 0;
        _NSGetExecutablePath(NULL, &size);
        std::vector<char> buffer(size + 1);
        if (_NSGetExecutablePath(&buffer[0], &size) == 0)
            path = &buffer[0];
#elif defined(__linux__)
        char buffer[PATH_MAX];
        ssize_t n = readlink("/proc/self/exe", buffer, sizeof(buffer));
        if (n > 0)
            path.assign(buffer, static_cast<size_t>(n));
#endif
        const size_t slash = path.find_last_of("/\\");
        return slash == std::string::npos ? std::string() : path.substr(0, slash + 1);
    }

    // Working directory, beside the executable, then a Mac app bundle's Resources.
    static std::string findConfigFile()
    {
        if (fileExists(kConfigFileName))
            return kConfigFileName;
        const std::string exeDir = executableDir();
        const std::string beside = exeDir + kConfigFileName;
        if (fileExists(beside))
            return beside;
#if defined(__APPLE__)
        const std::string macOS = "Contents/MacOS/";
        if (exeDir.size() > macOS.size() && exeDir.compare(exeDir.size() - macOS.size(), macOS.size(), macOS) == 0)
        {
            const std::string resources = exeDir.substr(0, exeDir.size() - 6) + "Resources/" + kConfigFileName;
            if (fileExists(resources))
                return resources;
        }
#endif
        return std::string();
    }
#endif

    bool loadDefaultNativeAppConfig(NativeAppConfig& out)
    {
        std::vector<uint8_t> tuning;
        if (!readTuning(tuning))
            return false;
        std::shared_ptr<braincloud::NativeConfig> config = std::make_shared<braincloud::NativeConfig>();
#if defined(__ANDROID__)
        std::vector<char> contents;
        const bool resolved = readAsset(kConfigFileName, contents) &&
                              config->ResolveContents(&contents[0], contents.size(), &tuning[0], tuning.size());
        std::fill(contents.begin(), contents.end(), '\0');
#else
        const std::string path = findConfigFile();
        const bool resolved = !path.empty() && config->Resolve(path.c_str(), &tuning[0], tuning.size());
#endif
        secureWipe(tuning);
        if (!resolved)
            return false;
        fillAppConfig(config, out);
        return true;
    }
}

#endif
