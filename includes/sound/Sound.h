#pragma once
#include "../core/App.h"

#ifndef AL_CHECK_ERR
#define AL_CHECK_ERR(ret) \
    if (alGetError() != AL_NO_ERROR) {\
        this->m_err = alGetError();\
        return ret; \
    }
#endif

#ifndef AL_HANDLE_ERROR
#define AL_HANDLE_ERROR(str, ret)\
    if (snd->alGetLastError() != AL_NO_ERROR) {\
        std::cerr << str << " with code: " << snd->alErrorString(snd->alGetLastError()) << "(" << snd->alGetLastError() << ")" << std::endl;\
        return ret;\
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