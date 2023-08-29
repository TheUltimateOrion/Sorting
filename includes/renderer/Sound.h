#pragma once
#include "../main.h"

#define AL_CHECK_ERROR() if(this->error != 0) std::cerr << "OpenAL Error: " << alErrorString(this->error) << ' ' << this->error << std::endl;

class SoundEngine
{
private:
    ALuint buf;
    ALuint src = 0;
    short *samples;
public:
    static SoundEngine* get();

    SoundEngine();
    ~SoundEngine();

    int init();
    void load(float ms, float freq);
    void play();
};