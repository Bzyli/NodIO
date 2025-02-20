#include "AudioMixer.h"
#include <math.h>  // Pour powf()

// 🎛️ Déclaration des objets audio
AudioSynthNoiseWhite noise1;
AudioSynthNoiseWhite noise2;
AudioMixer4 mixer;
AudioFilterBiquad filter1;
AudioFilterBiquad filter2;
AudioOutputI2S audioOutput;
AudioControlSGTL5000 audioShield;

// 🔗 Connexions Audio (ajout des filtres avant le mixeur)
AudioConnection patchCord1(noise1, 0, filter1, 0);  
AudioConnection patchCord2(noise2, 0, filter2, 0);  
AudioConnection patchCord3(filter1, 0, mixer, 0);
AudioConnection patchCord4(filter2, 0, mixer, 1);
AudioConnection patchCord5(mixer, 0, audioOutput, 0);
AudioConnection patchCord6(mixer, 0, audioOutput, 1);

// 🎚️ Initialisation de l'audio
void setupAudio() {
    AudioMemory(20);
    audioShield.enable();
    audioShield.volume(0.5);

    // Configuration du bruit blanc
    noise1.amplitude(0.5);
    noise2.amplitude(0.5);

    // Valeurs initiales du mixeur
    mixer.gain(0, 1.0);
    mixer.gain(1, 1.0);

    // 🎛️ Initialisation des filtres
    setBandEQ(0, 500, 5000, 0);
    setBandEQ(1, 500, 5000, 0);
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

    if (command.startsWith("FILTER")) {
        int channel;
        float freq1, freq2, gainDB;
        sscanf(command.c_str(), "FILTER %d %f %f %f", &channel, &freq1, &freq2, &gainDB);
        setBandEQ(channel, freq1, freq2, gainDB);
    }
    else {
        Serial.println("Commande inconnue !");
    }
}
