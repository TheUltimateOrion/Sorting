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
    ALCdevice*  dev = nullptr;
    ALCcontext* ctx = nullptr;

    LOG_INFO("Finding default OpenAL audio device");
    char const* defname = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    LOG_INFO("Default device: " << defname);

    LOG_INFO("Opening OpenAL audio device: " << defname);
    dev = alcOpenDevice(defname);

    LOG_INFO("Creating new OpenAL context");
    ctx = alcCreateContext(dev, nullptr);

    LOG_INFO("Making current context");
    alcMakeContextCurrent(ctx);

    LOG_INFO("Generating OpenAL sources");
    alGenSources(1, &m_src);

    AL_CHECK_ERR(Utils::Signal::Error);

    return Utils::Signal::Success;
}

Utils::Signal SoundEngine::load(float t_ms, float t_freq)
{
    alGenBuffers(1, &m_buf);
    AL_CHECK_ERR(Utils::Signal::Error);

    /* Fill buffer with Sine-Wave */
    constexpr unsigned sample_rate = 22050;
    std::size_t        buf_size    = t_ms * sample_rate;

    m_samples                      = new short[buf_size];

    for (std::size_t i = 0; i < buf_size; ++i)
    {
        m_samples[i] = 32760 * std::sin((2.f * M_PI * t_freq) / sample_rate * i);
    }

    /* Download buffer to OpenAL */
    alBufferData(m_buf, AL_FORMAT_MONO16, m_samples, buf_size, sample_rate);
    AL_CHECK_ERR(Utils::Signal::Error);

    return Utils::Signal::Success;
}

Utils::Signal SoundEngine::play()
{
    alSourcei(m_src, AL_BUFFER, m_buf);
    AL_CHECK_ERR(Utils::Signal::Error);

    constexpr float gain = 0.1f;

    alSourcef(m_src, AL_GAIN, gain);
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
