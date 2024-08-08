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

            // check for AndroidBridge class, if it exists assume java activity will initialize
            jclass jcAndroidBridge = appEnv->FindClass("com/bitheads/braincloud/AndroidBridge");
            if (!appEnv->ExceptionCheck()) {
                // do NOT set countryCode etc here as the android
                // java layer is responsible for setting it.
                return;
            }
            appEnv->ExceptionClear();

            jclass jcLocale = appEnv->FindClass("java/util/Locale");
            jmethodID jmDefault = appEnv->GetStaticMethodID(jcLocale, "getDefault",
                                                            "()Ljava/util/Locale;");

            jobject joLocale = appEnv->CallStaticObjectMethod(jcLocale, jmDefault);

            jmethodID jmCountry = appEnv->GetMethodID(jcLocale, "getCountry",
                                                      "()Ljava/lang/String;");
            auto value = (jstring) (appEnv->CallObjectMethod(joLocale, jmCountry));
            const char *valueP = appEnv->GetStringUTFChars(value, nullptr);
            *out_countryCode = std::string(valueP);
            if (*out_countryCode == "419")
            {
                *out_countryCode = "_LA_";
            }

            if (*out_countryCode == "Hans" || *out_countryCode == "Hant")
            {
                *out_countryCode = "CN";
            }

            appEnv->ReleaseStringUTFChars(value, valueP);

            jmethodID jmLanguage = appEnv->GetMethodID(jcLocale, "getLanguage",
                                                       "()Ljava/lang/String;");
            value = (jstring) (appEnv->CallObjectMethod(joLocale, jmLanguage));
            valueP = appEnv->GetStringUTFChars(value, nullptr);
            *out_languageCode = std::string(valueP);
            appEnv->ReleaseStringUTFChars(value, valueP);

            jclass jcDateType = appEnv->FindClass("java/util/Date");
            jmethodID jmDateInit= appEnv->GetMethodID(jcDateType, "<init>", "()V");
            jobject joDateObject= appEnv->NewObject(jcDateType , jmDateInit);
            jmethodID jmDateTime= appEnv->GetMethodID(jcDateType, "getTime", "()J");

            jlong currtime = appEnv->CallLongMethod(joDateObject, jmDateTime);

            jclass jcTimeZone = appEnv->FindClass("java/util/TimeZone");
            jmDefault = appEnv->GetStaticMethodID(jcTimeZone, "getDefault",
                                                  "()Ljava/util/TimeZone;");
            jobject joTimeZone = appEnv->CallStaticObjectMethod(jcTimeZone, jmDefault);
            jmethodID jmTimeZone = appEnv->GetMethodID(jcTimeZone, "getOffset", "(J)I");

            *out_timezoneOffset = appEnv->CallIntMethod(joTimeZone, jmTimeZone, currtime)  / static_cast<float>(60 * 60 * 1000);
        }
    }
}
