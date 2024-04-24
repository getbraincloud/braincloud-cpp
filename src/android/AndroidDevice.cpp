#include "braincloud/internal/Device.h"
#include "braincloud/internal/android/AndroidGlobals.h"

#include <jni.h>
#include <time.h>

static const std::string PLATFORM_NAME("ANG");

namespace BrainCloud
{
    namespace Device
    {
        const std::string& getPlatformName()
        {
            return PLATFORM_NAME;
        }

        void getLocale(float* out_timezoneOffset, std::string* out_languageCode, std::string* out_countryCode) {

            jclass jcLocale = appEnv->FindClass("java/util/Locale");
            jmethodID jmDefault = appEnv->GetStaticMethodID(jcLocale, "getDefault",
                                                            "()Ljava/util/Locale;");

            jobject joLocale = appEnv->CallStaticObjectMethod(jcLocale, jmDefault);

            jmethodID jmCountry = appEnv->GetMethodID(jcLocale, "getCountry",
                                                      "()Ljava/lang/String;");
            auto value = (jstring) (appEnv->CallObjectMethod(joLocale, jmCountry));
            const char *valueP = appEnv->GetStringUTFChars(value, nullptr);
            *out_countryCode = std::string(valueP);
            appEnv->ReleaseStringUTFChars(value, valueP);

            jmethodID jmLanguage = appEnv->GetMethodID(jcLocale, "getLanguage",
                                                       "()Ljava/lang/String;");
            value = (jstring) (appEnv->CallObjectMethod(joLocale, jmLanguage));
            valueP = appEnv->GetStringUTFChars(value, nullptr);
            *out_languageCode = std::string(valueP);
            appEnv->ReleaseStringUTFChars(value, valueP);

            jclass jcTimeZone = appEnv->FindClass("java/util/TimeZone");
            jmDefault = appEnv->GetStaticMethodID(jcTimeZone, "getDefault",
                                                  "()Ljava/util/TimeZone;");
            jobject joTimeZone = appEnv->CallStaticObjectMethod(jcTimeZone, jmDefault);
            jmethodID jmTimeZone = appEnv->GetMethodID(jcTimeZone, "getOffset", "(J)I");
            struct timespec res;

            struct timeval now;
            gettimeofday(&now, NULL);
            long currtime = static_cast<long>(now.tv_sec) * 1000 + now.tv_usec / 1000;

            *out_timezoneOffset = appEnv->CallIntMethod(joTimeZone, jmTimeZone, currtime)  / static_cast<float>(60 * 60 * 1000);
        }
    }
}
