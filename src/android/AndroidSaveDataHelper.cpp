
//#if defined(__ANDROID__)

#include "braincloud/internal/android/AndroidSaveDataHelper.h"

namespace BrainCloud
{
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
        // TODO: Or call the java version of this directly
        /* TODO: need env
        jclass jcContext = env->FindClass("android/content/Context");
        jclass jcSharedPreferences = env->FindClass("android/content/SharedPreferences");

        if(jcContext==nullptr || jcSharedPreferences== nullptr){
            __android_log_print(ANDROID_LOG_DEBUG, "SharedPreferences","Cannot find classes");
        }

        jmGetString=env->GetMethodID(jcSharedPreferences,"getString","(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;");
        jmethodID jmGetSharedPreferences=env->GetMethodID(jcContext,"getSharedPreferences","(Ljava/lang/String;I)Landroid/content/SharedPreferences;");
        */
    }

    void AndroidSaveDataHelper::saveData(const char * key, const char * data)
    {
        //env->CallObjectMethod(jmEdit,jmPutString,env->NewStringUTF(key),env->NewStringUTF(value));
    }

    std::string AndroidSaveDataHelper::readData(const char * key)
    {
        std::string ret = "";
        /*
        joSharedPreferences=env->CallObjectMethod(androidContext,jmGetSharedPreferences,env->NewStringUTF(name),MODE_PRIVATE);

        /*
        auto value=(jstring)(env->CallObjectMethod(joSharedPreferences,jmGetString,env->NewStringUTF(id),env->NewStringUTF(defaultValue)));
            const char* valueP = env->GetStringUTFChars(value, nullptr);
            const std::string ret=std::string(valueP);
            env->ReleaseStringUTFChars(value,valueP);
            
            */
        return ret;
    }

    void AndroidSaveDataHelper::deleteData(const char * key)
    {
        //env->CallObjectMethod(jmEdit,jmPutString,env->NewStringUTF(key),env->NewStringUTF(""));
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
    bool commit()const{
        return (bool)env->CallBooleanMethod(joSharedPreferences_Edit,jmCommit);
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
