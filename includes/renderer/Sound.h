#pragma once
#include "../App.h"

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
    static SoundEngine *get();

    SoundEngine();
    ~SoundEngine();

    ALenum alGetLastError() const noexcept;
    const char* alErrorString(ALenum err) const noexcept;

    [[nodiscard]] int init();
    void load(float ms, float freq) noexcept;
    void play() noexcept;
};