
//#if defined(__ANDROID__)

#include "braincloud/internal/android/AndroidSaveDataHelper.h"

namespace BrainCloud
{
    JNIEnv *appEnv = NULL; // the java native interface from startup
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
            //if (strlen(prefix.c_str()) > 0) {
                prefix += ".";
            //}

            m_savePath = prefix + companyNameStr + "." + gameNameStr;

            assert(appEnv != NULL && appContext != NULL);

            jclass jcContext = appEnv->FindClass("android/content/Context");
            jclass jcSharedPreferences = appEnv->FindClass("android/content/SharedPreferences");

            assert(jcContext != nullptr && jcSharedPreferences != nullptr);

            jmEdit=appEnv->GetMethodID(jcSharedPreferences, "edit", "()Landroid/content/SharedPreferences$Editor;");

            jmethodID jmGetSharedPreferences=appEnv->GetMethodID(jcContext, "getSharedPreferences", "(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
            joSharedPreferences=appEnv->CallObjectMethod(appContext, jmGetSharedPreferences, appEnv->NewStringUTF(m_savePath.c_str()), MODE_PRIVATE);
            joSharedPreferences_Edit=appEnv->CallObjectMethod(joSharedPreferences, jmEdit);

            assert(joSharedPreferences != nullptr && joSharedPreferences_Edit != nullptr);

            jclass jcSharedPreferences_Editor = appEnv->GetObjectClass(joSharedPreferences_Edit);
            jmPutString = appEnv->GetMethodID(jcSharedPreferences_Editor, "putString",
                                              "(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
            jmGetString=appEnv->GetMethodID(jcSharedPreferences, "getString", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
            jmCommit = appEnv->GetMethodID(jcSharedPreferences_Editor, "commit", "()Z");
        }
    }

    void AndroidSaveDataHelper::saveData(const char * key, const char * data)
    {
        //if(strcmp(data,"") == 0) return;
        appEnv->CallBooleanMethod(appEnv->CallObjectMethod(joSharedPreferences_Edit, jmPutString, appEnv->NewStringUTF(key), appEnv->NewStringUTF(data)), jmCommit);
  //      commit();
    }

    std::string AndroidSaveDataHelper::readData(const char * key)
    {
        auto value=(jstring)(appEnv->CallObjectMethod(joSharedPreferences, jmGetString, appEnv->NewStringUTF(key), appEnv->NewStringUTF("")));
        const char* valueP = appEnv->GetStringUTFChars(value, nullptr);
        const std::string ret=std::string(valueP);
        appEnv->ReleaseStringUTFChars(value, valueP);

        return ret;
    }

    void AndroidSaveDataHelper::deleteData(const char * key)
    {
        appEnv->CallBooleanMethod(appEnv->CallObjectMethod(joSharedPreferences_Edit, jmPutString, appEnv->NewStringUTF(key), appEnv->NewStringUTF("")), jmCommit);
//        commit();
    }

    bool AndroidSaveDataHelper::commit(){
        //return true;
       return (bool)appEnv->CallBooleanMethod(joSharedPreferences_Edit, jmCommit);
    }
}

/*
void AppleSaveDataHelper::initialize(JNIEnv *env, jobject androidContext,const char * companyName, const char * appName, const char * wrapperName)
	{
		if (companyName != NULL && appName != NULL)
		{
			std::string companyNameStr = companyName;
			std::string gameNameStr = appName;

			std::string prefix = wrapperName == NULL ? "" : wrapperName;
			if(strlen(prefix.c_str()) > 0) {
				prefix += ".";
			}

			std::string name = prefix + companyNameStr + "." + gameNameStr;

                    this->env=env;
                    jclass jcContext = env->FindClass("android/content/Context");
                    jclass jcSharedPreferences = env->FindClass("android/content/SharedPreferences");
                    if(jcContext==nullptr || jcSharedPreferences== nullptr){
                        __android_log_print(ANDROID_LOG_DEBUG, "SharedPreferences","Cannot find classes");
                    }
                    jmGetString=env->GetMethodID(jcSharedPreferences,"getString","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
                    jmEdit=env->GetMethodID(jcSharedPreferences,"edit", "()Landroid/content/SharedPreferences$Editor;");
                    jmPutString=env->GetMethodID(jmEdit,"putString","(Ljava/lang/String;Ljava/lang/String;)Landroid/content/SharedPreferences$Editor;");
                    jmCommit=env->GetMethodID(jmEdit,"commit","()Z");
                    jmethodID jmGetSharedPreferences=env->GetMethodID(jcContext,"getSharedPreferences","(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
                    joSharedPreferences=env->CallObjectMethod(androidContext,jmGetSharedPreferences,env->NewStringUTF(name),0);
                    if(keepReference){
                        joSharedPreferences=env->NewWeakGlobalRef(joSharedPreferences);
                    }
                }
                void replaceJNI(JNIEnv* newEnv){
                    env=newEnv;
                }
            private:
                JNIEnv* env;
                jobject joSharedPreferences;
                jmethodID jmGetString;
                jmethodID jmEdit;
                jmPutString;
                jmCommit;

		}
	}



	std::string readData(const char* id,const char* defaultValue="")const{
            auto value=(jstring)(env->CallObjectMethod(joSharedPreferences,jmGetString,env->NewStringUTF(id),env->NewStringUTF(defaultValue)));
            const char* valueP = env->GetStringUTFChars(value, nullptr);
            const std::string ret=std::string(valueP);
            env->ReleaseStringUTFChars(value,valueP);
            return ret;
        }


 void saveData(const char* key,const char* value)const{
        env->CallObjectMethod(jmEdit,jmPutString,env->NewStringUTF(key),env->NewStringUTF(value));
        return *this;
    }



     void deleteData(const char* key)const{
            env->CallObjectMethod(jmEdit,jmPutString,env->NewStringUTF(key),env->NewStringUTF(""));
            return *this;
        }
        bool commit()const{
            return (bool)env->CallBooleanMethod(joSharedPreferences_Edit,jmCommit);
        }
*/
//#endif
