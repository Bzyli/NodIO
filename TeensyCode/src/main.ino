#include <Arduino.h>
#include <Audio.h>
#include "nodes/Compressor/Compressor.h"

#define MAX_NODES 10
#define MAX_CONNS 10

typedef struct {
    int id;
    char type[10]; 
    float params[5];
    int paramCount = 0;    
    void *audioObj; 
} Node;

typedef struct {
    int from;
    int to;
} Connection;

Node nodes[MAX_NODES];
Connection conns[MAX_CONNS];
AudioConnection* audioConns[MAX_CONNS];
int nodeCount = 0, connCount = 0;

// Teensy Audio Objects
AudioInputI2S  audioInput;
AudioOutputI2S audioOutput;
AudioStream      *effects[MAX_NODES] = {nullptr};  // Handles both lowcut and delay

// Audio Control
AudioControlSGTL5000 audioShield;

char *skipSpaces(char *s) {
    while (*s && isspace((unsigned char)*s)) s++;
    return s;
}

void resetAudioGraph() {
    for (int i = 0; i < connCount; i++) {
        delete audioConns[i];
        audioConns[i] = nullptr;
    }
    connCount = 0;

    for (int i = 0; i < MAX_NODES; i++) {
        if (effects[i]) {
            delete effects[i];
            effects[i] = nullptr;
        }
    }
}

void parseConfig(char *config) {
    resetAudioGraph();
    nodeCount = 0;
    connCount = 0;

    char *nodesStart = strstr(config, "NODES[");
    if (!nodesStart) return;
    nodesStart += strlen("NODES[");

    char *nodesEnd = strchr(nodesStart, ']');
    if (!nodesEnd) return;
    *nodesEnd = '\0';

    char *token = strtok(nodesStart, ",");
    while (token && nodeCount < MAX_NODES) {
        token = skipSpaces(token);
        Node n;

         // Read id and type first
        char *ptr = token;
        int offset = 0;
        int items = sscanf(ptr, "%d %10s%n", &n.id, n.type, &offset);
        if (items < 2) {
            token = strtok(NULL, ",");
            continue;  // Skip invalid nodes
        }
        ptr += offset;  // Advance pointer to after the parsed content

        float param;
        int numChars = 0;
        while (ptr && sscanf(ptr, "%f%n", &param, &numChars) == 1) {
            n.params[n.paramCount++] = param;
            ptr += numChars;           // Advance pointer past the scanned number
            ptr = skipSpaces(ptr);     // Skip any whitespace if necessary
        }

        Serial.println(n.type);
        if (strcmp(n.type, "in") == 0) {
            n.audioObj = &audioInput;
        }
        else if (strcmp(n.type, "out") == 0) {
            n.audioObj = &audioOutput;
        }
        else if (strcmp(n.type, "lowcut") == 0) {
            effects[nodeCount] = new AudioFilterBiquad();
            ((AudioFilterBiquad*)effects[nodeCount])->setHighpass(0, n.params[0] / AUDIO_SAMPLE_RATE_EXACT, 0.7);
            n.audioObj = effects[nodeCount];
        }
        else if (strcmp(n.type, "delay") == 0) {
            effects[nodeCount] = new AudioEffectDelay();
            ((AudioEffectDelay*)effects[nodeCount])->delay(0,(int) n.params[0]); // Apply delay in milliseconds
            n.audioObj = effects[nodeCount];
        } else if (strcmp(n.type, "volume") == 0) {
            effects[nodeCount] = new AudioAmplifier();
            ((AudioAmplifier*)effects[nodeCount])->gain(n.params[0]);
            n.audioObj = effects[nodeCount];
        } else if (strcmp(n.type, "reverb") == 0) {
            effects[nodeCount] = new AudioEffectReverb();
            ((AudioEffectReverb*)effects[nodeCount])->reverbTime((int) n.params[0]);
            n.audioObj = effects[nodeCount];
        } else if (strcmp(n.type, "compressor") == 0) {
            effects[nodeCount] = new Compressor();
            ((Compressor*)effects[nodeCount])->setParamValue("threshold", n.params[0]);
            ((Compressor*)effects[nodeCount])->setParamValue("makeupGain", 3);
            ((Compressor*)effects[nodeCount])->setParamValue("ratio", n.params[1]);
            ((Compressor*)effects[nodeCount])->setParamValue("attack", n.params[2]);
            ((Compressor*)effects[nodeCount])->setParamValue("release", n.params[3]);
            Serial.println(((Compressor*)effects[nodeCount])->getParamValue("threshold"));
        } else if (strcmp(n.type, "gate") == 0) {
            Serial.println("Gate");
        }
        nodes[nodeCount++] = n;
        token = strtok(NULL, ",");
    }
    *nodesEnd = ']';

    char *connsStart = strstr(nodesEnd, "CONNS[");
    if (!connsStart) return;
    connsStart += strlen("CONNS[");

    char *connsEnd = strchr(connsStart, ']');
    if (!connsEnd) return;
    *connsEnd = '\0';

    token = strtok(connsStart, ",");
    while (token && connCount < MAX_CONNS) {
        token = skipSpaces(token);
        Connection c;
        if (sscanf(token, "%d %d", &c.from, &c.to) == 2) {
            conns[connCount++] = c;
        }
        token = strtok(NULL, ",");
    }
    *connsEnd = ']';

    for (int i = 0; i < connCount; i++) {
        Node *fromNode = nullptr;
        Node *toNode = nullptr;

        for (int j = 0; j < nodeCount; j++) {
            if (nodes[j].id == conns[i].from) fromNode = &nodes[j];
            if (nodes[j].id == conns[i].to) toNode = &nodes[j];
        }

        if (fromNode && toNode && fromNode->audioObj && toNode->audioObj) {
            int fromChannel = (strcmp(fromNode->type, "in") == 0) ? fromNode->params[0] - 1 : 0;
            int toChannel = (strcmp(toNode->type, "out") == 0) ? toNode->params[0] - 1 : 0;
            audioConns[i] = new AudioConnection(*(AudioStream*)fromNode->audioObj, fromChannel, *(AudioStream*)toNode->audioObj, toChannel);
        }
    }

    Serial.println("Audio graph updated.");
}

void setup() {
    Serial.begin(115200);
    while (!Serial) {}

    AudioMemory(400);
    audioShield.enable();
    audioShield.inputSelect(AUDIO_INPUT_LINEIN);
    audioShield.volume(0.8);
    Serial.println("Waiting for input...");
}

void loop() {
    static char inputBuffer[256];
    static int bufferIndex = 0;

    while (Serial.available()) {
        char c = Serial.read();
        if (c == '\n') {
            inputBuffer[bufferIndex] = '\0';
            Serial.println("\nReceived new config:");
            Serial.println(inputBuffer);
            parseConfig(inputBuffer);
            bufferIndex = 0;
        } else if (bufferIndex < sizeof(inputBuffer) - 1) {
            inputBuffer[bufferIndex++] = c;
        }
    }
}
