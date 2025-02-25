declare filename "spatialisation3D.dsp";
declare name "spatialisation3D";
// Spatialisation 3D avec des lignes à retard
// Faust Code

import("stdfaust.lib");

// Paramètres de spatialisation
angle = hslider("angle",0.5,0,1,0.01); // Angle de la source sonore (-180 à 180 degrés)
distance = hslider("distance[unit:m]", 1, 0.1, 10, 0.1);  // Distance de la source sonore (0.1 à 10 mètres)

// Constantes
speedOfSound = 343;  // Vitesse du son en m/s (à 20°C)
maxDelay = 0.1;      // Délai maximum en secondes (pour éviter des délais trop longs)


maxDelHear= 0.2*ma.SR/speedOfSound;

// Lignes à retard pour les canaux gauche et droit
leftChannel = de.fdelay(maxDelay * ma.SR, maxDelHear*(1-angle));  // Retard pour le canal gauche
rightChannel = de.fdelay(maxDelay * ma.SR, maxDelHear*angle);  // Retard pour le canal droit

// Atténuation en fonction de la distance
gain = 1 / distance;  // L'atténuation est inversement proportionnelle à la distance

// Application du gain et mixage
process = _ * gain <: (leftChannel, rightChannel);