#include "audio_engine.h"

#include <irrKlang.h>

namespace shiro {

audio_engine::audio_engine() {
    this->engine = irrklang::createIrrKlangDevice();
    if (!this->engine) {
        // @todo throw an exception
    }
}

audio_engine::~audio_engine() {
    this->engine->drop();
}

int audio_engine::load(const std::string &path) {
    sound_source sound;
    sound.source = this->engine->addSoundSourceFromFile(path.c_str());
    this->sounds.push_back(sound);
    return this->sounds.size() - 1;
}

void audio_engine::play(int id) {
    this->sounds[id].instance = this->engine->play2D(this->sounds[id].source, false, false, false, true);
}

void audio_engine::stop(int id) {
    this->sounds[id].instance->stop();
    this->sounds[id].instance->drop();
}

void audio_engine::stop_all_sounds() {
    this->engine->stopAllSounds();
}

void audio_engine::pause(int id) {
    this->sounds[id].instance->setIsPaused(true);
}

void audio_engine::resume(int id) {
    this->sounds[id].instance->setIsPaused(false);
}

bool audio_engine::is_playing(int id) {
    return !this->sounds[id].instance->isFinished();
}

}
