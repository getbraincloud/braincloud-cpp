
#if defined(__ANDROID__)

#include "braincloud/internal/android/AndroidSaveDataHelper.h"

namespace BrainCloud
{
    // these MUST be set from the app's MainActivity loop (native-lib.cpp)
    // to env and context pointer values passed in automatically from MainActivity.java
    JNIEnv *appEnv = NULL;
    jobject appContext = NULL;

	AndroidSaveDataHelper::AndroidSaveDataHelper()
    {
        
    }

    SaveDataHelper* SaveDataHelper::create()
    {
        return new AndroidSaveDataHelper();
    }

    // initialize 
    void AndroidSaveDataHelper::initialize(const char * companyName, const char * appName, const char * wrapperName/* = ""*/)
    {

        if (companyName != NULL && appName != NULL) {
            std::string companyNameStr = companyName;
            std::string gameNameStr = appName;

            std::string prefix = wrapperName == NULL ? "" : wrapperName;
            if (strlen(prefix.c_str()) > 0) {
                prefix += ".";
            }

            m_savePath = prefix + companyNameStr + "." + gameNameStr;
        }
    }

    void AndroidSaveDataHelper::saveData(const char * key, const char * data)
    {
        jclass jcSharedPreferences = appEnv->FindClass("android/content/SharedPreferences");
        jclass jcContext = appEnv->FindClass("android/content/Context");
        jmethodID jmGetSharedPreferences=appEnv->GetMethodID(jcContext, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jmethodID jmEdit=appEnv->GetMethodID(jcSharedPreferences, "edit", "()Landroid/content/SharedPreferences$Editor;");
        jobject joSharedPreferences=appEnv->CallObjectMethod(appContext, jmGetSharedPreferences, appEnv->NewStringUTF(m_savePath.c_str()), MODE_PRIVATE);
        jobject joSharedPreferences_Edit=appEnv->CallObjectMethod(joSharedPreferences, jmEdit);
        jclass jcSharedPreferences_Editor = appEnv->GetObjectClass(joSharedPreferences_Edit);
        jmethodID jmPutString = appEnv->GetMethodID(jcSharedPreferences_Editor, "putString",
                                          "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
        jmethodID jmCommit = appEnv->GetMethodID(jcSharedPreferences_Editor, "commit", "()Z");

        appEnv->CallBooleanMethod(appEnv->CallObjectMethod(joSharedPreferences_Edit, jmPutString, appEnv->NewStringUTF(key), appEnv->NewStringUTF(data)), jmCommit);
    }

    std::string AndroidSaveDataHelper::readData(const char * key)
    {
        jclass jcSharedPreferences = appEnv->FindClass("android/content/SharedPreferences");
        jclass jcContext = appEnv->FindClass("android/content/Context");
        jmethodID jmGetSharedPreferences=appEnv->GetMethodID(jcContext, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        jobject joSharedPreferences=appEnv->CallObjectMethod(appContext, jmGetSharedPreferences, appEnv->NewStringUTF(m_savePath.c_str()), MODE_PRIVATE);
        jmethodID jmGetString=appEnv->GetMethodID(jcSharedPreferences, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");

        auto value=(jstring)(appEnv->CallObjectMethod(joSharedPreferences, jmGetString, appEnv->NewStringUTF(key), appEnv->NewStringUTF("")));
        const char* valueP = appEnv->GetStringUTFChars(value, nullptr);
        const std::string ret=std::string(valueP);
        appEnv->ReleaseStringUTFChars(value, valueP);

        return ret;
    }

    void AndroidSaveDataHelper::deleteData(const char * key)
    {
        saveData(key, "");
    }
}
#endif