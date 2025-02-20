#include "AudioMixer.h"

void setup() {
    Serial.begin(115200);
    setupAudio();  // Initialisation du mixage audio
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n'); // Lire la commande
        processCommand(command);
    }
}