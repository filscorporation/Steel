#include <Steel.h>

#include <jni.h>

Application* app;

void init()
{
    if (app != nullptr)
        // Surface recreated. TODO: check render context is still valid
        return;

    // Create application with default settings
    ApplicationSettings settings = ApplicationSettings();
    settings.DoubleBuffer = false;
    app = new Application();
    app->Init(settings);

    // Prepare to run in step calls
    app->PrepareRun();
}

void resize(jint width, jint height)
{
    Screen::Resize(width, height);
}

void step()
{
    // Call update every frame
    app->RunUpdate();
}

void terminate()
{
    // Terminate application
    app->Terminate();
    app = nullptr;
}

extern "C"
{
    JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_init(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_step(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_terminate(JNIEnv* env, jobject obj);
};

JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_init(JNIEnv* env, jobject obj)
{
    init();
}

JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_resize(JNIEnv* env, jobject obj, jint width, jint height)
{
    resize(width, height);
}

JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_step(JNIEnv* env, jobject obj)
{
    step();
}

JNIEXPORT void JNICALL Java_com_android_appsteel_AppSteelLib_terminate(JNIEnv* env, jobject obj)
{
    terminate();
}