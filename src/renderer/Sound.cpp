#include "renderer/Sound.h"

SoundEngine::SoundEngine() : buf(0), src(0), samples(nullptr) {}

SoundEngine *SoundEngine::get() {
    static SoundEngine instance;
    return &instance;
}

const char* SoundEngine::alErrorString(ALenum err) const noexcept {
    switch (err) {
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
    alGenSources(1, &this->src);
    AL_CHECK_ERR(-1);
    return 0;
}

void SoundEngine::load(float ms, float freq) noexcept
{
    alGenBuffers(1, &this->buf);
    AL_CHECK_ERR();

    /* Fill buffer with Sine-Wave */
    constexpr unsigned sample_rate = 22050;
    size_t buf_size = ms * sample_rate;

    this->samples = new short[buf_size];
    for(int i = 0; i < buf_size; ++i) {
        this->samples[i] = 32760 * sin((2.f * float(M_PI) * freq) / sample_rate * i);
    }

    /* Download buffer to OpenAL */
    alBufferData(this->buf, AL_FORMAT_MONO16, this->samples, buf_size, sample_rate);
    AL_CHECK_ERR();
}

void SoundEngine::play() noexcept
{
    alSourcei(this->src, AL_BUFFER, this->buf);
    AL_CHECK_ERR();

    constexpr float gain = 0.1f;

    alSourcef(this->src, AL_GAIN, gain);
    AL_CHECK_ERR();

    alSourcePlay(this->src);
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
    if (this->samples != nullptr) delete samples;
}

ALenum SoundEngine::alGetLastError() const noexcept { return this->err; }