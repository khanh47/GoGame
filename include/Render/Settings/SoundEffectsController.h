#pragma once
#include "SoundController.h"

class SoundEffectsController : public SoundController {
public:
    SoundEffectsController(AudioManager* audioManager);
    ~SoundEffectsController() override = default;

protected:
    void applyVolume() override;
    void applyMute() override;
    void syncFromAudioManager() override;
};
