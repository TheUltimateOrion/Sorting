#pragma once
#include "../main.h"

#ifndef AL_CHECK_ERR
#define AL_CHECK_ERR(ret) \
    if (alGetError() != AL_NO_ERROR) {\
        this->err = alGetError();\
        return ret; \
    }
#endif

class SoundEngine
{
private:
    ALuint buf;
    ALuint src = 0;
    short *samples;
    ALenum err = AL_NO_ERROR;
public:
    static SoundEngine* get();

    SoundEngine();
    ~SoundEngine();

    ALenum alGetLastError();
    const char* alErrorString(ALenum err);

    int init();
    void load(float ms, float freq);
    void play();
};