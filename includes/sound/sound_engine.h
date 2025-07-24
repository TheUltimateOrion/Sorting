#pragma once

#include <AL/al.h>
#include <AL/alc.h>

#ifndef AL_CHECK_ERR
#define AL_CHECK_ERR(ret) \
    if (alGetError() != AL_NO_ERROR)\
    {\
        this->m_err = alGetError();\
        return ret; \
    }
#endif

class SoundEngine
{
private:
    ALuint m_buf;
    ALuint m_src = 0;
    short *m_samples;
    ALenum m_err = AL_NO_ERROR;

public:
    static SoundEngine *get();

    SoundEngine();
    ~SoundEngine();

    ALenum alGetLastError() const noexcept;
    const char* alErrorString(ALenum t_err) const noexcept;

    [[nodiscard]] int init();
    void load(float t_ms, float t_freq) noexcept;
    void play() noexcept;
};