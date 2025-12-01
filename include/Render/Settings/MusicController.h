#pragma once
#include "SoundController.h"

class MusicController : public SoundController {
public:
    MusicController(AudioManager* audioManager);
    ~MusicController() override = default;

protected:
    void applyVolume() override;
    void applyMute() override;
    void syncFromAudioManager() override;
};
