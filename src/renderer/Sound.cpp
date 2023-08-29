#include "renderer/Sound.h"

SoundEngine::SoundEngine() : buf(0), src(0), samples(NULL) {}

SoundEngine* SoundEngine::get() { return new SoundEngine(); }

int SoundEngine::init()
{
    ALCdevice *dev = NULL;
    ALCcontext *ctx = NULL;

    const char *defname = alcGetString(NULL, ALC_DEFAULT_DEVICE_SPECIFIER);
    //std::cout << "Default device: " << defname << std::endl;

    dev = alcOpenDevice(defname);
    ctx = alcCreateContext(dev, NULL);
    alcMakeContextCurrent(ctx);
    alGenSources(1, &this->src);

    if (alGetError() != AL_NO_ERROR) return -1;
    return 0;
}

void SoundEngine::load(float ms, float freq)
{
    alGenBuffers(1, &this->buf);

    /* Fill buffer with Sine-Wave */
    unsigned sample_rate = 22050;
    size_t buf_size = ms * sample_rate;

    this->samples = new short[buf_size];
    for(int i=0; i<buf_size; ++i) {
        this->samples[i] = 32760 * sin( (2.f*float(M_PI)*freq)/sample_rate * i );
    }

    /* Download buffer to OpenAL */
    alBufferData(this->buf, AL_FORMAT_MONO16, this->samples, buf_size, sample_rate);
}

void SoundEngine::play()
{
    alSourcei(this->src, AL_BUFFER, this->buf);

    alSourcef(this->src, AL_GAIN, 0.1f);

    alSourcePlay(this->src);
    return;
}

SoundEngine::~SoundEngine()
{
    ALCdevice *dev = NULL;
    ALCcontext *ctx = NULL;
    ctx = alcGetCurrentContext();
    dev = alcGetContextsDevice(ctx);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(ctx);
    alcCloseDevice(dev);
    if (this->samples != NULL) delete samples;
}