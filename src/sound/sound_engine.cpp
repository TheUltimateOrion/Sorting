#include "sound/sound_engine.h"

#include "core/logging/logging.h"

#include <cmath>

SoundEngine* SoundEngine::Get()
{
    static SoundEngine instance;
    return &instance;
}

char const* SoundEngine::alErrorString(ALenum t_err) const noexcept
{
    switch (t_err)
    {
        [[likely]]
        case AL_NO_ERROR:
            return "AL_NO_ERROR";
        case ALC_INVALID_DEVICE : return "ALC_INVALID_DEVICE";
        case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
        case AL_INVALID_VALUE:
            return "AL_INVALID_VALUE";
        [[unlikely]]
        case AL_OUT_OF_MEMORY:
            return "AL_OUT_OF_MEMORY";
        default: return "Unknown error code";
    }
}

Utils::Signal SoundEngine::init()
{
    LOG_INFO("Opening default OpenAL audio device");
    ALCdevice* dev = alcOpenDevice("OpenSL");
    if (!dev)
    {
        m_err = alcGetError(nullptr);
        return Utils::Signal::Error;
    }

    LOG_INFO("Creating new OpenAL context");
    ALCcontext* ctx = alcCreateContext(dev, nullptr);
    if (!ctx || alcMakeContextCurrent(ctx) == ALC_FALSE)
    {
        m_err = alcGetError(dev);
        if (ctx) { alcDestroyContext(ctx); }
        alcCloseDevice(dev);
        return Utils::Signal::Error;
    }

    LOG_INFO("Generating OpenAL sources");
    alGenSources(1, &m_src);

    AL_CHECK_ERR(Utils::Signal::Error);

    // Ensure listener volume is audible on all platforms
    alListenerf(AL_GAIN, 1.0f);
    AL_CHECK_ERR(Utils::Signal::Error);

    return Utils::Signal::Success;
}

Utils::Signal SoundEngine::load(float t_ms, float t_freq)
{
    alGenBuffers(1, &m_buf);
    AL_CHECK_ERR(Utils::Signal::Error);

    /* Fill buffer with Sine-Wave */
    constexpr unsigned sample_rate = 44100;
    std::size_t        sample_cnt  = static_cast<std::size_t>((t_ms / 1000.f) * sample_rate);

    m_samples                      = new short[sample_cnt];

    for (std::size_t i = 0; i < sample_cnt; ++i)
    {
        m_samples[i] = static_cast<short>(
            32760 * std::sin((2.f * M_PI * t_freq * i) / sample_rate)
        );
    }

    /* Download buffer to OpenAL */
    alBufferData(
        m_buf, AL_FORMAT_MONO16, m_samples, static_cast<ALsizei>(sample_cnt * sizeof(short)),
        sample_rate
    );
    AL_CHECK_ERR(Utils::Signal::Error);

    return Utils::Signal::Success;
}

Utils::Signal SoundEngine::play()
{
    alSourcei(m_src, AL_BUFFER, m_buf);
    AL_CHECK_ERR(Utils::Signal::Error);

    // Play at full volume so output is clearly audible
    alSourcef(m_src, AL_GAIN, 1.0f);
    AL_CHECK_ERR(Utils::Signal::Error);

    alSourcePlay(m_src);
    AL_CHECK_ERR(Utils::Signal::Error);

    return Utils::Signal::Success;
}

SoundEngine::~SoundEngine() noexcept
{
    ALCcontext* ctx = alcGetCurrentContext();
    ALCdevice*  dev = alcGetContextsDevice(ctx);

    LOG_INFO("Deleting current OpenAL context");
    if (ctx) { alcDestroyContext(ctx); }
    alcMakeContextCurrent(nullptr);

    LOG_INFO("Closing OpenAL device");
    if (dev) { alcCloseDevice(dev); }

    if (m_samples != nullptr) { delete m_samples; }
}

ALenum SoundEngine::alGetLastError() const noexcept { return m_err; }
