#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// 🎛️ Définition des objets audio
extern AudioSynthNoiseWhite noise1;
extern AudioSynthNoiseWhite noise2;
extern AudioMixer4 mixer;
extern AudioFilterBiquad filter1;  // 🎚️ Filtre pour canal 0
extern AudioFilterBiquad filter2;  // 🎚️ Filtre pour canal 1
extern AudioOutputI2S audioOutput;
extern AudioControlSGTL5000 audioShield;

// 🔊 Fonctions
void setupAudio();
void processCommand(String command);
void setBandEQ(int channel, float freq1, float freq2, float gainDB);

#endif // AUDIO_MIXER_H
