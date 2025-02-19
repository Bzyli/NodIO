#include "AudioMixer.h"

// 🎛️ Déclaration des objets audio
AudioSynthWaveform sineWave1;
AudioSynthWaveform sineWave2;
AudioMixer4 mixer;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

// Connexion des objets audio
AudioConnection patchCord1(sineWave1, 0, mixer, 0);
AudioConnection patchCord2(sineWave2, 0, mixer, 1);
AudioConnection patchCord3(mixer, 0, audioOutput, 0);
AudioConnection patchCord4(mixer, 0, audioOutput, 1);

// 🎚️ Initialisation de l'audio
void setupAudio() {
    AudioMemory(10);
    audioShield.enable();
    audioShield.volume(0.5);

    // Configuration des oscillateurs
    sineWave1.frequency(440); // 440 Hz (La)
    sineWave1.amplitude(0.5);

    sineWave2.frequency(880); // 880 Hz (La octave supérieure)
    sineWave2.amplitude(0.5);

    // Valeurs initiales du mixeur
    mixer.gain(0, 0.5); // Canal 0 à 50%
    mixer.gain(1, 0.5); // Canal 1 à 50%
}

// 🎛️ Traitement des commandes reçues par USB
void processCommand(String command) {
    command.trim(); // Supprime les espaces inutiles

    if (command.startsWith("MIX")) {
        int channel;
        float gain;
        sscanf(command.c_str(), "MIX %d %f", &channel, &gain);

        if (channel >= 0 && channel < 4) {
            mixer.gain(channel, gain);
            Serial.printf("Canal %d réglé à %.2f\n", channel, gain);
        } else {
            Serial.println("Erreur: canal invalide !");
        }
    } 
    else if (command.startsWith("FREQ")) {
        int osc;
        float freq;
        sscanf(command.c_str(), "FREQ %d %f", &osc, &freq);

        if (osc == 0) sineWave1.frequency(freq);
        else if (osc == 1) sineWave2.frequency(freq);
        
        Serial.printf("Oscillateur %d réglé à %.2f Hz\n", osc, freq);
    }
    else {
        Serial.println("Commande inconnue !");
    }
}
