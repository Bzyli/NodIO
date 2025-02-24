declare filename "compresseur.dsp";
declare name "compresseur";
import("stdfaust.lib");

// Paramètres réglables du compresseur
threshold = hslider("Threshold [dB]", -10, -40, 0, 0.1); // Seuil de compression en dB
ratio = hslider("Ratio", 4, 1, 20, 0.1);                // Ratio de compression
attack = hslider("Attack [s]", 0.01, 0.001, 0.1, 0.001); // Temps d'attaque en secondes
release = hslider("Release [s]", 0.1, 0.01, 0.5, 0.01);  // Temps de relâchement en secondes
makeupGain = hslider("Makeup Gain [dB]", 6, 0, 20, 0.1); // Gain de compensation en dB

// Conversion dB -> linéaire
db2linear(x) = 10^(x / 20);
linear2db(x) = 20 * log10(x);

// Détection du niveau RMS en dB
levelDetector(x) = abs(x) : si.smoo : linear2db;

// Fonction de compression
compress(level) = 
    ba.if(level > threshold, 
          db2linear(threshold + (level - threshold) / ratio), 
          1);

// Compresseur audio avec enveloppe d'attaque et de relâche
compressor(x) =
    x * (levelDetector(x)
        : en.ar(attack, release)
        : compress)
    * db2linear(makeupGain);

// Traitement du signal stéréo
process = _,_ : compressor, compressor;