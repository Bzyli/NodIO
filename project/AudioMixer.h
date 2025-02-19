#ifndef AUDIO_MIXER_H
#define AUDIO_MIXER_H

#include <Arduino.h>
#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>

// 🎛️ Définition des objets audio
extern AudioSynthWaveform sineWave1;  // Onde sinusoïdale 1
extern AudioSynthWaveform sineWave2;  // Onde sinusoïdale 2
extern AudioMixer4 mixer;             // Mixer 4 entrées
extern AudioOutputI2S audioOutput;    // Sortie audio (casque, haut-parleur)
extern AudioControlSGTL5000 audioShield; // Contrôle du shield audio

// 🔊 Fonctions
void setupAudio();
void processCommand(String command);

#endif // AUDIO_MIXER_H
