#include "AudioMixer.h"
#include <math.h>  // Pour powf()

// 🎛️ Déclaration des objets audio
AudioSynthWaveform sineWave1;
AudioSynthWaveform sineWave2;
AudioMixer4 mixer;
AudioFilterBiquad filter1;
AudioFilterBiquad filter2;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

// 🎚️ Stockage des gains des canaux
float mixGain[4] = {0.5, 0.5, 0.5, 0.5};  // Par défaut : 50% (0.5)

// 🔗 Connexions Audio
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
    for (int i = 0; i < 4; i++) {
        mixer.gain(i, mixGain[i]); // Appliquer les gains initiaux
    }
}

// 🎚️ Fonction améliorée pour modifier le volume d'un canal avec transition douce
void setMix(int channel, float gainDB) {
    if (channel < 0 || channel > 3) {
        Serial.println("Erreur: canal invalide !");
        return;
    }

    // 🎛️ Définition des limites pour éviter la distorsion
    if (gainDB > 6.0) gainDB = 6.0;  // Max +6 dB
    if (gainDB < -60.0) gainDB = -60.0;  // Min -60 dB (quasi-silence)

    // Convertir dB en gain linéaire
    float targetGain = powf(10.0, gainDB / 20.0);
    float currentGain = mixGain[channel]; // Récupérer le gain actuel stocké

    // Transition douce pour éviter les sauts brusques
    float step = (targetGain - currentGain) / 10.0;  // Ajustement progressif en 10 étapes
    for (int i = 0; i < 10; i++) {
        currentGain += step;
        mixer.gain(channel, currentGain);
        mixGain[channel] = currentGain; // Mettre à jour la valeur stockée
        delay(10);  // Petite pause pour éviter une transition brutale
    }

    // Appliquer le gain final
    mixer.gain(channel, targetGain);
    mixGain[channel] = targetGain; // Mise à jour définitive
    Serial.printf("🎛️ Mixeur canal %d : %.1f dB (linéaire %.3f)\n", channel, gainDB, targetGain);
}

// 🎚️ Fonction pour modifier l'égalisation
void setBandEQ(int channel, float freq1, float freq2, float gainDB) {
    if (channel < 0 || channel > 1 || freq1 < 20 || freq2 > 20000 || freq1 >= freq2) {
        Serial.println("Erreur: paramètres invalides !");
        return;
    }

    // Convertir dB en gain linéaire
    float linearGain = powf(10.0, gainDB / 20.0);  

    // Calcul de la fréquence centrale et du facteur Q
    float centerFreq = (freq1 + freq2) / 2.0;
    float qFactor = centerFreq / (freq2 - freq1);

    // Appliquer le filtre passe-bande et ajuster le gain en sortie
    if (channel == 0) {
        filter1.setBandpass(0, centerFreq, qFactor);
        mixer.gain(0, linearGain);  // Appliquer l'atténuation après filtrage
        Serial.printf("🎚️ EQ canal 0 : %.1f Hz - %.1f Hz, Gain = %.1f dB (linéaire %.3f), Q = %.2f\n", 
                      freq1, freq2, gainDB, linearGain, qFactor);
    } else if (channel == 1) {
        filter2.setBandpass(0, centerFreq, qFactor);
        mixer.gain(1, linearGain);  // Appliquer l'atténuation après filtrage
        Serial.printf("🎚️ EQ canal 1 : %.1f Hz - %.1f Hz, Gain = %.1f dB (linéaire %.3f), Q = %.2f\n", 
                      freq1, freq2, gainDB, linearGain, qFactor);
    }
}

// 🎛️ Traitement des commandes USB
void processCommand(String command) {
    command.trim();

    if (command.startsWith("MIX")) {
        int channel;
        float gainDB;
        sscanf(command.c_str(), "MIX %d %f", &channel, &gainDB);
        setMix(channel, gainDB);
    }
    if (command.startsWith("FILTER")) {
        int channel;
        float freq1, freq2, gainDB;
        sscanf(command.c_str(), "FILTER %d %f %f %f", &channel, &freq1, &freq2, &gainDB);
        setBandEQ(channel, freq1, freq2, gainDB);
    }
}
