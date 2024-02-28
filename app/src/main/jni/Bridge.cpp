#include <unordered_map>

#include <jni.h>
#include <android/log.h>

#include "Track.hpp"

JavaVM* g_vm;

// track id to track map
std::unordered_map<int, std::unique_ptr<test::GstreamerTrack>> g_tracks;

#ifdef __cplusplus
extern "C" {
#endif

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
        // Failed to get JNI environment
        return JNI_ERR;
    }

    g_vm = vm;

    __android_log_print(ANDROID_LOG_INFO, "Native", "Native lib loaded");

    return JNI_VERSION_1_6;
}


JNIEXPORT void JNICALL JNI_OnUnload(JavaVM* vm, void* reserved) {
    __android_log_print(ANDROID_LOG_INFO, "Native", "Native lib unloaded");
}

JNIEXPORT void JNICALL
Java_com_example_mytestapp_NativeTest_addTrack(
    JNIEnv *env,
    jobject /* this */,
    jint trackID,
    jstring path
) {
    if (g_tracks.find(trackID) != g_tracks.end()) {
        // Track already exists
        return;
    }

    __android_log_print(ANDROID_LOG_INFO, "Native", "Adding the track: %d", trackID);

    const char *pathStr = env->GetStringUTFChars(path, nullptr);

    // convert to std::string
    std::string pathString(pathStr);

    // Add track to composition
    g_tracks[trackID] = std::make_unique<test::GstreamerTrack>(trackID, pathString);
    __android_log_print(ANDROID_LOG_INFO, "Native", "Track %d has been added", trackID);

    // Release the string when done
    env->ReleaseStringUTFChars(path, pathStr);
}

JNIEXPORT void JNICALL
Java_com_example_mytestapp_NativeTest_nextFrame(
    JNIEnv *env,
    jobject /* this */,
    jint trackID
) {
    // next frame
    g_tracks[trackID]->nextFrame();
}

#ifdef __cplusplus
}
#endif