#pragma once

#include <string>
#include <map>
#include <vector>

#include <irrKlang.h>

namespace shiro {

class sound_source {
public:
    irrklang::ISoundSource *source = nullptr;
    irrklang::ISound *instance = nullptr;
};

class audio_engine {
public:
    audio_engine();
    ~audio_engine();
    int load(const std::string &path);
    void play(int id);
    void stop(int id);
    void stop_all_sounds();
    void pause(int id);
    void resume(int id);
    bool is_playing(int id);
private:
    irrklang::ISoundEngine *engine;
    std::vector<sound_source> sounds;
};

}
