//
// Created by Joanne Hoar on 2022-06-09.
//

#ifndef ANDROIDGLOBALS_H
#define ANDROIDGLOBALS_H

#include <jni.h>
namespace BrainCloud {
    extern JNIEnv *appEnv; // the java native interface from startup
    extern jobject appContext;
}

#endif //ANDROIDGLOBALS_H
