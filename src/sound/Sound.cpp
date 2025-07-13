#include "core/logging/Logging.h"
#include "sound/Sound.h"

SoundEngine::SoundEngine() : m_buf(0), m_src(0), m_samples(nullptr) {}

SoundEngine *SoundEngine::get() {
    static SoundEngine instance;
    return &instance;
}

const char* SoundEngine::alErrorString(ALenum t_err) const noexcept {
    switch (t_err) {
        case AL_NO_ERROR: return "AL_NO_ERROR";
        case ALC_INVALID_DEVICE: return "ALC_INVALID_DEVICE";
        case ALC_INVALID_CONTEXT: return "ALC_INVALID_CONTEXT";
        case AL_INVALID_VALUE: return "AL_INVALID_VALUE";
        case AL_OUT_OF_MEMORY: return "AL_OUT_OF_MEMORY";
        /* ... */
        default:
            return "Unknown error code";
    }
}

int SoundEngine::init()
{
    ALCdevice *dev = nullptr;
    ALCcontext *ctx = nullptr;

    LOGINFO("Finding default OpenAL audio device");
    const char *defname = alcGetString(nullptr, ALC_DEFAULT_DEVICE_SPECIFIER);
    //std::cout << "Default device: " << defname << std::endl;

    LOGINFO("Opening OpenAL audio device: " << defname);
    dev = alcOpenDevice(defname);
    
    LOGINFO("Creating new OpenAL context");
    ctx = alcCreateContext(dev, nullptr);

    LOGINFO("Making current context");
    alcMakeContextCurrent(ctx);

    LOGINFO("Generating OpenAL sources");
    alGenSources(1, &this->m_src);

    AL_CHECK_ERR(-1);
    return 0;
}

void SoundEngine::load(float t_ms, float t_freq) noexcept
{
    alGenBuffers(1, &this->m_buf);
    AL_CHECK_ERR();

    /* Fill buffer with Sine-Wave */
    constexpr unsigned sample_rate = 22050;
    size_t buf_size = t_ms * sample_rate;

    this->m_samples = new short[buf_size];
    
    for(size_t i = 0; i < buf_size; ++i) {
        this->m_samples[i] = 32760 * sin((2.f * float(M_PI) * t_freq) / sample_rate * i);
    }

    /* Download buffer to OpenAL */
    alBufferData(this->m_buf, AL_FORMAT_MONO16, this->m_samples, buf_size, sample_rate);
    AL_CHECK_ERR();
}

void SoundEngine::play() noexcept
{
    alSourcei(this->m_src, AL_BUFFER, this->m_buf);
    AL_CHECK_ERR();

    constexpr float gain = 0.1f;

    alSourcef(this->m_src, AL_GAIN, gain);
    AL_CHECK_ERR();

    alSourcePlay(this->m_src);
    AL_CHECK_ERR();
    return;
}

SoundEngine::~SoundEngine()
{
    ALCdevice *dev = nullptr;
    ALCcontext *ctx = nullptr;
    LOGINFO("Deleting current OpenAL context");
    ctx = alcGetCurrentContext();
    dev = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(nullptr);
    alcDestroyContext(ctx);
    LOGINFO("Closing OpenAL device");
    alcCloseDevice(dev);
    if (this->m_samples != nullptr) delete m_samples;
}

ALenum SoundEngine::alGetLastError() const noexcept { return this->m_err; }