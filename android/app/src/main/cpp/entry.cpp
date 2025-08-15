#include <jni.h>

extern int main();

extern "C" JNIEXPORT void JNICALL
Java_com_theultimateorion_orionsort_1android_MainActivity_nativeInit(JNIEnv*, jobject)
{
    main();
}
